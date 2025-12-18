#include "WelcomePage.h"

#include <QLabel>
#include <QVBoxLayout>

WelcomePage::WelcomePage(SyncClient* syncer, QWidget* parent) : syncer(syncer)
{

    setTitle("Welcome to Modsync!");
    auto* layout = new QVBoxLayout();
    setLayout(layout);
    setCommitPage(true);
    setButtonText(QWizard::WizardButton::CommitButton, "Create >");

    auto* welcome = new QLabel("Modsync is a platform distributed by your server owner"
            " to streamline the installation and updating of mods."
            " <br><br><b>Having trouble connecting to the Minecraft server?</b>"
            " Try re-running Modsync.");
    welcome->setWordWrap(true);

    layout->addWidget(welcome);
}

int WelcomePage::nextId() const
{
    if (syncer->getStepNum() == 0)
    {
        return 2;
    }

    return QWizardPage::nextId();
}

bool WelcomePage::validatePage()
{
    if (syncer->getStepNum() == 0)
    {
        syncer->startSync();
        return true;
    }
    return true;
}