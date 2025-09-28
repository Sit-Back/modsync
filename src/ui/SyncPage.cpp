#include "SyncPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QProgressBar>

SyncPage::SyncPage(SyncClient& syncer, QWidget *parent)
    : QWizardPage(parent), syncer(syncer)
{
    setTitle("Welcome to Modsync!");
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
    std::vector<QUrl> modUrls;
    for (const QString& modName : syncer.getModDownload())
    {
        QUrl modUrl = SyncClient::ROOTURL;
        modUrl.setPath("/mods/" + modName);
        modUrls.push_back(modUrl);
    }

    SyncClient::createProfileDir();
    sync(modUrls);
}

bool SyncPage::isComplete() const
{
    return downloadsComplete;
}


void SyncPage::sync(std::vector<QUrl> urls)
{
    auto* downloader = new Downloader(
        std::filesystem::path(syncer.INSTALLDIR.toStdString()), urls);
    downloadProgressBar->setMaximum(static_cast<int>(downloader->getDownloadsTotal()));
    connect(downloader, &Downloader::downloadFinished, this, [this, downloader]() {
        downloadProgressBar->setValue(static_cast<int>(downloader->getDownloadsFinished()));

        if (downloader->getDownloadsFinished() >= downloader->getDownloadsTotal())
        {
            downloadsComplete = true;
            emit completeChanged();
        }
    });
}
