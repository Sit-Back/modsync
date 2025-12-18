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
    calcStepNum();
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
    return stepNum;
}

void SyncClient::calcStepNum()
{
    if (loaderInstaller->loaderVersionExists())
    {
        stepNum = fileSyncer->modsToDownloadCount();
    } else
    {
        stepNum = fileSyncer->modsToDownloadCount() + 2;
    }
}
