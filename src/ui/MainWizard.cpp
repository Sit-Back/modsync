#include "MainWizard.h"

#include <QApplication>
#include <QMessageBox>

#include "SyncPage.h"
#include "WelcomePage.h"
#include "FinishingUpPage.h"

MainWizard::MainWizard(SyncClient& syncer) : syncer(syncer)
{
    syncer.prepSync();
    connect(&syncer, &SyncClient::fetchError, this, [](const QString& msg)
    {
        QMessageBox::critical(nullptr,"Error!", msg);
        QApplication::exit();
    });
    QWizardPage* welcome = new WelcomePage(syncer);
    QWizardPage* sync = new SyncPage(syncer);
    QWizardPage* finishing = new FinishingUpPage(syncer);
    setOptions(QWizard::NoBackButtonOnLastPage | QWizard::NoCancelButtonOnLastPage);

    addPage(welcome);
    addPage(sync);
    addPage(finishing);
    setWindowTitle("Modsync");
}


