#pragma once
#include <QDir>
#include <QNetworkAccessManager>
#include <QObject>

class Downloader : public QObject
{
    Q_OBJECT

public:
    Downloader(QDir path, const std::vector<QUrl>& urls);
    [[nodiscard]] unsigned long getDownloadsTotal() const;
    [[nodiscard]] unsigned long getDownloadsFinished() const;

private:
    void download(const QUrl& url);
    const QDir downloadPath;
    QNetworkAccessManager* manager;
    int finishedDownloads;
    unsigned long downloadTotal;

    signals :
    
    void downloadFinished();
};