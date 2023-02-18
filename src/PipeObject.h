#pragma once

#include <QVector3D>
#include <QColor>
#include <QVariantList>

class PipeObject
{
public:
    PipeObject();
    PipeObject(const QVector3D &index, const QColor &color, const QVariantList &sideList);

    QVector3D index() const { return m_index; }
    QColor color() const { return m_color; }
    QVariantList sideList() const { return m_sideList; }

    void setSideList(const QVariantList &sideList) { m_sideList = sideList; }

private:
    QVector3D m_index;
    QColor m_color;
    QVariantList m_sideList;
};
