#include "WelcomePage.h"

#include <QLabel>
#include <QVBoxLayout>

WelcomePage::WelcomePage(QWidget* parent)
{

    setTitle("Welcome to Modsync!");
    auto* layout = new QVBoxLayout();
    setLayout(layout);

    auto* welcome = new QLabel("Modsync is a platform distributed by your server owner"
            " to streamline the installation and updating of mods."
            " <br><br><b>Having trouble connecting to the Minecraft server?</b>"
            " Try re-running Modsync.");
    welcome->setWordWrap(true);

    layout->addWidget(welcome);
}
