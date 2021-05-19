#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

#include "Lang/parser.h"
#include "Render/window.h"
#include "Calculators/recursivecalculator.h"
#include "Calculators/matrixcalculator.h"

using namespace std;

int main(int argc, char** argv)
{
    Window window;
    window.Render();

    string sourceFiles[]{
        "1",        //0
        "1_new",    //1
        "Bone",     //2
        "Chainik",  //3
        "sphere",   //4
    };
    int fileId = 0;

    Parser parser("../data/NewFuncs/"+sourceFiles[fileId]+".txt");
    Program program = parser.GetProgram();

    if(program.IsError())
    {
        cout<<program.GetError();
        return -2;
    }

    auto lims = program.GetArgs()[0].limits;
    int zoom = lims.second-lims.first;
    BaseCalculator* calculator;
//    calculator = new RecursiveCalculator(4);
    calculator = new MatrixCalculator({40, 40, 40});
    deque<VoxelData> voxels;

    auto t1 = chrono::high_resolution_clock::now();
    voxels = calculator->Calculate(program, Zone::Zero);
    auto t2 = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>( t2 - t1 ).count()/1000.;

    cout << "Time: " << duration<<endl;

    window.SetZoom(zoom);

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
