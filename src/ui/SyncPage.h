#pragma once

#include <QMainWindow>
#include <QProgressBar>
#include <QWizardPage>

#include "../Downloader.h"
#include "../CreateInstanceAction.h"

class SyncPage final : public QWizardPage
{
    Q_OBJECT

public:
    explicit SyncPage(SyncAction* syncer, QWidget* parent = nullptr);

private:
    int progress = 0;
    SyncAction* syncer;
    bool loaderDownloadComplete = false;
    void initializePage() override;
    bool isComplete() const override;
    QProgressBar* downloadProgressBar;
    void sync();
};