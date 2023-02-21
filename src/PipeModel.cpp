#include "PipeModel.h"

#include <QRandomGenerator>

static auto constexpr MAX_PIPES = 1000;
static auto constexpr PIPE_UPDATE_TIME_MS = 50;

PipeModel::PipeModel(QObject *parent) : QAbstractListModel(parent)
{
    connect(m_timer, &QTimer::timeout, this, QOverload<>::of(&PipeModel::movePipes));
    restartPipes();
}

int PipeModel::rowCount(const QModelIndex &) const
{
    return m_pipeObjects.count();
}

QVariant PipeModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_pipeObjects.count())
        return {};

    const PipeObject* object = m_pipeObjects[index.row()];

    if (role == IndexRole)
        return object->index();
    else if (role == ColorRole)
        return object->color();
    else if (role == DirectionListRole)
        return object->directionList();

    return {};
}

void PipeModel::restartPipes()
{
    m_timer->stop();

    beginResetModel();
    m_pipeObjects.clear();
    endResetModel();

    m_visitedIndexes.clear();
    m_movingPipes.clear();

    int halfMaxIndex = m_maxIndex / 2;
    QVector3D index = QVector3D(QRandomGenerator::global()->bounded(halfMaxIndex), QRandomGenerator::global()->bounded(halfMaxIndex), QRandomGenerator::global()->bounded(halfMaxIndex));
    createNewPipe(index, Direction::Up);

    index = QVector3D(halfMaxIndex + QRandomGenerator::global()->bounded(halfMaxIndex), QRandomGenerator::global()->bounded(halfMaxIndex), QRandomGenerator::global()->bounded(halfMaxIndex));
    createNewPipe(index, Direction::Forward);

    index =  QVector3D(QRandomGenerator::global()->bounded(halfMaxIndex), halfMaxIndex + QRandomGenerator::global()->bounded(halfMaxIndex), QRandomGenerator::global()->bounded(halfMaxIndex));
    createNewPipe(index, Direction::Down);

    index = QVector3D(QRandomGenerator::global()->bounded(halfMaxIndex), QRandomGenerator::global()->bounded(halfMaxIndex), halfMaxIndex + QRandomGenerator::global()->bounded(halfMaxIndex));
    createNewPipe(index, Direction::Left);

    m_timer->start(PIPE_UPDATE_TIME_MS);
}

int PipeModel::maxIndex() const
{
    return m_maxIndex;
}

int PipeModel::maxDistance() const
{
    return m_maxDistance;
}

QHash<int, QByteArray> PipeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "role_index";
    roles[ColorRole] = "role_color";
    roles[DirectionListRole] = "role_directionList";

    return roles;
}

void PipeModel::createNewPipe(QVector3D index, Direction direction)
{
    MovingPipe movingPipe;
    movingPipe.direction = direction;
    movingPipe.previousIndex = index;
    movingPipe.nextIndex = nextIndexInDirection(movingPipe.previousIndex, movingPipe.direction);
    movingPipe.color = QColor::fromRgb(QRandomGenerator::global()->generate());

    beginInsertRows(QModelIndex(), m_pipeObjects.count(), m_pipeObjects.count());
    m_pipeObjects.append(new PipeObject(movingPipe.previousIndex, movingPipe.color, { movingPipe.direction }));
    endInsertRows();

    m_visitedIndexes.append(movingPipe.previousIndex);
    m_movingPipes.append(movingPipe);
}

void PipeModel::movePipes() {
    if (m_pipeObjects.count() < MAX_PIPES) {

        for (auto &movingPipe : m_movingPipes) {
            movePipe(movingPipe, m_visitedIndexes);
        }
    }
    else {
        restartPipes();
    }
}

void PipeModel::movePipe(MovingPipe &movingPipe, QList<QVector3D> &visitedIndexes) {
    Direction oppositeDirection = PipeEnums::oppositeDirection(movingPipe.direction);

    Direction nextDirection;
    QVector3D nextIndex;
    int count = 0;

    do {
        nextDirection = PipeEnums::randomDirection();
        nextIndex = nextIndexInDirection(movingPipe.nextIndex, nextDirection);
        count++;

        if (count > 50) {
            break;
        }
    }
    while (nextDirection == oppositeDirection || visitedIndexes.contains(nextIndex) || nextIndex == movingPipe.nextIndex);

    beginInsertRows(index(m_pipeObjects.length()), m_pipeObjects.length(), m_pipeObjects.length());
    m_pipeObjects.append(new PipeObject(movingPipe.nextIndex, movingPipe.color, { nextDirection, PipeEnums::oppositeDirection(movingPipe.direction)}));
    endInsertRows();

    visitedIndexes.append(nextIndex);

    movingPipe.direction = nextDirection;
    movingPipe.previousIndex = movingPipe.nextIndex;
    movingPipe.nextIndex = nextIndexInDirection(movingPipe.previousIndex, movingPipe.direction);
}

QVector3D PipeModel::nextIndexInDirection(QVector3D currentIndex, Direction direction)
{
    QVector3D nextIndex = currentIndex;

    switch (direction) {
    case Direction::Up: nextIndex.setY(nextIndex.y() + 1); break;
    case Direction::Down: nextIndex.setY(nextIndex.y() - 1); break;
    case Direction::Left: nextIndex.setX(nextIndex.x() - 1); break;
    case Direction::Right: nextIndex.setX(nextIndex.x() + 1); break;
    case Direction::Forward: nextIndex.setZ(nextIndex.z() + 1); break;
    case Direction::Back: nextIndex.setZ(nextIndex.z() - 1); break;
    default: break;
    }

    if (nextIndex.x() >= m_maxIndex
        || nextIndex.y() >= m_maxIndex
        || nextIndex.z() >= m_maxIndex
        || nextIndex.x() < 0
        || nextIndex.y() < 0
        || nextIndex.z() < 0) {
        return currentIndex;
    }

    return nextIndex;
}
