#include "Downloader.h"

#include <QNetworkReply>
#include "SyncClient.h"
#include <QFile>

Downloader::Downloader()
{manager = new QNetworkAccessManager();}

void Downloader::download(const std::vector<QUrl>& urls, const QDir& path)
{
    for (auto url : urls)
    {
        download(url, path);
    }
}

void Downloader::download(const QUrl& url, const QDir& path)
{
    auto* request = new QNetworkRequest(url);
    QNetworkReply* reply = manager->get(*request);

    auto* file = new QFile(path.filePath(url.fileName()));
    if (!file->open(QIODevice::WriteOnly)) {throw std::runtime_error("Can't open file");}

    bool renamed = false;

    QObject::connect(reply, &QNetworkReply::finished, [reply, file]()
    {
        //Cleanup
        file->close();
        reply->deleteLater();

    });
    QObject::connect(reply, &QNetworkReply::readyRead, [reply, file]()
    {
        file->write(reply->readAll());
    });
}
