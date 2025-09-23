#include "WelcomePage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QDir>
#include <QPushButton>
#include <QDesktopServices>
#include "MainWizard.h"
#include "SyncPage.h"

WelcomePage::WelcomePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Welcome to Modsync!");
    auto* layout = new QVBoxLayout();
    setLayout(layout);

    if (!SyncClient::installDirExists())
    {
        auto* welcome = new QLabel("Modsync is a platform distributed by your server owner"
                             " to steamline the <b>install and updating</b> of mods."
                             " Having trouble connecting? <b>Try re-running Modsync.</b>");
        welcome->setWordWrap(true);
        welcome->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        layout->addWidget(welcome);
    } else
    {
        setButtonText(QWizard::WizardButton::NextButton, "Update >");
        auto* welcome = new QLabel("It seems you <b>already have an instance</b> of Modsync installed."
                                 " Pressing next will update your mods.");
        welcome->setWordWrap(true);
        welcome->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        auto* actionBar = new QWidget;
        auto* actionBarLayout = new QHBoxLayout;
        actionBar->setLayout(actionBarLayout);

        auto* removeButton = new QPushButton("Remove Current Instance");
        connect(removeButton, &QPushButton::pressed, this, []()
        {
            SyncClient::removeInstallDir();
        });

        auto* browseButton = new QPushButton("Browse...");
        connect(browseButton, &QPushButton::pressed, this, []()
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(SyncClient::INSTALLDIR));
        });

        actionBarLayout->addWidget(removeButton);
        actionBarLayout->addWidget(browseButton);

        layout->addWidget(welcome);
        layout->addWidget(actionBar);
    }


}
