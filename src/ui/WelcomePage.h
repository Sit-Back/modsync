#pragma once
#include <QWizardPage>
#include "../SyncClient.h"


class WelcomePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit WelcomePage(SyncClient* syncer, QWidget* parent = nullptr);
    int nextId() const override;
    bool validatePage() override;
private:
    SyncClient* syncer{};
};
