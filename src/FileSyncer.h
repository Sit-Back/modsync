//
// Created by mitch on 20/11/25.
//

#pragma once
#include <QStringList>

#include "Downloader.h"


class FileSyncer : public QObject
{
    Q_OBJECT

    public:
    FileSyncer(QStringList modsToRemove, QStringList modsToDownload);
    void removeExtras() const;
    void downloadMods();
    int modsToDownloadCount() const;
private:
    QStringList modsToRemove;
    QStringList modsToDownload;
    QNetworkAccessManager manager;

    void downloadMod(const QString& modName);

    signals:
    void modDownloaded() const;
};
