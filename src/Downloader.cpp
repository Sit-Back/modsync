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

    for (const QUrl& url : urls)
    {
        download(url);
    }
}

void Downloader::download(const QUrl& url)
{
    auto* request = new QNetworkRequest(url);
    QNetworkReply* reply = manager->get(*request);

    std::filesystem::path path(downloadPath.string() + "/mods/" + url.fileName().toStdString());
    auto *file = new QFile(path);
    file->open(QIODevice::WriteOnly);

    bool renamed = false;

    QObject::connect(reply, &QNetworkReply::finished, [reply, file, this]()
    {
        //Cleanup
        file->close();
        reply->deleteLater();

        //Needed for UI
        finishedDownloads++;
        assert(finishedDownloads <= downloadTotal);
        emit this->downloadFinished();
    });
    QObject::connect(reply, &QNetworkReply::readyRead, [reply, file](){
        file->write(reply->readAll());
    });
}