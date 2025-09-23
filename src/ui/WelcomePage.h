#pragma once

#include <QMainWindow>
#include <QWizardPage>

class WelcomePage final : public QWizardPage
{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = nullptr);
};

