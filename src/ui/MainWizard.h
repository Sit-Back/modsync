#pragma once
#include <QWizard>

#include "../SyncClient.h"

class MainWizard final : public QWizard
{
public:
    MainWizard(SyncClient& syncer);
private:
    SyncClient& syncer;
};
