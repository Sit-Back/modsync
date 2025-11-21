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

    auto watcher = new QFutureWatcher<SyncMetadata>;
    QObject::connect(watcher, &QFutureWatcher<SyncMetadata>::finished, [watcher]()
    {
        try
        {
            SyncMetadata metadataresult = watcher->future().result();
            SyncClient* syncer = Initialise::createSyncPackage(metadataresult);
            auto wizard = new MainWizard(syncer);
            wizard->show();
        }
        catch (const MetadataFetchError &e)
        {
            QMessageBox::critical(nullptr, "Error Fetching Sync Metadata",e.what());
            QApplication::quit();
        }
    });

    QFuture<SyncMetadata> metatdata = Initialise::fetchSyncMetadata();
    watcher->setFuture(metatdata);
    return a.exec();
}
