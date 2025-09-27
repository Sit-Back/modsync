#include "SyncClient.h"
#include <QNetworkReply>
#include <QFile>

SyncClient::SyncClient()
{
    networkManager = new QNetworkAccessManager();
    downloadTotal = 2;
    finishedDownloads = 0;
}

bool SyncClient::createProfileDir()
{
    return QDir().mkpath(INSTALLDIR + "/mods");
}

bool SyncClient::installDirExists()
{
    return QDir(SyncClient::INSTALLDIR).exists();
}

bool SyncClient::removeInstallDir()
{
    return QDir(INSTALLDIR).removeRecursively();
}

