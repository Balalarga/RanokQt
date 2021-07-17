#ifndef OPENCLCALCULATOR_H
#define OPENCLCALCULATOR_H

#include "ISpaceCalculator.h"
#include <QObject>

class OpenclCalculator : public ISpaceCalculator
{
public:
    explicit OpenclCalculator(QObject *parent = nullptr);
    ~OpenclCalculator();

    void CalcModel();
    void CalcMImage();

    QString CreateOpenclSource(const Program& program);

private:
    cl_platform_id   platform_id = NULL;// обратите внимание на типы данных
    cl_device_id     device_id = NULL;
    cl_context       context;
    cl_command_queue command_queue;
    cl_uint          ret_num_devices;
    cl_uint          ret_num_platforms;
    cl_int           ret;// сюда будут записываться сообщения об ошибках
    cl_program       program; // сюда будет записанна наша программа
    cl_kernel        kernel;  // сюда будет записан наш кернель
    QString          prevSource;
};

#endif // OPENCLCALCULATOR_H
