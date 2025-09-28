#include "FinishingUpPage.h"

#include <QLabel>
#include <QVBoxLayout>

#include "../SyncClient.h"

FinishingUpPage::FinishingUpPage(SyncClient& syncer, QWidget *parent) :
syncer(syncer)
{
    setTitle("Done");
    auto* layout = new QVBoxLayout();
    setLayout(layout);
}

void FinishingUpPage::initializePage()
{
    std::optional<SyncClient::SyncMetadata> metadata = syncer.getMetadata();

    if (metadata)
    {
        auto* usage = new QLabel(
            "Your server owner has recommended that you download"
            " the <b>" + metadata->loaderName + "</b>"
            " version available <a href=\"" + metadata->loaderURL + "\">here</a>."
            " If you haven't already, click the button below to add the profile"
            " to the Minecraft launcher (Ensure you download the mod loader before"
            " clicking the button)."
        );
        usage->setWordWrap(true);
        usage->setOpenExternalLinks(true);
        layout()->addWidget(usage);
    }
}


