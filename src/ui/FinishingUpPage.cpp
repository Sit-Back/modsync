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
        "Your server owner has recommended that you download"
        " the <b>" + metadata.loaderName + "</b>"
        " version available <a href=\"" + metadata.loaderURL + "\">here</a>."
        " If you haven't already, click the button below to add the profile"
        " to the Minecraft launcher (Ensure you download the mod loader before"
        " clicking the button)."
    );
    usage->setWordWrap(true);
    usage->setOpenExternalLinks(true);
    layout()->addWidget(usage);

    addProfileButton = new QPushButton("Add Profile To Launcher");
    layout()->addWidget(addProfileButton);

    connect(addProfileButton, &QPushButton::pressed, this, [this]()
    {
        bool result = syncer.addProfile();
        if (result)
        {
            QMessageBox::information(nullptr, "Added Launcher Profile",
                                     "Finished adding profile to launcher, you can now start the "
                                     "Minecraft Launcher to play!");
            addProfileButton->setDisabled(true);
        }
        else
        {
            QMessageBox::critical(nullptr, "Launcher Profile Add Failed",
                                  "Could not add launcher profile! Ensure that the file"
                                  " exists, is both readable and writable and is not corrupt.");
        }
    });
}