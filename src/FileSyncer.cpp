//
// Created by mitch on 20/11/25.
//

#include "FileSyncer.h"

#include <QFile>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <utility>
#include "Locations.h"
#include <QDebug>

FileSyncer::FileSyncer(QStringList modsToRemove, QStringList modsToDownload) :
modsToRemove(modsToRemove), modsToDownload(modsToDownload)
{
}

void FileSyncer::downloadMods() const
{
    for (const QString& mod : modsToDownload.toVector())
    {
        downloadMod(mod);
    }

}

void FileSyncer::removeExtras() const
{
    for (const QString& file : modsToRemove)
    {
        QFile(MODSDIR.path() + "/" + file).remove();
    }
}

int FileSyncer::modsToDownloadCount() const
{
    auto vec = modsToDownload.toVector();
    return modsToDownload.count();
}

void FileSyncer::downloadMod(const QString& modName) const
{
    QUrl modUrl = ROOTURL;
    modUrl.setPath("mods/" + modName);
    auto* request = new QNetworkRequest();
    QNetworkReply* reply = manager->get(*request);

    auto* file = new QFile(MODSDIR.filePath(modUrl.fileName()));
    if (!file->open(QIODevice::WriteOnly)) {throw std::runtime_error("Can't open file");}

    bool renamed = false;

    QObject::connect(reply, &QNetworkReply::finished, [this, reply, file]()
    {
        //Cleanup
        file->close();
        reply->deleteLater();
        emit modDownloaded();

    });
    QObject::connect(reply, &QNetworkReply::readyRead, [reply, file]()
    {
        file->write(reply->readAll());
    });
}
