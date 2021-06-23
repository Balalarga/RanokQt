#include "MImageMatrixCalculator.h"
#include <vector>

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


MImageMatrixCalculator::MImageMatrixCalculator(Vector3i step, QObject *parent):
    MImageCalculator(parent),
    m_step(step)
{

}

void MImageMatrixCalculator::Calculate()
{
    ClearResults();
    auto args = GetProgram()->GetSymbolTable().GetAllArgs();

    if(args.size() == 1)
        Calculate1d();
    else if(args.size() == 2)
        Calculate2d();
    else
        Calculate3d();
}

void MImageMatrixCalculator::Calculate1d()
{
    auto program = GetProgram();
    auto args = program->GetSymbolTable().GetAllArgs();
    Vector3f size{
        static_cast<float>((args[0]->limits.second-args[0]->limits.first)/m_step.x),
                static_cast<float>((args[0]->limits.second-args[0]->limits.first)/m_step.x),
                static_cast<float>((args[0]->limits.second-args[0]->limits.first)/m_step.x),
    };
    Vector3f halfSize{
        size.x/2.f,
                size.x/2.f,
                size.x/2.f,
    };

    double x = args[0]->limits.first + halfSize.x;
    while(x < args[0]->limits.second)
    {
        vector<pair<Vector3d, double>> values{
            {{ x+halfSize.x, 0, 0 }, 0},
            {{ x-halfSize.x, 0, 0 }, 0}
        };
        for(int i = 0; i < 2; i++)
            values[i].second = program->Compute(values[i].first);
        x+= size.x;
    }
}

void MImageMatrixCalculator::Calculate2d()
{
    auto program = GetProgram();
    auto args = program->GetSymbolTable().GetAllArgs();
    Vector3f size{
        static_cast<float>((args[0]->limits.second-args[0]->limits.first)/m_step.x),
                static_cast<float>((args[1]->limits.second-args[1]->limits.first)/m_step.y),
                static_cast<float>((args[1]->limits.second-args[1]->limits.first)/m_step.y),
    };
    Vector3f halfSize{
        size.x/2.f,
                size.y/2.f,
                size.y/2.f,
    };

    vector<double> zv(3);
    float x = args[0]->limits.first + halfSize.x;
    while(x < args[0]->limits.second)
    {
        float y = args[1]->limits.first + halfSize.y;
        while(y < args[1]->limits.second)
        {
            zv[0] = program->Compute({x, y, 0});
            zv[1] = program->Compute({x+size.x, y, 0});
            zv[2] = program->Compute({x, y+size.y, 0});

            int flag = 0;
            for(auto& i: zv)
                if(i >= 0)
                    flag++;

            vector<vector<double>> a{
                {y,        zv[0], 1},
                {y,        zv[1], 1},
                {y+size.y, zv[2], 1},
            };
            vector<vector<double>> b{
                {x,        zv[0], 1},
                {x+size.x, zv[1], 1},
                {x,        zv[2], 1},
            };
            vector<vector<double>> c{
                {x,        y,        1},
                {x+size.x, y,        1},
                {x,        y+size.y, 1},
            };
            vector<vector<double>> d{
                {x,        y,        zv[0]},
                {x+size.x, y,        zv[1]},
                {x,        y+size.y, zv[2]},
            };

            double detA = -determinantOfMatrix(a, 3);
            double detB = -determinantOfMatrix(b, 3);
            double detC = determinantOfMatrix(c, 3);
            double detD = -determinantOfMatrix(d, 3);

            detA *= 100;
            detB *= 100;

            double div = sqrt(pow(detA, 2)+pow(detB, 2)+pow(detC, 2)+pow(detD, 2));

            MImageData normals{
                { MImageType::Cx, detA/div},
                { MImageType::Cy, detB/div},
                { MImageType::Cz, detC/div},
                { MImageType::Cw, detD/div},
                { MImageType::Ct, detD/div}
            };

            int color;
            auto type = GetType();
            if(type == MImageType::Cx)
            {
                color = 255*(normals[0].second+1)/2.;
            }
            else if(type == MImageType::Cy)
            {
                color = 255*(normals[1].second+1)/2.;
            }
            else if(type == MImageType::Cz)
            {
                color = 255*(normals[2].second+1)/2.;
            }
            else if(type == MImageType::Cw)
            {
                color = 255*(normals[3].second+1)/2.;
            }
            else if(type == MImageType::Ct)
            {
                color = 255*(normals[4].second+1)/2.;
            }
            VoxelData data;
            if(flag < 2)
                data = VoxelData({x, y, 0}, halfSize, QColor(color, 0, color, 255));
            else
                data = VoxelData({x, y, 0}, halfSize, QColor(color, color, color, 255));
            AddVoxel(data);
            if(m_addVoxelFunc)
                m_addVoxelFunc(data);
            AddImageData(normals);
            y+= size.y;
        }
        x+= size.x;
    }
}

void MImageMatrixCalculator::Calculate3d()
{
    auto program = GetProgram();
    auto args = program->GetSymbolTable().GetAllArgs();
    Vector3f size{
        static_cast<float>((args[0]->limits.second-args[0]->limits.first)/m_step.x),
                static_cast<float>((args[1]->limits.second-args[1]->limits.first)/m_step.y),
                static_cast<float>((args[2]->limits.second-args[2]->limits.first)/m_step.z),
    };
    Vector3f halfSize{
        size.x/2.f,
                size.y/2.f,
                size.z/2.f,
    };
    vector<double> wv(4);
    float x = args[0]->limits.first + halfSize.x;
    while(x < args[0]->limits.second)
    {
        float y = args[1]->limits.first + halfSize.y;
        while(y < args[1]->limits.second)
        {
            float z = args[2]->limits.first + halfSize.z;
            while(z < args[2]->limits.second)
            {
                wv[0] = program->Compute({x, y, z});
                wv[1] = program->Compute({x+size.x, y, z});
                wv[2] = program->Compute({x, y+size.y, z});
                wv[3] = program->Compute({x, y, z+size.z});

                int flag = 0;
                for(auto& i: wv)
                    if(i >= 0)
                        flag++;

                vector<vector<double>> a{
                    {y,          z,        wv[0], 1},
                    {y,          z,        wv[1], 1},
                    {y+size.y,   z,        wv[2], 1},
                    {y,          z+size.z, wv[3], 1},
                };
                vector<vector<double>> b{
                    {x,          z,        wv[0], 1},
                    {x+size.x,   z,        wv[1], 1},
                    {x,          z,        wv[2], 1},
                    {x,          z+size.z, wv[3], 1},
                };
                vector<vector<double>> c{
                    {x,          y,        wv[0], 1},
                    {x+size.x,   y,        wv[1], 1},
                    {x,          y+size.y, wv[2], 1},
                    {x,          y,        wv[3], 1},
                };
                vector<vector<double>> d{
                    {x,          y,        z,        1},
                    {x+size.x,   y,        z,        1},
                    {x,          y+size.y, z,        1},
                    {x,          y,        z+size.z, 1},
                };
                vector<vector<double>> f{
                    {x,          y,        z,          wv[0]},
                    {x+size.x,   y,        z,          wv[1]},
                    {x,          y+size.y, z,          wv[2]},
                    {x,          y,        z+size.z,   wv[3]},
                };

                double detA = determinantOfMatrix(a, 4);
                double detB = determinantOfMatrix(b, 4);
                double detC = determinantOfMatrix(c, 4);
                double detD = determinantOfMatrix(d, 4);
                double detF = determinantOfMatrix(f, 4);

                double div = sqrt(pow(detA, 2)+pow(detB, 2)+pow(detC, 2)+pow(detD, 2)+pow(detF, 2));

                MImageData normals{
                    { MImageType::Cx, detA/div  },
                    { MImageType::Cy, -detB/div },
                    { MImageType::Cz, -detC/div },
                    { MImageType::Cw, detD/div  },
                    { MImageType::Ct, detF/div  }
                };

                unsigned color;
                auto type = GetType();
                if(type == MImageType::Cx)
                {
                    color = 255*(normals[0].second+1)/2.;
                }
                else if(type == MImageType::Cy)
                {
                    color = 255*(normals[1].second+1)/2.;
                }
                else if(type == MImageType::Cz)
                {
                    color = 255*(normals[2].second+1)/2.;
                }
                else if(type == MImageType::Cw)
                {
                    color = 255*(normals[3].second+1)/2.;
                }
                else if(type == MImageType::Ct)
                {
                    color = 255*(normals[4].second+1)/2.;
                }
                AddVoxel(VoxelData({x, y, z}, halfSize, QColor(color, color, color, 100)));
                if(m_addVoxelFunc)
                    m_addVoxelFunc(VoxelData({x, y, z}, halfSize, QColor(color, color, color, 100)));
                AddImageData(normals);
                z+= size.z;
            }
            y+= size.y;
        }
        x+= size.x;
    }
}

