#include "OpenclGenerator.h"


using namespace std;


OpenclGenerator::~OpenclGenerator()
{
    ret = clFlush(command_queue);                   // отчищаем очередь команд
    ret = clFinish(command_queue);                  // завершаем выполнение всех команд в очереди
    ret = clReleaseCommandQueue(command_queue);     // удаляем очередь команд
    ret = clReleaseContext(context);                // удаляем контекст OpenCL
}

OpenclGenerator::OpenclGenerator()
{
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if(ret != CL_SUCCESS)
        cout<<endl<<ret<<endl<<endl;
    cout<<"platform_id: "<<platform_id<<endl;
    cout<<"ret_num_platforms: "<<ret_num_platforms<<endl;
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1,
                &device_id, &ret_num_devices);
    if(ret != CL_SUCCESS)
        cout<<endl<<ret<<endl<<endl;
    cout<<"device_id: "<<device_id<<endl;
    cout<<"ret_num_devices: "<<ret_num_devices<<endl;
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    cout<<"context: "<<context<<endl;
    command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);

    char *driver_version;
    clGetDeviceInfo(0, CL_DRIVER_VERSION, sizeof(char*), &driver_version, NULL);
    std::cout <<  driver_version << std::endl;
}


void OpenclGenerator::Compute(const string& source, std::function<void(VoxelData)> adder)
{
    auto space = SpaceBuilder::Instance().Get3dSpace();

    const char* src_str = source.c_str();
    const size_t src_size = source.size();

    if(source != prevSource)
    {
        if(!prevSource.empty())
        {
            ret = clReleaseKernel(kernel);   // удаляем кернель
            ret = clReleaseProgram(program); // удаляем программу OpenCL
        }

        program = clCreateProgramWithSource(context, 1,
                    (const char **)&src_str, (const size_t *)&src_size, &ret); // создаём программу из исходного кода
        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);        // собираем программу (онлайн компиляция)
        kernel = clCreateKernel(program, "model_compute", &ret);               // создаём кернель

        prevSource = source;
    }

    const int inDataSize = space->points.size()*3;// data has vec3d type
    const int outDataSize = space->points.size();

    cl_mem in_mem_obj; // опять, обратите внимание на тип данных
    cl_mem out_mem_obj; // cl_mem это тип буфера памяти OpenCL

    in_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
                inDataSize * sizeof(cl_double), NULL, &ret);
    out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                outDataSize * sizeof(cl_char), NULL, &ret);

    double* inputArgData = new double[inDataSize];
    double inputPointData = space->pointSize.z/2.;

    for(int i = 0; i < space->points.size(); i++)
    {
        inputArgData[i*2] = space->points[i].x;
        inputArgData[i*2+1] = space->points[i].y;
        inputArgData[i*2+2] = space->points[i].z;
    }

    ret = clEnqueueWriteBuffer(command_queue, in_mem_obj, CL_TRUE, 0,
                inDataSize * sizeof(double), inputArgData, 0, NULL, NULL);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in_mem_obj);  // объект А
    ret = clSetKernelArg(kernel, 1, sizeof(double), &inputPointData);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out_mem_obj);  // объект C

    size_t NDRange;   // здесь мы указываем размер вычислительной сетки
    size_t work_size; // размер рабочей группы (work-group)
    NDRange = outDataSize;
    work_size = 32;        // NDRange должен быть кратен размеру work-group

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,     // исполняем кернель
                &NDRange, &work_size, 0, NULL, NULL);

    char *result;
    result = new char[outDataSize]; // выделяем память для массива с ответами
    ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0, // записываем ответы
                outDataSize * sizeof(char), result, 0, NULL, NULL);

    Zone zone;
    for(int i = 0; i < outDataSize; i++)
    {
        if(result[i] == -1)
            adder(VoxelData(space->points[i], space->pointSize/2., Zone::Negative, {255, 255, 255, 20}));
        else if(result[i] == 0)
            adder(VoxelData(space->points[i], space->pointSize/2., Zone::Zero, {255, 255, 255, 20}));
        else
            adder(VoxelData(space->points[i], space->pointSize/2., Zone::Positive, {255, 255, 255, 20}));
    }
    ret = clFlush(command_queue);  // отчищаем очередь команд
    ret = clFinish(command_queue); // завершаем выполнение всех команд в очереди
    ret = clReleaseMemObject(in_mem_obj); // отчищаем OpenCL буфер А
    ret = clReleaseMemObject(out_mem_obj);  // отчищаем OpenCL буфер C
    delete[] result;
    delete[] inputArgData;
}

//void OpenclGenerator::Compute(const string& source, const vector<Vector2d> &data)
//{

//}

//void OpenclGenerator::Compute(const string& source, const vector<double> &data)
//{

//}
