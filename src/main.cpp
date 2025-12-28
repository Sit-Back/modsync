#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "Initialise.h"
#include <QFutureWatcher>
#include <QProgressDialog>
#include <QObject>
#include "InstanceTools.h"
#include <QPushButton>

void install(const SyncMetadata& metadata)
{
    auto createInstance = new CreateInstanceAction(metadata);
    QProgressDialog createProgress("Installing instance...",
        nullptr,
        0,
        createInstance->getStepNumber()-1);

    QObject::connect(createInstance, &CreateInstanceAction::finishStep, [&createProgress]()
    {
        createProgress.setValue(createProgress.value() + 1);
    });

    createInstance->startAction();
    createProgress.exec();
}

void initUI(const SyncMetadata& metadata)
{
    if (!Initialise::isInstallDirExist())
    {
        QMessageBox installPrompt;
        installPrompt.setText("No modsync instance was found."
                              " Would you like to create one?");
        installPrompt.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        installPrompt.setDefaultButton(QMessageBox::Yes);

        if (installPrompt.exec() == QMessageBox::Yes)
        {
            install(metadata);
            QMessageBox completePrompt;
            completePrompt.setText("Install complete. Launch the Minecraft Launcher to play.");
            completePrompt.addButton(QMessageBox::Ok);
            QAbstractButton *editButton = completePrompt.addButton("Edit Instance", QMessageBox::YesRole);
            completePrompt.setDefaultButton(QMessageBox::Ok);

            completePrompt.exec();
            if (completePrompt.clickedButton() == editButton)
            {
                auto* instanceToolsWindow = new InstanceTools(metadata);
                instanceToolsWindow->show();
            }
        }
    } else
    {
        auto* instanceToolsWindow = new InstanceTools(metadata);
        instanceToolsWindow->show();
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
