#include <iostream>

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
        "lopatka",  //5
    };
    int fileId = 5;

    Parser parser("../data/NewFuncs/"+sourceFiles[fileId]+".txt");
    Program program = parser.GetProgram();

    if(program.IsError())
    {
        cout<<program.GetError();
        return -2;
    }

    program.PrintTreeDepth(-1);

    auto lims = program.GetArgs()[0].limits;
    int zoom = lims.second-lims.first;
    BaseCalculator* calculator;
    calculator = new RecursiveCalculator(4);
//    calculator = new MatrixCalculator({40, 40, 40});
    deque<VoxelData> voxels;

    auto t1 = chrono::high_resolution_clock::now();
    voxels = calculator->Calculate(program, Zone::Zero);
    auto t2 = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>( t2 - t1 );

    cout << "Time: " << duration.count()/1000.<<endl;

    window.SetZoom(zoom);

    VoxelData choice;
    bool wasChoosed1 = false;
    bool wasChoosed2 = false;
    for(auto& i: voxels)
    {
        if((!wasChoosed1 || !wasChoosed2) && i.center.z > -1 && i.center.z < 1)
        {
            if(!wasChoosed1)
            {
                i.color.r = 255;
                wasChoosed1 = true;
                choice = i;
                cout<<i.center.x<<", "<<i.center.y<<", "<<i.center.z<<endl;
                for(auto& j: i.values)
                {
                    cout<<"\t";
                    cout<<"("<<j.first.x<<", "<<j.first.y<<", "<<j.first.z<<") = "<<j.second<<endl;
                }
            }
            else if(!wasChoosed2 && choice.center.z == -i.center.z)
            {
                i.color.r = 255;
                wasChoosed2 = true;
                cout<<i.center.x<<", "<<i.center.y<<", "<<i.center.z<<endl;
                for(auto& j: i.values)
                {
                    cout<<"\t";
                    cout<<"("<<j.first.x<<", "<<j.first.y<<", "<<j.first.z<<") = "<<j.second<<endl;
                }
            }
        }
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
