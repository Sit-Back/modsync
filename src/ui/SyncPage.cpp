#include "SyncPage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QProgressBar>

#include "../Initialise.h"

SyncPage::SyncPage(SyncClient* syncer, QWidget* parent)
    : QWizardPage(parent), syncer(*syncer)
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
    //Create profile directory before syncing.
    Initialise::createProfileDir();
    sync();
}

bool SyncPage::isComplete() const
{
    return downloadsComplete;
}


void SyncPage::sync()
{
        downloadProgressBar->setMaximum(syncer.getStepNum());

    connect(&syncer, &SyncClient::finishStep, this, [this]()
        {
            int currentValue = downloadProgressBar->value();
            downloadProgressBar->setValue(static_cast<int>(currentValue + 1));

            if (currentValue >= syncer.getStepNum())
            {
                downloadsComplete = true;
                emit completeChanged();
            }
        });

    /*if (!SyncClient::versionExists(syncer.getMetadata().loaderID))
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
    }*/
}