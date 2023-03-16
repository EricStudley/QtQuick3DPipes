#include "PipeModel.h"

#include <QRandomGenerator>
#include <QMetaEnum>

PipeModel::PipeModel(QObject *parent) : QAbstractListModel(parent)
{
}

int PipeModel::rowCount(const QModelIndex &) const
{
    return m_pipeObjects.count();
}

QVariant PipeModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_pipeObjects.count())
        return {};

    const PipeObject* object = m_pipeObjects.at(index.row());

    if (role == CellIndexRole)
        return object->cellIndex();
    else if (role == ColorRole)
        return object->color();
    else if (role == DirectionListRole)
        return object->directionList();

    return {};
}

void PipeModel::clearPipes()
{
    beginResetModel();
    m_pipeObjects.clear();
    endResetModel();

    m_visitedCellIndexes.clear();
    m_movingPipes.clear();
}

int PipeModel::cellRowCount() const
{
    return m_cellRowCount;
}

int PipeModel::maxDistance() const
{
    return m_maxDistance;
}

int PipeModel::pipeCount() const
{
    return m_pipeCount;
}

void PipeModel::setPipeCount(const int newPipeCount)
{
    if (m_pipeCount == newPipeCount || newPipeCount < 0)
        return;

    m_pipeCount = newPipeCount;

    emit pipeCountChanged();
}

void PipeModel::setCellRowCount(int newCellRowCount)
{
    if (m_cellRowCount == newCellRowCount)
        return;

    m_cellRowCount = newCellRowCount;

    emit cellRowCountChanged();
}

void PipeModel::setMaxDistance(int newMaxDistance)
{
    if (m_maxDistance == newMaxDistance)
        return;

    m_maxDistance = newMaxDistance;

    emit maxDistanceChanged();
}

QHash<int, QByteArray> PipeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CellIndexRole] = "role_cellIndex";
    roles[ColorRole] = "role_color";
    roles[DirectionListRole] = "role_directionList";

    return roles;
}

void PipeModel::createNewPipe()
{
    MovingPipe movingPipe;
    movingPipe.direction = PipeEnums::randomDirection();
    movingPipe.previousIndex = QVector3D(QRandomGenerator::global()->bounded(m_cellRowCount),
                                         QRandomGenerator::global()->bounded(m_cellRowCount),
                                         QRandomGenerator::global()->bounded(m_cellRowCount));
    movingPipe.nextIndex = nextCellIndexInDirection(movingPipe.previousIndex, movingPipe.direction);
    movingPipe.color = QColor::fromRgb(QRandomGenerator::global()->generate());

    beginInsertRows(QModelIndex(), m_pipeObjects.count(), m_pipeObjects.count());
    m_pipeObjects.append(new PipeObject(movingPipe.previousIndex, movingPipe.color, { movingPipe.direction }));
    endInsertRows();

    m_visitedCellIndexes.append(movingPipe.previousIndex);
    m_movingPipes.append(movingPipe);
}

void PipeModel::movePipes()
{
    for (auto &movingPipe : m_movingPipes) {
        movePipe(movingPipe, m_visitedCellIndexes);
    }
}

void PipeModel::movePipe(MovingPipe &movingPipe, QList<QVector3D> &visitedIndexes)
{
    QMap<Direction, QVector3D> validDirectionIndexes = allValidDirectionIndexes(movingPipe);

    if (validDirectionIndexes.isEmpty())
        return;

    QList<Direction> validDirections = validDirectionIndexes.keys();
    Direction nextDirection = validDirections.at(QRandomGenerator::global()->bounded(validDirections.count()));
    QVector3D nextIndex = validDirectionIndexes.value(nextDirection);

    beginInsertRows(index(m_pipeObjects.length()), m_pipeObjects.length(), m_pipeObjects.length());
    m_pipeObjects.append(new PipeObject(movingPipe.nextIndex, movingPipe.color, { nextDirection, PipeEnums::oppositeDirection(movingPipe.direction)}));
    endInsertRows();

    visitedIndexes.append(nextIndex);

    movingPipe.direction = nextDirection;
    movingPipe.previousIndex = movingPipe.nextIndex;
    movingPipe.nextIndex = nextCellIndexInDirection(movingPipe.previousIndex, movingPipe.direction);
}

QVector3D PipeModel::nextCellIndexInDirection(const QVector3D &currentIndex, const Direction &direction)
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

    if (nextIndex.x() >= m_cellRowCount
            || nextIndex.y() >= m_cellRowCount
            || nextIndex.z() >= m_cellRowCount
            || nextIndex.x() < 0
            || nextIndex.y() < 0
            || nextIndex.z() < 0) {
        return currentIndex;
    }

    return nextIndex;
}

QMap<Direction, QVector3D> PipeModel::allValidDirectionIndexes(const MovingPipe &movingPipe)
{
    QMap<Direction, QVector3D> validDirectionIndexes;

    QMetaEnum directionMetaEnum = QMetaEnum::fromType<Direction>();
    QList<Direction> directionList;

    for (int key = 0; key < directionMetaEnum.keyCount(); key++) {
        directionList += static_cast<Direction>(directionMetaEnum.value(key));
    }

    Direction oppositeDirection = PipeEnums::oppositeDirection(movingPipe.direction);
    directionList.removeAll(oppositeDirection);

    for (const auto direction : directionList) {
        const auto nextIndex = nextCellIndexInDirection(movingPipe.nextIndex, direction);

        bool validDirectionIndex = nextIndex != movingPipe.nextIndex
                && !m_visitedCellIndexes.contains(nextIndex);

        if (validDirectionIndex) {
            validDirectionIndexes.insert(direction, nextIndex);
        }
    }

    return validDirectionIndexes;
}
