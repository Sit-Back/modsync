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

void SyncClient::fetchMetadata()
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
                modnames.emplace_back(reply->readLine().trimmed());
            }

            fetchedMetadata();
        } else
        {
            fetchError(reply->errorString());
        }
    });

}

std::optional<std::vector<QString>> SyncClient::getModNames() const
{
    if (modnames.empty())
    {
        return {};
    }

    return {modnames};
}

std::optional<SyncClient::SyncMetadata> SyncClient::getMetadata() const
{
    if (loaderID.isEmpty() || loaderName.isEmpty() || loaderURL.isEmpty())
    {
        return {};
    }

    return {
        SyncMetadata{
            loaderID,
            loaderName,
            loaderURL
        }
    };
}


