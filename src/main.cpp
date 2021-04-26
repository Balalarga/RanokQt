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
    Window window;
    window.Render();
    Lexer lexer;
    lexer.SetText(ReadFile("../data/NewFuncs/1_new.txt"));
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

    calculator = new RecursiveCalculator(5);

    thread cThread([calculator, &program, &window](){
        auto t1 = chrono::high_resolution_clock::now();
        auto voxels = calculator->Calculate(program, Zone::Zero);
        auto t2 = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<chrono::milliseconds>( t2 - t1 ).count()/1000.;

        cout << "Time: " << duration<<endl;
        for(auto& i: voxels)
        {
            window.AddCube(i.center, i.size, i.color);
        }
    });
    while(window.isOpen)
    {
        window.HandleEvent();
        window.Render();
    }
    cThread.join();
    delete calculator;
    return 0;
}
