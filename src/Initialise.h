//
// Created by mitch on 20/11/25.
//
#pragma once
#include <QFuture>

#include "SyncClient.h"
#include "SyncMetadata.h"

class Initialise
{
    public:
    //Actions
    static bool createProfileDir();
    static bool removeInstallDir();

    //Checks
    static bool isJavaInstalled();
    static bool isInstallDirExist();
    static bool isMinecraftDirExist();

    static QFuture<SyncClient*> createSyncAction();
private:
    static QFuture<SyncMetadata> fetchSyncMetadata();
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
