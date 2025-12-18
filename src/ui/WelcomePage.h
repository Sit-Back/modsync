#pragma once
#include <QWizardPage>
#include "../CreateInstanceAction.h"


class WelcomePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit WelcomePage(SyncAction* syncer, QWidget* parent = nullptr);
    int nextId() const override;
    bool validatePage() override;
private:
    SyncAction* syncer{};
};
