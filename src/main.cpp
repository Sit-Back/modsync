#include <QApplication>
#include <QMessageBox>

#include "ui/MainWizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SyncClient syncer;

#ifdef _WIN32
    QApplication::setStyle("windowsvista");
#endif

    if (!SyncClient::minecraftDirExists())
    {
        QMessageBox::critical(nullptr, "Invalid Launcher",
            "Could not find official Minecraft"
            " Launcher! Please install and use it.");
        return 1;
    }

    MainWizard wizard(syncer);
    wizard.show();
    return a.exec();
}
