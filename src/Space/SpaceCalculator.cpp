#include "SpaceCalculator.h"
#include "SpaceBuilder.h"


using namespace std;

void getCofactor(vector<vector<double>>& mat,
                 vector<vector<double>>& temp,
                 int p, int q, int n)
{
    int i = 0, j = 0;
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (row != p && col != q)
            {
                temp[i][j++] = mat[row][col];
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}
double determinantOfMatrix(vector<vector<double>> mat, int n)
{
    if (n == 1)
        return mat[0][0];

    double D = 0;
    vector<vector<double>> temp(n, vector<double>(n));
    int sign = 1;
    for (int f = 0; f < n; f++)
    {
        getCofactor(mat, temp, 0, f, n);
        D += sign * mat[0][f]
                * determinantOfMatrix(temp, n - 1);
        sign = -sign;
    }
    return D;
}

int CheckZone(const std::vector<double> &values)
{
    bool plus = false;
    bool zero = false;
    bool minus = false;

    for(size_t i = 0; i < values.size(); i++)
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
}


QColor SpaceCalculator::_voxelColor = {255, 255, 255, 20};

bool SpaceCalculator::GetModel(const Program &program)
{
    auto space = SpaceBuilder::Instance().GetSpace();
    if(!space)
        return false;

    space->CreateZoneData();

    cl_double3 halfSize{space->pointSize.x/2.f,
                space->pointSize.y/2.f,
                space->pointSize.z/2.f};
    constexpr unsigned verticesSize = 8;
    vector<double> values(8);
    cl_double3 vertices[verticesSize];
    for(unsigned ix = 0; ix < space->spaceUnits.x; ix++)
    {
        for(unsigned iy = 0; iy < space->spaceUnits.y; iy++)
        {
            for(unsigned iz = 0; iz < space->spaceUnits.z; iz++)
            {
                cl_double3 point = {space->startPoint.x + space->pointSize.x * ix,
                                    space->startPoint.y + space->pointSize.y * iy,
                                    space->startPoint.z + space->pointSize.z * iz};
                vertices[0] = { point.x + halfSize.x, point.y + halfSize.y, point.z + halfSize.z };
                vertices[0] = { point.x + halfSize.x, point.y + halfSize.y, point.z - halfSize.z };
                vertices[0] = { point.x + halfSize.x, point.y - halfSize.y, point.z + halfSize.z };
                vertices[0] = { point.x + halfSize.x, point.y - halfSize.y, point.z - halfSize.z };
                vertices[0] = { point.x - halfSize.x, point.y + halfSize.y, point.z + halfSize.z };
                vertices[0] = { point.x - halfSize.x, point.y + halfSize.y, point.z - halfSize.z };
                vertices[0] = { point.x - halfSize.x, point.y - halfSize.y, point.z + halfSize.z };
                vertices[0] = { point.x - halfSize.x, point.y - halfSize.y, point.z - halfSize.z };

                for(size_t i = 0; i < verticesSize; i++)
                    values[i] = program.Compute(vertices[i]);
                space->zoneData->At(ix, iy, iz) = CheckZone(values);
            }
        }
    }
    return true;
}

bool SpaceCalculator::GetMImage(const Program &program)
{
    auto space = SpaceBuilder::Instance().GetSpace();
    if(!space)
        return false;

    space->CreateMimageData();

    cl_double3 size = space->pointSize;

    vector<double> wv(4);
    vector<vector<double>> a;
    vector<vector<double>> b;
    vector<vector<double>> c;
    vector<vector<double>> d;
    vector<vector<double>> f;
    for(unsigned ix = 0; ix < space->spaceUnits.x; ix++)
    {
        for(unsigned iy = 0; iy < space->spaceUnits.y; iy++)
        {
            for(unsigned iz = 0; iz < space->spaceUnits.z; iz++)
            {
                cl_double3 point = {space->startPoint.x + space->pointSize.x * ix,
                                    space->startPoint.y + space->pointSize.y * iy,
                                    space->startPoint.z + space->pointSize.z * iz};

                wv[0] = program.Compute(Vector3{point.x,        point.y,        point.z       });
                wv[1] = program.Compute(Vector3{point.x+size.x, point.y,        point.z       });
                wv[2] = program.Compute(Vector3{point.x,        point.y+size.y, point.z       });
                wv[3] = program.Compute(Vector3{point.x,        point.y,        point.z+size.z});

                a = {
                    {point.y,        point.z,        wv[0], 1},
                    {point.y,        point.z,        wv[1], 1},
                    {point.y+size.y, point.z,        wv[2], 1},
                    {point.y,        point.z+size.z, wv[3], 1},
                };
                b = {
                    {point.x,        point.z,        wv[0], 1},
                    {point.x+size.x, point.z,        wv[1], 1},
                    {point.x,        point.z,        wv[2], 1},
                    {point.x,        point.z+size.z, wv[3], 1},
                };
                c = {
                    {point.x,        point.y,        wv[0], 1},
                    {point.x+size.x, point.y,        wv[1], 1},
                    {point.x,        point.y+size.y, wv[2], 1},
                    {point.x,        point.y,        wv[3], 1},
                };
                d = {
                    {point.x,        point.y,        point.z,        1},
                    {point.x+size.x, point.y,        point.z,        1},
                    {point.x,        point.y+size.y, point.z,        1},
                    {point.x,        point.y,        point.z+size.z, 1},
                };
                f = {
                    {point.x,        point.y,        point.z,        wv[0]},
                    {point.x+size.x, point.y,        point.z,        wv[1]},
                    {point.x,        point.y+size.y, point.z,        wv[2]},
                    {point.x,        point.y,        point.z+size.z, wv[3]},
                };

                double detA = determinantOfMatrix(a, 4);
                double detB = determinantOfMatrix(b, 4);
                double detC = determinantOfMatrix(c, 4);
                double detD = determinantOfMatrix(d, 4);
                double detF = determinantOfMatrix(f, 4);
                double div = sqrt(pow(detA, 2)+pow(detB, 2)+
                                  pow(detC, 2)+pow(detD, 2)+pow(detF, 2));

                space->mimageData->At(ix, iy, iz).Cx = detA/div;
                space->mimageData->At(ix, iy, iz).Cy = -detB/div;
                space->mimageData->At(ix, iy, iz).Cz = -detC/div;
                space->mimageData->At(ix, iy, iz).Cw = detD/div;
                space->mimageData->At(ix, iy, iz).Ct = detF/div;
            }
        }
    }

    return true;
}

QColor SpaceCalculator::GetVoxelColor()
{
    return _voxelColor;
}

void SpaceCalculator::SetVoxelColor(const QColor &newDefaultColor)
{
    _voxelColor = newDefaultColor;
}
