#include <iostream>

#include "Calculators/MatrixCalculator.h"
#include "Calculators/RecursiveCalculator.h"
#include "Calculators/MImageMatrixCalculator.h"
#include "Language/Parser.h"
#include "Render/OpenglSystem.h"
#include "Render/MainWindow.h"
#include "Render/Cube.h"
#include "Render/Square.h"

using namespace std;

// Обертки для удобства
struct ModelData
{
    Zone zone;
    BaseCalculator* calculator;
};
struct ImageCalcData
{
    ImageType type;
    MImageCalculator* calculator;
};

const char* files[]{
    "NewFuncs/1",       //0
    "NewFuncs/1_new",   //1
    "NewFuncs/2dspace", //2
    "NewFuncs/Bone",    //3
    "NewFuncs/Chainik", //4
    "NewFuncs/euclid",  //5
    "NewFuncs/sphere",  //6
    "NewFuncs/TeaCup",  //7
};
int fileId = 6;


const char* imageTypes[]{
    "Cx",
    "Cy",
    "Cz",
    "Cw",
    "Ct"
};
int imageTypeId = 0;

const char* calculators[]{
    "Recursive",
    "Matrix"
};
int calculatorId = 0;
int recurDepth = 5;
int matrSize = 20;
ImageType imageType = ImageType::Cx;
bool paramsChanged = false;

ModelData runData{
    Zone::Zero,
    new RecursiveCalculator(recurDepth)
};

ImageCalcData imageData{
    imageType,
    new MImageMatrixCalculator({matrSize, matrSize, matrSize})
};
string sourceFile = "../examples/"+string(files[fileId])+".txt";
bool mode = false; // true - model, false - image

sf::Thread* modelThread = nullptr;
sf::Thread* imageThread = nullptr;

MainWindow* mainWindow;

ImageType FromString(string type)
{
    if(type == "Cx")
        return ImageType::Cx;
    if(type == "Cy")
        return ImageType::Cy;
    if(type == "Cz")
        return ImageType::Cz;
    if(type == "Cw")
        return ImageType::Cw;
    return ImageType::Ct;
}

// Остановка расчетов
void ResetThreads()
{
    if(modelThread)
    {
        modelThread->terminate();
        delete modelThread;
        modelThread = nullptr;
    }
    if(imageThread)
    {
        imageThread->terminate();
        delete imageThread;
        imageThread = nullptr;
    }
}

// Функция расчета воксельной модели
void ModelCalculating(ModelData data)
{
    Parser parser(sourceFile);
    Program program = parser.GetProgram();
    auto voxelData = data.calculator->Calculate(program, data.zone,[](VoxelData& voxel)
    {
        if(voxel.dementions == 3)
            mainWindow->AddObject(new Cube(voxel.center, voxel.size, voxel.color));
        if(voxel.dementions == 2)
            mainWindow->AddObject(new Square(voxel.center, voxel.size, voxel.color));
        if(voxel.dementions == 1)
            mainWindow->AddObject(new Square(voxel.center, voxel.size, voxel.color));
    });
    data.calculator->SaveDataToFile("Res.model");
}

// Функция расчета м-образов
void ImageCalculating(ImageCalcData data)
{
    Parser parser(sourceFile);
    Program program = parser.GetProgram();
    auto voxelData = data.calculator->Calculate(program, data.type, [](MImageData& voxel)
    {
        if(voxel.dementions == 3)
            mainWindow->AddObject(new Cube(voxel.center, voxel.size, voxel.color));
        if(voxel.dementions == 2)
            mainWindow->AddObject(new Square(voxel.center, voxel.size, voxel.color));
        if(voxel.dementions == 1)
            mainWindow->AddObject(new Square(voxel.center, voxel.size, voxel.color));
    });
    data.calculator->SaveToFile("Res.mimage");
}

// Запуск расчетов
void RunThreads()
{
    ResetThreads();
    mainWindow->ClearObjects();
    auto recreate = [](bool recur){
        if(recur)
        {
            delete runData.calculator;
            runData.calculator = new RecursiveCalculator(recurDepth);
        }
        else
        {
            delete runData.calculator;
            runData.calculator = new MatrixCalculator({matrSize, matrSize, matrSize});
            delete imageData.calculator;
            imageData.calculator = new MImageMatrixCalculator({matrSize, matrSize, matrSize});
        }
    };

    if(dynamic_cast<MatrixCalculator*>(runData.calculator) &&
            !strcmp(calculators[calculatorId], "Recursive"))
        recreate(true);
    else if(dynamic_cast<RecursiveCalculator*>(runData.calculator) &&
            !strcmp(calculators[calculatorId], "Matrix"))
        recreate(false);
    else if(paramsChanged)
        recreate(strcmp(calculators[calculatorId], "Matrix"));

    imageData.type = FromString(imageTypes[imageTypeId]);
    modelThread = new sf::Thread(&ModelCalculating, runData);
    imageThread = new sf::Thread(&ImageCalculating, imageData);
    if(!mode)
        modelThread->launch();
    else
        imageThread->launch();
}

// Функция, описывающая элементы меню и их действия
void MenuConfig()
{
    ImGui::Begin("Settings");
    ImGui::SetWindowSize({220, 200});
    if (ImGui::BeginCombo("File", files[fileId]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(files); n++)
        {
            const bool is_selected = (fileId == n);
            if (ImGui::Selectable(files[n], is_selected))
            {
                fileId = n;
                sourceFile = "../examples/"+string(files[fileId])+".txt";
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if(mode)
        calculatorId = 1;

    if (ImGui::BeginCombo("Calculator", calculators[calculatorId]))
    {
        if(!mode)
        {
            for (int n = 0; n < IM_ARRAYSIZE(calculators); n++)
            {
                const bool is_selected = (calculatorId == n);
                if (ImGui::Selectable(calculators[n], is_selected))
                {
                    calculatorId = n;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if(!strcmp(calculators[calculatorId], "Recursive") && !mode)
    {
        if(ImGui::SliderInt("Depth", &recurDepth, 1, 10))
            paramsChanged = true;
    }
    else
    {
        if(ImGui::SliderInt("Blocks", &matrSize, 1, 200))
            paramsChanged = true;
    }
    if(mode)
    {
        if (ImGui::BeginCombo("Image type", imageTypes[imageTypeId]))
        {
            for (int n = 0; n < IM_ARRAYSIZE(imageTypes); n++)
            {
                const bool is_selected = (imageTypeId == n);
                if (ImGui::Selectable(imageTypes[n], is_selected))
                {
                    imageTypeId = n;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
    ImGui::Checkbox("Show image", &mode);
    if (ImGui::Button("Start"))
        RunThreads();
    if (ImGui::Button("Stop"))
        ResetThreads();
    ImGui::End();
}

int main(int argc, char** argv)
{
    mainWindow = new MainWindow();
    OpenglSystem::Instance().Init();
    mainWindow->CreateMenu(&MenuConfig);

    mainWindow->Show();

    ResetThreads();

    delete runData.calculator;
    delete imageData.calculator;
    delete mainWindow;

    ImGui::SFML::Shutdown();
    OpenglSystem::Destroy();

    return 0;
}
