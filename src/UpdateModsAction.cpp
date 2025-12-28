//
// Created by mitch on 18/12/25.
//

#include "UpdateModsAction.h"

int UpdateModsAction::getStepNumber() const
{
    return fileSyncer.modsToDownloadCount();
}

void UpdateModsAction::startAction()
{
    fileSyncer.downloadMods();

    connect(&fileSyncer, &FileSyncer::modDownloaded, this, [this]()
    {
        emit finishStep();
    });

    fileSyncer.removeExtras();
}

UpdateModsAction::UpdateModsAction(const SyncMetadata& metadata) : fileSyncer(metadata.modsToRemove, metadata.modsToDownload)
{}
