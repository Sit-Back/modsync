#pragma once
#include <QDir>
#include <string>
#include <QNetworkAccessManager>

class SyncClient final : public QObject
{
    Q_OBJECT

public:
    //Structs
    struct SyncMetadata
    {
        QString loaderID;
        QString loaderURL;
        QString loaderName;
    };

    //Constants
    inline static const auto MINECRAFTDIR = QDir(QDir::homePath() + "/.minecraft");
    inline static const auto PROFILEDIR = QDir(MINECRAFTDIR.path() + "/modsyncprofile");
    inline static const auto MODSDIR = QDir(PROFILEDIR.path() + "/mods");
    inline static const QUrl ROOTURL = []
    {
        QUrl tmp("http://localhost");
        tmp.setPort(5000);
        return tmp;
    }();

    //Constructor
    explicit SyncClient();

    //Profile Directory Operations
    static bool createProfileDir();
    static bool installDirExists();
    static bool removeInstallDir();
    void addProfile() const;
    void removeExtras() const;


    //Server Query Related
    void prepSync();
    [[nodiscard]] SyncMetadata getMetadata() const;
    [[nodiscard]] std::vector<QString> getModDownload() const;

private:
    void calcSyncDiffs(std::vector<QString> mods);
    QNetworkAccessManager *networkManager;

    // From Server
    bool needToSync = true;
    QString loaderID;
    QString loaderURL;
    QString loaderName;
    std::vector<QString> modnamesdownload;
    std::vector<QString> modnamesremove;

signals:
    void prepFinished();
    void fetchError(const QString& msg);
};
