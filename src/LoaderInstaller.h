//
// Created by mitch on 20/11/25.
//
#pragma once
#include <QObject>


class LoaderInstaller : public QObject
{
    Q_OBJECT

    public:
    LoaderInstaller(QString loaderID, QString silentInstallCMD);
    void downloadLoader() const;
    bool installLoader() const;
    bool addProfile() const;

private:
    QString loaderID;
    QString silentInstallCMD;

    signals:
    void loaderDownloadFinished() const;
    void loaderInstalled() const;
};
