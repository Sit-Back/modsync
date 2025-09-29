#pragma once

#include <QMainWindow>
#include <QWizardPage>

class SyncClient;

class WelcomePage final : public QWizardPage
{
    Q_OBJECT

bool fetchingFinished = false;

public:
    bool isComplete() const override;
    explicit WelcomePage(SyncClient& syncer, QWidget *parent = nullptr);

private:
    QPushButton* removeButton;
};

