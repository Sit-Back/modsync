#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QFuture>
#include "Initialise.h"
#include "Initialise.h"
#include <QFutureWatcher>

void initUI(SyncMetadata metadata)
{
    if (Initialise::isInstallDirExist())
    {

        QMessageBox::critical(nullptr, "Exists", "Exists");
    } else
    {
        QMessageBox::critical(nullptr, "None", "None");
    }
}

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
            auto metadata = watcher->future().result();
            initUI(metadata);
        }
        catch (const MetadataFetchError& e)
        {
            QMessageBox::critical(nullptr, "Error Fetching Sync Metadata", e.what());
            QApplication::quit();
        }
    });

    QFuture<SyncMetadata> syncerFuture = Initialise::fetchSyncMetadata();
    watcher->setFuture(syncerFuture);

    return a.exec();
}
