#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include <QThread>
#include <QDebug>

#include "Space/VoxelData.h"
#include "Space/VoxelImageData.h"
#include "Space/spacecalculator.h"


class ModelThread : public QThread
{
    Q_OBJECT
public:
    ModelThread(std::function<void(VoxelData)> adder, QObject *parent = nullptr):
        QThread(parent),
        _adder(adder)
    {
    }
    void SetProgram(Program* program)
    {
        _program = program;
    }


protected:
    void run() override
    {
        if(_program)
            SpaceCalculator::GetModel(*_program, _adder);
        else
            qDebug()<<"[ModelThread] Program is null";
    }
    std::function<void(VoxelData)> _adder;
    Program* _program = nullptr;
};


class ImageThread : public QThread
{
    Q_OBJECT
public:
    ImageThread(std::function<void(VoxelImageData)> adder, QObject *parent = nullptr):
        QThread(parent),
        _adder(adder)
    {
    }
    void SetProgram(Program* program)
    {
        if(_program)
            _program = program;
        else
            qDebug()<<"[ImageThread] Program is null";
    }


protected:
    void run() override
    {
        SpaceCalculator::GetMImage(*_program, _adder);
    }
    std::function<void(VoxelImageData)> _adder;
    Program* _program = nullptr;
};

#endif // TASKTHREAD_H
