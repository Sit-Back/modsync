//
// Created by mitch on 20/11/25.
//
#pragma once
#include <QFuture>

#include "CreateInstanceAction.h"
#include "SyncMetadata.h"

class Initialise
{
    public:
    //Actions
    static bool removeInstallDir();

    //Checks
    static bool isJavaInstalled();
    static bool isInstallDirExist();
    static bool isMinecraftDirExist();

    static QFuture<SyncAction*> createSyncAction();
    static QFuture<SyncMetadata> fetchSyncMetadata();
private:
    static QStringList getModsToDownload(const QStringList& mods);
    static QStringList getModsToRemove(const QStringList& mods);
    static bool isValidMetadata(QString loaderID, QString loaderCMD);
};

class MetadataFetchError : public std::runtime_error
{
    public:
    MetadataFetchError(const char* error) : runtime_error(error)
    {}
};
