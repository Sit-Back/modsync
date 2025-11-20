#include "SyncClient.h"
#include <QNetworkReply>
#include <QMessageBox>
#include <QDebug>
#include <qregularexpression.h>
#include <QStandardPaths>

#include "Downloader.h"
#include <QProcess>


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