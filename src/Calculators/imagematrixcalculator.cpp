#include "imagematrixcalculator.h"
#include <vector>

using namespace std;

ImageMatrixCalculator::ImageMatrixCalculator(Vector3 step):
    m_step(step)
{

}

const std::deque<ImageData> &ImageMatrixCalculator::Calculate(Program &program, ImageType type, std::function<void(ImageData&)> iterFunc)
{
    auto args = program.GetArgs();

    if(args.size() == 1)
        return matrix1(program, type, iterFunc);
    else if(args.size() == 2)
        return matrix2(program, type, iterFunc);
    else
        return matrix3(program, type, iterFunc);
}

const std::deque<ImageData> &ImageMatrixCalculator::matrix1(Program &program, ImageType type, std::function<void (ImageData &)> iterFunc)
{
    auto args = program.GetArgs();
    Vector3 size{
        (args[0].limits.second-args[0].limits.first)/m_step.x(),
                (args[0].limits.second-args[0].limits.first)/m_step.x(),
                (args[0].limits.second-args[0].limits.first)/m_step.x(),
    };
    Vector3 halfSize{
        size.x()/2.,
                size.x()/2.,
                size.x()/2.,
    };

    double x = args[0].limits.first + halfSize.x();
    while(x < args[0].limits.second)
    {
        vector<pair<Vector3, double>> values{
            {{ x+halfSize.x(), 0, 0 }, 0},
            {{ x-halfSize.x(), 0, 0 }, 0}
        };
        for(int i = 0; i < 2; i++)
            values[i].second = program.Compute(values[i].first);
        x+= size.x();
    }
    return *m_results;
}

const std::deque<ImageData> &ImageMatrixCalculator::matrix2(Program &program, ImageType type, std::function<void (ImageData &)> iterFunc)
{
    auto args = program.GetArgs();
    Vector3 size{
        (args[0].limits.second-args[0].limits.first)/m_step.x(),
                (args[1].limits.second-args[1].limits.first)/m_step.y(),
                (args[1].limits.second-args[1].limits.first)/m_step.y(),
    };
    Vector3 halfSize{
        size.x()/2.,
                size.y()/2.,
                size.y()/2.,
    };

    double x = args[0].limits.first + halfSize.x();
    while(x < args[0].limits.second)
    {
        double y = args[1].limits.first + halfSize.y();
        while(y < args[1].limits.second)
        {
            vector<pair<Vector3, double>> values{
                {{ x+halfSize.x(), y+halfSize.y(), 0 }, 0},
                {{ x+halfSize.x(), y-halfSize.y(), 0 }, 0},
                {{ x-halfSize.x(), y+halfSize.y(), 0 }, 0},
                {{ x-halfSize.x(), y-halfSize.y(), 0 }, 0}
            };
            for(int i = 0; i < 4; i++)
                values[i].second = program.Compute(values[i].first);

            y+= size.y();
        }
        x+= size.x();
    }
    return *m_results;
}

double det(vector<vector<double>> m)
{
    return m[0][3] * m[1][2] * m[2][1] * m[3][0] - m[0][2] * m[1][3] * m[2][1] * m[3][0] -
            m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][1] * m[1][3] * m[2][2] * m[3][0] +
            m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][1] * m[1][2] * m[2][3] * m[3][0] -
            m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][2] * m[1][3] * m[2][0] * m[3][1] +
            m[0][3] * m[1][0] * m[2][2] * m[3][1] - m[0][0] * m[1][3] * m[2][2] * m[3][1] -
            m[0][2] * m[1][0] * m[2][3] * m[3][1] + m[0][0] * m[1][2] * m[2][3] * m[3][1] +
            m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][1] * m[1][3] * m[2][0] * m[3][2] -
            m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][0] * m[1][3] * m[2][1] * m[3][2] +
            m[0][1] * m[1][0] * m[2][3] * m[3][2] - m[0][0] * m[1][1] * m[2][3] * m[3][2] -
            m[0][2] * m[1][1] * m[2][0] * m[3][3] + m[0][1] * m[1][2] * m[2][0] * m[3][3] +
            m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][0] * m[1][2] * m[2][1] * m[3][3] -
            m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][0] * m[1][1] * m[2][2] * m[3][3];
}

const std::deque<ImageData> &ImageMatrixCalculator::matrix3(Program &program, ImageType type, std::function<void (ImageData &)> iterFunc)
{
    auto args = program.GetArgs();
    Vector3 size{
        (args[0].limits.second-args[0].limits.first)/m_step.x(),
                (args[1].limits.second-args[1].limits.first)/m_step.y(),
                (args[2].limits.second-args[2].limits.first)/m_step.z(),
    };
    Vector3 halfSize{
        size.x()/2.,
                size.y()/2.,
                size.z()/2.,
    };
    vector<double> wv(4);
    double x = args[0].limits.first + halfSize.x();
    while(x < args[0].limits.second)
    {
        double y = args[1].limits.first + halfSize.y();
        while(y < args[1].limits.second)
        {
            double z = args[2].limits.first + halfSize.z();
            while(z < args[2].limits.second)
            {
                wv[0] = program.Compute({x, y, z});
                wv[1] = program.Compute({x+size.x(), y, z});
                wv[2] = program.Compute({x, y+size.y(), z});
                wv[3] = program.Compute({x, y, z+size.z()});

                int flag = 0;
                for(auto& i: wv)
                    if(i >= 0)
                        flag++;

                vector<vector<double>> a{
                    {y,          z,          wv[0], 1},
                    {y,          z,          wv[1], 1},
                    {y+size.y(), z,          wv[2], 1},
                    {y,          z+size.z(), wv[3], 1},
                };
                vector<vector<double>> b{
                    {x,          z,          wv[0], 1},
                    {x+size.x(), z,          wv[1], 1},
                    {x,          z,          wv[2], 1},
                    {x,          z+size.z(), wv[3], 1},
                };
                vector<vector<double>> c{
                    {x,          y,          wv[0], 1},
                    {x+size.x(), y,          wv[1], 1},
                    {x,          y+size.y(), wv[2], 1},
                    {x,          y,          wv[3], 1},
                };
                vector<vector<double>> d{
                    {x,          y,          z,          1},
                    {x+size.x(), y,          z,          1},
                    {x,          y+size.y(), z,          1},
                    {x,          y,          z+size.z(), 1},
                };
                vector<vector<double>> f{
                    {x,          y,          z,          wv[0]},
                    {x+size.x(), y,          z,          wv[1]},
                    {x,          y+size.y(), z,          wv[2]},
                    {x,          y,          z+size.z(), wv[3]},
                };

                double detA = det(a);
                double detB = det(b);
                double detC = det(c);
                double detD = det(d);
                double detF = det(f);

                double div = sqrt(pow(detA, 2)+pow(detB, 2)+pow(detC, 2)+pow(detD, 2)+pow(detF, 2));

                double nA = detA/div;
                double nB = -detB/div;
                double nC = -detC/div;
                double nD = detD/div;
                double nF = detF/div;

                float color;
                if(type == ImageType::Cx)
                {
                    color = (nA+1)/2.;
                }
                else if(type == ImageType::Cy)
                {
                    color = (nB+1)/2.;
                }
                else if(type == ImageType::Cz)
                {
                    color = (nC+1)/2.;
                }
                else if(type == ImageType::Cw)
                {
                    color = (nD+1)/2.;
                }
                else if(type == ImageType::Ct)
                {
                    color = (nF+1)/2.;
                }
                if(flag == 1)
                    m_results->push_back(ImageData({x, y, z}, halfSize, Color(color, color, 0, 0.5), wv[0]));
                else if(flag == 2)
                    m_results->push_back(ImageData({x, y, z}, halfSize, Color(color, 0, color, 0.5), wv[0]));
                else if(flag == 3)
                    m_results->push_back(ImageData({x, y, z}, halfSize, Color(0, 0, color, 0.5), wv[0]));
                else if(flag == 4)
                    m_results->push_back(ImageData({x, y, z}, halfSize, Color(0, color, color, 0.2), wv[0]));
                else
                    m_results->push_back(ImageData({x, y, z}, halfSize, Color(color, color, color, 0.2), wv[0]));
                if(iterFunc)
                    iterFunc(m_results->back());
                z+= size.z();
            }
            y+= size.y();
        }
        x+= size.x();
    }
    return *m_results;
}
