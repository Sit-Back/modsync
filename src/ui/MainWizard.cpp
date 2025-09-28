#include "MainWizard.h"

#include <QLabel>
#include <QVBoxLayout>
#include "SyncPage.h"
#include "WelcomePage.h"
#include <qcoreapplication.h>
#include <QMessageBox>

#include "FinishingUpPage.h"

MainWizard::MainWizard()
{
    syncer.prepSync();
    connect(&syncer, &SyncClient::fetchError, this, [](const QString& msg)
    {
        QMessageBox::critical(nullptr,"Error!", msg);
        QCoreApplication::exit();
    });
    QWizardPage* welcome = new WelcomePage(syncer);
    QWizardPage* sync = new SyncPage(syncer);
    QWizardPage* finishing = new FinishingUpPage(syncer);
    setOptions(QWizard::NoBackButtonOnLastPage | QWizard::NoCancelButtonOnLastPage);

    addPage(welcome);
    addPage(sync);
    addPage(finishing);
    setWindowTitle("Modsync");
    show();
}


