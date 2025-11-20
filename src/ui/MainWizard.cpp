#include "MainWizard.h"

#include <QApplication>
#include <QMessageBox>

#include "SyncPage.h"
#include "ExistingInstancePage.h"
#include "FinishingUpPage.h"
#include "WelcomePage.h"
#include "../Initialise.h"

MainWizard::MainWizard(SyncClient* syncer) : syncer(*syncer)
{

    /*connect(&syncer, &SyncClient::fetchError, this, [](const QString& msg)
    {
        QMessageBox::critical(nullptr, "Error!", msg);
        QApplication::exit();
    });*/

    setOptions(QWizard::DisabledBackButtonOnLastPage | QWizard::NoCancelButtonOnLastPage);

    if (Initialise::isInstallDirExist())
    {
        QWizardPage* existingInstance = new ExistingInstancePage();
        addPage(existingInstance);
    } else
    {
        QWizardPage* welcome = new WelcomePage();
        addPage(welcome);
    }
    QWizardPage* sync = new SyncPage(syncer);
    addPage(sync);
    QWizardPage* finishing = new FinishingUpPage(syncer);
    addPage(finishing);

    setWindowTitle("Modsync");
}


