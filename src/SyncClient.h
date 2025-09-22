#pragma once
#include <string>
#include <QNetworkAccessManager>

class SyncClient final : public QObject
{
    Q_OBJECT

public:
    explicit SyncClient();
    void download(const QUrl& url);
    int getDownloadsTotal() const;
    int getDownloadsFinished() const;

private:
    QNetworkAccessManager* manager;
    int downloadTotal = 2;
    int finishedDownloads = 0;

signals:
    void downloadFinished();
};
