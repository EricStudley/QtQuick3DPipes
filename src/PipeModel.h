#pragma once

#include "PipeObject.h"
#include "PipeEnums.h"

#include <QAbstractListModel>
#include <QTimer>

struct MovingPipe {
    Direction direction;
    QVector3D previousIndex;
    QVector3D nextIndex;
    QColor color;
};

class PipeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int maxIndex READ maxIndex NOTIFY maxIndexChanged)
    Q_PROPERTY(int maxDistance READ maxDistance NOTIFY maxDistanceChanged)

public:
    explicit PipeModel(QObject *parent = nullptr);

    enum DisplayRoles {
        IndexRole = Qt::UserRole + 1,
        ColorRole,
        DirectionListRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void restartPipes();

    // Q_PROPERTY READ
    int maxIndex() const;
    int maxDistance() const;

signals:
    // Q_PROPERTY NOTIFY
    void maxIndexChanged();
    void maxDistanceChanged();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    void createNewPipe(QVector3D index, Direction direction);

    void movePipes();
    void movePipe(MovingPipe &movingPipe, QList<QVector3D> &visitedIndexes);

    QVector3D nextIndexInDirection(QVector3D index, Direction direction);

private:
    QTimer *m_timer = new QTimer(this);
    QList<PipeObject*> m_pipeObjects;
    QList<MovingPipe> m_movingPipes;
    QList<QVector3D> m_visitedIndexes;

    // Q_PROPERTY MEMBER
    int m_maxIndex = 20;
    int m_maxDistance = 6000;
};
