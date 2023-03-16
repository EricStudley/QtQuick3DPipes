#include "PipeController.h"

static auto constexpr MAX_PIPES = 1000;
static auto constexpr PIPE_UPDATE_TIME_MS = 50;

PipeController::PipeController(QObject *parent, PipeModel *pipeModel)
    : QObject { parent }
    , m_pipeModel { pipeModel }
{
    connect(m_timer, &QTimer::timeout, this, QOverload<>::of(&PipeController::movePipes));
    restartPipes();
}

void PipeController::restartPipes()
{
    m_timer->stop();

    m_pipeModel->clearPipes();

    for (int i = 0; i < m_pipeModel->pipeCount(); i++) {
        m_pipeModel->createNewPipe();
    }

    m_timer->start(PIPE_UPDATE_TIME_MS);

    QTimer::singleShot(10000, this, &PipeController::restartPipes);
}

void PipeController::movePipes()
{
    if (pipeCount() < MAX_PIPES) {
        m_pipeModel->movePipes();
    }
    else {
        restartPipes();
    }
}

int PipeController::pipeCount() const
{
    return m_pipeModel->pipeCount();
}

int PipeController::cellRowCount() const
{
    return m_pipeModel->cellRowCount();
}

int PipeController::maxDistance() const
{
    return m_pipeModel->maxDistance();
}

void PipeController::setPipeCount(int newPipeCount)
{
    m_pipeModel->setPipeCount(newPipeCount);
    restartPipes();

    emit pipeCountChanged();
}

void PipeController::setCellRowCount(int newCellRowCount)
{
    m_pipeModel->setCellRowCount(newCellRowCount);
    restartPipes();

    emit cellRowCountChanged();
}

void PipeController::setMaxDistance(int newMaxDistance)
{
    m_pipeModel->setMaxDistance(newMaxDistance);

    restartPipes();
    emit maxDistanceChanged();
}
