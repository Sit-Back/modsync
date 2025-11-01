#pragma once
#include <QDir>
#include <QNetworkAccessManager>

class SyncClient final : public QObject
{
    Q_OBJECT

public:
    //Structs
    struct SyncMetadata
    {
        QString loaderID;
        QString loaderCMD;
    };

    //Constants
#ifdef Q_OS_WIN
    inline static const auto MINECRAFTDIR = QDir(QDir::homePath() + "/AppData/Roaming/.minecraft");
#else
    inline static const auto MINECRAFTDIR = QDir(QDir::homePath() + "/.minecraft");
#endif

    inline static const auto PROFILEDIR = QDir(MINECRAFTDIR.path() + "/modsyncprofile");
    inline static const auto MODSDIR = QDir(PROFILEDIR.path() + "/mods");

    inline static const QUrl ROOTURL = []
    {
        QUrl tmp(SERVERURL);
        tmp.setPort(5000);
        return tmp;
    }();

    //Constructor
    explicit SyncClient();

    //Profile Directory Operations
    static bool createProfileDir();
    static bool installDirExists();
    static bool minecraftDirExists();
    static bool removeInstallDir();
    [[nodiscard]] bool addProfile() const;
    void removeExtras() const;

    //Loader
    static bool javaInstalled();
    static bool versionExists(QString versionName);
    void downloadLoader() const;
    void installLoader() const;

    //Server Query Related
    void prepSync();
    [[nodiscard]] SyncMetadata getMetadata() const;
    [[nodiscard]] std::vector<QString> getModDownload() const;

private:
    void calcSyncDiffs(std::vector<QString> mods);
    QNetworkAccessManager* networkManager;

    // From Server
    bool needToSync = true;
    QString loaderID;
    QString loaderCMD;
    std::vector<QString> modNamesDownload;
    std::vector<QString> modNamesRemove;

signals:
    void prepFinished() const;
    void loaderDownloadFinished() const;
    void fetchError(const QString& msg) const;
};
