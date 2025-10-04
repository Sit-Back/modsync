#include "FinishingUpPage.h"

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "../SyncClient.h"

FinishingUpPage::FinishingUpPage(SyncClient& syncer, QWidget* parent) :
    syncer(syncer)
{
    setTitle("Done");
    auto* layout = new QVBoxLayout();
    setLayout(layout);
}

void FinishingUpPage::initializePage()
{
    SyncClient::SyncMetadata metadata = syncer.getMetadata();

    auto* usage = new QLabel(
        "Sync has finished! Start the Minecraft launcher to play."
    );
    usage->setWordWrap(true);
    usage->setOpenExternalLinks(true);
    layout()->addWidget(usage);
}