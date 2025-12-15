#pragma once
#include "FileSyncer.h"
#include "LoaderInstaller.h"
#include "SyncMetadata.h"

class SyncClient final : public QObject
{
    Q_OBJECT

public:
    SyncClient(LoaderInstaller* loaderInstaller, FileSyncer* fileSyncer);
    void startSync();
    int getStepNum() const;

private:
    LoaderInstaller* loaderInstaller;
    FileSyncer* fileSyncer;

signals:
    void finishStep() const;
};
