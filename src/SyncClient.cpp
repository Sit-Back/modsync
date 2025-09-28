#include "SyncClient.h"
#include <QNetworkReply>
#include <QMessageBox>

SyncClient::SyncClient() {
    networkManager = new QNetworkAccessManager();
}

bool SyncClient::createProfileDir()
{
    return QDir().mkpath(INSTALLDIR + "/mods");
}

bool SyncClient::installDirExists()
{
    return QDir(SyncClient::INSTALLDIR).exists();
}

bool SyncClient::removeInstallDir()
{
    return QDir(INSTALLDIR).removeRecursively();
}

void SyncClient::removeExtras() const
{
    if (needToSync)
    {
        throw std::runtime_error("Data has not been prepped yet.");
    }

    for (const QString& file : modnamesremove)
    {
        QFile(INSTALLDIR + "/mods/" + file).remove();
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
        QFile file = QFile(INSTALLDIR + "/mods/" + filename);
        if (!file.exists())
        {
            modnamesdownload.push_back(filename);
        }
    }

    // Calc Remove Needed
    modnamesremove = {};
    for (QString filename : QDir(INSTALLDIR + "/mods/").entryList(QDir::Files | QDir::NoDotAndDotDot))
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


