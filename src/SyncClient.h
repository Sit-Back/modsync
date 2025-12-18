#pragma once
#include "FileSyncer.h"
#include "LoaderInstaller.h"
#include "SyncAction.h"
#include "SyncMetadata.h"

class SyncClient final : public SyncAction
{
public:
    SyncClient(LoaderInstaller* loaderInstaller, FileSyncer* fileSyncer);
    void startAction() override;;
    [[nodiscard]] int getStepNumber() const override;

private:
    LoaderInstaller* loaderInstaller;
    FileSyncer* fileSyncer;
    int stepNum{};

    void calcStepNum();
};
