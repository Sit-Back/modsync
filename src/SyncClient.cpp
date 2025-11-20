#include "SyncClient.h"
#include <QNetworkReply>
#include <QMessageBox>
#include <QDebug>
#include <qregularexpression.h>
#include <QStandardPaths>

#include "Downloader.h"
#include <QProcess>

SyncClient::SyncClient()
{
    networkManager = new QNetworkAccessManager();
}

bool SyncClient::createProfileDir()
{
    return QDir().mkpath(MODSDIR.path());
}

bool SyncClient::installDirExists()
{
    return SyncClient::PROFILEDIR.exists();
}

bool SyncClient::minecraftDirExists()
{
    return SyncClient::MINECRAFTDIR.exists();
}

bool SyncClient::versionExists(QString versionName)
{
    QDir versionsPath(MINECRAFTDIR);
    versionsPath.cd("versions");
    QStringList versions = versionsPath.entryList(QDir::Dirs);
    return std::any_of(versions.begin(), versions.end(), [versionName](const QString& version)
    {
        return version == versionName;
    });
}

void SyncClient::downloadLoader() const
{
    if (needToSync)
    {
        throw std::runtime_error("Data has not been prepped yet.");
    }

    auto* downloader = new Downloader();
    auto loaderURL = ROOTURL;
    loaderURL.setPath("/loader");
    downloader->download(loaderURL, PROFILEDIR);

    connect(downloader, &Downloader::downloadFinished, this, [this]()
    {
       emit loaderDownloadFinished();
    });
}

bool SyncClient::removeInstallDir()
{
    return QDir(PROFILEDIR).removeRecursively();
}

void SyncClient::removeExtras() const
{
    if (needToSync)
    {
        throw std::runtime_error("Data has not been prepped yet.");
    }

    for (const QString& file : modNamesRemove)
    {
        QFile(MODSDIR.path() + "/" + file).remove();
    }
}

bool SyncClient::javaInstalled()
{
    const QString javaExe = QStandardPaths::findExecutable("java");
    return !javaExe.isEmpty();
}

bool SyncClient::addProfile() const //Will be private method in syncer
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

void SyncClient::prepSync() //Will use a factory instead.
{
    QUrl metadataURL = ROOTURL;
    metadataURL.setPath("/meta");
    auto request = QNetworkRequest(metadataURL);

    QNetworkReply* reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [reply, this]()
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            loaderID = reply->readLine().trimmed();
            loaderCMD = reply->readLine().trimmed();

            if (loaderID.isEmpty() || loaderCMD.isEmpty())
            {
                emit fetchError("Metadata is not complete, please contact server owner.");
            }

            std::vector<QString> mods;
            while (!reply->atEnd())
            {
                mods.emplace_back(reply->readLine().trimmed());
            }

            reply->deleteLater();
            calcSyncDiffs(mods);
            needToSync = false;
            emit prepFinished();
        }
        else
        {
            emit fetchError(reply->errorString());
        }
    });
}

std::vector<QString> SyncClient::getModDownload() const
{
    if (needToSync)
    {
        throw std::runtime_error("Data has not been prepped yet");
    }

    return {modNamesDownload};
}

void SyncClient::calcSyncDiffs(std::vector<QString> mods)
{
    modNamesDownload = {};
    // Calc Download Needed
    for (QString filename : mods)
    {
        QFile file = QFile(MODSDIR.path() + "/" + filename);
        if (!file.exists())
        {
            modNamesDownload.push_back(filename);
        }
    }

    // Calc Remove Needed
    modNamesRemove = {};
    for (QString filename : MODSDIR.entryList(QDir::Files | QDir::NoDotAndDotDot))
    {
        if (filename.front() != QChar('!'))
        {
            bool filenameFound = false;
            for (QString mod : mods)
            {
                if (mod == filename)
                {
                    filenameFound = true;
                    break;
                }
            }

            if (filenameFound == false)
            {
                modNamesRemove.push_back(filename);
            }
        }
        else
        {
            qInfo() << "Mod omitted due to '!' tag: " << filename;
        }
    }
}

SyncClient::SyncMetadata SyncClient::getMetadata() const
{
    if (needToSync)
    {
        throw std::runtime_error("Data has not been prepped yet.");
    }

    return {
        loaderID,
        loaderCMD,
    };
}
