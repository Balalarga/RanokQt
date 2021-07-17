#include <QApplication>
#include <vector>
#include <fstream>
#include "Space/Calculators/OpenclCalculator.h"
#include "Space/Calculators/CommonCalculator.h"
#include "AppWindow.h"

using namespace std;

//./Ranok {cpu|gpu} {model|mimage} {recurDepth} {sourceFile.txt} {resultFileName}
void ParseArguments(int argc, char *argv[])
{
//    cout<<"Start compute\n";

//    QString device = QString(argv[1]).toLower();
//    QString mode = QString(argv[2]).toLower();
//    int recurDepth = stoi(argv[3]);
//    QString codeFile = argv[4];
//    QString resultFile = argv[5];

//    if(mode == "model")
//        resultFile += ".zdt";
//    else if(mode == "mimage")
//        resultFile += ".idt";

//    Parser parser;
//    parser.SetFile(codeFile.toStdString());
//    Program* program = parser.GetProgram();
//    auto args = program->GetSymbolTable().GetAllArgs();
//    auto space = SpaceBuilder::Instance().CreateSpace(args[0]->limits, args[1]->limits,
//            args[2]->limits, recurDepth);
//    ofstream outputFile(resultFile.toStdString(), ios_base::binary);
//    if(!outputFile)
//    {
//        cout<<"Result file("<<resultFile.toStdString()<<") error\n";
//        return;
//    }

//    ISpaceCalculator* calcualtor;
//    if(device == "gpu")
//        calcualtor = new OpenclCalculator;
//    else
//        calcualtor = new CommonCalculator;

//    calcualtor->SetCalculatorMode(mode == "mimage" ? CalculatorMode::Mimage : CalculatorMode::Model);

//    outputFile<<space->startPoint.x<<space->startPoint.y<<space->startPoint.z;
//    outputFile<<space->pointSize.x<<space->pointSize.y<<space->pointSize.z;
//    outputFile<<space->GetSize();
//    if(mode == "model")
//    {
//        if(device == "cpu")
//            SpaceCalculator::Get().GetModel(*program);
//        else if(device == "gpu")
//            OpenclCalculator::Get().ComputeModel(*program);
//        else
//            return;
//        for(int i = 0; i < space->GetSize(); ++i)
//        {
//            outputFile<<space->zoneData->At(i);
//        }
//    }
//    else if(mode == "mimage")
//    {
//        if(device == "cpu")
//            SpaceCalculator::Get().GetMImage(*program);
//        else if(device == "gpu")
//            OpenclCalculator::Get().ComputeImage(*program);
//        else
//            return;

//        for(int i = 0; i < space->GetSize(); ++i)
//        {
//            outputFile<<space->mimageData->At(i).Cx;
//            outputFile<<space->mimageData->At(i).Cy;
//            outputFile<<space->mimageData->At(i).Cz;
//            outputFile<<space->mimageData->At(i).Cw;
//            outputFile<<space->mimageData->At(i).Ct;
//        }
//    }
//    cout<<"Finish\n";
//    outputFile.close();
//    delete program;
//    SpaceBuilder::Instance().DeleteSpace();

//    return;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    static_assert (sizeof(cl_double) == sizeof(double), "Type size error cl_double != double");

    if(argc == 6)
        ParseArguments(argc, argv);
    else
    {
        AppWindow w;
        w.resize(1280, 720);

        qApp->installEventFilter(&w);

        w.show();
        return a.exec();
    }
    return a.exec();
}
