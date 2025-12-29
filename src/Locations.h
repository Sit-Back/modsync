//
// Created by mitch on 20/11/25.
//

#pragma once

#include <QUrl>

//Constants
#ifdef Q_OS_WIN
  inline static const auto MINECRAFTDIR = QDir(QDir::homePath() + "/AppData/Roaming/.minecraft");
#else
inline static const auto MINECRAFTDIR = QDir(QDir::homePath() + "/.minecraft");
#endif

inline static const auto PROFILEDIR = QDir(MINECRAFTDIR.path() + "/modsyncprofile");
inline static const auto MODSDIR = QDir(PROFILEDIR.path() + "/mods");

inline static const QUrl ROOTURL = []
{
    QUrl tmp(SERVERURL);
    tmp.setPort(5000);
    return tmp;
}();