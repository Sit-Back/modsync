#include "SyncClient.h"
#include <QNetworkReply>
#include <QMessageBox>
#include <QDebug>
#include <qregularexpression.h>

SyncClient::SyncClient() {
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

    for (const QString& file : modnamesremove)
    {
        QFile(MODSDIR.path() + file).remove();
    }
}

bool SyncClient::addProfile() const
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
        std::string searchString = "\"profiles\":{";
        std::size_t pos = data.toStdString().find(searchString);

        if (pos == std::string::npos)
        {
            qCritical() << "Corrupt launcher_profiles.json";
            profiles.close();
            return false;
        }

        data.insert(pos+searchString.size(), profileString);
        profiles.close();
        if (profiles.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            profiles.write(data.toUtf8());
            profiles.close();
            return true;
        } else
        {
            qCritical() << "Cannot write to launcher_profiles.json";
            return false;
        }
    } else
    {
        qCritical() << "Insufficient permissions to read from launcher_profiles.json";
        return false;
    }


}

void SyncClient::prepSync()
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
            loaderURL = reply->readLine().trimmed();
            loaderName = reply->readLine().trimmed();

            std::vector<QString> mods;
            while (!reply->atEnd())
            {
                mods.emplace_back(reply->readLine().trimmed());
            }

            reply->deleteLater();
            calcSyncDiffs(mods);
            needToSync = false;
            emit prepFinished();
        } else
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

    return {modnamesdownload};
}

void SyncClient::calcSyncDiffs(std::vector<QString> mods)
{
    modnamesdownload = {};
    // Calc Download Needed
    for (QString filename : mods)
    {
        QFile file = QFile(MODSDIR.path() + filename);
        if (!file.exists())
        {
            modnamesdownload.push_back(filename);
        }
    }

    // Calc Remove Needed
    modnamesremove = {};
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
                modnamesremove.push_back(filename);
            }
        } else
        {
            qInfo() << "Mod omitted due to '!' tag: " << filename;
        }
    }
}

SyncClient::SyncMetadata SyncClient::getMetadata() const
{
    if (loaderID.isEmpty() || loaderName.isEmpty() || loaderURL.isEmpty()) {
        throw std::runtime_error("Metadata is not complete");
    } else if (needToSync) {
        throw std::runtime_error("Data has not been prepped yet.");
    }

    return {
        loaderID,
        loaderURL,
        loaderName
    };
}


