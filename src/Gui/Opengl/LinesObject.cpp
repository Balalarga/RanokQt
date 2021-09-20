#include "LinesObject.h"


bool LinesObject::Create(int count, QOpenGLShaderProgram* shader)
{
    vertexCount = count;
    bufferSize = sizeof(float)*count*10;
    if(!vao.isCreated())
        vao.create();
    vao.bind();

    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vbo.bind();
    vbo.allocate(nullptr, bufferSize);
    shader->enableAttributeArray(0);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 10*sizeof(float));
    shader->enableAttributeArray(1);
    shader->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 3, 10*sizeof(float));
    shader->enableAttributeArray(2);
    shader->setAttributeBuffer(2, GL_FLOAT, 6*sizeof(float), 4, 10*sizeof(float));

    vao.release();
    vbo.release();
    isCreated = vbo.isCreated() && vao.isCreated();
    return isCreated;
}

void LinesObject::AddData(float x, float y, float z, float x2, float y2, float z2,
                          float r, float g, float b, float a)
{
    buffer.push_back(x);
    buffer.push_back(y);
    buffer.push_back(z);
    buffer.push_back(x2);
    buffer.push_back(y2);
    buffer.push_back(z2);
    buffer.push_back(r);
    buffer.push_back(g);
    buffer.push_back(b);
    buffer.push_back(a);
    if(buffer.size() >= flushCount)
        Flush();
}

void LinesObject::Flush()
{
    vbo.bind();
    vbo.write(bufferFill*10*sizeof(float), buffer.data(), buffer.size()*sizeof(float));
    vbo.release();

    bufferFill += buffer.size()/10;
    buffer.clear();
}

void LinesObject::Destroy()
{
    vao.destroy();
    vbo.destroy();
    isCreated = false;
    bufferFill = 0;
    bufferSize = 0;
    buffer.clear();
}

void LinesObject::Render()
{
    vao.bind();
    glDrawArrays(GL_POINTS, 0, bufferFill);
    vao.release();
}

bool LinesObject::IsCreated()
{
    return isCreated;
}

void LinesObject::Recreate(QOpenGLShaderProgram *shader)
{
    Destroy();
    Create(vertexCount, shader);
}
