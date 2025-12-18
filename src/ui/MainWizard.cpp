#include "MainWizard.h"

#include <QApplication>
#include <QMessageBox>

#include "SyncPage.h"
#include "ExistingInstancePage.h"
#include "FinishingUpPage.h"
#include "WelcomePage.h"
#include "../Initialise.h"

MainWizard::MainWizard(SyncAction* syncer) : syncer(syncer)
{
    setOptions(QWizard::NoCancelButtonOnLastPage);

    if (Initialise::isInstallDirExist())
    {
        QWizardPage* existingInstance = new ExistingInstancePage(syncer);
        addPage(existingInstance);
    } else
    {
        QWizardPage* welcome = new WelcomePage(syncer);
        addPage(welcome);
    }
    QWizardPage* sync = new SyncPage(syncer);
    addPage(sync);
    QWizardPage* finishing = new FinishingUpPage();
    addPage(finishing);

    setWindowTitle("Modsync");
}


