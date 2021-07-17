#ifndef COMMONCALCULATOR_H
#define COMMONCALCULATOR_H

#include "ISpaceCalculator.h"
#include <QObject>

class CommonCalculator : public ISpaceCalculator
{
public:
    explicit CommonCalculator(QObject *parent = nullptr);

    void CalcModel();
    void CalcMImage();

protected:
    double DeterminantOfMatrix(QVector<QVector<double> > &mat, int n);
    void GetCofactor(QVector<QVector<double>>& mat,
                     QVector<QVector<double>>& temp,
                     int p, int q, int n);
    int GetZone(const QVector<double>& values);
};

#endif // COMMONCALCULATOR_H
