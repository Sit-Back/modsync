#pragma once

#include <QPushButton>
#include <QWizardPage>
#include "../SyncClient.h"


class SyncClient;

class FinishingUpPage : public QWizardPage
{
public:
    explicit FinishingUpPage(SyncClient& syncer, QWidget* parent = nullptr);
    void initializePage() override;

private:
    SyncClient& syncer;
    QPushButton* addProfileButton;
};