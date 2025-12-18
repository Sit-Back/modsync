//
// Created by mitch on 18/12/25.
//

#pragma once
#include <QObject>

class SyncAction : public QObject
{
Q_OBJECT

public:
    ~SyncAction() override = default;
    [[nodiscard]] virtual int getStepNumber() const = 0;
    virtual void startAction() = 0;
signals:
    void finishStep() const;
};
