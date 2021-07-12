#include "OpenclGenerator.h"
#include <sstream>
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

string OpenclGenerator::CreateOpenclSource(const Program& program)
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
kernel void calcualteModel(global int *resultZones,
                           global const double3 *points,
                           const double3 pointSize)
{
    int id = get_global_id(0);

    double values[8];
    values[0] = __resultFunc(points[id].x+pointSize.x, points[id].y+pointSize.y, points[id].z+pointSize.z);
    values[1] = __resultFunc(points[id].x+pointSize.x, points[id].y+pointSize.y, points[id].z-pointSize.z);
    values[2] = __resultFunc(points[id].x+pointSize.x, points[id].y-pointSize.y, points[id].z+pointSize.z);
    values[3] = __resultFunc(points[id].x+pointSize.x, points[id].y-pointSize.y, points[id].z-pointSize.z);
    values[4] = __resultFunc(points[id].x-pointSize.x, points[id].y+pointSize.y, points[id].z+pointSize.z);
    values[5] = __resultFunc(points[id].x-pointSize.x, points[id].y+pointSize.y, points[id].z-pointSize.z);
    values[6] = __resultFunc(points[id].x-pointSize.x, points[id].y-pointSize.y, points[id].z+pointSize.z);
    values[7] = __resultFunc(points[id].x-pointSize.x, points[id].y-pointSize.y, points[id].z-pointSize.z);

    resultZones[id] = checkZone(values);
}

kernel void calculateMImage(global double8 *result,
                            global const double3 *points,
                            const double3 pointSize)
{
    int id = get_global_id(0);
    result[id] = (double8)(1);

    double wv[4];
    wv[0] = __resultFunc(points[id].x,             points[id].y,             points[id].z            );
    wv[1] = __resultFunc(points[id].x+pointSize.x, points[id].y,             points[id].z            );
    wv[2] = __resultFunc(points[id].x,             points[id].y+pointSize.y, points[id].z            );
    wv[3] = __resultFunc(points[id].x,             points[id].y,             points[id].z+pointSize.z);

    double a[] = {
        points[id].y,             points[id].z,             wv[0], 1,
        points[id].y,             points[id].z,             wv[1], 1,
        points[id].y+pointSize.y, points[id].z,             wv[2], 1,
        points[id].y,             points[id].z+pointSize.z, wv[3], 1
    };
    double b[] = {
        points[id].x,             points[id].z,             wv[0], 1,
        points[id].x+pointSize.x, points[id].z,             wv[1], 1,
        points[id].x,             points[id].z,             wv[2], 1,
        points[id].x,             points[id].z+pointSize.z, wv[3], 1
    };
    double c[] = {
        points[id].x,             points[id].y,             wv[0], 1,
        points[id].x+pointSize.x, points[id].y,             wv[1], 1,
        points[id].x,             points[id].y+pointSize.y, wv[2], 1,
        points[id].x,             points[id].y,             wv[3], 1
    };
    double d[] = {
        points[id].x,             points[id].y,             points[id].z,              1,
        points[id].x+pointSize.x, points[id].y,             points[id].z,              1,
        points[id].x,             points[id].y+pointSize.y, points[id].z,              1,
        points[id].x,             points[id].y,             points[id].z+pointSize.z, 1
    };
    double f[] = {
        points[id].x,             points[id].y,             points[id].z,             wv[0],
        points[id].x+pointSize.x, points[id].y,             points[id].z,             wv[1],
        points[id].x,             points[id].y+pointSize.y, points[id].z,             wv[2],
        points[id].x,             points[id].y,             points[id].z+pointSize.z, wv[3]
    };
    double detA = __matrix4x4Det(a, 4);
    double detB = __matrix4x4Det(b, 4);
    double detC = __matrix4x4Det(c, 4);
    double detD = __matrix4x4Det(d, 4);
    double detF = __matrix4x4Det(f, 4);
    double div = sqrt(pow(detA, 2)+pow(detB, 2)+pow(detC, 2)+pow(detD, 2)+pow(detF, 2));

    result[id].s0 = detA/div;
    result[id].s1 = -detB/div;
    result[id].s2 = -detC/div;
    result[id].s3 = detD/div;
    result[id].s4 = detF/div;
}


)";
    return result.str();
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


void OpenclGenerator::ComputeModel(const Program& prog,
                              std::function<void(VoxelData)> adder)
{
    auto space = SpaceBuilder::Instance().Get3dSpace();
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
        kernel = clCreateKernel(program, "calcualteModel", &ret);
        if (!kernel || ret != CL_SUCCESS)
        {
            qDebug()<<"Error: Failed to create compute kernel!";
            return;
        }
        prevSource = source;
    }

    // Prepeat cpu data
    cl_double3 inputPointSize = {space->pointSize.x/2.,
                                 space->pointSize.y/2.,
                                 space->pointSize.z/2.};

    // Create gpu buffers
    cl_mem in_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
                space->points.size() * sizeof(cl_double3), NULL, &ret);
    cl_mem out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                space->points.size() * sizeof(cl_int), NULL, &ret);
    if (!in_mem_obj || !out_mem_obj)
    {
        qDebug()<<"Error: Failed to allocate device memory!";
        return;
    }

    // Fill gpu buffer from cpu memory
    ret = clEnqueueWriteBuffer(command_queue, in_mem_obj, CL_TRUE, 0,
                space->points.size() * sizeof(cl_double3), space->points.data(), 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to write to source array!";
        return;
    }
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&out_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&in_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_double3), &inputPointSize);
    if (ret != CL_SUCCESS)
    {
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
        qDebug()<<"Error: Failed to execute kernel!\n";
        return;
    }
    clFinish(command_queue);

    Zone* result = new Zone[space->points.size()];
    ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0,
                space->points.size() * sizeof(int), result, 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to read output array! "<<ret;
        return;
    }

    Zone zone;
    for(int i = 0; i < space->points.size(); i++)
    {
        adder(VoxelData(space->points[i], inputPointSize,
                        result[i], {255, 255, 255, 20}));
    }
    ret = clReleaseMemObject(in_mem_obj);
    ret = clReleaseMemObject(out_mem_obj);
    delete[] result;
    qDebug()<<"Complete\n";
}

void OpenclGenerator::ComputeImage(const Program &prog, function<void (VoxelImageData)> adder)
{
    auto space = SpaceBuilder::Instance().Get3dSpace();
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
        kernel = clCreateKernel(program, "calculateMImage", &ret);
        if (!kernel || ret != CL_SUCCESS)
        {
            qDebug()<<"Error: Failed to create compute kernel!";
            return;
        }
        prevSource = source;
    }

    cl_double3 halfSize = {space->pointSize.x/2.,
                           space->pointSize.y/2.,
                           space->pointSize.z/2.};
    // Create gpu buffers
    cl_mem in_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
                space->points.size() * sizeof(cl_double3), NULL, &ret);
    cl_mem out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                space->points.size() * sizeof(cl_double8), NULL, &ret);
    if (!in_mem_obj || !out_mem_obj)
    {
        qDebug()<<"Error: Failed to allocate device memory!";
        return;
    }

    // Fill gpu buffer from cpu memory
    ret = clEnqueueWriteBuffer(command_queue, in_mem_obj, CL_TRUE, 0,
                space->points.size() * sizeof(cl_double3), space->points.data(), 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to write to source array!";
        return;
    }
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&out_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&in_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_double3), &space->pointSize);
    if (ret != CL_SUCCESS)
    {
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
        qDebug()<<"Error: Failed to retrieve kernel work group info! " << ret;
        return;
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    global = space->points.size();

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                &global, &local, 0, NULL, NULL);
    if (ret)
    {
        qDebug()<<"Error: Failed to execute kernel!\n";
        return;
    }
    clFinish(command_queue);

    cl_double8* result = new cl_double8[space->points.size()];
    ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0,
                space->points.size() * sizeof(cl_double8), result, 0, NULL, NULL);
    if (ret != CL_SUCCESS)
    {
        qDebug()<<"Error: Failed to read output array! "<<ret;
        return;
    }

    for(int i = 0; i < space->points.size(); i++)
    {
        cl_double8 res = result[i];
        adder(VoxelImageData(space->points[i], halfSize, {
                   {MImageType::Cx, res.s0},
                   {MImageType::Cy, res.s1},
                   {MImageType::Cz, res.s2},
                   {MImageType::Cw, res.s3},
                   {MImageType::Ct, res.s4},
               }));
    }
    ret = clReleaseMemObject(in_mem_obj);
    ret = clReleaseMemObject(out_mem_obj);
    delete[] result;
    qDebug()<<"Complete\n";
}
//void OpenclGenerator::Compute(const string& source, const vector<Vector2d> &data)
//{

//}

//void OpenclGenerator::Compute(const string& source, const vector<double> &data)
//{

//}
