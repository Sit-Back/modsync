//
// Created by mitch on 20/11/25.
//

#include "LoaderInstaller.h"

#include <iostream>

#include "Downloader.h"
#include "Locations.h"
#include <QObject>
#include <QProcess>
#include <qregularexpression.h>

LoaderInstaller::LoaderInstaller(QString loaderID, QString loaderCMD) :
loaderID(std::move(loaderID)),
silentInstallCMD(std::move(loaderCMD))
{
    silentInstallCMD = silentInstallCMD.replace("%minecraft",
        MINECRAFTDIR.absolutePath());
    silentInstallCMD = silentInstallCMD.replace("%loader",
        QDir::toNativeSeparators(PROFILEDIR.filePath("loader.jar")));
}

void LoaderInstaller::downloadLoader() const
{
    auto* downloader = new Downloader();
    auto loaderURL = ROOTURL;
    loaderURL.setPath("/loader.jar");
    downloader->download(loaderURL, PROFILEDIR);

    connect(downloader, &Downloader::downloadFinished, this, [this]()
    {
       emit loaderDownloadFinished();
    });
}



void LoaderInstaller::installLoader() const
{
    QFile file(PROFILEDIR.filePath("loader.jar"));
    if (!file.exists())
    {
        throw std::runtime_error("Cannot find loader.jar");
    }

    qInfo() << silentInstallCMD;
    auto *loaderInstallerProcess = new QProcess();
    loaderInstallerProcess->start("java", silentInstallCMD.split(" "));

    connect(loaderInstallerProcess, &QProcess::readyReadStandardOutput, this, [loaderInstallerProcess]()
    {
       std::cout << loaderInstallerProcess->readAllStandardOutput().toStdString();
    });

    connect(loaderInstallerProcess, &QProcess::readyReadStandardError, this, [loaderInstallerProcess]()
    {
       std::cerr << loaderInstallerProcess->readAllStandardError().toStdString();
    });

    connect(loaderInstallerProcess, &QProcess::finished, this, [this]()
    {
       emit loaderInstalled();
    });
}

bool LoaderInstaller::removeProfile()
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
    std::size_t modsyncFlagPos = data.toStdString().find(modsyncFlag);

    if (modsyncFlagPos == std::string::npos)
    {
        return false;
    }


    int finishOffset = 0;
    for (int i = modsyncFlagPos; i < data.size(); i++)
    {
        finishOffset++;
        if (data[i] == "}" )
        {
            finishOffset++;
            break;
        }
    }
    data.remove(modsyncFlagPos, finishOffset);
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

void LoaderInstaller::addProfile() const
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

bool LoaderInstaller::loaderVersionExists() const
{
    QDir versionsPath(MINECRAFTDIR);
    versionsPath.cd("versions");
    QStringList versions = versionsPath.entryList(QDir::Dirs);
    return std::any_of(versions.begin(), versions.end(), [this](const QString& version)
    {
        return version == loaderID;
    });
}