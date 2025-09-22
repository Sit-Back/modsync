#pragma once
#include <QWizard>

class MainWizard final : public QWizard
{
public:
    MainWizard();
private:
    static QWizardPage* createConclusionPage();
};
