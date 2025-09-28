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
    void fetchMetadata();
    void prepSync();
    [[nodiscard]] std::optional<SyncMetadata> getMetadata() const;
    [[nodiscard]] std::optional<std::vector<QString>> getModNames() const;

private:
    QNetworkAccessManager *networkManager;

    // From Server
    QString loaderID;
    QString loaderURL;
    QString loaderName;
    std::vector<QString> modnames;

signals:
    void fetchFinished();
    void fetchError(const QString& msg);
};
