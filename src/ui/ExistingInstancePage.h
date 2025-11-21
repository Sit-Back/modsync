#pragma once

#include <QMainWindow>
#include <QWizardPage>

class SyncClient;

class ExistingInstancePage final : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExistingInstancePage(SyncClient* syncer,QWidget* parent = nullptr);
    int nextId() const override;
    bool validatePage() override;

private:
    QPushButton* removeButton;
    SyncClient* syncer;
};
