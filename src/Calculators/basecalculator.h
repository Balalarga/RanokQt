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

// Информация по расчету точки функции
struct VoxelData
{
    VoxelData(){};
    VoxelData(const sf::Vector3<double>& center, const sf::Vector3<double>& size,
              const sf::Color& color, Zone zone, std::vector<std::pair<sf::Vector3<double>, double>> values = {},
              int dementions = 3):
        center(center), size(size), color(color), values(values),
        dementions(dementions)
    {}
    sf::Vector3<double> center;
    sf::Vector3<double> size;
    sf::Color color;
    std::vector<std::pair<sf::Vector3<double>, double>> values; // Значения
    Zone zone;
    int dementions;
};

// Базовый класс калькулятор по зонам
class BaseCalculator
{
public:
    BaseCalculator();
    virtual ~BaseCalculator();
    // В функцию передается: программа расчета, требуемая зона, и callback для итеративной отрисовки
    virtual const std::deque<VoxelData>& Calculate(Program& program, Zone zone, std::function<void(VoxelData&)> = nullptr) = 0;
    const std::deque<VoxelData>& GetResults();
    void SaveDataToFile(std::string file);
    void LoadDataFromFile(std::string file);
    void SetVoxelColor(sf::Color color);
    sf::Color GetVoxelColor();

protected:
    // посчитанные данные модели
    std::deque<VoxelData>* m_results;
    sf::Color baseColor{255, 255, 255, 120};

    // Получение зоны вокселя по его узловым значениям
    Zone GetZone(const std::vector<std::pair<sf::Vector3<double>, double> > &values) const;
};

#endif // BASECALCULATOR_H
