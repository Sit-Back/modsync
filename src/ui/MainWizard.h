#pragma once
#include <QWizard>

#include "../SyncClient.h"

class MainWizard final : public QWizard
{
public:
    MainWizard();
private:
    static QWizardPage* createConclusionPage();
    SyncClient syncer;
};
