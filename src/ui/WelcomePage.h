#pragma once
#include <QWizardPage>
#include "../SyncClient.h"


class WelcomePage : public QWizardPage
{
    Q_OBJECT
public:
    bool isComplete() const override;
    WelcomePage(SyncClient& syncer, QWidget* parent = nullptr);
private:
    bool fetchingFinished = false;
};
