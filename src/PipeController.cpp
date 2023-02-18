#include "PipeController.h"

#include "PipeEnums.h"

PipeController::PipeController(PipeModel *pipeModel, QObject *parent)
    : QObject(parent), m_pipeModel(pipeModel)
{
    QObject::connect(processCommands, &QTimer::timeout, this, &PipeController::onProcessCommands);

    processCommands->start(100);
}

bool PipeController::processingCommands() const
{
    return !m_incomingCommands.isEmpty();
}

void PipeController::onProcessCommands()
{
    if (!m_incomingCommands.isEmpty()) {
        const PipeCommand& command = m_incomingCommands.dequeue();

//        m_pipeModel->rotatePipeSide(command.side, command.clockwise);

        emit processingCommandsChanged();
    }
}

void PipeController::queueCommand(const PipeCommand& pipeCommand)
{
    m_allCommands.push(pipeCommand);
    m_incomingCommands.enqueue(pipeCommand);
}
