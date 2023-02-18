#include "PipeObject.h"

PipeObject::PipeObject()
{

}

PipeObject::PipeObject(const QVector3D &index, const QColor &color, const QVariantList &sideList)
    : m_index(index)
    , m_color(color)
    , m_sideList(sideList)
{
}

