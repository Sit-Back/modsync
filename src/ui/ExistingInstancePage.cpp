#include "ExistingInstancePage.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QDesktopServices>
#include <QMessageBox>

#include "MainWizard.h"
#include "SyncPage.h"
#include "../Initialise.h"
#include "../Locations.h"
#include <QGroupBox>

ExistingInstancePage::ExistingInstancePage(SyncClient* syncer, QWidget* parent)
    : syncer(syncer)
{
    setTitle("Existing Instance Found!");
    auto* layout = new QVBoxLayout();
    setLayout(layout);
    setCommitPage(true);
    setButtonText(QWizard::WizardButton::CommitButton, "Sync >");

    auto* welcome = new QLabel("It seems you already have an instance of Modsync installed!"
        " If you with to update, continue to the next page."
        "<br><br>If you want to <b>add custom mods</b> that are excluded from syncing,"
        " browser the profile using the button below and "
        " add a '!' to the start of the file names of custom mods (e.g. !test.jar).");
    welcome->setWordWrap(true);

    auto* actionBar = new QGroupBox("Instance Tools");
    auto* actionBarLayout = new QGridLayout(actionBar);
    actionBar->setLayout(actionBarLayout);

    removeButton = new QPushButton("Remove");
    connect(removeButton, &QPushButton::pressed, this, [this]()
    {
        auto removeProfileWarning = new QMessageBox();
        removeProfileWarning->setText("Are you sure you want to remove the current profile?");
        removeProfileWarning->setInformativeText("This will remove all client data including binds,"
                                                 " map data and video settings!");
        removeProfileWarning->setIcon(QMessageBox::Warning);
        removeProfileWarning->setStandardButtons(QMessageBox::Apply);
        removeProfileWarning->show();
        connect(removeProfileWarning, &QMessageBox::buttonClicked, [removeProfileWarning]()
        {
            removeProfileWarning->hide();
            Initialise::removeInstallDir();
            LoaderInstaller::removeProfile();
            QMessageBox::information(nullptr, "Removed Profile", "Finished removing profile.");
            QApplication::quit();
        });
    });

    auto* browseButton = new QPushButton("Browse...");
    connect(browseButton, &QPushButton::pressed, this, []()
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(PROFILEDIR.path()));
    });

    actionBarLayout->addWidget(removeButton);
    actionBarLayout->addWidget(browseButton);

    layout->addWidget(welcome);
    layout->addWidget(actionBar);
}

int ExistingInstancePage::nextId() const
{
    if (syncer->getStepNum() == 0)
    {
        return 2;
    }
    else
    {
        return QWizardPage::nextId();
    }
}

bool ExistingInstancePage::validatePage()
{
    if (syncer->getStepNum() == 0)
    {
        syncer->startSync();
        return true;
    }
    return true;
}
