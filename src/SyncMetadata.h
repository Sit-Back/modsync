//
// Created by mitch on 20/11/25.
//

#pragma once
#include <QString>
#include <QStringList>

//Structs
struct SyncMetadata
{
    QString loaderID;
    QString loaderCMD;
    QStringList modsToDownload;
    QStringList modsToRemove;
};