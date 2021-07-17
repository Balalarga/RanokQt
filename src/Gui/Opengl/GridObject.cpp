#include "GridObject.h"


GridObject::~GridObject()
{
    Destroy();
}

void GridObject::Create(QOpenGLShaderProgram* shader)
{
    this->shader = shader;

    mainColor = QColor(255*0.5f, 255*0.5f,  255*0.7f, 255*0.5f);
    UpdateVbo();
}

void GridObject::Destroy()
{
    vao.destroy();
    vbo.destroy();
}

void GridObject::Render()
{
    vao.bind();
    glDrawArrays(GL_LINES, 0, bufferSize);
    vao.release();
}

void GridObject::SetMainColor(const QColor &color)
{
    mainColor = color;
    UpdateVbo();
}

void GridObject::UpdateVbo()
{
    if(vao.isCreated())
        vao.destroy();
    if(vbo.isCreated())
        vbo.destroy();

    QVector<float> gridVertexBufferData;
    float x1 = -lineWidth*0.5;
    float x2 = lineWidth*0.5;
    for (int i = 0; i < lineWidth; ++i)
    {
        float d = lineWidth/(linesCount-1)*i-lineWidth*0.5;
        gridVertexBufferData.push_back(d);
        gridVertexBufferData.push_back(x1);
        gridVertexBufferData.push_back(0);
        gridVertexBufferData.push_back(mainColor.redF());
        gridVertexBufferData.push_back(mainColor.greenF());
        gridVertexBufferData.push_back(mainColor.blueF());
        gridVertexBufferData.push_back(mainColor.alphaF());

        gridVertexBufferData.push_back(d);
        gridVertexBufferData.push_back(x2);
        gridVertexBufferData.push_back(0);
        gridVertexBufferData.push_back(mainColor.redF());
        gridVertexBufferData.push_back(mainColor.greenF());
        gridVertexBufferData.push_back(mainColor.blueF());
        gridVertexBufferData.push_back(mainColor.alphaF());

        gridVertexBufferData.push_back(x1);
        gridVertexBufferData.push_back(d);
        gridVertexBufferData.push_back(0);
        gridVertexBufferData.push_back(mainColor.redF());
        gridVertexBufferData.push_back(mainColor.greenF());
        gridVertexBufferData.push_back(mainColor.blueF());
        gridVertexBufferData.push_back(mainColor.alphaF());

        gridVertexBufferData.push_back(x2);
        gridVertexBufferData.push_back(d);
        gridVertexBufferData.push_back(0);
        gridVertexBufferData.push_back(mainColor.redF());
        gridVertexBufferData.push_back(mainColor.greenF());
        gridVertexBufferData.push_back(mainColor.blueF());
        gridVertexBufferData.push_back(mainColor.alphaF());
    }
    bufferSize = gridVertexBufferData.size();
    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(gridVertexBufferData.data(), bufferSize*sizeof(float));
    shader->enableAttributeArray(0);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 7*sizeof(float));
    shader->enableAttributeArray(1);
    shader->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 4, 7*sizeof(float));

    vao.release();
    vbo.release();
}
