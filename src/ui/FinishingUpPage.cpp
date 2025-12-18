#include "FinishingUpPage.h"

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "../CreateInstanceAction.h"

FinishingUpPage::FinishingUpPage()
{
    setTitle("Done");
    auto* layout = new QVBoxLayout();
    setLayout(layout);

    auto* usage = new QLabel(
        "Sync has finished! Start the Minecraft launcher to play."
    );
    usage->setWordWrap(true);
    layout->addWidget(usage);
}