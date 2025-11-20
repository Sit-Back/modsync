#pragma once

#include <QMainWindow>
#include <QProgressBar>
#include <QWizardPage>

#include "../Downloader.h"
#include "../SyncClient.h"

class SyncPage final : public QWizardPage
{
    Q_OBJECT

public:
    explicit SyncPage(SyncClient* syncer, QWidget* parent = nullptr);

private:
    int progress = 0;
    SyncClient* syncer;
    bool loaderDownloadComplete = false;
    void initializePage() override;
    bool isComplete() const override;
    QProgressBar* downloadProgressBar;
    void sync();
};