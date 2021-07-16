#include <QApplication>
#include <vector>
#include <fstream>
#include "AppWindow.h"

using namespace std;

bool ParseArguments(int argc, char *argv[])
{
    if(argc != 6)
        return false;
    cout<<"Start compute\n";

    std::string device = argv[1];
    std::string mode = argv[2];
    int recurDepth = stoi(argv[3]);
    std::string codeFile = argv[4];



    std::string resultFile = argv[5];
    if(mode == "Model")
        resultFile += ".zdt";
    else if(mode == "Mimage")
        resultFile += ".idt";

    Parser parser;
    parser.SetFile(codeFile);
    Program* program = parser.GetProgram();
    auto args = program->GetSymbolTable().GetAllArgs();
    auto space = SpaceBuilder::Instance().CreateSpace(args[0]->limits, args[1]->limits,
                                         args[2]->limits, recurDepth);
    ofstream outputFile(resultFile, ios_base::binary);
    if(!outputFile)
    {
        cout<<"Result file("<<resultFile<<") error\n";
        return false;
    }

    outputFile<<space->startPoint.x<<space->startPoint.y<<space->startPoint.z;
    outputFile<<space->pointSize.x<<space->pointSize.y<<space->pointSize.z;
    outputFile<<space->GetSize();
    if(mode == "Model")
    {
        if(device == "Cpu")
            SpaceCalculator::Get().GetModel(*program);
        else
            OpenclCalculator::Get().ComputeModel(*program);

        for(int i = 0; i < space->GetSize(); ++i)
        {
            outputFile<<space->zoneData->At(i);
        }
    }
    else if(mode == "Mimage")
    {
        if(device == "Cpu")
            SpaceCalculator::Get().GetMImage(*program);
        else
            OpenclCalculator::Get().ComputeImage(*program);

        for(int i = 0; i < space->GetSize(); ++i)
        {
            outputFile<<space->mimageData->At(i).Cx;
            outputFile<<space->mimageData->At(i).Cy;
            outputFile<<space->mimageData->At(i).Cz;
            outputFile<<space->mimageData->At(i).Cw;
            outputFile<<space->mimageData->At(i).Ct;
        }
    }
    cout<<"Finish\n";
    outputFile.close();
    delete program;

    return true;
}


//     0       1              2              3             4               5
// ./ranok_nd {Cpu|Gpu} {Model|Mimage} {recurDepth} {sourceFile.txt} {resultFile}
int main(int argc, char *argv[])
{

    if(ParseArguments(argc, argv))
    {
        return 0;
    }
    cout<<"You can use me like: \n./ranok_nd {Cpu|Gpu} {Model|Mimage} {recurDepth} {sourceFile.txt} {resultFileName}\n";
    static_assert (sizeof(cl_double) == sizeof(double), "Type size error cl_double != double");

    QApplication a(argc, argv);

    AppWindow w;
    w.resize(1280, 720);

    qApp->installEventFilter(&w);

    w.show();
    return a.exec();
}
