#include "OpenclGenerator.h"
#include <QDebug>

using namespace std;

OpenclGenerator::~OpenclGenerator()
{
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseProgram(program);
    ret = clReleaseKernel(kernel);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
}

OpenclGenerator::OpenclGenerator()
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


void OpenclGenerator::Compute(const string& kernelName, const string& source,
                              std::function<void(VoxelData)> adder)
{
    auto space = SpaceBuilder::Instance().Get3dSpace();

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
        kernel = clCreateKernel(program, kernelName.c_str(), &ret);
        if (!kernel || ret != CL_SUCCESS)
        {
            qDebug()<<"Error: Failed to create compute kernel!";
            return;
        }
        prevSource = source;
    }

    // Prepeat cpu data
    cl_double3* inputPoints = new cl_double3[space->points.size()];
    cl_double3 inputPointSize = {space->pointSize.x/2.,
                                 space->pointSize.y/2.,
                                 space->pointSize.z/2.};
    for(int i = 0; i < space->points.size(); i++)
    {
        inputPoints[i] = {space->points[i].x,
                          space->points[i].y,
                          space->points[i].z};
    }

    // Create gpu buffers
    cl_mem in_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
                space->points.size() * sizeof(cl_double3), NULL, &ret);
    cl_mem decoy_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                space->points.size() * sizeof(cl_double3), NULL, &ret);
    cl_mem out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                space->points.size() * sizeof(cl_int), NULL, &ret);
    if (!in_mem_obj || !out_mem_obj || !decoy_mem_obj)
    {
        delete[] inputPoints;
        qDebug()<<"Error: Failed to allocate device memory!";
        return;
    }

    // Fill gpu buffer from cpu memory
    ret = clEnqueueWriteBuffer(command_queue, in_mem_obj, CL_TRUE, 0,
                space->points.size() * sizeof(cl_double3), inputPoints, 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        delete[] inputPoints;
        qDebug()<<"Error: Failed to write to source array!";
        return;
    }
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&out_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&in_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&decoy_mem_obj);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_double3), &inputPointSize);
    if (ret != CL_SUCCESS)
    {
        delete[] inputPoints;
        qDebug()<<"Error: Failed to set kernel arguments! "<<ret;
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
        delete[] inputPoints;
        qDebug()<<"Error: Failed to retrieve kernel work group info! " << ret;
        return;
    }
    qDebug() << "Local working groups = "<<local;

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    global = space->points.size();

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                &global, &local, 0, NULL, NULL);
    if (ret)
    {
        delete[] inputPoints;
        qDebug()<<"Error: Failed to execute kernel!\n";
        return;
    }
    clFinish(command_queue);

    cl_double3* decoyRes = new cl_double3[space->points.size()];
    int* result = new int[space->points.size()];
    ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0,
                space->points.size() * sizeof(int), result, 0, NULL, NULL);
    ret = clEnqueueReadBuffer(command_queue, decoy_mem_obj, CL_TRUE, 0,
                space->points.size() * sizeof(cl_double3), decoyRes, 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        delete[] inputPoints;
        qDebug()<<"Error: Failed to read output array! "<<ret;
        return;
    }

    Zone zone;
    for(int i = 0; i < space->points.size(); i++)
    {
        Vector3f decoyPos = {static_cast<float>(decoyRes[i].x),
                             static_cast<float>(decoyRes[i].y),
                             static_cast<float>(decoyRes[i].z)};
        if(result[i] == 0)
            zone = Zone::Zero;
        else if(result[i] == 1)
            zone = Zone::Positive;
        else
            zone = Zone::Negative;
        adder(VoxelData(decoyPos, space->pointSize/2.,
                        zone, {255, 255, 255, 20}));
    }
    ret = clReleaseMemObject(in_mem_obj);
    ret = clReleaseMemObject(out_mem_obj);
    ret = clReleaseMemObject(decoy_mem_obj);
    delete[] decoyRes;
    delete[] inputPoints;
    delete[] result;
    qDebug()<<"Complete\n";
}
//void OpenclGenerator::Compute(const string& source, const vector<Vector2d> &data)
//{

//}

//void OpenclGenerator::Compute(const string& source, const vector<double> &data)
//{

//}
