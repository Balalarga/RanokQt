#ifndef SHADERLAYOUT_H
#define SHADERLAYOUT_H

#include <QDebug>
#include <QVector>
#include "GL/gl.h"

struct VaoLayoutItem
{
    VaoLayoutItem():
        count(0),
        type(0),
        size(0)
    {}
    VaoLayoutItem(unsigned count, unsigned type):
        count(count),
        type(type),
        size(count * GetTypeSize(type))
    {}
    unsigned count;
    unsigned type;
    unsigned size;

    static unsigned GetTypeSize(unsigned type)
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
};

class VaoLayout
{
public:
    VaoLayout() = default;
    VaoLayout(const QVector<VaoLayoutItem>& items):
        _layoutItems(items)
    {
        for(auto& i: items)
            _stride += i.size;
    }

    void Add(unsigned count, unsigned type)
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

        _stride += count * size;
        _layoutItems.append(VaoLayoutItem(count, type));
    }

    inline const QVector<VaoLayoutItem>& GetLayoutItems() const
    {
        return _layoutItems;
    }

    unsigned GetStride() const
    {
        return _stride;
    }


private:
    QVector<VaoLayoutItem> _layoutItems;
    unsigned _stride = 0;
};

#endif // SHADERLAYOUT_H
