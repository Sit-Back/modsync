//
// Created by mitch on 28/12/25.
//

#include "InstanceTools.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QPushButton>

#include "Initialise.h"
#include "Locations.h"
#include <QDesktopServices>
#include <QLabel>
#include <QProgressDialog>
#include "UpdateModsAction.h"

InstanceTools::InstanceTools(const SyncMetadata& metadata, QWidget* parent) : metadata(metadata)
{
    setFixedSize(400, 250);
    auto* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    auto* updateGroup = createUpdateGroup();
    mainLayout->addWidget(updateGroup, 0, Qt::AlignCenter);

    auto* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(line);

    auto* toolGroup = createToolGroup();
    mainLayout->addWidget(toolGroup);
}

QWidget* InstanceTools::createUpdateGroup()
{
    auto* wrapper = new QWidget;
    auto* layout = new QVBoxLayout;
    wrapper->setLayout(layout);

    QPushButton* updateButton = createUpdateButton();
    layout->addWidget(updateButton,0, Qt::AlignCenter);

    auto* updateLabel = new QLabel;
    QFont font;
    font.setPointSizeF(font.pointSizeF() * 0.9);
    font.setItalic(true);
    updateLabel->setFont(font);
    layout->addWidget(updateLabel,0, Qt::AlignCenter);

    if (metadata.modsToDownload.empty() && metadata.modsToRemove.empty())
    {
        updateLabel->setText("Up to Date!");
        updateButton->setDisabled(true);
    } else
    {
        updateLabel->setText("Update availiable!");

        connect(updateButton, &QPushButton::pressed, this, [this]()
        {
            update();
        });
    }

    return wrapper;
}

void InstanceTools::update()
{
    auto action = new UpdateModsAction(metadata);
    QProgressDialog updateProgress("Updating...",
        nullptr,
        0,
        action->getStepNumber()-1);

    QObject::connect(action, &SyncAction::finishStep, [&updateProgress]()
    {
        updateProgress.setValue(updateProgress.value() + 1);
    });

    action->startAction();
    updateProgress.exec();


}

QPushButton* InstanceTools::createUpdateButton()
{
    auto* updateButton = new QPushButton("Update");
    QFont font;
    font.setPointSizeF(font.pointSizeF() * 1.2);
    font.setBold(true);
    updateButton->setFont(font);
    updateButton->setStyleSheet("padding: 10px 25px;");
    QSizePolicy policy = updateButton->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    updateButton->setSizePolicy(policy);
    return updateButton;
}

QGroupBox* InstanceTools::createToolGroup()
{
    auto* toolGroup = new QGroupBox("Tools");
    auto* toolGroupLayout = new QGridLayout;
    toolGroup->setLayout(toolGroupLayout);

    auto* removeButton = new QPushButton("Remove");
    connect(removeButton, &QPushButton::pressed, this, [this]()
    {
        auto removeProfileWarning = new QMessageBox();
        removeProfileWarning->setText("Are you sure you want to remove the current profile?");
        removeProfileWarning->setInformativeText("This will remove all client data including binds,"
                                                 " map data and video settings!");
        removeProfileWarning->setIcon(QMessageBox::Warning);
        removeProfileWarning->setStandardButtons(QMessageBox::Apply);
        removeProfileWarning->show();
        connect(removeProfileWarning, &QMessageBox::buttonClicked, [removeProfileWarning]()
        {
            removeProfileWarning->hide();
            Initialise::removeInstallDir();
            LoaderInstaller::removeProfile();
            QMessageBox::information(nullptr, "Removed Profile", "Finished removing profile.");
            QApplication::quit();
        });
    });

    auto* browseButton = new QPushButton("Browse...");
    connect(browseButton, &QPushButton::pressed, this, []()
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(PROFILEDIR.path()));
    });

    toolGroupLayout->addWidget(removeButton);
    toolGroupLayout->addWidget(browseButton);

    return toolGroup;
}
