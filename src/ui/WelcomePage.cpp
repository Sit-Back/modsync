#include "WelcomePage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>



WelcomePage::WelcomePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle("Welcome to Modsync!");
    auto* layout = new QVBoxLayout();
    setLayout(layout);

    auto* usage = new QLabel("Select an existing server to update or create a new one through the dropdown.");
    usage->setWordWrap(true);
    usage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto* input = new QComboBox();
    input->addItem("Create new server entry…");

    layout->addWidget(usage);
    layout->addWidget(input);
}