#pragma once

#include <QStack>
#include <QQueue>
#include <QTimer>

#include "PipeModel.h"
#include "PipeEnums.h"

class PipeCommand {
public:
    PipeCommand() {}
    PipeCommand(Side side, bool clockwise)
    {
        this->side = side;
        this->clockwise = clockwise;
    }

    void reverse() {
        clockwise = !clockwise;
    }

    Side side;
    bool clockwise;
};

class PipeController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool processingCommands READ processingCommands NOTIFY processingCommandsChanged)

public:
    explicit PipeController(PipeModel *pipeModel, QObject *parent = nullptr);

    // Q_PROPERTY READ
    bool processingCommands() const;

signals:
    // Q_PROPERTY NOTIFY
    void processingCommandsChanged();

private slots:
    void onProcessCommands();

private:
    void queueCommand(const PipeCommand& pipeCommand);

private:
    PipeModel* m_pipeModel = nullptr;

    QStack<PipeCommand> m_allCommands;
    QQueue<PipeCommand> m_incomingCommands;
    QTimer* processCommands = new QTimer(this);
};
