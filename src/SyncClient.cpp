#include "SyncClient.h"
#include <QNetworkReply>
#include <QMessageBox>
#include <QDebug>
#include <qregularexpression.h>
#include <QStandardPaths>

#include "Downloader.h"
#include <QProcess>

/*
bool SyncClient::versionExists(QString versionName)
{
    QDir versionsPath(MINECRAFTDIR);
    versionsPath.cd("versions");
    QStringList versions = versionsPath.entryList(QDir::Dirs);
    return std::any_of(versions.begin(), versions.end(), [versionName](const QString& version)
    {
        return version == versionName;
    });
}

bool SyncClient::removeInstallDir()
{
    return QDir(PROFILEDIR).removeRecursively();
}

*/

SyncClient::SyncClient(LoaderInstaller* loaderInstaller, FileSyncer* fileSyncer) :
loaderInstaller(loaderInstaller), fileSyncer(fileSyncer)
{}

void SyncClient::startSync()
{
    fileSyncer->removeExtras();
    fileSyncer->downloadMods();

    connect(fileSyncer, &FileSyncer::modDownloaded, this, [this]()
    {
        emit finishStep();
    });
}

int SyncClient::getStepNum() const
{
    return fileSyncer->modsToDownloadCount();
}