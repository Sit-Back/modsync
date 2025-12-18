//
// Created by mitch on 20/11/25.
//

#include "Initialise.h"

#include <QDir>
#include <QFutureWatcher>
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

bool Initialise::removeInstallDir()
{
    return QDir(PROFILEDIR).removeRecursively();
}

bool Initialise::isValidMetadata(QString loaderID, QString loaderCMD)
{
    if (!loaderCMD.isEmpty() && !loaderID.isEmpty())
    {
        return loaderID.slice(0, 3) == "LID" && loaderCMD.slice(0, 3) == "CMD";
    }

    return false;
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
        if (reply->error() != QNetworkReply::NoError)
        {
            auto exception = MetadataFetchError("Network error occured and failed to fetch metadata");
            promise->setException(std::make_exception_ptr(exception));
            promise->finish();
        }
        else
        {
            QString loaderID = reply->readLine().trimmed();
            QString loaderCMD = reply->readLine().trimmed();

            if (!isValidMetadata(loaderID, loaderCMD))
            {
                promise->setException(std::make_exception_ptr(MetadataFetchError("Malformed metadata")));
                promise->finish();
            } else
            {
                loaderID = loaderID.slice(3);
                loaderCMD = loaderCMD.slice(3);

                QStringList mods;
                while (!reply->atEnd())
                {
                    mods.emplace_back(reply->readLine().trimmed());
                }

                reply->deleteLater();
                QStringList modsToDownload = getModsToDownload(mods);
                QStringList modsToRemove = getModsToRemove(mods);
                promise->addResult(SyncMetadata{loaderID, loaderCMD, modsToDownload, modsToRemove});
                promise->finish();
            }
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
        }
        else
        {
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

QFuture<SyncClient*> Initialise::createSyncAction()
{
    auto promise = QSharedPointer<QPromise<SyncClient*>>::create();
    QFuture<SyncClient*> future = promise->future();

    auto watcher = new QFutureWatcher<SyncMetadata>;
    QObject::connect(watcher, &QFutureWatcher<SyncMetadata>::finished, [watcher, promise]()
    {
            SyncMetadata metadata = watcher->future().result();
            auto loader = new LoaderInstaller(metadata.loaderID, metadata.loaderCMD);
            auto file = new FileSyncer(metadata.modsToRemove, metadata.modsToDownload);
            promise->addResult(new SyncClient(loader, file));
            promise->finish();
    });

    QFuture<SyncMetadata> metatdata = Initialise::fetchSyncMetadata();
    watcher->setFuture(metatdata);

    return future;
}
