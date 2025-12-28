//
// Created by mitch on 28/12/25.
//
#pragma once
#include <QGroupBox>
#include <QPushButton>
#include <qwidget.h>

#include "SyncMetadata.h"


class InstanceTools : public QWidget
{
public:
    explicit InstanceTools(const SyncMetadata& metadata, QWidget* parent = nullptr);

private:
    void update();
    QPushButton* createUpdateButton();
    QGroupBox* createToolGroup();
    QWidget* createUpdateGroup();
    SyncMetadata metadata;

};
