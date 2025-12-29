//
// Created by mitch on 29/12/25.
//

#pragma once
#include "CreateInstanceAction.h"


class ReplaceInstanceAction : public CreateInstanceAction
{
    public:
    ReplaceInstanceAction(SyncMetadata metadata);
    void startAction() override;
};
