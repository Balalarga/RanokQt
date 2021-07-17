#ifndef ISPACECALCULATOR_H
#define ISPACECALCULATOR_H

#include <QThread>
#include <QColor>
#include "../SpaceBuilder.h"
#include "Language/Program.h"
#include "LinearGradientModel.h"


enum class CalculatorMode
{
    Model, Mimage
};

class ISpaceCalculator : public QThread
{
    Q_OBJECT
public:
    explicit ISpaceCalculator(QObject *parent = nullptr);
    virtual ~ISpaceCalculator(){}

    virtual void CalcModel() = 0;
    virtual void CalcMImage() = 0;

    void SetCalculatorMode(CalculatorMode mode);
    CalculatorMode GetCalculatorMode();

    QColor GetMImageColor(double value);
    void SetMImageColorGradiend(const QVector<QColor>& colors);

    QColor GetModelColor();
    void SetModelColor(const QColor &newDefaultColor);

    void SetProgram(Program *program);
    void SetBatchSize(int batchSize);

    Program* GetProgram();
    int GetBatchSize();

signals:
    void ComputedModel(int start, int count);
    void ComputedMimage(int start, int count);


protected:
    void run() override
    {
        if(_program)
        {
            if(_mode == CalculatorMode::Model)
                CalcModel();
            else
                CalcMImage();
        }
        else
            qDebug()<<"[ISpaceCalculator] Program is null";
    }


private:
    CalculatorMode _mode;

    QColor _modelColor;
    LinearGradientModel _mimageColorModel;

    Program* _program;
    int _batchSize;
};

#endif // ISPACECALCULATOR_H
