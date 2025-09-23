#pragma once

#include <QMainWindow>
#include <QProgressBar>
#include <QWizardPage>
#include "../SyncClient.h"

class SyncPage final : public QWizardPage
{
    Q_OBJECT

public:
    explicit SyncPage(QWidget *parent = nullptr);
private:
    SyncClient syncClient;
    void initializePage() override;
    bool isComplete() const override;
    QProgressBar* downloadProgressBar;
    void sync();
};

