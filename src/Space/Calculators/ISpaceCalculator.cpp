#include "ISpaceCalculator.h"

ISpaceCalculator::ISpaceCalculator(QObject *parent):
    QThread(parent),
    _mode(CalculatorMode::Model),
    _modelColor({255, 255, 255, 20}),
    _program(0),
    _batchSize(0)
{
    QVector<QColor> gradColors;
    gradColors.push_back(QColor(255, 255, 0,   20));
    gradColors.push_back(QColor(0,   255, 162, 20));
    gradColors.push_back(QColor(0,   0,   255, 20));
    gradColors.push_back(QColor(255, 145, 0,   20));
    gradColors.push_back(QColor(214, 0,   255, 20));
    SetMImageColorGradiend(gradColors);
}

void ISpaceCalculator::SetCalculatorMode(CalculatorMode mode)
{
    _mode = mode;
}

CalculatorMode ISpaceCalculator::GetCalculatorMode()
{
    return _mode;
}

QColor ISpaceCalculator::GetMImageColor(double value)
{
    value = (1. + value)/2.;
    unsigned uValue = UINT_MAX*value;
    return _mimageColorModel.GetColor(uValue);
}

void ISpaceCalculator::SetMImageColorGradiend(const QVector<QColor> &colors)
{
    _mimageColorModel.SetColors(colors);
}

QColor ISpaceCalculator::GetModelColor()
{
    return _modelColor;
}

void ISpaceCalculator::SetModelColor(const QColor &newDefaultColor)
{
    _modelColor = newDefaultColor;
}

void ISpaceCalculator::SetProgram(Program *program)
{
    _program = program;
}

void ISpaceCalculator::SetBatchSize(int batchSize)
{
    _batchSize = batchSize;
}

Program *ISpaceCalculator::GetProgram()
{
    return _program;
}

int ISpaceCalculator::GetBatchSize()
{
    return _batchSize;
}
