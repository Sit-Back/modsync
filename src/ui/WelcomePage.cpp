#include "WelcomePage.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QDesktopServices>
#include <QMessageBox>

#include "MainWizard.h"
#include "SyncPage.h"

bool WelcomePage::isComplete() const
{
    return fetchingFinished;
}

WelcomePage::WelcomePage(SyncClient& syncer, QWidget *parent)
    : QWizardPage(parent)
{
    connect(&syncer, &SyncClient::prepFinished, this, [this]()
    {
        fetchingFinished = true;
        emit completeChanged();
    });

    setTitle("Welcome to Modsync!");
    auto* layout = new QVBoxLayout();
    setLayout(layout);

    if (!SyncClient::installDirExists())
    {
        auto* welcome = new QLabel("Modsync is a platform distributed by your server owner"
                             " to streamline the installation and updating of mods."
                             " <br><br><b>Having trouble connecting to the Minecraft server?</b>"
                             " Try re-running Modsync.");
        welcome->setWordWrap(true);

        layout->addWidget(welcome);
    } else
    {
                setButtonText(QWizard::WizardButton::NextButton, "Update >");
        auto* welcome = new QLabel("It seems you already have an instance of Modsync installed!"
                                 " If you with to update, continue to the next page."
                                 "<br><br>If you want to <b>add custom mods</b> that are excluded from syncing,"
                                 " browser the profile using the button below and "
                                 " add a '!' to the start of the file names of custom mods (e.g. !test.jar).");
        welcome->setWordWrap(true);

        auto* actionBar = new QWidget;
        auto* actionBarLayout = new QHBoxLayout;
        actionBar->setLayout(actionBarLayout);

        removeButton = new QPushButton("Remove Current Instance");
        connect(removeButton, &QPushButton::pressed, this, [&syncer, this]()
        {
            SyncClient::removeInstallDir();
            fetchingFinished = false;
            emit completeChanged();
            syncer.prepSync();
            QMessageBox::information(nullptr, "Removed Profile", "Finished removing profile.");
            removeButton->setDisabled(true);
        });

        auto* browseButton = new QPushButton("Browse...");
        connect(browseButton, &QPushButton::pressed, this, []()
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(SyncClient::PROFILEDIR.path()));
        });

        actionBarLayout->addWidget(removeButton);
        actionBarLayout->addWidget(browseButton);

        layout->addWidget(welcome);
        layout->addWidget(actionBar);
    }
}