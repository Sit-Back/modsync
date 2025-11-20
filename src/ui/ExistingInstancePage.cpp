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

ExistingInstancePage::ExistingInstancePage(QWidget* parent)
    : QWizardPage(parent)
{
    setTitle("Existing Instance Found!");
    auto* layout = new QVBoxLayout();
    setLayout(layout);

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
    connect(removeButton, &QPushButton::pressed, this, [this]()
    {
        //Initialise::removeInstallDir();
        QMessageBox::information(nullptr, "Removed Profile", "Finished removing profile.");
        removeButton->setDisabled(true);
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