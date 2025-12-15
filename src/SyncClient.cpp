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
{
}

void SyncClient::startSync()
{
    fileSyncer->downloadMods();
    fileSyncer->removeExtras();


    connect(fileSyncer, &FileSyncer::modDownloaded, this, [this]()
    {
        emit finishStep();
    });

    if (!loaderInstaller->loaderVersionExists())
    {
        loaderInstaller->downloadLoader();

        connect(loaderInstaller, &LoaderInstaller::loaderDownloadFinished, this, [this]()
        {
            emit finishStep();
            loaderInstaller->installLoader();

            connect(loaderInstaller, &LoaderInstaller::loaderInstalled, this, [this]()
            {
                emit finishStep();
            });
        });

    }

    loaderInstaller->addProfile();
}

int SyncClient::getStepNum() const
{
    if (loaderInstaller->loaderVersionExists())
    {
        return fileSyncer->modsToDownloadCount();
    }
    return fileSyncer->modsToDownloadCount() + 2;
}
