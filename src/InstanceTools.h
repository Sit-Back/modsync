//
// Created by mitch on 29/12/25.
//

#ifndef MODSYNC_INSTANCETOOLS_H
#define MODSYNC_INSTANCETOOLS_H
#include <QString>

namespace InstanceTools
{
    bool removeInstallDir();
    void addProfile(QString loaderID);
    bool removeProfile();
    bool loaderVersionExists(QString loaderID);
}



#endif //MODSYNC_INSTANCETOOLS_H