#include "SyncClient.h"
#include <QNetworkReply>
#include <QMessageBox>
#include <QDebug>

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

void SyncClient::addProfile() const
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
    profileString = profileString.arg(loaderID).arg(PROFILEDIR.path());

    auto profiles = QFile(MINECRAFTDIR.filePath("launcher_profiles.json"));
    if (profiles.open(QIODevice::ReadOnly))
    {
        QString data = profiles.readAll();
        std::string searchString = "\"profiles\" : {";
        std::size_t pos = data.toStdString().find(searchString) + searchString.size();
        data.insert(pos, profileString);
        profiles.close();
        if (profiles.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            profiles.write(data.toUtf8());
            profiles.close();
        } else
        {
            qDebug() << "Cannot write to launcher_profiles.json!";
        }

    } else
    {

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
        throw std::runtime_error("Data has not been prepped yet.");
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


