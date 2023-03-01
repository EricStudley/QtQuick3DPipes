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
    Q_PROPERTY(int pipeCount READ pipeCount WRITE setPipeCount NOTIFY pipeCountChanged)
    Q_PROPERTY(int cellRowCount READ cellRowCount WRITE setCellRowCount NOTIFY cellRowCountChanged)
    Q_PROPERTY(int maxDistance READ maxDistance WRITE setMaxDistance NOTIFY maxDistanceChanged)

public:
    explicit PipeModel(QObject *parent = nullptr);

    enum DisplayRoles {
        CellIndexRole = Qt::UserRole + 1,
        ColorRole,
        DirectionListRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void restartPipes();

    // Q_PROPERTY READ
    int cellRowCount() const;
    int maxDistance() const;
    int pipeCount() const;

public slots:
    // Q_PROPERTY WRITE
    void setPipeCount(const int newPipeCount);
    void setCellRowCount(const int newCellRowCount);
    void setMaxDistance(const int newMaxDistance);

signals:
    // Q_PROPERTY NOTIFY
    void cellRowCountChanged();
    void maxDistanceChanged();
    void pipeCountChanged();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    void createNewPipe();

    void movePipes();
    void movePipe(MovingPipe &movingPipe, QList<QVector3D> &visitedCellIndexes);

    QVector3D nextCellIndexInDirection(const QVector3D &index, const Direction &direction);
    QMap<Direction, QVector3D> allValidDirectionIndexes(const MovingPipe &movingPipe);

private:
    QTimer *m_timer = new QTimer(this);
    QList<PipeObject*> m_pipeObjects;
    QList<MovingPipe> m_movingPipes;
    QList<QVector3D> m_visitedCellIndexes;

    // Q_PROPERTY MEMBER
    int m_cellRowCount = 15;
    int m_maxDistance = 3000;
    int m_pipeCount = 4;
};
