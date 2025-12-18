#include "Downloader.h"

#include <QNetworkReply>
#include "CreateInstanceAction.h"
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

    connect(reply, &QNetworkReply::finished, [this, reply, file]()
    {
        //Cleanup
        file->close();
        reply->deleteLater();
        emit downloadFinished();

    });
    connect(reply, &QNetworkReply::readyRead, [reply, file]()
    {
        file->write(reply->readAll());
    });
}
