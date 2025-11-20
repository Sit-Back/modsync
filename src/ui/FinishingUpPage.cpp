#include "FinishingUpPage.h"

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "../SyncClient.h"

FinishingUpPage::FinishingUpPage(QWidget* parent)
{
    setTitle("Done");
    auto* layout = new QVBoxLayout();
    setLayout(layout);
}

void FinishingUpPage::initializePage()
{
    auto* usage = new QLabel(
        "Sync has finished! Start the Minecraft launcher to play."
    );
    usage->setWordWrap(true);
    usage->setOpenExternalLinks(true);
    layout()->addWidget(usage);
}