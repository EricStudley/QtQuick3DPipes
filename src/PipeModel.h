#pragma once

#include "PipeObject.h"
#include "PipeEnums.h"

#include <QAbstractListModel>
#include <QTimer>

struct CurrentMove {
    Side side;
    QVector3D previousIndex;
    QVector3D nextIndex;
};

class PipeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PipeModel(QObject *parent = nullptr);

    enum DisplayRoles {
        IndexRole = Qt::UserRole + 1,
        ColorRole,
        SideListRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QVector3D nextIndex(QVector3D currentIndex, Side side);
    void move(CurrentMove &currentMove, QList<QVector3D> &visitedIndexes);
    void onTimeout();

    QList<PipeObject*> m_objects;
    QTimer *m_timer = new QTimer(this);
    CurrentMove m_currentMove;
    QList<QVector3D> m_visitedIndexes;
};
