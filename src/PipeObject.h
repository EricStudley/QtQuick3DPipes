#pragma once

#include <QVector3D>
#include <QColor>
#include <QVariantList>

class PipeObject
{
public:
    PipeObject();
    PipeObject(const QVector3D &index, const QColor &color, const QVariantList &directionList);

    QVector3D index() const { return m_index; }
    QColor color() const { return m_color; }
    QVariantList directionList() const { return m_directionList; }

    void setDirectionList(const QVariantList &directionList) { m_directionList = directionList; }

private:
    QVector3D m_index;
    QColor m_color;
    QVariantList m_directionList;
};
