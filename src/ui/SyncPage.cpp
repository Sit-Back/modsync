#include "SyncPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QProgressBar>

SyncPage::SyncPage(SyncClient& syncer, QWidget* parent)
    : QWizardPage(parent), syncer(syncer)
{
    setTitle("Syncing...");

    auto* layout = new QVBoxLayout();
    setLayout(layout);

    auto* downloadingLabel = new QLabel("Starting to sync, progress to the next"
        " page once syncing has completed to complete install if you haven't already");
    downloadingLabel->setWordWrap(true);
    layout->addWidget(downloadingLabel);
    downloadProgressBar = new QProgressBar();
    layout->addWidget(downloadProgressBar);
}

void SyncPage::initializePage()
{
    // Create list of mod URLs to pass to sync function
    std::vector<QUrl> modUrls;
    for (const QString& modName : syncer.getModDownload())
    {
        QUrl modUrl = SyncClient::ROOTURL;
        modUrl.setPath("/mods/" + modName);
        modUrls.push_back(modUrl);
    }

    //Create profile directory before syncing.
    SyncClient::createProfileDir();
    sync(modUrls);
}

bool SyncPage::isComplete() const
{
    return downloadsComplete;
}


void SyncPage::sync(std::vector<QUrl> urls)
{
    //1. Remove extra mods that arn't on server.
    syncer.removeExtras();

    //2. Check if loader installed
    //3. then download and install if needed.
    //4. Download mods

    if (!urls.empty())
    {
        auto* downloader = new Downloader(SyncClient::MODSDIR.path(), urls);

        downloadProgressBar->setMaximum(static_cast<int>(downloader->getDownloadsTotal()));

        connect(downloader, &Downloader::downloadFinished, this, [this, downloader]()
        {
            downloadProgressBar->setValue(static_cast<int>(downloader->getDownloadsFinished()));

            if (downloader->getDownloadsFinished() >= downloader->getDownloadsTotal())
            {
                downloadsComplete = true;
                emit completeChanged();
            }
        });
    }
    else
    {
        downloadsComplete = true;
    }

    if (!SyncClient::versionExists(syncer.getMetadata().loaderID))
    {
        //log if version exists
        syncer.downloadLoader();
        downloadProgressBar->setMaximum(downloadProgressBar->maximum()+1);

        connect(&syncer, &SyncClient::loaderDownloadFinished, this, [this]()
        {
            downloadProgressBar->setValue(static_cast<int>(downloadProgressBar->value() + 1));
            loaderDownloadComplete = true;
            emit completeChanged();
        });
    } else
    {
        loaderDownloadComplete = true;
        emit completeChanged();
    }

    //5. Add profile to launcher
    bool profileAddSuccess = syncer.addProfile();
    if (!profileAddSuccess)
    {
        QMessageBox::critical(nullptr, "Launcher Profile Add Failed",
                              "Could not add launcher profile! Ensure that the file"
                              " exists, is both readable and writable and is not corrupt.");
    }
}