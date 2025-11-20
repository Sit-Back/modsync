#pragma once

#include <QMainWindow>
#include <QWizardPage>

class SyncClient;

class ExistingInstancePage final : public QWizardPage
{
    Q_OBJECT

public:
    explicit ExistingInstancePage(QWidget* parent = nullptr);

private:
    QPushButton* removeButton;
};