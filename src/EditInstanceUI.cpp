//
// Created by mitch on 28/12/25.
//

#include "EditInstanceUI.h"

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
#include <QFileDialog>

#include "InstanceTools.h"

EditInstanceUI::EditInstanceUI(const SyncMetadata& metadata, bool uptodate, QWidget* parent) :
metadata(metadata), uptodate(uptodate)
{
    setFixedSize(400, 300);
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

QWidget* EditInstanceUI::createUpdateGroup()
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

    if ((metadata.modsToDownload.empty() && metadata.modsToRemove.empty()) || uptodate)
    {
        updateLabel->setText("Up to Date!");
        updateButton->setDisabled(true);
    } else
    {
        updateLabel->setText("Update availiable!");

        connect(updateButton, &QPushButton::pressed, this, [this,updateButton, updateLabel]()
        {
            update(updateButton, updateLabel);
        });
    }

    return wrapper;
}

void EditInstanceUI::update(QPushButton* updateButton, QLabel* label)
{
    auto action = new UpdateModsAction(metadata);

    if (action->getStepNumber() > 0)
    {
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
    } else
    {
        action->startAction();
    }

    updateButton->setDisabled(true);
    label->setText("Finished!");
}

QPushButton* EditInstanceUI::createUpdateButton()
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

QGroupBox* EditInstanceUI::createToolGroup()
{
    auto* toolGroup = new QGroupBox("Tools");
    auto* toolGroupLayout = new QGridLayout;
    toolGroup->setLayout(toolGroupLayout);

    auto* removeButton = new QPushButton("Remove...");
    connect(removeButton, &QPushButton::pressed, this, [this]()
    {
        QMessageBox removeProfileWarning;
        removeProfileWarning.setText("Do you want to remove or replace your modsync instance?");
        removeProfileWarning.setInformativeText("This will remove all client data including binds,"
                                                 " map data and video settings!");
        removeProfileWarning.setIcon(QMessageBox::Warning);
        auto* replaceButton = removeProfileWarning.addButton("Replace", QMessageBox::ApplyRole);
        auto removeButton = removeProfileWarning.addButton("Remove", QMessageBox::ApplyRole);
        removeProfileWarning.exec();

        if (removeProfileWarning.clickedButton() == replaceButton)
        {

        } else if (removeProfileWarning.clickedButton() == removeButton)
        {
            InstanceTools::removeInstallDir();
            InstanceTools::removeProfile();
            QMessageBox::information(nullptr, "Removed Profile", "Finished removing profile.");
            QApplication::quit();
        }
    });

    auto* addModsButton = new QPushButton("Add Mods...");
    connect(addModsButton, &QPushButton::pressed, this, [this]()
    {
        auto modFileNames = QFileDialog::getOpenFileNames(this,
            "Select one or more mods to add",
            QDir::homePath(), "Java Archives  (*.jar)");

        for (QString filePath : modFileNames)
        {
            QString newFilePath = MODSDIR.absolutePath() + "/!" + QFileInfo(filePath).fileName();
            if (QFile::exists(newFilePath))
            {
                QMessageBox existsWarning(QMessageBox::Icon::NoIcon,"File already exists", QFileInfo(filePath).fileName() +
                    " already exists in the mods folder!");
                auto* replaceButton = existsWarning.addButton("Replace", QMessageBox::ApplyRole);
                existsWarning.addButton("Ignore", QMessageBox::AcceptRole);

                existsWarning.exec();

                if (existsWarning.clickedButton() == replaceButton)
                {
                    QFile::remove(newFilePath);
                    QFile::copy(filePath, newFilePath);
                }

            } else
            {
                QFile::copy(filePath, newFilePath);
            }

        }
    });

    auto* browseButton = new QPushButton("Browse...");
    connect(browseButton, &QPushButton::pressed, this, []()
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(PROFILEDIR.path()));
    });

    toolGroupLayout->addWidget(removeButton);
    toolGroupLayout->addWidget(addModsButton);
    toolGroupLayout->addWidget(browseButton);

    return toolGroup;
}
