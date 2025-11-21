//
// Created by mitch on 20/11/25.
//
#pragma once
#include <QObject>


class LoaderInstaller : public QObject
{
    Q_OBJECT

    public:
    LoaderInstaller(QString loaderID, QString loaderCMD);
    void downloadLoader() const;
    void installLoader() const;
    bool addProfile() const;
    bool loaderVersionExists() const;

private:
    QString loaderID;
    QString silentInstallCMD;

    signals:
    void loaderDownloadFinished() const;
    void loaderInstalled() const;
};
