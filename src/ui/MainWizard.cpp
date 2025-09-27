#include "MainWizard.h"

#include <QLabel>
#include <QVBoxLayout>
#include "SyncPage.h"
#include "WelcomePage.h"
#include <qcoreapplication.h>
#include <QMessageBox>

MainWizard::MainWizard()
{
    syncer.fetchMetadata();
    connect(&syncer, &SyncClient::fetchError, this, [](const QString& msg)
    {
        QMessageBox::critical(nullptr,"Error!", msg);
        QCoreApplication::exit();
    });
    QWizardPage* welcome = new WelcomePage(syncer);
    QWizardPage* sync = new SyncPage(syncer);
    setOptions(QWizard::NoBackButtonOnLastPage | QWizard::NoCancelButtonOnLastPage);

    addPage(welcome);
    addPage(sync);
    addPage(createConclusionPage());
    setWindowTitle("Modsync");
    show();
}

QWizardPage* MainWizard::createConclusionPage()
{
    auto* page = new QWizardPage();
    page->setTitle("Done");

    auto* layout = new QVBoxLayout();
    page->setLayout(layout);

    auto* usage = new QLabel("Modloader should be installed along with mods. Start the minecraft launcher to play.");
    usage->setWordWrap(true);
    usage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    layout->addWidget(usage);

    return page;
}

