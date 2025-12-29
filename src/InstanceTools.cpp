//
// Created by mitch on 29/12/25.
//

#include "InstanceTools.h"

#include <QDir>
#include <QRegularExpression>

#include "Locations.h"

bool InstanceTools::removeInstallDir()
{
    return QDir(PROFILEDIR).removeRecursively();
}

bool InstanceTools::removeProfile()
{
    static const QRegularExpression rs("\\s");

    auto launcherJson = QFile(
        MINECRAFTDIR.filePath("launcher_profiles.json"));

    if (!launcherJson.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("Insufficient read permissions"
                                 " to remove profile from launcher_profiles.json");
    }

    QString data = launcherJson.readAll();
    data.remove(rs);
    std::string modsyncFlag = "\"modsync\":{";
    std::size_t startOffset = data.toStdString().find(modsyncFlag);

    if (startOffset == std::string::npos)
    {
        return false;
    }

    if (data[startOffset - 1] != '{')
    {
        startOffset--;
    }

    int finishOffset = 0;
    for (int i = startOffset; i < data.size(); i++)
    {
        finishOffset++;
        if (data[i] == "}" )
        {
            if (data[i + 1] == ',')
            {
                finishOffset++;
            }
            break;
        }
    }
    data.remove(startOffset, finishOffset);
    launcherJson.close();

    if (!launcherJson.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        throw std::runtime_error("Insufficient read permissions"
                                 " to remove profile from launcher_profiles.json");
    }

    launcherJson.write(data.toUtf8());
    launcherJson.close();

    return true;
}

void InstanceTools::addProfile(QString loaderID)
{
    QString profileString = R"(
    "modsync": {
      "lastUsed": "1970-01-02T00:00:00.000Z",
      "lastVersionId": "%1",
      "created": "1970-01-02T00:00:00.000Z",
      "gameDir" : "%2",
      "name": "Modsync",
      "icon": "Dirt",
      "type": "custom"
    },)";
    QString gameDir = QDir::toNativeSeparators(PROFILEDIR.path());
#ifdef Q_OS_WIN
    gameDir = gameDir.replace("\\","\\\\");
#endif
    profileString = profileString.arg(loaderID, gameDir);


    static const QRegularExpression rs("\\s");
    profileString.remove(rs);

    auto profiles = QFile(MINECRAFTDIR.filePath("launcher_profiles.json"));
    if (!profiles.exists())
    {
        throw std::runtime_error("launcher_profiles.json does not exist");
    }

    if (removeProfile())
    {
        qInfo() << "Existing launcher profile found, attempting replace";
    }

    if (profiles.open(QIODevice::ReadOnly))
    {
        QString data = profiles.readAll();
        data.remove(rs);


        std::string profilesFlag = "\"profiles\":{";
        std::size_t profilesFlagPos = data.toStdString().find(profilesFlag);

        if (profilesFlagPos == std::string::npos)
        {
            profiles.close();
            throw std::runtime_error("Corrupt launcher_profiles.json");
        }

        data.insert(profilesFlagPos + profilesFlag.size(), profileString);
        profiles.close();
        if (profiles.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            profiles.write(data.toUtf8());
            profiles.close();
            qInfo() << "Launcher profile added successfully";
        }
        else
        {
            throw std::runtime_error("Cannot write to launcher_profiles.json");
        }
    }
    else
    {
        throw std::runtime_error("Insufficient permissions to read from launcher_profiles.json");
    }
}

bool InstanceTools::loaderVersionExists(QString loaderID)
{
    QDir versionsPath(MINECRAFTDIR);
    versionsPath.cd("versions");
    QStringList versions = versionsPath.entryList(QDir::Dirs);
    if (std::any_of(versions.begin(), versions.end(), [loaderID](const QString& version)
    {
        return version == loaderID;
    }))
    {
        qInfo() << "Loader found.";
        return true;
    } else
    {
        qInfo() << "No loader found.";
        return false;
    }
}