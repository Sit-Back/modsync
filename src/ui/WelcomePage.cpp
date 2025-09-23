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

    auto* usage = new QLabel("Modsync is a platform distributed by your server owner"
                             " to steamline the <b>install and updating</b> of mods."
                             " Having trouble connecting? <b>Try re-running Modsync.</b>");
    usage->setWordWrap(true);
    usage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    layout->addWidget(usage);
}