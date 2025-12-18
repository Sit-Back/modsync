#pragma once
#include <QWizard>

#include "../CreateInstanceAction.h"

class MainWizard final : public QWizard
{
public:
    MainWizard(SyncAction* syncer);

private:
    SyncAction* syncer;
};