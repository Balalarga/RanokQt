#include "GridObject.h"


void GridObject::Create(QOpenGLShaderProgram* shader)
{
    QVector<float> gridVertexBufferData;
    float x1 = -lineWidth*0.5;
    float x2 = lineWidth*0.5;
    for (int i = 0; i < lineWidth; ++i)
    {
        float d = lineWidth/(linesCount-1)*i-lineWidth*0.5;
        gridVertexBufferData.push_back(d);
        gridVertexBufferData.push_back(x1);
        gridVertexBufferData.push_back(d);
        gridVertexBufferData.push_back(x2);

        gridVertexBufferData.push_back(x1);
        gridVertexBufferData.push_back(d);
        gridVertexBufferData.push_back(x2);
        gridVertexBufferData.push_back(d);
    }
    bufferSize = gridVertexBufferData.size();
    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(gridVertexBufferData.data(), bufferSize*sizeof(float));
    shader->enableAttributeArray(0);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);

    vao.release();
    vbo.release();
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
