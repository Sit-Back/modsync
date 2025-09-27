#pragma once
#include <QDir>
#include <string>
#include <QNetworkAccessManager>

class SyncClient final : public QObject
{
    Q_OBJECT

    struct syncMetadata
    {
        std::string loaderID;
        std::string loaderURL;
        std::string loaderName;
    };

public:
    inline static const QString INSTALLDIR = QString(QDir::homePath() + "/.minecraft/modsyncprofile");
    explicit SyncClient();
    static bool createProfileDir();
    static bool installDirExists();
    static bool removeInstallDir();

private:
    QNetworkAccessManager* networkManager;

signals:
    void fetchedMetadata();

};
