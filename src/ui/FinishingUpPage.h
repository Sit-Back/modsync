#pragma once

#include <QPushButton>
#include <QWizardPage>
#include "../SyncClient.h"


class SyncClient;

class FinishingUpPage : public QWizardPage
{
public:
    explicit FinishingUpPage(QWidget* parent = nullptr);
    void initializePage() override;

private:
    QPushButton* addProfileButton;
};