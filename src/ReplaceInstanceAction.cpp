//
// Created by mitch on 29/12/25.
//

#include "ReplaceInstanceAction.h"

#include <utility>

#include "Initialise.h"
#include "InstanceTools.h"

ReplaceInstanceAction::ReplaceInstanceAction(SyncMetadata metadata) :
    CreateInstanceAction(std::move(metadata))
{}

void ReplaceInstanceAction::startAction()
{
    InstanceTools::removeInstallDir();
    InstanceTools::removeProfile();
    CreateInstanceAction::startAction();
}