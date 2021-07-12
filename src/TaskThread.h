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
    ModelThread(std::function<void(VoxelData)> adder, QObject *parent = nullptr):
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
                SpaceCalculator::GetModel(*_program, _adder);
            else
                OpenclGenerator::Instance().ComputeModel(*_program, _adder);
        }
        else
            qDebug()<<"[ModelThread] Program is null";
    }
    std::function<void(VoxelData)> _adder;
    Program* _program = nullptr;
    ComputeMode _mode = ComputeMode::Cpu;
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
                SpaceCalculator::GetMImage(*_program, _adder);
            else
                OpenclGenerator::Instance().ComputeImage(*_program, _adder);
        }
        else
            qDebug()<<"[ModelThread] Program is null";
    }
    std::function<void(VoxelImageData)> _adder;
    Program* _program = nullptr;
    ComputeMode _mode = ComputeMode::Cpu;
};

#endif // TASKTHREAD_H
