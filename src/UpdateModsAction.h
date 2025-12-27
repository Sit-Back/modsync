//
// Created by mitch on 18/12/25.
//

#pragma once
#include "FileSyncer.h"
#include "SyncAction.h"
#include "SyncMetadata.h"


class UpdateModsAction : public SyncAction
{
public:
    UpdateModsAction(SyncMetadata metadata);
    [[nodiscard]] int getStepNumber() const override;
    void startAction() override;
private:
    FileSyncer fileSyncer;
};
