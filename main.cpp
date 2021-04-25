#include <iostream>
#include <fstream>
#include <sstream>
#include <future>
#include <thread>
#include <chrono>
#include "Lang/parser.h"
#include "Render/window.h"

using namespace std;

ofstream file;

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

inline double roundTill3( double val )
{
    return val;
    if( val < 0 ) return ceil(val - 0.5);
    return floor(val + 0.5);
}

string Vec3ToStr(glm::vec3 vec)
{
    return "("+to_string(vec.x)+", "+to_string(vec.y)+", "+to_string(vec.z)+")";
}

void Save(glm::vec3 point, double value)
{
    file<<Vec3ToStr({ point.x, point.y, point.z })<<" = "<<value<<endl;
}

int counter = 0;
void recursiveComputing(Program& program, Window& window, glm::vec3 coords, glm::vec3 size, int step)
{
    if(step == 1)
    {
        double currentValues[8];
        currentValues[0] = program.Compute({ coords.x+size.x, coords.y+size.y, coords.z+size.z });
        currentValues[1] = program.Compute({ coords.x+size.x, coords.y+size.y, coords.z-size.z });
        currentValues[2] = program.Compute({ coords.x+size.x, coords.y-size.y, coords.z+size.z });
        currentValues[3] = program.Compute({ coords.x+size.x, coords.y-size.y, coords.z-size.z });
        currentValues[4] = program.Compute({ coords.x-size.x, coords.y+size.y, coords.z+size.z });
        currentValues[5] = program.Compute({ coords.x-size.x, coords.y+size.y, coords.z-size.z });
        currentValues[6] = program.Compute({ coords.x-size.x, coords.y-size.y, coords.z+size.z });
        currentValues[7] = program.Compute({ coords.x-size.x, coords.y-size.y, coords.z-size.z });

        Save({ coords.x+size.x, coords.y+size.y, coords.z+size.z }, currentValues[0]);
        Save({ coords.x+size.x, coords.y-size.y, coords.z+size.z }, currentValues[1]);
        Save({ coords.x+size.x, coords.y+size.y, coords.z-size.z }, currentValues[2]);
        Save({ coords.x+size.x, coords.y-size.y, coords.z-size.z }, currentValues[3]);
        Save({ coords.x-size.x, coords.y+size.y, coords.z+size.z }, currentValues[4]);
        Save({ coords.x-size.x, coords.y+size.y, coords.z-size.z }, currentValues[5]);
        Save({ coords.x-size.x, coords.y-size.y, coords.z+size.z }, currentValues[6]);
        Save({ coords.x-size.x, coords.y-size.y, coords.z-size.z }, currentValues[7]);
        file<<"\n";

        bool plus = false;
        bool minus = false;
        bool zero = false;
        for(auto& i: currentValues)
        {
            if(i > 0)
                plus = true;
            if(i < 0)
                minus = true;
            if(i == 0)
                zero = true;
        }
        if(zero || (plus && minus))
        {
            window.AddCube(coords, size, {1, 1, 1, 0.1});
            counter++;
        }
    }
    else
    {
        glm::vec3 newCubes[8];
        glm::vec3 newSize = { size.x / 2, size.y / 2, size.z / 2 };
        newCubes[0] = { coords.x+newSize.x, coords.y+newSize.y, coords.z+newSize.z };
        newCubes[1] = { coords.x+newSize.x, coords.y+newSize.y, coords.z-newSize.z };
        newCubes[2] = { coords.x+newSize.x, coords.y-newSize.y, coords.z+newSize.z };
        newCubes[3] = { coords.x+newSize.x, coords.y-newSize.y, coords.z-newSize.z };
        newCubes[4] = { coords.x-newSize.x, coords.y+newSize.y, coords.z+newSize.z };
        newCubes[5] = { coords.x-newSize.x, coords.y+newSize.y, coords.z-newSize.z };
        newCubes[6] = { coords.x-newSize.x, coords.y-newSize.y, coords.z+newSize.z };
        newCubes[7] = { coords.x-newSize.x, coords.y-newSize.y, coords.z-newSize.z };

        //cout<<"Step: "<<step<<endl;
        for(int i = 0; i < 8; i++)
            recursiveComputing(program, window, newCubes[i], newSize, step-1);
    }
}


void renderVoxelTree(Program& program, Window& window, int recursiveSteps)
{
    auto args = program.GetArgs();
    if(args.size() != 3)
    {
        cout<<"Arguments count error: "<<args.size()<<endl;
        return;
    }
    glm::vec3 size = {
        (args[0].limits.second - args[0].limits.first)/2.,
        (args[1].limits.second - args[1].limits.first)/2.,
        (args[2].limits.second - args[2].limits.first)/2.
    };
    glm::vec3 center = {
        args[0].limits.first + size.x,
        args[1].limits.first + size.y,
        args[2].limits.first + size.z
    };
    recursiveComputing(program, window, center, size, recursiveSteps);
}


int main(int argc, char** argv)
{
    Window window;
    window.SetZoom(5);
    Lexer lexer;
    lexer.SetText(ReadFile("../Examples/sphere.txt"));
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
    file.open("../Examples/sphere_values.txt");
    renderVoxelTree(program, window, 6);
    file.close();
    cout<<"Cubes "<<counter<<endl;
    while(window.isOpen)
    {
        window.HandleEvent();
        window.Render();
    }
    return 0;
}
