#include "GridObject.h"

GridObject::GridObject(ShaderProgram *shaderProgram, const VaoLayout& vaoLayout, QObject *parent):
    OpenglDrawableObject(shaderProgram, vaoLayout, parent)
{
    mainColor = QColor(255*0.5f, 255*0.5f,  255*0.7f, 255*0.5f);

    SetPrimitive(GL_LINES);
}

GridObject::~GridObject()
{
}

void GridObject::SetMainColor(const QColor &color)
{
    mainColor = color;
    Destroy();
    Create();
}

void GridObject::Create()
{
    QVector<float> buffer;
    float x1 = -lineWidth*0.5;
    float x2 = lineWidth*0.5;
    for (int i = 0; i < lineWidth; ++i)
    {
        float d = lineWidth/(linesCount-1)*i-lineWidth*0.5;
        buffer.push_back(d);
        buffer.push_back(x1);
        buffer.push_back(0);
        buffer.push_back(mainColor.redF());
        buffer.push_back(mainColor.greenF());
        buffer.push_back(mainColor.blueF());
        buffer.push_back(mainColor.alphaF());

        buffer.push_back(d);
        buffer.push_back(x2);
        buffer.push_back(0);
        buffer.push_back(mainColor.redF());
        buffer.push_back(mainColor.greenF());
        buffer.push_back(mainColor.blueF());
        buffer.push_back(mainColor.alphaF());

        buffer.push_back(x1);
        buffer.push_back(d);
        buffer.push_back(0);
        buffer.push_back(mainColor.redF());
        buffer.push_back(mainColor.greenF());
        buffer.push_back(mainColor.blueF());
        buffer.push_back(mainColor.alphaF());

        buffer.push_back(x2);
        buffer.push_back(d);
        buffer.push_back(0);
        buffer.push_back(mainColor.redF());
        buffer.push_back(mainColor.greenF());
        buffer.push_back(mainColor.blueF());
        buffer.push_back(mainColor.alphaF());
    }

    OpenglDrawableObject::Create(buffer);
}
