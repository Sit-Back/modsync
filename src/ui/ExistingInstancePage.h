#pragma once

#include <QMainWindow>
#include <QWizardPage>

class SyncClient;

class ExistingInstancePage final : public QWizardPage
{
    Q_OBJECT

public:
    bool isComplete() const override;
    explicit ExistingInstancePage(SyncClient& syncer, QWidget* parent = nullptr);

private:
    bool fetchingFinished = false;
    QPushButton* removeButton;
};