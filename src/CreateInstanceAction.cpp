#include "CreateInstanceAction.h"
#include <QNetworkReply>
#include <QMessageBox>
#include <QStandardPaths>
#include <QProcess>
#include "LoaderInstaller.h"


CreateInstanceAction::CreateInstanceAction(LoaderInstaller* loaderInstaller, FileSyncer* fileSyncer) :
    loaderInstaller(loaderInstaller), fileSyncer(fileSyncer)
{}

void CreateInstanceAction::startAction()
{
    fileSyncer->downloadMods();

    connect(fileSyncer, &FileSyncer::modDownloaded, this, [this]()
    {
        emit finishStep();
    });

    if (loaderInstaller->loaderVersionExists())
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

int CreateInstanceAction::getStepNumber() const
{
    return fileSyncer->modsToDownloadCount() + 2;
}
