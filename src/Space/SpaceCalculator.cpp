#include "SpaceCalculator.h"
#include "SpaceBuilder.h"


using namespace std;

void getCofactor(vector<vector<double>>& mat, vector<vector<double>>& temp,
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

Zone CheckZone(const std::vector<double> &values)
{
    bool plus = false;
    bool zero = false;
    bool minus = false;

    for(size_t i = 0; i < values.size(); i++)
    {
        if(values[i] == 0)
            zero = true;
        if(values[i] < 0)
            minus = true;
        if(values[i] > 0)
            plus = true;
    }

    if(zero || (plus && minus))
        return Zone::Zero;

    if(plus)
        return Zone::Positive;

    return Zone::Negative;
}


QColor SpaceCalculator::_voxelColor = {255, 255, 255, 20};

bool SpaceCalculator::GetModel(const Program &program, std::function<void (VoxelData)> adder)
{
    auto space = SpaceBuilder::Instance().Get3dSpace();
    if(!space)
        return false;
    Vector3f halfSize(space->pointSize.x/2.f,
                      space->pointSize.y/2.f,
                      space->pointSize.z/2.f);
    for(auto& point : space->points)
    {
        constexpr unsigned verticesSize = 8;
        Vector3d vertices[verticesSize]{
            { point.x+halfSize.x, point.y+halfSize.y, point.z+halfSize.z },
            { point.x+halfSize.x, point.y+halfSize.y, point.z-halfSize.z },
            { point.x+halfSize.x, point.y-halfSize.y, point.z+halfSize.z },
            { point.x+halfSize.x, point.y-halfSize.y, point.z-halfSize.z },
            { point.x-halfSize.x, point.y+halfSize.y, point.z+halfSize.z },
            { point.x-halfSize.x, point.y+halfSize.y, point.z-halfSize.z },
            { point.x-halfSize.x, point.y-halfSize.y, point.z+halfSize.z },
            { point.x-halfSize.x, point.y-halfSize.y, point.z-halfSize.z }
        };
        vector<double> values(verticesSize);
        for(size_t i = 0; i < verticesSize; i++)
            values[i] = program.Compute(vertices[i]);
        adder(VoxelData(point, halfSize, CheckZone(values), _voxelColor));
    }

    return true;
}

bool SpaceCalculator::GetMImage(const Program &program, std::function<void (VoxelImageData)> adder)
{
    auto space = SpaceBuilder::Instance().Get3dSpace();
    if(!space)
        return false;
    Vector3f size = space->pointSize;
    Vector3f halfSize(size.x/2.f,
                      size.y/2.f,
                      size.z/2.f);

    vector<double> wv(4);
    for(auto& point: space->points)
    {
        wv[0] = program.Compute({point.x,        point.y,        point.z       });
        wv[1] = program.Compute({point.x+size.x, point.y,        point.z       });
        wv[2] = program.Compute({point.x,        point.y+size.y, point.z       });
        wv[3] = program.Compute({point.x,        point.y,        point.z+size.z});

        int flag = 0;
        for(auto& i: wv)
            if(i >= 0)
                flag++;

        vector<vector<double>> a{
            {point.y,        point.z,        wv[0], 1},
            {point.y,        point.z,        wv[1], 1},
            {point.y+size.y, point.z,        wv[2], 1},
            {point.y,        point.z+size.z, wv[3], 1},
        };
        vector<vector<double>> b{
            {point.x,        point.z,        wv[0], 1},
            {point.x+size.x, point.z,        wv[1], 1},
            {point.x,        point.z,        wv[2], 1},
            {point.x,        point.z+size.z, wv[3], 1},
        };
        vector<vector<double>> c{
            {point.x,        point.y,        wv[0], 1},
            {point.x+size.x, point.y,        wv[1], 1},
            {point.x,        point.y+size.y, wv[2], 1},
            {point.x,        point.y,        wv[3], 1},
        };
        vector<vector<double>> d{
            {point.x,        point.y,        point.z,        1},
            {point.x+size.x, point.y,        point.z,        1},
            {point.x,        point.y+size.y, point.z,        1},
            {point.x,        point.y,        point.z+size.z, 1},
        };
        vector<vector<double>> f{
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

        double div = sqrt(pow(detA, 2)+pow(detB, 2)+pow(detC, 2)+pow(detD, 2)+pow(detF, 2));

        adder(VoxelImageData(point, halfSize,{
                                 { MImageType::Cx, detA/div  },
                                 { MImageType::Cy, -detB/div },
                                 { MImageType::Cz, -detC/div },
                                 { MImageType::Cw, detD/div  },
                                 { MImageType::Ct, detF/div  }}));
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
