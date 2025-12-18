#include "CreateInstanceAction.h"
#include <QNetworkReply>
#include <QMessageBox>
#include <QStandardPaths>
#include <QProcess>


CreateInstanceAction::CreateInstanceAction(LoaderInstaller* loaderInstaller, FileSyncer* fileSyncer) :
    loaderInstaller(loaderInstaller), fileSyncer(fileSyncer)
{
    calcStepNum();
}

void CreateInstanceAction::startAction()
{
    fileSyncer->downloadMods();

    connect(fileSyncer, &FileSyncer::modDownloaded, this, [this]()
    {
        emit finishStep();
    });

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

    loaderInstaller->addProfile();
}

int CreateInstanceAction::getStepNumber() const
{
    return stepNum;
}

void CreateInstanceAction::calcStepNum()
{
    stepNum = fileSyncer->modsToDownloadCount() + 2;
}
