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

            while (!reply->atEnd())
            {
                modnamestotal.emplace_back(reply->readLine().trimmed());
            }

            reply->deleteLater();

            emit prepFinished();
        } else
        {
            emit fetchError(reply->errorString());
        }
    });

}

std::vector<QString> SyncClient::getModDownload() const
{
    if (modnamestotal.empty())
    {
        throw std::runtime_error("Data has not been prepped yet.");
    }

    return {modnamestotal};
}

SyncClient::SyncMetadata SyncClient::getMetadata() const
{
    if (loaderID.isEmpty() || loaderName.isEmpty() || loaderURL.isEmpty())
    {
        throw std::runtime_error("Data has not been prepped yet.");
    }

    return {
        loaderID,
        loaderURL,
        loaderName
    };
}


