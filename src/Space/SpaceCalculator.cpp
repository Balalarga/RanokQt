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

SpaceCalculator &SpaceCalculator::Get()
{
    static SpaceCalculator calc;
    return calc;
}

bool SpaceCalculator::GetModel(const Program &program, int batchSize)
{
    auto space = SpaceBuilder::Instance().GetSpace();
    if(!space)
        return false;

    space->CreateZoneData();

    cl_float3 halfSize = space->pointHalfSize;
    constexpr unsigned verticesSize = 8;
    vector<double> values(8);
    cl_double3 vertices[verticesSize];
    cl_float3 point;

    if(batchSize == 0)
        batchSize = space->GetSize();

    for(int batchStart = 0; batchStart < space->GetSize(); )
    {
        int start = batchStart;
        int currentSize = batchSize+batchStart < space->GetSize() ? batchSize+batchStart : space->GetSize();

        for(; batchStart < currentSize; ++batchStart)
        {
            point = space->GetPos(batchStart);
            vertices[0] = { point.x + halfSize.x, point.y + halfSize.y, point.z + halfSize.z };
            vertices[1] = { point.x + halfSize.x, point.y + halfSize.y, point.z - halfSize.z };
            vertices[2] = { point.x + halfSize.x, point.y - halfSize.y, point.z + halfSize.z };
            vertices[3] = { point.x + halfSize.x, point.y - halfSize.y, point.z - halfSize.z };
            vertices[4] = { point.x - halfSize.x, point.y + halfSize.y, point.z + halfSize.z };
            vertices[5] = { point.x - halfSize.x, point.y + halfSize.y, point.z - halfSize.z };
            vertices[6] = { point.x - halfSize.x, point.y - halfSize.y, point.z + halfSize.z };
            vertices[7] = { point.x - halfSize.x, point.y - halfSize.y, point.z - halfSize.z };

            for(size_t i = 0; i < verticesSize; i++)
                values[i] = program.Compute(vertices[i]);
            space->zoneData->At(batchStart) = CheckZone(values);
        }
        emit ComputedModel(start, currentSize);
    }
    return true;
}

bool SpaceCalculator::GetMImage(const Program &program, int batchSize)
{
    auto space = SpaceBuilder::Instance().GetSpace();
    if(!space)
        return false;

    space->CreateMimageData();

    cl_float3 size = space->pointSize;
    vector<double> wv(4);
    vector<vector<double>> a;
    vector<vector<double>> b;
    vector<vector<double>> c;
    vector<vector<double>> d;
    vector<vector<double>> f;
    cl_float3 point;

    if(batchSize == 0)
        batchSize = space->GetSize();

    for(int batchStart = 0; batchStart < space->GetSize(); )
    {
        int start = batchStart;
        int currentSize = batchSize+batchStart < space->GetSize() ? batchSize+batchStart : space->GetSize();

        for(; batchStart < currentSize; ++batchStart)
        {
            point = space->GetPos(batchStart);
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

            space->mimageData->At(batchStart).Cx = detA/div;
            space->mimageData->At(batchStart).Cy = -detB/div;
            space->mimageData->At(batchStart).Cz = -detC/div;
            space->mimageData->At(batchStart).Cw = detD/div;
            space->mimageData->At(batchStart).Ct = detF/div;
        }
        emit ComputedMimage(start, currentSize);
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

SpaceCalculator::SpaceCalculator(QObject *parent):
    QObject(parent)
{
    _voxelColor = {255, 255, 255, 20};
}
