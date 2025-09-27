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
    downloadingLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout->addWidget(downloadingLabel);
    downloadProgressBar = new QProgressBar();
    layout->addWidget(downloadProgressBar);
}

void SyncPage::initializePage()
{
    SyncClient::createProfileDir();
    sync();
}

bool SyncPage::isComplete() const
{
    return downloadsComplete;
}


void SyncPage::sync()
{
    auto* downloader = new Downloader(
        std::filesystem::path(syncer.INSTALLDIR.toStdString()),
        std::vector<QUrl>{
            QUrl("https://videos.pexels.com/video-files/1409899/1409899-uhd_3840_2160_25fps.mp4"),
            QUrl("https://videos.pexels.com/video-files/1409899/1409899-hd_1920_1080_25fps.mp4")
        }
        );
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
