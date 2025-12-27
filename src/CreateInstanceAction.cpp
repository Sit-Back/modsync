#include "CreateInstanceAction.h"
#include <QNetworkReply>
#include <QMessageBox>
#include <QStandardPaths>
#include <QProcess>
#include "LoaderInstaller.h"


CreateInstanceAction::CreateInstanceAction(SyncMetadata metadata) :
    loaderInstaller(metadata.loaderID, metadata.loaderCMD),
    fileSyncer(metadata.modsToRemove, metadata.modsToDownload)
{}

void CreateInstanceAction::startAction()
{
    fileSyncer.downloadMods();

    connect(&fileSyncer, &FileSyncer::modDownloaded, this, [this]()
    {
        emit finishStep();
    });

    loaderInstaller.downloadLoader();

    connect(&loaderInstaller, &LoaderInstaller::loaderDownloadFinished, this, [this]()
    {
        emit finishStep();
        loaderInstaller.installLoader();

        connect(&loaderInstaller, &LoaderInstaller::loaderInstalled, this, [this]()
        {
            emit finishStep();
        });
    });


    loaderInstaller.addProfile();
}

int CreateInstanceAction::getStepNumber() const
{
    return fileSyncer.modsToDownloadCount() + 2;
}
