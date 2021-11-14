#include "VaoLayout.h"

VaoLayoutItem::VaoLayoutItem():
    count(0),
    type(0),
    size(0)
{}

VaoLayoutItem::VaoLayoutItem(unsigned count, unsigned type):
    count(count),
    type(type),
    size(count * GetTypeSize(type))
{}

unsigned VaoLayoutItem::GetTypeSize(unsigned type)
{
    switch(type)
    {
    case GL_FLOAT:
        return sizeof(float);
    case GL_INT:
        return sizeof(int);
    case GL_UNSIGNED_INT:
        return sizeof(unsigned);
    }
    assert(false);
    return 0;
}

VaoLayout::VaoLayout(const QVector<VaoLayoutItem> &items):
    _layoutItems(items)
{
    for(auto& i: items)
    {
        _width += i.count;
        _stride += i.size;
    }
}

void VaoLayout::Add(unsigned count, unsigned type)
{
    unsigned size = 0;
    switch(type)
    {
    case GL_UNSIGNED_INT:
        size = sizeof(unsigned);
        break;
    case GL_INT:
        size = sizeof(int);
        break;
    case GL_FLOAT:
        size = sizeof(float);
        break;
    default:
        qDebug()<<"VaoLayout error: Unsupported data type";
    }
    _width += count;
    _stride += count * size;
    _layoutItems.append(VaoLayoutItem(count, type));
}

