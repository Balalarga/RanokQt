#include <iostream>

#include <GLEW/glew.h>
#include <GL/glu.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "imgui-SFML.h"
#include "imgui.h"

#include "Vectors.h"
#include "Color.h"
#include "Cube.h"
#include "Square.h"
#include "AsyncVector.h"

#include "Calculators/matrixcalculator.h"
#include "Calculators/recursivecalculator.h"
#include "Calculators/imagematrixcalculator.h"
#include "Language/parser.h"

using namespace std;

struct ModelData
{
    string filename;
    BaseCalculator* calculator;
};
struct ImageCalcData
{
    string filename;
    ImageType type;
    ImageCalculator* calculator;
};
void ImageCalculating(ImageCalcData data);
void ModelCalculating(ModelData data);

AsyncVector<IDrawableObject*> voxels;
sf::RenderWindow* window = nullptr;
Vector2i windowSize = {800, 600};

float yAngle = 0;
float xAngle = 0;
float zoom = 10;
bool mousePressed = false;
Vector2i prevMousePos{0, 0};
const char* files[]{
    "NewFuncs/1",       //0
    "NewFuncs/1_new",   //1
    "NewFuncs/Bone",    //2
    "NewFuncs/Chainik", //3
    "NewFuncs/kris",    //4
    "NewFuncs/lopatka", //5
    "NewFuncs/sphere",  //6
    "NewFuncs/TeaCup",  //7
    "NewFuncs/2dspace", //8
};
int fileId = 7;
bool fileChoosen = false;

sf::Thread* modelThread = nullptr;
sf::Thread* imageThread = nullptr;
ModelData runData{
    "../examples/"+string(files[fileId])+".txt",
            //        new MatrixCalculator({10, 10, 10})
            new RecursiveCalculator(5)
};

ImageCalcData imageData{
    "../examples/"+string(files[fileId])+".txt",
            ImageType::Cw,
            new ImageMatrixCalculator({40, 40, 40})
};
bool mode = true; // true - model, false - image

void ResetThreads()
{
    for(unsigned i = 0; i < voxels.Size(); i++)
        delete voxels.At(i);
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
void RunThreads()
{
    ResetThreads();
    modelThread = new sf::Thread(&ModelCalculating, runData);
    imageThread = new sf::Thread(&ImageCalculating, imageData);
    if(mode)
    {
        modelThread->launch();
    }
    else
    {
        imageThread->launch();
    }
}

void HandleEvents()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }
        else if (event.type == sf::Event::Resized)
        {
            glViewport(0, 0, event.size.width, event.size.height);
            windowSize.x() = event.size.width;
            windowSize.y() = event.size.height;
        }
        else if(event.type == sf::Event::MouseButtonPressed)
        {
            mousePressed = true;
            prevMousePos = {event.mouseButton.x, event.mouseButton.y};
        }
        else if(event.type == sf::Event::MouseButtonReleased)
        {
            mousePressed = false;
        }
        else if(mousePressed && event.type == sf::Event::MouseMoved)
        {
            xAngle += (prevMousePos.y() - event.mouseMove.y)*0.5;
            yAngle += (event.mouseMove.x - prevMousePos.x())*0.5;
            prevMousePos = {event.mouseMove.x, event.mouseMove.y};
        }
        else if(event.type == sf::Event::MouseWheelScrolled)
        {
            zoom -= event.mouseWheelScroll.delta*0.2;
        }
    }
}

void RenderMenu(sf::Clock& deltaClock)
{
    window->pushGLStates();

    ImGui::SFML::Update(*window, deltaClock.restart());

    ImGui::Begin("Settings");

    if (ImGui::BeginCombo("File", files[fileId]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(files); n++)
        {
            const bool is_selected = (fileId == n);
            if (ImGui::Selectable(files[n], is_selected))
            {
                fileId = n;
                runData.filename = "../examples/"+string(files[fileId])+".txt";
                imageData.filename = "../examples/"+string(files[fileId])+".txt";
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::Button("Start"))
    {
        ResetThreads();
        RunThreads();
    }
    if (ImGui::Button("Stop"))
    {
        ResetThreads();
    }
    ImGui::End();

    ImGui::SFML::Render(*window);

    window->popGLStates();
}

void Render()
{
    sf::Clock deltaClock;
    window->setActive(true);
    while (window->isOpen())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0f, 0.0f, -zoom,
                  0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f);
        glRotatef(xAngle, 1, 0, 0);
        glRotatef(yAngle, 0, 1, 0);
        for(unsigned i = 0; i < voxels.Size(); i++)
        {
            voxels.Get(i)->Draw();
        }
        RenderMenu(deltaClock);
        window->display();
    }
}

void ModelCalculating(ModelData data)
{
    Parser parser(data.filename);
    Program program = parser.GetProgram();
    auto voxelData = data.calculator->Calculate(program, Zone::Zero, [](VoxelData& voxel)
    {
            if(voxel.dementions == 3)
            voxels.PushBack(new Cube(voxel.center, voxel.size, voxel.color));
            if(voxel.dementions == 2)
            voxels.PushBack(new Square(voxel.center, voxel.size, voxel.color));
            if(voxel.dementions == 1)
            voxels.PushBack(new Square(voxel.center, voxel.size, voxel.color));
});
}

void ImageCalculating(ImageCalcData data)
{
    Parser parser(data.filename);
    Program program = parser.GetProgram();
    auto voxelData = data.calculator->Calculate(program, data.type, [](ImageData& voxel)
    {
            if(voxel.dementions == 3)
            voxels.PushBack(new Cube(voxel.center, voxel.size, voxel.color));
            if(voxel.dementions == 2)
            voxels.PushBack(new Square(voxel.center, voxel.size, voxel.color));
            if(voxel.dementions == 1)
            voxels.PushBack(new Square(voxel.center, voxel.size, voxel.color));
});
}

void Init()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    window = new sf::RenderWindow(sf::VideoMode(windowSize.x(), windowSize.y()),
                                  "OpenGL", sf::Style::Default, settings);
    window->setFramerateLimit(60);
    ImGui::CreateContext();
    ImGui::SFML::Init(*window);

    GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
    GLfloat LightAmbient[] = {0.25f, 0.25f, 0.25f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    GLfloat LightDiffuse[] = {0.75f, 0.75f, 0.75f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
    GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
    float LightPosition[] = {0.0f, 2.5f, 2.5f};
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)windowSize.x() /
                   (windowSize.y()> 0 ? (float)600: 1.0f),
                   0.125f, 512.0f);
    window->setActive(false);
}

int main(int argc, char** argv)
{
    Init();
    sf::Thread renderThread(&Render);
    renderThread.launch();

    auto runThread = [](bool witch){
    };
    runThread(true);
    while (window->isOpen())
    {
        HandleEvents();
    }

    renderThread.terminate();

    cout<<"Voxels size = "<<voxels.Size()<<endl;
    ResetThreads();

    delete runData.calculator;
    delete imageData.calculator;
    delete window;

    ImGui::SFML::Shutdown();

    return 0;
}
