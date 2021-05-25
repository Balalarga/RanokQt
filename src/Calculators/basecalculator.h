#ifndef BASECALCULATOR_H
#define BASECALCULATOR_H

#include <deque>
#include <vector>
#include <functional>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include "Language/Program.h"

enum class Zone
{
    Zero,
    Positive,
    Negative
};

struct ZoneFlags
{
    bool plus = false;
    bool zero = false;
    bool minus = false;
};

struct VoxelData
{
    VoxelData(){};
    VoxelData(const sf::Vector3<double>& center, const sf::Vector3<double>& size,
              const sf::Color& color, std::vector<std::pair<sf::Vector3<double>, double>> values = {},
              int dementions = 3):
        center(center), size(size), color(color), values(values),
        dementions(dementions)
    {}
    sf::Vector3<double> center;
    sf::Vector3<double> size;
    sf::Color color;
    std::vector<std::pair<sf::Vector3<double>, double>> values;
    int dementions;
};

class BaseCalculator
{
public:
    BaseCalculator();
    virtual ~BaseCalculator();
    virtual const std::deque<VoxelData>& Calculate(Program& program, Zone zone, std::function<void(VoxelData&)> = nullptr) = 0;
    const std::deque<VoxelData>& GetResults();
    void SaveDataToFile(std::string file);
    void LoadDataFromFile(std::string file);
    void SetVoxelColor(sf::Color color);
    sf::Color GetVoxelColor();

protected:
    std::deque<VoxelData>* m_results;
    sf::Color baseColor{255, 255, 255, 120};

    bool CheckZone(Zone zone, ZoneFlags flags) const;
    ZoneFlags GetZoneFlags(const std::vector<std::pair<sf::Vector3<double>, double> > &values);
};

#endif // BASECALCULATOR_H
