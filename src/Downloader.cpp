#include "Downloader.h"

#include <QNetworkReply>
#include <utility>

unsigned long Downloader::getDownloadsTotal() const {return downloadTotal;}

unsigned long Downloader::getDownloadsFinished() const {return finishedDownloads;}

Downloader::Downloader(std::filesystem::path  path, const std::vector<QUrl>& urls) :
downloadPath(std::move(path))
{
    manager = new QNetworkAccessManager();
    downloadTotal = urls.size();
    finishedDownloads = 0;
}

void Downloader::download(const QUrl& url)
{
    auto* request = new QNetworkRequest(url);
    QNetworkReply* reply = manager->get(*request);

    auto *file = new QFile(downloadPath);
    file->open(QIODevice::WriteOnly);

    QObject::connect(reply, &QNetworkReply::finished, [reply, file, this]()
    {
        file->close();
        reply->deleteLater();
        finishedDownloads++;
        emit this->downloadFinished();
    });
    QObject::connect(reply, &QNetworkReply::readyRead, [reply, file](){
        file->write(reply->readAll());
    });
}