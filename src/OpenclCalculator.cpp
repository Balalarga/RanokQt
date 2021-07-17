#include "OpenclCalculator.h"
#include <sstream>
#include <QDebug>

using namespace std;

OpenclCalculator::~OpenclCalculator()
{
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseProgram(program);
    ret = clReleaseKernel(kernel);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
}

string OpenclCalculator::CreateOpenclSource(const Program& program)
{
    stringstream result;
    result << R"(
double __matrix4x4Det(double* m, int n)
{
      return
         m[3] * m[1*n+2] * m[2*n+1] * m[3*n+0] - m[0*n+2] * m[1*n+3] * m[2*n+1] * m[3*n+0] -
         m[3] * m[1*n+1] * m[2*n+2] * m[3*n+0] + m[0*n+1] * m[1*n+3] * m[2*n+2] * m[3*n+0] +
         m[2] * m[1*n+1] * m[2*n+3] * m[3*n+0] - m[0*n+1] * m[1*n+2] * m[2*n+3] * m[3*n+0] -
         m[3] * m[1*n+2] * m[2*n+0] * m[3*n+1] + m[0*n+2] * m[1*n+3] * m[2*n+0] * m[3*n+1] +
         m[3] * m[1*n+0] * m[2*n+2] * m[3*n+1] - m[0*n+0] * m[1*n+3] * m[2*n+2] * m[3*n+1] -
         m[2] * m[1*n+0] * m[2*n+3] * m[3*n+1] + m[0*n+0] * m[1*n+2] * m[2*n+3] * m[3*n+1] +
         m[3] * m[1*n+1] * m[2*n+0] * m[3*n+2] - m[0*n+1] * m[1*n+3] * m[2*n+0] * m[3*n+2] -
         m[3] * m[1*n+0] * m[2*n+1] * m[3*n+2] + m[0*n+0] * m[1*n+3] * m[2*n+1] * m[3*n+2] +
         m[1] * m[1*n+0] * m[2*n+3] * m[3*n+2] - m[0*n+0] * m[1*n+1] * m[2*n+3] * m[3*n+2] -
         m[2] * m[1*n+1] * m[2*n+0] * m[3*n+3] + m[0*n+1] * m[1*n+2] * m[2*n+0] * m[3*n+3] +
         m[2] * m[1*n+0] * m[2*n+1] * m[3*n+3] - m[0*n+0] * m[1*n+2] * m[2*n+1] * m[3*n+3] -
         m[1] * m[1*n+0] * m[2*n+2] * m[3*n+3] + m[0*n+0] * m[1*n+1] * m[2*n+2] * m[3*n+3];
}


double __rand(double a, double b)
{
    return a + b - sqrt(pow(a, 2) + pow(b, 2));
}
double __ror(double a, double b)
{
    return a + b + sqrt(pow(a, 2) + pow(b, 2));
}

int checkZone(double *values)
{
    bool plus = false;
    bool zero = false;
    bool minus = false;
    for(int i = 0; i < 8; i++)
    {
        if(values[i] == 0)
            zero = true;
        else if(values[i] < 0)
            minus = true;
        else if(values[i] > 0)
            plus = true;
    }
    if(zero || (plus && minus))
        return 0;
    if(plus)
        return 1;
    return -1;
})";
    result << program.GetOpenclCode();

    result << R"(
kernel void __calcualteModel(global int *resultZones,
                           const uint3 spaceSize,
                           const float3 startPoint,
                           const float3 pointSize,
                           const float3 halfSize)
{
    int id = get_global_id(0);
    float3 point;
    point.x = startPoint.x + pointSize.x * (id / ( spaceSize.z * spaceSize.y ));
    point.y = startPoint.y + pointSize.y * (( id / spaceSize.z ) % spaceSize.y);
    point.z = startPoint.z + pointSize.z * (id % spaceSize.z);


    double values[8];
    values[0] = __resultFunc(point.x+halfSize.x, point.y+halfSize.y, point.z+halfSize.z);
    values[1] = __resultFunc(point.x+halfSize.x, point.y+halfSize.y, point.z-halfSize.z);
    values[2] = __resultFunc(point.x+halfSize.x, point.y-halfSize.y, point.z+halfSize.z);
    values[3] = __resultFunc(point.x+halfSize.x, point.y-halfSize.y, point.z-halfSize.z);
    values[4] = __resultFunc(point.x-halfSize.x, point.y+halfSize.y, point.z+halfSize.z);
    values[5] = __resultFunc(point.x-halfSize.x, point.y+halfSize.y, point.z-halfSize.z);
    values[6] = __resultFunc(point.x-halfSize.x, point.y-halfSize.y, point.z+halfSize.z);
    values[7] = __resultFunc(point.x-halfSize.x, point.y-halfSize.y, point.z-halfSize.z);

    resultZones[id] = checkZone(values);
}

kernel void __calculateMImage(global double *result,
                           const uint3 spaceSize,
                           const float3 startPoint,
                           const float3 pointSize,
                           const float3 halfSize)
{
    int id = get_global_id(0);
    float3 point;
    point.x = startPoint.x + pointSize.x * (id / ( spaceSize.z * spaceSize.y ));
    point.y = startPoint.y + pointSize.y * (( id / spaceSize.z ) % spaceSize.y);
    point.z = startPoint.z + pointSize.z * (id % spaceSize.z);

    double wv[4];
    wv[0] = __resultFunc(point.x,             point.y,             point.z            );
    wv[1] = __resultFunc(point.x+pointSize.x, point.y,             point.z            );
    wv[2] = __resultFunc(point.x,             point.y+pointSize.y, point.z            );
    wv[3] = __resultFunc(point.x,             point.y,             point.z+pointSize.z);

    double a[] = {
        point.y,             point.z,             wv[0], 1,
        point.y,             point.z,             wv[1], 1,
        point.y+pointSize.y, point.z,             wv[2], 1,
        point.y,             point.z+pointSize.z, wv[3], 1
    };
    double b[] = {
        point.x,             point.z,             wv[0], 1,
        point.x+pointSize.x, point.z,             wv[1], 1,
        point.x,             point.z,             wv[2], 1,
        point.x,             point.z+pointSize.z, wv[3], 1
    };
    double c[] = {
        point.x,             point.y,             wv[0], 1,
        point.x+pointSize.x, point.y,             wv[1], 1,
        point.x,             point.y+pointSize.y, wv[2], 1,
        point.x,             point.y,             wv[3], 1
    };
    double d[] = {
        point.x,             point.y,             point.z,              1,
        point.x+pointSize.x, point.y,             point.z,              1,
        point.x,             point.y+pointSize.y, point.z,              1,
        point.x,             point.y,             point.z+pointSize.z, 1
    };
    double f[] = {
        point.x,             point.y,             point.z,             wv[0],
        point.x+pointSize.x, point.y,             point.z,             wv[1],
        point.x,             point.y+pointSize.y, point.z,             wv[2],
        point.x,             point.y,             point.z+pointSize.z, wv[3]
    };
    double detA = __matrix4x4Det(a, 4);
    double detB = __matrix4x4Det(b, 4);
    double detC = __matrix4x4Det(c, 4);
    double detD = __matrix4x4Det(d, 4);
    double detF = __matrix4x4Det(f, 4);
    double div = sqrt(pow(detA, 2)+pow(detB, 2)+pow(detC, 2)+pow(detD, 2)+pow(detF, 2));

    result[id*5  ] = detA/div;
    result[id*5+1] = -detB/div;
    result[id*5+2] = -detC/div;
    result[id*5+3] = detD/div;
    result[id*5+4] = detF/div;
}

)";
    return result.str();
}

OpenclCalculator::OpenclCalculator()
{
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if(ret != CL_SUCCESS)
        qDebug()<<Qt::endl<<ret<<Qt::endl;
    qDebug()<<"platform_id: "<<platform_id;
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1,
                         &device_id, &ret_num_devices);
    if(ret != CL_SUCCESS)
        qDebug()<<Qt::endl<<ret;
    qDebug()<<"device_id: "<<device_id;
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    qDebug()<<"context: "<<context;
    command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);
}


void OpenclCalculator::ComputeModel(const Program& prog, int batchSize)
{
    auto space = SpaceBuilder::Instance().GetSpace();
    if(!space)
        return;
    space->CreateZoneData();

    string source = CreateOpenclSource(prog);
    const char* src_str = source.c_str();

    if(source != prevSource)
    {
        if(!prevSource.empty())
        {
            ret = clReleaseProgram(program);
            ret = clReleaseKernel(kernel);
        }

        program = clCreateProgramWithSource(context, 1,
                                            (const char **)&src_str, NULL, &ret);
        if (!program)
        {
            qDebug()<<"Error: Failed to create compute program!";
            return;
        }
        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
        if (ret != CL_SUCCESS)
        {
            size_t len;
            char buffer[2048];

            qDebug()<<"Error: Failed to build program executable!";
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
                                  sizeof(buffer), buffer, &len);
            qDebug()<<buffer;
            return;
        }
        prevSource = source;
    }

    kernel = clCreateKernel(program, "__calcualteModel", &ret);
    if (!kernel || ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to create compute kernel!";
        return;
    }

    if(batchSize == 0)
        batchSize = space->GetSize();

    for(int batchStart = 0; batchStart < space->GetSize(); batchStart += batchSize)
    {
        int batchEnd = batchSize+batchStart < space->GetSize() ? batchSize+batchStart : space->GetSize();

        // Create gpu buffers
        cl_mem out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                            (batchEnd-batchStart) * sizeof(cl_int), NULL, &ret);
        if (!out_mem_obj)
        {
            qDebug()<<"Error: Failed to allocate device memory!";
            return;
        }

        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&out_mem_obj);
        ret = clSetKernelArg(kernel, 1, sizeof(cl_uint3), &space->spaceUnits);
        ret = clSetKernelArg(kernel, 2, sizeof(cl_float3), &space->startPoint);
        ret = clSetKernelArg(kernel, 3, sizeof(cl_float3), &space->pointSize);
        ret = clSetKernelArg(kernel, 4, sizeof(cl_float3), &space->pointHalfSize);
        if (ret != CL_SUCCESS)
        {
            qDebug()<<"Error: Failed to set kernel arguments! "<<ret;
            clReleaseMemObject(out_mem_obj);
            return;
        }
        // Get the maximum work group size for executing the kernel on the device
        //
        size_t global;                      // global domain size for our calculation
        size_t local;                       // local domain size for our calculation
        ret = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE,
                                       sizeof(local), &local, NULL);
        if (ret != CL_SUCCESS)
        {
            qDebug()<<"Error: Failed to retrieve kernel work group info! " << ret;
            clReleaseMemObject(out_mem_obj);
            return;
        }

        // Execute the kernel over the entire range of our 1d input data set
        // using the maximum number of work group items for this device
        //
        global = (batchEnd-batchStart);

        ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                                     &global, &local, 0, NULL, NULL);
        if (ret)
        {
            qDebug()<<"Error: Failed to execute kernel!\n";
            clReleaseMemObject(out_mem_obj);
            return;
        }
        clFinish(command_queue);

        ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0,
                                  (batchEnd-batchStart) * sizeof(int), space->zoneData->GetPointer()+batchStart, 0, NULL, NULL);
        if (ret != CL_SUCCESS)
        {
            qDebug()<<"Error: Failed to read output array! "<<ret;
            clReleaseMemObject(out_mem_obj);
            return;
        }
        ret = clReleaseMemObject(out_mem_obj);
        emit ComputedModel(batchStart, batchEnd);
    }
    qDebug()<<"Complete\n";
}

void OpenclCalculator::ComputeImage(const Program &prog, int batchSize)
{
    auto space = SpaceBuilder::Instance().GetSpace();
    if(!space)
        return;
    space->CreateMimageData();

    string source = CreateOpenclSource(prog);
    const char* src_str = source.c_str();

    if(source != prevSource)
    {
        if(!prevSource.empty())
        {
            ret = clReleaseProgram(program);
            ret = clReleaseKernel(kernel);
        }

        program = clCreateProgramWithSource(context, 1,
                                            (const char **)&src_str, NULL, &ret);
        if (!program)
        {
            qDebug()<<"Error: Failed to create compute program!";
            return;
        }
        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
        if (ret != CL_SUCCESS)
        {
            size_t len;
            char buffer[2048];

            qDebug()<<"Error: Failed to build program executable!";
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
                                  sizeof(buffer), buffer, &len);
            qDebug()<<buffer;
            return;
        }
        prevSource = source;
    }

    kernel = clCreateKernel(program, "__calculateMImage", &ret);
    if (!kernel || ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to create compute kernel!";
        return;
    }

    if(batchSize == 0)
        batchSize = space->GetSize();


    for(int batchStart = 0; batchStart < space->GetSize(); batchStart += batchSize)
    {
        int end = batchSize+batchStart < space->GetSize() ? batchSize+batchStart : space->GetSize();

        // Create gpu buffers
        cl_mem out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                            (end-batchStart) * 5 * sizeof(cl_double), NULL, &ret);
        if (!out_mem_obj)
        {
            qDebug()<<"Error: Failed to allocate device memory!";
            return;
        }

        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&out_mem_obj);
        ret = clSetKernelArg(kernel, 1, sizeof(cl_uint3), &space->spaceUnits);
        ret = clSetKernelArg(kernel, 2, sizeof(cl_float3), &space->startPoint);
        ret = clSetKernelArg(kernel, 3, sizeof(cl_float3), &space->pointSize);
        ret = clSetKernelArg(kernel, 4, sizeof(cl_float3), &space->pointHalfSize);
        if (ret != CL_SUCCESS)
        {
            qDebug()<<"Error: Failed to set kernel arguments! "<<ret;
            ret = clReleaseMemObject(out_mem_obj);
            return;
        }
        // Get the maximum work group size for executing the kernel on the device
        //
        size_t global;                      // global domain size for our calculation
        size_t local;                       // local domain size for our calculation
        ret = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE,
                                       sizeof(local), &local, NULL);
        if (ret != CL_SUCCESS)
        {
            qDebug()<<"Error: Failed to retrieve kernel work group info! " << ret;
            clReleaseMemObject(out_mem_obj);
            return;
        }

        // Execute the kernel over the entire range of our 1d input data set
        // using the maximum number of work group items for this device
        //
        global = (end-batchStart);

        ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                                     &global, &local, 0, NULL, NULL);
        if (ret)
        {
            qDebug()<<"Error: Failed to execute kernel!\n";
            clReleaseMemObject(out_mem_obj);
            return;
        }
        clFinish(command_queue);

        ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0,
                                  (end-batchStart) * 5 * sizeof(cl_double), space->mimageData->GetPointer()+batchStart,
                                  0, NULL, NULL);
        if (ret != CL_SUCCESS)
        {
            qDebug()<<"Error: Failed to read output array! "<<ret;
            clReleaseMemObject(out_mem_obj);
            return;
        }
        ret = clReleaseMemObject(out_mem_obj);
        emit ComputedMimage(batchStart, end);
    }
    qDebug()<<"Complete\n";
}
//void OpenclCalculator::Compute(const string& source, const vector<Vector2d> &data)
//{

//}

//void OpenclCalculator::Compute(const string& source, const vector<double> &data)
//{

//}
