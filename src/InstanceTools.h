//
// Created by mitch on 28/12/25.
//
#pragma once
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <qwidget.h>

#include "SyncMetadata.h"


class InstanceTools : public QWidget
{
public:
    explicit InstanceTools(const SyncMetadata& metadata, bool uptodate = false, QWidget* parent = nullptr);

private:
    void update(QPushButton* updateButton, QLabel* label);
    QPushButton* createUpdateButton();
    QGroupBox* createToolGroup();
    QWidget* createUpdateGroup();
    SyncMetadata metadata;
    bool uptodate;

};
