//
// Created by mitch on 18/12/25.
//

#pragma once
#include "FileSyncer.h"
#include "SyncAction.h"


class UpdateModsAction : public SyncAction
{
public:
    UpdateModsAction(FileSyncer* fileSyncer);
    [[nodiscard]] int getStepNumber() const override;
    void startAction() override;
private:
    FileSyncer* fileSyncer;
};
