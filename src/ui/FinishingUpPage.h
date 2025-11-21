#pragma once

#include <QPushButton>
#include <QWizardPage>
#include "../SyncClient.h"


class SyncClient;

class FinishingUpPage : public QWizardPage
{
public:
    explicit FinishingUpPage();
};