#include <QApplication>
#include <vector>
#include <fstream>
#include "AppWindow.h"

using namespace std;

bool ParseArguments(int argc, char *argv[])
{
    if(argc != 6)
        return false;

    std::string device = argv[1];
    std::string mode = argv[2];
    int recurDepth = stoi(argv[3]);
    std::string codeFile = argv[4];
    std::string resultFile = argv[5];

    Parser parser;
    parser.SetFile(codeFile);
    Program* program = parser.GetProgram();
    auto args = program->GetSymbolTable().GetAllArgs();
    SpaceBuilder::Instance().CreateSpace(args[0]->limits, args[1]->limits,
                                         args[2]->limits, recurDepth);
    fstream outputFile(resultFile);
    if(!outputFile)
    {
        cout<<"Result file("<<resultFile<<") error\n";
        return false;
    }
    if(mode == "Model")
    {
        vector<VoxelData> allData;
        auto modelAdder = [&allData](VoxelData data){
            allData.push_back(data);
        };
        if(device == "Cpu")
            SpaceCalculator::GetModel(*program, modelAdder);
        else
            OpenclGenerator::Instance().ComputeModel(*program, modelAdder);
        outputFile<<allData[0].size.x<<" "<<allData[0].size.y<<" "<<allData[0].size.z<<"\n";
        for(auto& v: allData)
        {
            outputFile<<v.position.x<<" "<<v.position.y<<" "<<v.position.z<<" "<<static_cast<int>(v.zone)<<"\n";
        }
    }
    else
    {
        vector<VoxelImageData> allData;
        auto imageAdder = [&allData](VoxelImageData data){
            allData.push_back(data);
        };
        if(device == "Cpu")
            SpaceCalculator::GetMImage(*program, imageAdder);
        else
            OpenclGenerator::Instance().ComputeImage(*program, imageAdder);

        outputFile<<allData[0].size.x<<" "<<allData[0].size.y<<" "<<allData[0].size.z<<"\n";
        for(auto& v: allData)
        {
            outputFile<<v.position.x<<" "<<v.position.y<<" "<<v.position.z<<"\n";
            for(auto& i: v.images)
            {
                outputFile<<static_cast<int>(i.first)<<" "<<i.second<<"\n";
            }
        }
    }

    outputFile.close();
    delete program;

    return true;
}


//     0       1    2   3     4              5
// ./ranok_nd Cpu Model 5 sourceFile.txt resFile.txt
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(ParseArguments(argc, argv))
    {
        a.exit(0);
        return 0;
    }

    AppWindow w;
    w.resize(1280, 720);

    qApp->installEventFilter(&w);

    w.show();
    return a.exec();
}
