#ifndef BOXMESH_H
#define BOXMESH_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

//#include "VoxelObject.h"


class VoxelMesh : public QObject
{
    Q_OBJECT
public:
    explicit VoxelMesh(int count, QObject *parent = nullptr);
    ~VoxelMesh();
    void Create(QOpenGLShaderProgram *shaderProgramm);
    void Destroy();

public slots:
    void Render();

signals:

private:
    QOpenGLBuffer _vbo;
    QOpenGLBuffer _ebo;
    QOpenGLVertexArrayObject _vao;
//    QVector<VoxelObject> _objects;
};

#endif // BOXMESH_H
