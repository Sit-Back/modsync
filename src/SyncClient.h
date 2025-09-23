#pragma once
#include <QDir>
#include <string>
#include <QNetworkAccessManager>

class SyncClient final : public QObject
{
    Q_OBJECT

public:
    inline static const QString INSTALLDIR = QString(QDir::homePath() + "/.minecraft/modsyncprofile");
    explicit SyncClient();
    void download(const QUrl& url);
    static bool createProfileDir();
    static bool installDirExists();
    static bool removeInstallDir();
    int getDownloadsTotal() const;
    int getDownloadsFinished() const;

private:
    QNetworkAccessManager* manager;
    int downloadTotal = 2;
    int finishedDownloads = 0;

signals:
    void requestFinished();
};
