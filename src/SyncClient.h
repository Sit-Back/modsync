#pragma once
#include <QDir>
#include <QNetworkAccessManager>

#include "FileSyncer.h"
#include "LoaderInstaller.h"
#include "SyncMetadata.h"

class SyncClient final : public QObject
{
    Q_OBJECT

public:
    SyncClient(LoaderInstaller& loaderInstaller, FileSyncer& fileSyncer);
    void startSync();
    int getStepNum() const;
    int test() const;

private:
    LoaderInstaller& loaderInstaller;
    FileSyncer& fileSyncer;

signals:
    void finishStep() const;
};
