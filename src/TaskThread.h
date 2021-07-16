#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include <QThread>
#include <QDebug>

#include "Space/SpaceCalculator.h"
#include "OpenclCalculator.h"

enum class ComputeMode
{
    Cpu, Gpu
};


class ModelThread : public QThread
{
    Q_OBJECT
public:
    ModelThread(QObject *parent = nullptr):
        QThread(parent),
        _batchSize(0)
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
    void SetBatchSize(int batchSize)
    {
        _batchSize = batchSize;
    }


protected:
    void run() override
    {
        if(_program)
        {
            if(_mode == ComputeMode::Cpu)
            {
                SpaceCalculator::Get().GetModel(*_program, _batchSize);
            }
            else
                OpenclCalculator::Get().ComputeModel(*_program, _batchSize);
        }
        else
            qDebug()<<"[ModelThread] Program is null";
    }
    Program* _program = nullptr;
    ComputeMode _mode = ComputeMode::Cpu;
    int _batchSize;
};


class ImageThread : public QThread
{
    Q_OBJECT
public:
    ImageThread( QObject *parent = nullptr):
        QThread(parent),
        _batchSize(0)
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
    void SetBatchSize(int batchSize)
    {
        _batchSize = batchSize;
    }



protected:
    void run() override
    {
        if(_program)
        {
            if(_mode == ComputeMode::Cpu)
                SpaceCalculator::Get().GetMImage(*_program, _batchSize);
            else
                OpenclCalculator::Get().ComputeImage(*_program, _batchSize);
        }
        else
            qDebug()<<"[ModelThread] Program is null";
    }
    Program* _program = nullptr;
    ComputeMode _mode = ComputeMode::Cpu;
    int _batchSize;
};

#endif // TASKTHREAD_H
