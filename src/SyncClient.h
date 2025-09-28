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
    inline static const auto INSTALLDIR = QString(QDir::homePath() + "/.minecraft/modsyncprofile");
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

    //Server Query Related
    void prepSync();
    [[nodiscard]] SyncMetadata getMetadata() const;
    [[nodiscard]] std::vector<QString> getModDownload() const;
    [[nodiscard]] std::vector<QString> getModRemove() const;

private:
    QNetworkAccessManager *networkManager;

    // From Server
    QString loaderID;
    QString loaderURL;
    QString loaderName;
    std::vector<QString> modnamestotal;
    std::vector<QString> modnamesdownload;
    std::vector<QString> modnamesremove;

signals:
    void prepFinished();
    void fetchError(const QString& msg);
};
