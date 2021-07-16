#ifndef OPENCLGENERATOR_H
#define OPENCLGENERATOR_H

#include <CL/cl.h>
#include <string>
#include <functional>


#include "Space/SpaceBuilder.h"
#include "Language/Program.h"
#include "Space/VoxelData.h"
#include "Space/VoxelImageData.h"

class OpenclCalculator: public QObject
{
    Q_OBJECT
public:
    static OpenclCalculator& Get(){ static OpenclCalculator gen; return gen; }

    ~OpenclCalculator();

    std::string CreateOpenclSource(const Program& program);

    void ComputeModel(const Program &prog, int batchSize = 0);
    void ComputeImage(const Program &prog, int batchSize = 0);
//    void Compute(const std::string &source, const std::vector<Vector2d> &data);
//    void Compute(const std::string &source, const std::vector<double> &data);

signals:
    void ComputedModel(int start, int count);
    void ComputedMimage(int start, int count);


protected:
    OpenclCalculator();


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
    std::string      prevSource;

};

#endif // OPENCLGENERATOR_H
