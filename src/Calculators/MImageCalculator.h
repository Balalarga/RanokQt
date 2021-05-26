#ifndef MImageCalculator_H
#define MImageCalculator_H

#include <deque>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Language/Program.h"


enum class ImageType
{
    Cx, Cy, Cz, Cw, Ct
};

// Информация о точке образа
struct MImageData
{
    MImageData(){};
    MImageData(const sf::Vector3<double>& center,
               const sf::Vector3<double>& size,
               const sf::Color& color, double value,
               const std::vector<double>& normals,
              int dementions = 3):
        center(center), size(size), color(color),
        normals(normals), value(value),
        dementions(dementions)
    {}
    sf::Vector3<double> center;
    sf::Vector3<double> size;
    sf::Color color;
    std::vector<double> normals;
    double value;
    int dementions;
};

// Базовый класс для расчета образов
class MImageCalculator
{
public:
    MImageCalculator(){m_results = new std::deque<MImageData>();}
    virtual ~MImageCalculator(){delete m_results;}
    virtual const std::deque<MImageData>& Calculate(Program& program, ImageType type, std::function<void(MImageData&)> iterFunc = nullptr) = 0;

    void SaveToFile(std::string file)
    {
        std::ofstream out(file);
        if(!out)
        {
            std::cout<<"Couldn't open file "<<file<<std::endl;
            return;
        }

        for(auto& i: *m_results)
        {
            out<<i.center.x<<' '<<i.center.y<<' '<<i.center.z<<' ';
            out<<i.size.x<<' '<<i.size.y<<' '<<i.size.z<<' ';
            for(auto& n: i.normals)
                out<<n<<' ';
            out<<'\n';
        }

        out.close();
    }
    void LoadFromFile(std::string file)
    {
        std::ifstream in(file);
        if(!in)
        {
            std::cout<<"Couldn't open file "<<file<<std::endl;
            return;
        }
        MImageData data;
        std::vector<double> normals;
        std::string line;
        while(!in.eof())
        {
            std::getline(in, line);
            std::stringstream lineStream(line);
            lineStream >> data.center.x >> data.center.y >> data.center.z;
            lineStream >> data.size.x >> data.size.y >> data.size.z;
            lineStream >> data.color.r >> data.color.g >> data.color.b >> data.color.a;
            double n;
            while(!lineStream.eof())
            {
                lineStream >> n;
                normals.push_back(n);
            }
            data.normals = normals;
            m_results->push_back(data);
        }
        in.close();
    }

protected:
    std::deque<MImageData>* m_results;
};

#endif // MImageCalculator_H
