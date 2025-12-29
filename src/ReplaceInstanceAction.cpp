//
// Created by mitch on 29/12/25.
//

#include "ReplaceInstanceAction.h"

#include <utility>

#include "Initialise.h"

ReplaceInstanceAction::ReplaceInstanceAction(SyncMetadata metadata) :
    CreateInstanceAction(std::move(metadata))
{}

void ReplaceInstanceAction::startAction()
{
    Initialise::removeInstallDir();
    LoaderInstaller::removeProfile();
    CreateInstanceAction::startAction();
}