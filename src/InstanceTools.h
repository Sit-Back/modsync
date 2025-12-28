//
// Created by mitch on 28/12/25.
//
#pragma once
#include <QGroupBox>
#include <QPushButton>
#include <qwidget.h>


class InstanceTools : public QWidget
{
public:
    explicit InstanceTools(QWidget* parent = nullptr);

private:
    QPushButton* createUpdateButton();
    QGroupBox* createToolGroup();
    QWidget* createUpdateGroup();
};
