#pragma once

#include <QMainWindow>
#include <QWizardPage>

#include "../SyncAction.h"

class SyncClient;

class ExistingInstancePage final : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExistingInstancePage(SyncAction* syncer,QWidget* parent = nullptr);
    int nextId() const override;
    bool validatePage() override;

private:
    QPushButton* removeButton;
    SyncAction* syncer;
};
