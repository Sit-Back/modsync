#pragma once
#include <QWizardPage>
#include "../SyncClient.h"


class WelcomePage : public QWizardPage
{
    Q_OBJECT
public:
    WelcomePage(QWidget* parent = nullptr);
};
