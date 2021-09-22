#include "LinesObject.h"


bool LinesObject::Create(int count, QOpenGLShaderProgram* lineShader,
                         QOpenGLShaderProgram* pointShader)
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
    lineShader->enableAttributeArray(0);
    lineShader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 10*sizeof(float));
    lineShader->enableAttributeArray(1);
    lineShader->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 3, 10*sizeof(float));
    lineShader->enableAttributeArray(2);
    lineShader->setAttributeBuffer(2, GL_FLOAT, 6*sizeof(float), 4, 10*sizeof(float));

    vao.release();
    vbo.release();

    if(!pointVao.isCreated())
        pointVao.create();
    pointVao.bind();
    vbo.bind();
    pointShader->enableAttributeArray(0);
    pointShader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 10*sizeof(float));

    pointVao.release();
    vbo.release();
    isCreated = vbo.isCreated() && vao.isCreated();
    return isCreated;
}

void LinesObject::AddData(float x, float y, float z, float x1, float y1, float z1,
                          float r, float g, float b, float a)
{
    buffer.push_back(x);
    buffer.push_back(y);
    buffer.push_back(z);
    buffer.push_back(x1);
    buffer.push_back(y1);
    buffer.push_back(z1);
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

void LinesObject::RenderPoints()
{
    pointVao.bind();
    glDrawArrays(GL_POINTS, 0, bufferFill);
    pointVao.release();
}

void LinesObject::RenderLines()
{
    vao.bind();
    glDrawArrays(GL_POINTS, 0, bufferFill);
    vao.release();
}

bool LinesObject::IsCreated()
{
    return isCreated;
}

void LinesObject::Recreate(QOpenGLShaderProgram *lineShader, QOpenGLShaderProgram *pointShader)
{
    Destroy();
    Create(vertexCount, lineShader, pointShader);
}
