#pragma once
#include <QDir>
#include <QNetworkAccessManager>
#include <QObject>

class Downloader : public QObject
{
    Q_OBJECT

public:
    Downloader();
    void download(const QUrl& url, const QDir& path);
    void download(const std::vector<QUrl>& urls, const QDir& path);

private:
    QNetworkAccessManager* manager;

    signals :
    void downloadFinished();
};