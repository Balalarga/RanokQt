#include "WcsObject.h"

WcsObject::~WcsObject()
{
    Destroy();
}

void WcsObject::Create(QOpenGLShaderProgram* shader)
{
    this->shader = shader;

    colors.push_back(QColor(255,   0,   0));
    colors.push_back(QColor(  0, 255,   0));
    colors.push_back(QColor(  0,   0, 255));
    UpdateVbo();
}

void WcsObject::Destroy()
{
    vao.destroy();
    vbo.destroy();
}

void WcsObject::Render()
{
    vao.bind();
    glDrawArrays(GL_LINES, 0, bufferSize);
    vao.release();
}

void WcsObject::SetColors(const QVector<QColor>& colors)
{
    for(int i = 0; i < 3; i++)
        if(i < colors.size())
            this->colors[i] = colors[i];
    UpdateVbo();
}

void WcsObject::UpdateVbo()
{
    if(vao.isCreated())
        vao.destroy();
    if(vbo.isCreated())
        vbo.destroy();

    QVector<float> gridVertexBufferData;
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(colors[0].redF());
    gridVertexBufferData.push_back(colors[0].greenF());
    gridVertexBufferData.push_back(colors[0].blueF());
    gridVertexBufferData.push_back(colors[0].alphaF());

    gridVertexBufferData.push_back(1);
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(colors[0].redF());
    gridVertexBufferData.push_back(colors[0].greenF());
    gridVertexBufferData.push_back(colors[0].blueF());
    gridVertexBufferData.push_back(colors[0].alphaF());

    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(colors[1].redF());
    gridVertexBufferData.push_back(colors[1].greenF());
    gridVertexBufferData.push_back(colors[1].blueF());
    gridVertexBufferData.push_back(colors[1].alphaF());

    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(1);
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(colors[1].redF());
    gridVertexBufferData.push_back(colors[1].greenF());
    gridVertexBufferData.push_back(colors[1].blueF());
    gridVertexBufferData.push_back(colors[1].alphaF());

    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(colors[2].redF());
    gridVertexBufferData.push_back(colors[2].greenF());
    gridVertexBufferData.push_back(colors[2].blueF());
    gridVertexBufferData.push_back(colors[2].alphaF());

    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(0);
    gridVertexBufferData.push_back(1);
    gridVertexBufferData.push_back(colors[2].redF());
    gridVertexBufferData.push_back(colors[2].greenF());
    gridVertexBufferData.push_back(colors[2].blueF());
    gridVertexBufferData.push_back(colors[2].alphaF());

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
