#include "voxelmesh.h"

VoxelMesh::VoxelMesh(int count, QObject *parent) :
    QObject(parent),
    _vbo(QOpenGLBuffer::VertexBuffer),
    _ebo(QOpenGLBuffer::IndexBuffer)
{

}

VoxelMesh::~VoxelMesh()
{
    Destroy();
}

void VoxelMesh::Create(QOpenGLShaderProgram *shaderProgramm)
{
    _vao.create();
    _vao.bind();

    _vbo.create();
    _vbo.bind();
    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int vertexMemSize = _objects.size()*VoxelObject::sizeOfVertices*sizeof(float);
    qDebug() << "BoxObject - VertexBuffer size =" << vertexMemSize/1024.0 << "kByte";
//    _vbo.allocate(m_vertexBufferData.data(), vertexMemSize);

    _ebo.create();
    _ebo.bind();
    _ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    int elementMemSize = m_elementBufferData.size()*sizeof(GLuint);
//    qDebug() << "BoxObject - ElementBuffer size =" << elementMemSize/1024.0 << "kByte";
//    _ebo.allocate(m_elementBufferData.data(), elementMemSize);

    shaderProgramm->enableAttributeArray(0); // array with index/id 0
    shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float)*3);
    // index 1 = normal
    shaderProgramm->enableAttributeArray(1); // array with index/id 1
//    shaderProgramm->setAttributeBuffer(1, GL_FLOAT, offsetof(VertexVNC, m), 3, sizeof(VertexVNC));

    _vao.release();
    _vbo.release();
    _ebo.release();

}

void VoxelMesh::Destroy()
{
    _vao.destroy();
    _vbo.destroy();
    _ebo.destroy();
}

void VoxelMesh::Render()
{
    _vao.bind();

//    glDrawElements(GL_TRIANGLES, m_elementBufferData.size(), GL_UNSIGNED_INT, nullptr);

    _vao.release();
}
