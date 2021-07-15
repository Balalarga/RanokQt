#ifndef SPACEBUILDER_H
#define SPACEBUILDER_H


#include <vector>
#include "Utils.h"
#include <CL/cl.h>

template<class T>
struct CubeMatrix
{
    CubeMatrix(cl_uint3 size):
        _size(size)
    {
        _rawPtr = new T[size.x*size.y*size.z];
    }
    ~CubeMatrix()
    {
        delete[] _rawPtr;
    }
    T& At(int x, int y, int z)
    {
        return _rawPtr[x + _size.y * (y + _size.z * z)];
    }

    cl_uint3 GetSize()
    {
        return _size;
    }

private:
    T* _rawPtr;
    cl_uint3 _size;

    // Flat[x + WIDTH * (y + DEPTH * z)]
};


struct MimageData
{
    double Cx = -10;
    double Cy = -10;
    double Cz = -10;
    double Cw = -10;
    double Ct = -10;
};


struct SpaceData
{
    SpaceData(cl_uint3 spaceUnits,
              cl_double3 startPoint,
              cl_double3 pointSize):
        spaceUnits(spaceUnits),
        startPoint(startPoint),
        pointSize(pointSize),
        mimageData(nullptr),
        zoneData(nullptr)
    {

    }
    ~SpaceData()
    {
        DeleteZoneData();
        DeleteMimageData();
    }
    void CreateMimageData()
    {
        mimageData = new CubeMatrix<MimageData>(spaceUnits);
    }
    void CreateZoneData()
    {
        zoneData = new CubeMatrix<int>(spaceUnits);
    }
    void DeleteMimageData()
    {
        if(mimageData)
        {
            delete mimageData;
            mimageData = nullptr;
        }
    }
    void DeleteZoneData()
    {
        if(zoneData)
        {
            delete zoneData;
            zoneData = nullptr;
        }
    }

    cl_uint3   spaceUnits;
    cl_double3 startPoint;
    cl_double3 pointSize;

    CubeMatrix<MimageData>* mimageData;
    CubeMatrix<int>*        zoneData;
};


class SpaceBuilder
{
public:
    static SpaceBuilder& Instance(){ static SpaceBuilder builder; return builder; }

    ~SpaceBuilder();

    SpaceData* CreateSpace(const std::pair<double, double> &dim1,
                           const std::pair<double, double> &dim2,
                           const std::pair<double, double> &dim3,
                           const cl_uint3 &units);
    SpaceData* CreateSpace(const std::pair<double, double> &dim1,
                           const std::pair<double, double> &dim2,
                           const std::pair<double, double> &dim3,
                           const int &recur);

    SpaceData* GetSpace();

    void DeleteSpace();

private:
    SpaceBuilder();

    SpaceData* _space;
};

#endif // SPACEBUILDER_H
