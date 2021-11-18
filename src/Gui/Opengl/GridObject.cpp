#include "GridObject.h"

GridObject::GridObject(ShaderProgram *shaderProgram, const VaoLayout& vaoLayout, QObject *parent):
    OpenglDrawableObject(shaderProgram, vaoLayout, parent),
    _linesStep(1.f),
    _linesSpace(-10.f, -10.f, 10.f, 10.f)
{
    _mainColor = QColor(255*0.5f, 255*0.5f,  255*0.7f, 255*0.5f);

    SetPrimitive(GL_LINES);
}

GridObject::~GridObject()
{
}

void GridObject::SetMainColor(const QColor &color)
{
    _mainColor = color;
    Recreate();
}

void GridObject::SetLinesSpace(const QVector2D &start, const QVector2D &end)
{
    _linesSpace.setX(start.x());
    _linesSpace.setY(start.y());
    _linesSpace.setZ(end.x());
    _linesSpace.setW(end.y());
    Recreate();
}

void GridObject::SetLinesStep(float step)
{
    _linesStep = step;
    Recreate();
}

void GridObject::Create()
{
    QVector<float> buffer;
    unsigned xCount = static_cast<unsigned>((_linesSpace.z() - _linesSpace.x()) / _linesStep);
    unsigned yCount = static_cast<unsigned>((_linesSpace.w() - _linesSpace.y()) / _linesStep);
    for(int x = 0; x < xCount+1; ++x)
    {
        float xPos = _linesSpace.x() + _linesStep * x;
        buffer.push_back(xPos);
        buffer.push_back(_linesSpace.w());
        buffer.push_back(0);
        buffer.push_back(_mainColor.redF());
        buffer.push_back(_mainColor.greenF());
        buffer.push_back(_mainColor.blueF());
        buffer.push_back(_mainColor.alphaF());

        buffer.push_back(xPos);
        buffer.push_back(_linesSpace.y());
        buffer.push_back(0);
        buffer.push_back(_mainColor.redF());
        buffer.push_back(_mainColor.greenF());
        buffer.push_back(_mainColor.blueF());
        buffer.push_back(_mainColor.alphaF());
    }
    for(int y = 0; y < yCount+1; ++y)
    {
        float yPos = _linesSpace.y() + _linesStep * y;
        buffer.push_back(_linesSpace.z());
        buffer.push_back(yPos);
        buffer.push_back(0);
        buffer.push_back(_mainColor.redF());
        buffer.push_back(_mainColor.greenF());
        buffer.push_back(_mainColor.blueF());
        buffer.push_back(_mainColor.alphaF());

        buffer.push_back(_linesSpace.x());
        buffer.push_back(yPos);
        buffer.push_back(0);
        buffer.push_back(_mainColor.redF());
        buffer.push_back(_mainColor.greenF());
        buffer.push_back(_mainColor.blueF());
        buffer.push_back(_mainColor.alphaF());
    }

    OpenglDrawableObject::Create(buffer);
}

void GridObject::Recreate()
{
    Destroy();
    Create();
}
