//
// Created by mitch on 20/11/25.
//

#include "LoaderInstaller.h"

#include <iostream>

#include "Downloader.h"
#include "Locations.h"
#include <QObject>
#include <QProcess>
#include <qregularexpression.h>

LoaderInstaller::LoaderInstaller(QString loaderID, QString loaderCMD) :
loaderID(std::move(loaderID)),
silentInstallCMD(std::move(loaderCMD))
{
    silentInstallCMD = silentInstallCMD.replace("%minecraft",
        MINECRAFTDIR.absolutePath());
    silentInstallCMD = silentInstallCMD.replace("%loader",
        QDir::toNativeSeparators(PROFILEDIR.filePath("loader.jar")));
}

void LoaderInstaller::downloadLoader() const
{
    auto* downloader = new Downloader();
    auto loaderURL = ROOTURL;
    loaderURL.setPath("/loader.jar");
    downloader->download(loaderURL, PROFILEDIR);

    connect(downloader, &Downloader::downloadFinished, this, [this]()
    {
       emit loaderDownloadFinished();
    });
}



void LoaderInstaller::installLoader() const
{
    QFile file(PROFILEDIR.filePath("loader.jar"));
    if (!file.exists())
    {
        throw std::runtime_error("Cannot find loader.jar");
    }

    auto *loaderInstallerProcess = new QProcess();
    loaderInstallerProcess->start("java", silentInstallCMD.split(" "));

    connect(loaderInstallerProcess, &QProcess::readyReadStandardOutput, this, [loaderInstallerProcess]()
    {
       std::cout << loaderInstallerProcess->readAllStandardOutput().toStdString();
    });

    connect(loaderInstallerProcess, &QProcess::readyReadStandardError, this, [loaderInstallerProcess]()
    {
       std::cerr << loaderInstallerProcess->readAllStandardError().toStdString();
    });

    connect(loaderInstallerProcess, &QProcess::finished, this, [this]()
    {
       emit loaderInstalled();
    });
}

