#include "PipeObject.h"

PipeObject::PipeObject()
{

}

PipeObject::PipeObject(const QVector3D &cellIndex, const QColor &color, const QVariantList &directionList)
    : m_cellIndex(cellIndex)
    , m_color(color)
    , m_directionList(directionList)
{
}

