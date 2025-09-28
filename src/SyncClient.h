#pragma once
#include <QDir>
#include <string>
#include <QNetworkAccessManager>

class SyncClient final : public QObject
{
    Q_OBJECT

public:
    struct SyncMetadata
    {
        QString loaderID;
        QString loaderURL;
        QString loaderName;
    };

    inline static const auto INSTALLDIR = QString(QDir::homePath() + "/.minecraft/modsyncprofile");

    inline static const QUrl ROOTURL = []
    {
        QUrl tmp("http://localhost");
        tmp.setPort(5000);
        return tmp;
    }();

    explicit SyncClient();
    static bool createProfileDir();
    static bool installDirExists();
    static bool removeInstallDir();
    void fetchMetadata();
    std::optional<SyncMetadata> getMetadata() const;
    std::optional<std::vector<QString>> getModNames() const;

private:
    QNetworkAccessManager *networkManager;
    QString loaderID;
    QString loaderURL;
    QString loaderName;
    std::vector<QString> modnames;

signals:
    void fetchedMetadata();
    void fetchError(const QString& msg);
};
