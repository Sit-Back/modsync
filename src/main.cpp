#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QFuture>
#include "Initialise.h"
#include "ui/MainWizard.h"
#include "Initialise.h"
#include <QFutureWatcher>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

#ifdef _WIN32
    QApplication::setStyle("windowsvista");
#endif

    if (!Initialise::isMinecraftDirExist())
    {
        QMessageBox::critical(nullptr, "Invalid Launcher",
                              "Could not find official Minecraft"
                              " Launcher! Please install and use it.");
        return 1;
    }

    if (!Initialise::isJavaInstalled())
    {
        QMessageBox::critical(nullptr, "No Java", "No Java was found on your system,"
                              " install at least Java 21.");
        return 1;
    }

    auto watcher = new QFutureWatcher<SyncClient*>;
    QObject::connect(watcher, &QFutureWatcher<SyncClient*>::finished, [watcher]()
    {
        try
        {
            SyncClient* syncer = watcher->future().result();
            auto wizard = new MainWizard(syncer);
            wizard->show();
        }
        catch (const MetadataFetchError& e)
        {
            QMessageBox::critical(nullptr, "Error Fetching Sync Metadata", e.what());
            QApplication::quit();
        }
    });

    QFuture<SyncClient*> syncerFuture = Initialise::createSyncAction();
    watcher->setFuture(syncerFuture);

    return a.exec();
}
