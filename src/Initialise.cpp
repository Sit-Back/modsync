//
// Created by mitch on 20/11/25.
//

#include "Initialise.h"

#include <QDir>
#include <QNetworkReply>
#include <QStandardPaths>
#include "Locations.h"
#include <QObject>
#include <QPromise>
#include <QSharedPointer>

bool Initialise::createProfileDir()
{
    return QDir().mkpath(MODSDIR.path());
}

bool Initialise::isInstallDirExist()
{
    return PROFILEDIR.exists();
}

bool Initialise::isMinecraftDirExist()
{
    return MINECRAFTDIR.exists();
}

bool Initialise::isJavaInstalled()
{
    const QString javaExe = QStandardPaths::findExecutable("java");
    return !javaExe.isEmpty();
}

QFuture<SyncMetadata> Initialise::fetchSyncMetadata()
{
    auto promise = QSharedPointer<QPromise<SyncMetadata>>::create();
    QFuture<SyncMetadata> future = promise->future();
    auto networkManager = new QNetworkAccessManager();
    QUrl metadataURL = ROOTURL;
    metadataURL.setPath("/meta");
    auto request = QNetworkRequest(metadataURL);

    QNetworkReply* reply = networkManager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, [reply, promise]()
    {
        promise->start();
        if (reply->error() == QNetworkReply::NoError)
        {
            QString loaderID = reply->readLine().trimmed();
            QString loaderCMD = reply->readLine().trimmed();

            if (loaderID.isEmpty() || loaderCMD.isEmpty())
            {
                //Create fancy error message here
            }

            QStringList mods;
            while (!reply->atEnd())
            {
                mods.emplace_back(reply->readLine().trimmed());
            }

            reply->deleteLater();
            QStringList modsToDownload = getModsToDownload(mods);
            QStringList modsToRemove = getModsToRemove(modsToDownload);
            promise->addResult(SyncMetadata{loaderID, loaderCMD, modsToDownload, modsToRemove});
            promise->finish();
        }
        else
        {
            //Throw custom error code and catch with fancy popup
        }
    });

    return future;
}

QStringList Initialise::getModsToDownload(const QStringList& mods)
{
    QStringList modNamesDownload;

    // Calc Download Needed
    for (const QString& filename : mods)
    {
        QFile file = QFile(MODSDIR.path() + "/" + filename);
        if (!file.exists())
        {
            modNamesDownload.push_back(filename);
        }
    }

    return modNamesDownload;
}

QStringList Initialise::getModsToRemove(const QStringList& mods)
{
    QStringList modNamesRemove = {};

    // Calc Remove Needed
    for (QString filename : MODSDIR.entryList(QDir::Files | QDir::NoDotAndDotDot))
    {
        if (filename.front() == QChar('!'))
        {
            qInfo() << "Mod omitted due to '!' tag: " << filename;
        } else {
            bool filenameFound = false;
            for (QString mod : mods)
            {
                if (mod == filename)
                {
                    filenameFound = true;
                    break;
                }
            }

            if (filenameFound == false)
            {
                modNamesRemove.push_back(filename);
            }
        }
    }

    return modNamesRemove;
}

SyncClient* Initialise::createSyncPackage(SyncMetadata metadata)
{
    auto loader = new LoaderInstaller(metadata.loaderID, metadata.loaderCMD);
    auto file = new FileSyncer(metadata.modsToRemove, metadata.modsToDownload);
    return new SyncClient(loader, file);
}
