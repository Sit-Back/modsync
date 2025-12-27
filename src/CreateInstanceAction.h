#pragma once
#include "FileSyncer.h"
#include "LoaderInstaller.h"
#include "SyncAction.h"
#include "SyncMetadata.h"

class CreateInstanceAction final : public SyncAction
{
public:
    CreateInstanceAction(SyncMetadata metadata);
    void startAction() override;;
    [[nodiscard]] int getStepNumber() const override;

private:
    bool createProfileDir();
    LoaderInstaller loaderInstaller;
    FileSyncer fileSyncer;
};
