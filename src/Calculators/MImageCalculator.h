#ifndef MImageCalculator_H
#define MImageCalculator_H

#include <deque>

#include <QColor>

#include "ICalculator.h"


enum class MImageType
{
    Cx, Cy, Cz, Cw, Ct
};
using MImageData = std::vector<std::pair<MImageType, double>>;


class MImageCalculator: public ICalculator
{
    Q_OBJECT

public:
    MImageCalculator(QObject* parent = nullptr):
        ICalculator(parent),
        m_type(MImageType::Cx)
    {}


    inline void SetType(MImageType type){ m_type = type; }
    inline MImageType GetType(){ return m_type; }


    inline void AddImageData(MImageData data){ m_imagesData.push_back(data); }


    void SaveDataToFile(std::string file) override {}
    void LoadDataFromFile(std::string file) override {}


private:
    std::vector<MImageData> m_imagesData;
    MImageType m_type;
};

#endif // MImageCalculator_H
