#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include <QThread>
#include <QDebug>

#include "Space/VoxelData.h"
#include "Space/VoxelImageData.h"
#include "Space/SpaceCalculator.h"
#include "OpenclGenerator.h"

enum class ComputeMode
{
    Cpu, Gpu
};


class ModelThread : public QThread
{
    Q_OBJECT
public:
    ModelThread(QObject *parent = nullptr):
        QThread(parent)
    {
    }
    void SetProgram(Program* program)
    {
        _program = program;
    }
    void SetComputeMode(ComputeMode mode)
    {
        _mode = mode;
    }


protected:
    void run() override
    {
        if(_program)
        {
            if(_mode == ComputeMode::Cpu)
                SpaceCalculator::GetModel(*_program);
            else
                OpenclGenerator::Instance().ComputeModel(*_program);
        }
        else
            qDebug()<<"[ModelThread] Program is null";
    }
    Program* _program = nullptr;
    ComputeMode _mode = ComputeMode::Cpu;
};


class ImageThread : public QThread
{
    Q_OBJECT
public:
    ImageThread(QObject *parent = nullptr):
        QThread(parent),
        _adder(adder)
    {
    }
    void SetProgram(Program* program)
    {
        _program = program;
    }
    void SetComputeMode(ComputeMode mode)
    {
        _mode = mode;
    }


protected:
    void run() override
    {
        if(_program)
        {
            if(_mode == ComputeMode::Cpu)
                SpaceCalculator::GetMImage(*_program);
            else
                OpenclGenerator::Instance().ComputeImage(*_program);
        }
        else
            qDebug()<<"[ModelThread] Program is null";
    }
    Program* _program = nullptr;
    ComputeMode _mode = ComputeMode::Cpu;
};

#endif // TASKTHREAD_H
