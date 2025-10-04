#include <QApplication>
#include <QMessageBox>
#include <QDebug>

#include "ui/MainWizard.h"

int main(int argc, char* argv[])
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
    } else if (!SyncClient::javaInstalled())
    {
        QMessageBox::critical(nullptr, "No Java", "No Java was found on your system,"
                                                  " install at least Java 21.");
        return 1;
    }

    MainWizard wizard(syncer);
    wizard.show();
    return a.exec();
}
