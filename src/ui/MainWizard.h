#pragma once
#include <QWizard>

#include "../SyncClient.h"

class MainWizard final : public QWizard
{
public:
    MainWizard(SyncAction* syncer);

private:
    SyncAction* syncer;
};