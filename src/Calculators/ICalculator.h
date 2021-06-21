#ifndef ICALCULATOR_H
#define ICALCULATOR_H

#include <vector>
#include <functional>

#include <QObject>

#include "VoxelData.h"
#include "Language/Program.h"


class ICalculator: public QObject
{
    Q_OBJECT

public slots:
    virtual void Calculate() = 0;

public:
    ICalculator(QObject* parent = nullptr):
        QObject(parent){}
    virtual ~ICalculator(){}


    virtual void SaveDataToFile(std::string file) = 0;
    virtual void LoadDataFromFile(std::string file) = 0;


    inline Program* GetProgram(){ return m_program; }
    inline void SetProgram(Program* program){ m_program = program; }


    inline std::vector<VoxelData>& GetResults(){ return m_voxelData;}
    inline void AddVoxel(VoxelData data){
        m_voxelData.push_back(data);
    }
    inline void ClearResults(){ m_voxelData.clear(); };

    inline void SetVoxelColor(QColor color){ m_voxelColor = color; }
    inline QColor GetVoxelColor(){ return m_voxelColor; }

    void SetAddVoxelFunc(std::function<void(VoxelData)> addVoxel)
    {
        m_addVoxelFunc = addVoxel;
    }

    void operator()()
    {
        Calculate();
    }

protected:
    std::function<void(VoxelData)> m_addVoxelFunc = 0;

private:
    Program* m_program{nullptr};
    QColor m_voxelColor{255, 255, 255, 20};
    std::vector<VoxelData> m_voxelData;
};

#endif // ICALCULATOR_H
