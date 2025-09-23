#include "WelcomePage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QDir>
#include <QPushButton>
#include <iostream>
#include <QDesktopServices>

#include "MainWizard.h"

WelcomePage::WelcomePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Welcome to Modsync!");
    auto* layout = new QVBoxLayout();
    setLayout(layout);

    const auto installDir = QDir(QDir::homePath() + "/.minecraft/modsyncprofile");
    std::cout << installDir.filesystemAbsolutePath().string() << '\n';

    if (!installDir.exists())
    {
        auto* welcome = new QLabel("Modsync is a platform distributed by your server owner"
                             " to steamline the <b>install and updating</b> of mods."
                             " Having trouble connecting? <b>Try re-running Modsync.</b>");
        welcome->setWordWrap(true);
        welcome->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        layout->addWidget(welcome);
    } else
    {
        auto* welcome = new QLabel("It seems you already have an instance of Modsync installed."
                                 " Pressing next will update your mods.");
        welcome->setWordWrap(true);
        welcome->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        auto* removeButton = new QPushButton("Remove Current Instance");
        connect(removeButton, &QPushButton::pressed, this, []()
        {
            QDir dir(QDir::homePath() + "/.minecraft/modsyncprofile");
            dir.removeRecursively();
        });

        auto* browseButton = new QPushButton("Browse...");
        connect(browseButton, &QPushButton::pressed, this, []()
        {
            QString dir(QDir::homePath() + "/.minecraft/modsyncprofile");
            QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
        });

        layout->addWidget(welcome);
        layout->addWidget(removeButton);
        layout->addWidget(browseButton);
    }


}
