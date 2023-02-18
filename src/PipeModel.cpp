#include "PipeModel.h"

#include <QRandomGenerator>

PipeModel::PipeModel(QObject *parent) : QAbstractListModel(parent)
{
    m_currentMove.side = Side::Up;
    m_currentMove.previousIndex = QVector3D(8, 10, 8);
    m_currentMove.nextIndex = this->nextIndex(m_currentMove.previousIndex, m_currentMove.side);

    m_objects.append(new PipeObject(m_currentMove.previousIndex, QColor(rand()%255, rand()%255, rand()%255), { m_currentMove.side }));

    m_visitedIndexes.append(m_currentMove.previousIndex);

    connect(m_timer, &QTimer::timeout, this, QOverload<>::of(&PipeModel::onTimeout));
    m_timer->start(200);

//    for (int i = 0; i < 100; i++) {
//        move(currentMove, visitedIndexes);
//    }

//    currentMove.side = Side::Left;
//    currentMove.previousIndex = QVector3D(12, 12, 12);
//    currentMove.nextIndex = this->nextIndex(currentMove.previousIndex, currentMove.side);

//    m_objects.append(new PipeObject(currentMove.previousIndex, QColor("green"), { currentMove.side }));

//    for (int i = 0; i < 100; i++) {
//        move(currentMove, visitedIndexes);
//    }

//    currentMove.side = Side::Right;
//    currentMove.previousIndex = QVector3D(10, 12, 10);
//    currentMove.nextIndex = this->nextIndex(currentMove.previousIndex, currentMove.side);

//    beginInsertRows(index(m_objects.length()), m_objects.length(), m_objects.length());
//    m_objects.append(new PipeObject(currentMove.previousIndex, QColor("red"), { currentMove.side }));
//    endInsertRows();

//    for (int i = 0; i < 100; i++) {
//        move(currentMove, visitedIndexes);
//    }



}

int PipeModel::rowCount(const QModelIndex &) const
{
    return m_objects.count();
}

QVariant PipeModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_objects.count())
        return {};

    const PipeObject* object = m_objects[index.row()];

    if (role == IndexRole)
        return object->index();
    else if (role == ColorRole)
        return object->color();
    else if (role == SideListRole)
        return object->sideList();

    return {};
}

QHash<int, QByteArray> PipeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "role_index";
    roles[ColorRole] = "role_color";
    roles[SideListRole] = "role_sideList";

    return roles;
}

void PipeModel::onTimeout() {
    move(m_currentMove, m_visitedIndexes);
}

void PipeModel::move(CurrentMove &currentMove, QList<QVector3D> &visitedIndexes) {
    PipeObject *lastPipe = m_objects.last();
    Side oppositeSide = PipeEnums::oppositeSide(currentMove.side);

    Side nextSide = static_cast<Side>(QRandomGenerator::global()->bounded(6));
    QVector3D nextIndex = this->nextIndex(currentMove.nextIndex, nextSide);

    int count = 0;
    while (nextSide == oppositeSide || visitedIndexes.contains(nextIndex) || nextIndex == currentMove.nextIndex) {
        nextSide = static_cast<Side>(QRandomGenerator::global()->bounded(6));
        nextIndex = this->nextIndex(currentMove.nextIndex, nextSide);
        count++;

        if (count > 50) {
            break;
        }
    }

    beginInsertRows(index(m_objects.length()), m_objects.length(), m_objects.length());
    m_objects.append(new PipeObject(currentMove.nextIndex, QColor(rand()%255, rand()%255, rand()%255), { nextSide, PipeEnums::oppositeSide(currentMove.side)}));
    endInsertRows();

    visitedIndexes.append(nextIndex);
    currentMove.side = nextSide;
    currentMove.previousIndex = currentMove.nextIndex;
    currentMove.nextIndex = this->nextIndex(currentMove.previousIndex, currentMove.side);
}

QVector3D PipeModel::nextIndex(QVector3D currentIndex, Side side)
{
    QVector3D nextIndex = currentIndex;

    switch (side) {
    case Side::Up: nextIndex.setY(nextIndex.y() + 1); break;
    case Side::Down: nextIndex.setY(nextIndex.y() - 1); break;
    case Side::Left: nextIndex.setX(nextIndex.x() - 1); break;
    case Side::Right: nextIndex.setX(nextIndex.x() + 1); break;
    case Side::Front: nextIndex.setZ(nextIndex.z() + 1); break;
    case Side::Back: nextIndex.setZ(nextIndex.z() - 1); break;
    }

    if (nextIndex.x() >= 20
        || nextIndex.y() >= 20
        || nextIndex.z() >= 20
        || nextIndex.x() < 0
        || nextIndex.y() < 0
        || nextIndex.z() < 0) {
        return currentIndex;
    }

    return nextIndex;
}
