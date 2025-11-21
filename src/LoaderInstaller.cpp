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
    silentInstallCMD = silentInstallCMD.arg(
        MINECRAFTDIR.absolutePath(),
        PROFILEDIR.filePath("loader.jar"));
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

bool LoaderInstaller::addProfile() const
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
    profileString = profileString.arg(loaderID, PROFILEDIR.path());
    static const QRegularExpression rs("\\s");
    profileString.remove(rs);

    auto profiles = QFile(MINECRAFTDIR.filePath("launcher_profiles.json"));
    if (!profiles.exists())
    {
        qCritical() << "launcher_profiles.json does not exist";
        return false;
    }
    if (profiles.open(QIODevice::ReadOnly))
    {
        QString data = profiles.readAll();
        data.remove(rs);
        std::string modsyncFlag = "\"modsync\":{";
        std::size_t modsyncFlagPos = data.toStdString().find(modsyncFlag);

        if (modsyncFlagPos != std::string::npos)
        {
            qInfo() << "Existing launcher profile found, attempting replace";

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
        }

        std::string profilesFlag = "\"profiles\":{";
        std::size_t profilesFlagPos = data.toStdString().find(profilesFlag);

        if (profilesFlagPos == std::string::npos)
        {
            qCritical() << "Corrupt launcher_profiles.json";
            profiles.close();
            return false;
        }

        data.insert(profilesFlagPos + profilesFlag.size(), profileString);
        profiles.close();
        if (profiles.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            profiles.write(data.toUtf8());
            profiles.close();
            qInfo() << "Launcher profile added successfully";
            return true;
        }
        else
        {
            qCritical() << "Cannot write to launcher_profiles.json";
            return false;
        }
    }
    else
    {
        qCritical() << "Insufficient permissions to read from launcher_profiles.json";
        return false;
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