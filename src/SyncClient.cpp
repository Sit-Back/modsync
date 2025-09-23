#include "SyncClient.h"
#include <QNetworkReply>
#include <QFile>

SyncClient::SyncClient()
{
    manager = new QNetworkAccessManager();
}

int SyncClient::getDownloadsTotal() const {return downloadTotal;}

int SyncClient::getDownloadsFinished() const {return finishedDownloads;}

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


void SyncClient::download(const QUrl& url)
{
    auto* request = new QNetworkRequest(url);
    QNetworkReply* reply = manager->get(*request);

    auto *file = new QFile(url.fileName());
    file->open(QIODevice::WriteOnly);

    bool renamed = false;

    QObject::connect(reply, &QNetworkReply::finished, [reply, file, this]()
    {
        file->close();
        reply->deleteLater();
        finishedDownloads++;
        emit this->requestFinished();
    });
    QObject::connect(reply, &QNetworkReply::readyRead, [reply, file](){
        file->write(reply->readAll());
    });
}