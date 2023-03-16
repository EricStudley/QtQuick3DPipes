#pragma once

#include <QObject>
#include <QTimer>

#include "PipeModel.h"

class PipeController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int pipeCount READ pipeCount WRITE setPipeCount NOTIFY pipeCountChanged)
    Q_PROPERTY(int cellRowCount READ cellRowCount WRITE setCellRowCount NOTIFY cellRowCountChanged)
    Q_PROPERTY(int maxDistance READ maxDistance WRITE setMaxDistance NOTIFY maxDistanceChanged)

public:
    explicit PipeController(QObject *parent, PipeModel *pipeModel);

    Q_INVOKABLE void restartPipes();

    // Q_PROPERTY READ
    int pipeCount() const;
    int cellRowCount() const;
    int maxDistance() const;

public slots:
    // Q_PROPERTY WRITE
    void setPipeCount(int newPipeCount);
    void setCellRowCount(int newCellRowCount);
    void setMaxDistance(int newMaxDistance);

signals:
    // Q_PROPERTY NOTIFY
    void pipeCountChanged();
    void cellRowCountChanged();
    void maxDistanceChanged();

private:
    void movePipes();

    PipeModel *m_pipeModel = nullptr;
    QTimer *m_timer = new QTimer(this);

    // Q_PROPERTY MEMBER
    int m_pipeCount;
    int m_cellRowCount;
    int m_maxDistance;
};
