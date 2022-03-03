#include "RayMarchingScreen.h"

#include <fstream>
#include <sstream>
using std::stringstream;


#include <QtMath>
#include <QDebug>
#include <QFileDialog>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QToolBar>
#include <QSplitter>
#include <QLabel>
#include <QProgressBar>
#include <QMessageBox>


#include "Space/SpaceManager.h"
#include "Space/Calculators/CommonCalculator.h"
#include "Space/Calculators/OpenclCalculator.h"

#include "Gui/BuildSettingsDialog.h"


RayMarchingScreen::RayMarchingScreen(QWidget *parent)
    : ClearableWidget(parent),
      _sceneView(new RayMarchingView(this)),
      _codeEditor(new CodeEditor(this)),
      _program(nullptr),
      _progressBar(new QProgressBar(_sceneView)),
      _openclCalculator(new OpenclCalculatorThread(this))
{
    QVBoxLayout* toolVLayout = new QVBoxLayout(this);

    QToolBar* _toolBar(new QToolBar(this));
    _toolBar->addAction(QPixmap("assets/images/playIcon.svg"),
                        "Показать", this, &RayMarchingScreen::UpdateScreen);
    _toolBar->addSeparator();
    _toolBar->addAction(QPixmap("assets/images/buildIcon.jpg"),
                        "Рассчитать", this, &RayMarchingScreen::BuildMimage);
    toolVLayout->addWidget(_toolBar);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    QWidget* wrapWidget = new QWidget(this);
    QVBoxLayout* modeLayout = new QVBoxLayout(wrapWidget);


    QHBoxLayout* widthLayout = new QHBoxLayout();
    QLabel* widthLabel = new QLabel("Ширина", this);
    widthLayout->addWidget(widthLabel);
    _widthSpin = new QSpinBox(this);
    _widthSpin->setRange(10, 1980);
    _widthSpin->setValue(800);
    _widthSpin->setSingleStep(50);
    widthLayout->addWidget(_widthSpin);

    QHBoxLayout* heightLayout = new QHBoxLayout();
    QLabel* heightLabel = new QLabel("Высота", this);
    heightLayout->addWidget(heightLabel);
    _heightSpin = new QSpinBox(this);
    _heightSpin->setRange(10, 1080);
    _heightSpin->setValue(600);
    _heightSpin->setSingleStep(50);
    heightLayout->addWidget(_heightSpin);

    connect(_widthSpin, SIGNAL(valueChanged(int)), this, SLOT(RenderWidthChanged(int)));
    connect(_heightSpin, SIGNAL(valueChanged(int)), this, SLOT(RenderHeightChanged(int)));

    QLabel* sizeLabel = new QLabel("Рендер", this);

    QHBoxLayout* sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(sizeLabel);
    sizeLayout->addLayout(widthLayout);
    sizeLayout->addLayout(heightLayout);

    modeLayout->addLayout(sizeLayout);
    modeLayout->addWidget(_codeEditor);

    wrapWidget->setLayout(modeLayout);

    splitter->addWidget(wrapWidget);
    splitter->addWidget(_sceneView);
    splitter->setMidLineWidth(2);
    _sceneView->setMinimumWidth(500);
    toolVLayout->addWidget(splitter);

    QMenuBar* menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("Файл");
    menuBar->addMenu(fileMenu);

    QAction* openAction = new QAction;
    openAction->setText("Открыть");
    connect(openAction, &QAction::triggered, this, &RayMarchingScreen::OpenFile);
    fileMenu->addAction(openAction);

    QAction* saveAction = new QAction;
    saveAction->setText("Сохранить");
    connect(saveAction, &QAction::triggered, this, &RayMarchingScreen::SaveFile);
    fileMenu->addAction(saveAction);

    QAction* saveAsAction = new QAction;
    saveAsAction->setText("Сохранить как...");
    connect(saveAsAction, &QAction::triggered, this, &RayMarchingScreen::SaveFileAs);
    fileMenu->addAction(saveAsAction);

    QAction* saveImageAction = new QAction;
    saveImageAction->setText("Сохранить изображение");
    connect(saveImageAction, &QAction::triggered, this, &RayMarchingScreen::SaveImage);
    fileMenu->addAction(saveImageAction);

    toolVLayout->setMenuBar(menuBar);

    _progressBar->setRange(0, 100);
    _progressBar->setValue(0);
    _progressBar->hide();

    _oldTabId = _codeEditor->currentIndex();

    qRegisterMetaType<CalculatorMode>("CalculatorMode");
    connect(_openclCalculator , &OpenclCalculatorThread::Computed,
            this, &RayMarchingScreen::BuildIteration);
}

RayMarchingScreen::~RayMarchingScreen()
{
    delete _openclCalculator;
    if(_program)
        delete _program;
}

void RayMarchingScreen::Cleanup()
{

}

void RayMarchingScreen::RenderWidthChanged(int value)
{
    _sceneView->SetRenderSize({value, _heightSpin->value()});
}

void RayMarchingScreen::RenderHeightChanged(int value)
{
    _sceneView->SetRenderSize({_widthSpin->value(), value});
}

void RayMarchingScreen::BuildIteration(CalculatorMode mode, int batchStart, int count)
{
    SpaceManager& space = SpaceManager::Self();
    if(mode == CalculatorMode::Model)
    {
        for(int i = 0; i < count; ++i)
        {
            if(space.GetZone(i) == 0)
                ++_metadata.zeroCount;
            else if(space.GetZone(i) == 1)
                ++_metadata.positiveCount;
            else
                ++_metadata.negativeCount;
        }
        space.SaveZoneRange(_resultFile, count);
    }
    else
        space.SaveMimageRange(_resultFile, count);

    float percent = 100.f*(batchStart+count)/space.GetSpaceSize();
    _progressBar->setValue(percent);
    qDebug()<<"Written "<<percent<<"% points";
    if(int(percent) == 100)
    {
        _progressBar->hide();

        _resultFile.flush();
        _resultFile.seekp(0);
        _resultFile.write((char*)&_metadata, sizeof(ModelMetadata));
        _resultFile.close();
    }
}

void RayMarchingScreen::GetShaderCode(int id)
{
    stringstream result;
    if(id == 0)
    {
        const std::string shaderBegin = R"(#version 330
    out vec4 color;

    uniform vec2 resolution;
    uniform float grad_step;
    uniform vec3 cameraPosition;
    uniform vec2 cameraRotation;

    // ray marching
    const int max_iterations = 2048;
    const float stop_threshold = 0.001;
    const float clip_far = 1000.0;

    // math
    const float PI = 3.14159265359;
    const float DEG_TO_RAD = PI / 180.0;

    float __rand(float a, float b)
    {
        return a + b - sqrt(pow(a, 2) + pow(b, 2));
    }
    float __ror(float a, float b)
    {
        return a + b + sqrt(pow(a, 2) + pow(b, 2));
    }

    )";
        const std::string shaderEnd = R"(

    // get distance in the world
    float dist_field( vec3 p ) {
        return -__resultFunc(p.x, p.y, p.z);
    }

    // get gradient in the world
    vec3 gradient( vec3 pos ) {
        vec3 dx = vec3( grad_step, 0.0, 0.0 );
        vec3 dy = vec3( 0.0, grad_step, 0.0 );
        vec3 dz = vec3( 0.0, 0.0, grad_step );
        return normalize (
                    vec3(
                        dist_field( pos + dx ) - dist_field( pos - dx ),
                        dist_field( pos + dy ) - dist_field( pos - dy ),
                        dist_field( pos + dz ) - dist_field( pos - dz )
                        )
                    );
    }

    vec3 fresnel( vec3 F0, vec3 h, vec3 l ) {
        return F0 + ( 1.0 - F0 ) * pow( clamp( 1.0 - dot( h, l ), 0.0, 1.0 ), 5.0 );
    }

    // phong shading
    vec3 shading( vec3 v, vec3 n, vec3 dir, vec3 eye ) {
        // ...add lights here...

        float shininess = 16.0;

        vec3 final = vec3( 0.0 );

        vec3 ref = reflect( dir, n );

        vec3 Ks = vec3( 0.5 );
        vec3 Kd = vec3( 1.0 );

        // light 0
        {
            vec3 light_pos   = vec3( 20.0, 20.0, 20.0 );
            vec3 light_color = vec3( 1.0, 0.7, 0.7 );

            vec3 vl = normalize( light_pos - v );

            vec3 diffuse  = Kd * vec3( max( 0.0, dot( vl, n ) ) );
            vec3 specular = vec3( max( 0.0, dot( vl, ref ) ) );

            vec3 F = fresnel( Ks, normalize( vl - dir ), vl );
            specular = pow( specular, vec3( shininess ) );

            final += light_color * mix( diffuse, specular, F );
        }

        // light 1
        {
            vec3 light_pos   = vec3( -20.0, -20.0, -30.0 );
            vec3 light_color = vec3( 0.5, 0.7, 1.0 );

            vec3 vl = normalize( light_pos - v );

            vec3 diffuse  = Kd * vec3( max( 0.0, dot( vl, n ) ) );
            vec3 specular = vec3( max( 0.0, dot( vl, ref ) ) );

            vec3 F = fresnel( Ks, normalize( vl - dir ), vl );
            specular = pow( specular, vec3( shininess ) );

            final += light_color * mix( diffuse, specular, F );
        }

        final += vec3(0.2, 0.1, 0.1) * fresnel( Ks, n, -dir );

        return final;
    }


    bool ray_vs_aabb(vec3 o, vec3 dir, vec3 bmin, vec3 bmax, inout vec2 e ) {
        vec3 a = ( bmin - o ) / dir;
        vec3 b = ( bmax - o ) / dir;

        vec3 s = min( a, b );
        vec3 t = max( a, b );

        e.x = max( max( s.x, s.y ), max( s.z, e.x ) );
        e.y = max( min( t.x, t.y ), max( t.z, e.y ) );

        return e.x < e.y;
    }

    // ray marching
    bool ray_marching( vec3 o, vec3 dir, inout float depth, inout vec3 n ) {
        float t = 0.0;
        float d = 10000.0;
        float dt = 0.0;
        for ( int i = 0; i < max_iterations; i++ ) {
            vec3 v = o + dir * t;
            d = dist_field( v );
            if ( d < 0.001 ) {
                break;
            }
            dt = min( abs(d), 0.1 );
            t += dt;
            if ( t > depth ) {
                break;
            }
        }

        if ( d >= 0.001 ) {
            return false;
        }

        t -= dt;
        for ( int i = 0; i < 4; i++ ) {
            dt *= 0.5;

            vec3 v = o + dir * ( t + dt );
            if ( dist_field( v ) >= 0.001 ) {
                t += dt;
            }
        }

        depth = t;
        n = normalize( gradient( o + dir * t ) );
        return true;

        return true;
    }

    // get ray direction
    vec3 ray_dir( float fov, vec2 size, vec2 pos ) {
        vec2 xy = pos - size * 0.5;

        float cot_half_fov = tan( ( 90.0 - fov * 0.5 ) * DEG_TO_RAD );
        float z = size.y * 0.5 * cot_half_fov;

        return normalize( vec3( xy, -z ) );
    }

    // camera rotation : pitch, yaw
    mat3 rotationXY( vec2 angle ) {
        vec2 c = cos( angle );
        vec2 s = sin( angle );

        return mat3(
                    c.y      ,  0.0, -s.y,
                    s.y * s.x,  c.x,  c.y * s.x,
                    s.y * c.x, -s.x,  c.y * c.x
                    );
    }


    void main()
    {
        // default ray dir
        vec3 dir = ray_dir( 45.0, resolution, gl_FragCoord.xy );

        // default ray origin
        vec3 eye = cameraPosition;
        //vec3 eye = vec3(0, 0, 5);

        // rotate camera
        mat3 rot = rotationXY( cameraRotation );
        dir = rot * dir;
        eye = rot * eye;

        // ray marching
        float depth = clip_far;
        vec3 n = vec3( 0.0 );
        if ( !ray_marching( eye, dir, depth, n ) ) {
            color = vec4(0.1, 0.1, 0.2, 1.0);
            return;
        }

        // shading
        vec3 pos = eye + dir * depth;

        vec3 newColor = shading( pos, n, dir, eye );
        color = vec4( pow( newColor, vec3(1.0/1.2) ), 1.0 );
    }

    )";

        result << shaderBegin;
        result << _program->GetShaderCode();
        result << shaderEnd;
    }
    else if(id == 1)
    {
        const std::string shaderBegin = R"(#version 330
    out vec4 color;

    uniform vec2 resolution;
    uniform float grad_step;
    uniform vec3 cameraPosition;
    uniform vec2 cameraRotation;

    const bool USE_BRANCHLESS_DDA = false;
    const int MAX_RAY_STEPS = 64;

    // math
    const float PI = 3.14159265359;
    const float DEG_TO_RAD = PI / 180.0;

    float __rand(float a, float b)
    {
        return a + b - sqrt(pow(a, 2) + pow(b, 2));
    }
    float __ror(float a, float b)
    {
        return a + b + sqrt(pow(a, 2) + pow(b, 2));
    }

    )";


        const std::string shaderEnd = R"(

bool getVoxel(ivec3 c) {
    vec3 p = vec3(c) + vec3(0.5);
    float d = -__resultFunc(p.x, p.y, p.z);
    return d < 0.0;
}

void main()
{
    vec2 screenPos = (gl_FragCoord.xy / resolution.xy) * 2.0 - 1.0;
    vec3 cameraDir = vec3(0.0, 0.0, 0.8);
    vec3 cameraPlaneU = vec3(1.0, 0.0, 0.0);
    vec3 cameraPlaneV = vec3(0.0, 1.0, 0.0) * resolution.y / resolution.x;
    vec3 rayDir = cameraDir + screenPos.x * cameraPlaneU + screenPos.y * cameraPlaneV;
    vec3 rayPos = vec3(0.0, 0.0, -12.0);

    ivec3 mapPos = ivec3(floor(rayPos + 0.));

    vec3 deltaDist = abs(vec3(length(rayDir)) / rayDir);

    ivec3 rayStep = ivec3(sign(rayDir));

    vec3 sideDist = (sign(rayDir) * (vec3(mapPos) - rayPos) + (sign(rayDir) * 0.5) + 0.5) * deltaDist;

    bvec3 mask;

    for (int i = 0; i < MAX_RAY_STEPS; i++) {
        if (getVoxel(mapPos)) continue;
        if (USE_BRANCHLESS_DDA) {
            //Thanks kzy for the suggestion!
            mask = lessThanEqual(sideDist.xyz, min(sideDist.yzx, sideDist.zxy));
            /*bvec3 b1 = lessThan(sideDist.xyz, sideDist.yzx);
            bvec3 b2 = lessThanEqual(sideDist.xyz, sideDist.zxy);
            mask.x = b1.x && b2.x;
            mask.y = b1.y && b2.y;
            mask.z = b1.z && b2.z;*/
            //Would've done mask = b1 && b2 but the compiler is making me do it component wise.

            //All components of mask are false except for the corresponding largest component
            //of sideDist, which is the axis along which the ray should be incremented.

            sideDist += vec3(mask) * deltaDist;
            mapPos += ivec3(vec3(mask)) * rayStep;
        }
        else {
            if (sideDist.x < sideDist.y) {
                if (sideDist.x < sideDist.z) {
                    sideDist.x += deltaDist.x;
                    mapPos.x += rayStep.x;
                    mask = bvec3(true, false, false);
                }
                else {
                    sideDist.z += deltaDist.z;
                    mapPos.z += rayStep.z;
                    mask = bvec3(false, false, true);
                }
            }
            else {
                if (sideDist.y < sideDist.z) {
                    sideDist.y += deltaDist.y;
                    mapPos.y += rayStep.y;
                    mask = bvec3(false, true, false);
                }
                else {
                    sideDist.z += deltaDist.z;
                    mapPos.z += rayStep.z;
                    mask = bvec3(false, false, true);
                }
            }
        }
    }

    vec3 resColor;
    if (mask.x) {
        resColor = vec3(0.5);
    }
    if (mask.y) {
        resColor = vec3(1.0);
    }
    if (mask.z) {
        resColor = vec3(0.75);
    }
    color = vec4(resColor, 1.0);
    //fragColor.rgb = vec3(0.1 * noiseDeriv);
}
        )";
        result << shaderBegin;
        result << _program->GetShaderCode();
        result << shaderEnd;

    }

    auto res = QString::fromStdString(result.str());
    res = res.replace("fabs(", "abs(");
    _sceneView->ShaderFromSource(res);
}

void RayMarchingScreen::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
        _sceneView->SetShiftState(true);
}

void RayMarchingScreen::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
        _sceneView->SetShiftState(false);
}

void RayMarchingScreen::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open program file"), "../",
                                                    tr("Plan text(*.txt)"));
    if(!fileName.isEmpty())
        _codeEditor->AddFile(fileName);
}

void RayMarchingScreen::BuildMimage()
{
    if(_openclCalculator->isRunning())
    {
        QMessageBox::information(this, "Ошибка", "Невозможно запустить еще один расчет до завершения прошлого");
        return;
    }

    BuildSettingsDialog settingsDialog(this);
    settingsDialog.resize(size()/2);
    settingsDialog.setFileName(_codeEditor->GetActiveFile().remove(".txt"));
    if(settingsDialog.exec() == QDialog::Rejected)
        return;

    _parser.SetText(_codeEditor->GetActiveText().toStdString());
    if(_program)
        delete _program;
    _program = _parser.GetProgram();

    QString resultPath = settingsDialog.fileName();
    if(settingsDialog.computeMode() == "Модель")
    {
        _openclCalculator->SetCalculatorMode(CalculatorMode::Model);
        resultPath += ".mbin";
    }
    else
    {
        _openclCalculator->SetCalculatorMode(CalculatorMode::Mimage);
        resultPath += ".ibin";
    }

    _resultFile.open(resultPath.toStdString(), std::ios_base::binary);
    if(!_resultFile)
    {
        qDebug()<<"Couldn't create or open file "<<resultPath;
        return;
    }

    _openclCalculator->SetProgram(_program);

    SpaceManager& space = SpaceManager::Self();
    auto args = _program->GetSymbolTable().GetAllArgs();
    space.InitSpace(args[0]->limits,
            args[1]->limits,
            args[2]->limits, settingsDialog.depth());
    space.ResetBufferSize(1024*1024*settingsDialog.memorySize());
    _metadata = space.GetMetadata();
    _resultFile.write((char*)&_metadata, sizeof(ModelMetadata));

    _progressBar->show();
    _openclCalculator->Run();
}

void RayMarchingScreen::UpdateScreen()
{
    _parser.SetText(_codeEditor->GetActiveText().toStdString());
    if(_program)
        delete _program;
    _program = _parser.GetProgram();

    GetShaderCode(0);
    _sceneView->updateGL();
}

void RayMarchingScreen::Save(const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::about(this, "Ошибка", "Невозможно сохранить файл");
        return;
    }
    QTextStream stream(&file);
    stream << _codeEditor->GetActiveText();
    file.close();
}

void RayMarchingScreen::SaveFile()
{
    Save(_codeEditor->GetActiveFilepath());
}

void RayMarchingScreen::SaveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save program file"), "../",
                                                    tr("Plan text(*.txt)"));
    if(!fileName.isEmpty())
        Save(fileName);
}

void RayMarchingScreen::SaveImage()
{
    auto& image = _sceneView->GetImage();

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save image"), "../",
                                                    tr("Image (*.png)"));
    if(!fileName.isEmpty())
        image.save(fileName);
}
