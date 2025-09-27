#pragma once
#include <QDir>
#include <QNetworkAccessManager>
#include <QObject>

class Downloader : public QObject
{
    Q_OBJECT

public:
    Downloader(std::filesystem::path  path, const std::vector<QUrl>& urls);
    [[nodiscard]] unsigned long getDownloadsTotal() const;
    [[nodiscard]] unsigned long getDownloadsFinished() const;

private:
    void download(const QUrl& url);
    const std::filesystem::path downloadPath;
    QNetworkAccessManager* manager;
    int finishedDownloads;
    unsigned long downloadTotal;

signals:
    void downloadFinished();
};