#include "SyncPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QProgressBar>

SyncPage::SyncPage(QWidget *parent)
    : QWizardPage(parent), syncClient()
{
    setTitle("Welcome to Modsync!");
    auto* layout = new QVBoxLayout();
    setLayout(layout);

    auto* downloadingLabel = new QLabel("Downloading...");
    downloadingLabel->setWordWrap(true);
    downloadingLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    downloadProgressBar = new QProgressBar();

    layout->addWidget(downloadingLabel);
    layout->addWidget(downloadProgressBar);
}

void SyncPage::initializePage()
{
    sync();
}

bool SyncPage::isComplete() const
{
    return syncClient.getDownloadsFinished() == syncClient.getDownloadsTotal();
}


void SyncPage::sync()
{
    downloadProgressBar->setMaximum(syncClient.getDownloadsTotal());
    connect(&syncClient, &SyncClient::downloadFinished, this, [this]() {
        downloadProgressBar->setValue(syncClient.getDownloadsFinished());
        emit completeChanged();
    });


    auto url3 = QUrl("https://videos.pexels.com/video-files/1409899/1409899-uhd_3840_2160_25fps.mp4");
    syncClient.download(url3);

    auto url2 = QUrl("https://videos.pexels.com/video-files/1409899/1409899-hd_1920_1080_25fps.mp4");
    syncClient.download(url2);

}
