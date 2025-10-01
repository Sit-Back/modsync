#include "Downloader.h"

#include <QNetworkReply>
#include <utility>

#include "SyncClient.h"

unsigned long Downloader::getDownloadsTotal() const { return downloadTotal; }

unsigned long Downloader::getDownloadsFinished() const { return finishedDownloads; }

Downloader::Downloader(QDir path, const std::vector<QUrl>& urls) :
    downloadPath(path)
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

    auto* file = new QFile(downloadPath.filePath(url.fileName()));
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
    QObject::connect(reply, &QNetworkReply::readyRead, [reply, file]()
    {
        file->write(reply->readAll());
    });
}
