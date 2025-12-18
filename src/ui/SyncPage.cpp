#include "SyncPage.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QProgressBar>

#include "../Initialise.h"

SyncPage::SyncPage(SyncAction* syncer, QWidget* parent)
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
    //Create profile directory before syncing.
    Initialise::createProfileDir();
    sync();
}

bool SyncPage::isComplete() const
{
    return progress >= syncer->getStepNumber();
}


void SyncPage::sync()
{
    downloadProgressBar->setMaximum(syncer->getStepNumber());
    downloadProgressBar->setValue(0);

    connect(syncer, &SyncClient::finishStep, this, [this]()
        {
            progress++;
            downloadProgressBar->setValue(progress);

            if (progress >= syncer->getStepNumber())
            {
                emit completeChanged();
            }
        });

    try
    {
        syncer->startAction();
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox::critical(nullptr, "Sync Error",e.what());
        QApplication::quit();
    }
}