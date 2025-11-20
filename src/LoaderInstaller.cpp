//
// Created by mitch on 20/11/25.
//

#include "LoaderInstaller.h"

#include "Downloader.h"
#include "Locations.h"
#include <QObject>
#include <qregularexpression.h>

LoaderInstaller::LoaderInstaller(QString loaderID, QString silentInstallCMD) :
loaderID(std::move(loaderID)), silentInstallCMD(std::move(silentInstallCMD))
{}


void LoaderInstaller::downloadLoader() const
{
    auto* downloader = new Downloader();
    auto loaderURL = ROOTURL;
    loaderURL.setPath("/loader");
    downloader->download(loaderURL, PROFILEDIR);

    connect(downloader, &Downloader::downloadFinished, this, [this]()
    {
       emit loaderDownloadFinished();
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