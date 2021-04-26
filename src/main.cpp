#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

#include "Lang/parser.h"
#include "Render/window.h"
#include "Calculators/recursivecalculator.h"
#include "Calculators/matrixcalculator.h"

using namespace std;

string ReadFile(const string& path)
{
    ifstream file(path);
    if(!file)
    {
        cout<< "File not found";
        return "";
    }
    stringstream stream;
    stream << file.rdbuf();
    file.close();
    return stream.str();
}

int main(int argc, char** argv)
{
    string sourceFiles[]{
        "1",        //0
        "1_new",    //1
        "Bone",     //2
        "Chainik",  //3
        "sphere",   //4
    };
    int fileId = 0;

    Window window;
    window.Render();
    Lexer lexer;
    lexer.SetText(ReadFile("../data/NewFuncs/"+sourceFiles[fileId]+".txt"));
    if(lexer.IsError())
    {
        cout<<lexer.GetError();
        return -1;
    }
    Parser parser(lexer);
    Program program = parser.GetProgram();
    if(program.IsError())
    {
        cout<<program.GetError();
        return -2;
    }
    auto lims = program.GetArgs()[0].limits;
    int zoom = lims.second-lims.first;
    window.SetZoom(zoom);
    BaseCalculator* calculator;

    calculator = new RecursiveCalculator(4);
    deque<VoxelData> voxels;
    if(false)
    {
        auto t1 = chrono::high_resolution_clock::now();
        voxels = calculator->Calculate(program, Zone::Zero);
        auto t2 = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<chrono::milliseconds>( t2 - t1 ).count()/1000.;

        cout << "Time: " << duration<<endl;
    }
    else
    {
        calculator->LoadDataFromFile(sourceFiles[fileId]+".vdata");
        voxels = calculator->GetResults();
    }
    calculator->SaveDataToFile(sourceFiles[fileId]+".vdata");
    for(auto& i: voxels)
    {
        window.AddCube(i.center, i.size, i.color);
    }
    while(window.isOpen)
    {
        window.HandleEvent();
        window.Render();
    }
    delete calculator;
    return 0;
}
