#include "MainWizard.h"

#include <QLabel>
#include <QVBoxLayout>
#include "SyncPage.h"
#include "WelcomePage.h"

MainWizard::MainWizard()
{
    QWizardPage* welcome = new WelcomePage();
    QWizardPage* sync = new SyncPage();
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

