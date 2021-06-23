#include "widget.h"

#include "styleloader.h"

#include "Opengl/openglcube.h"
#include "Opengl/openglsquare.h"

#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      m_mode(Mode::Common),
      m_toolBar(new QToolBar(this)),
      m_sceneView(new SceneView(this)),
      m_codeEditor(new CodeEditor(this)),
      m_lineEditor(new LineEditor(this)),
      m_modelCalculator(new RecursiveCalculator(6)),
      m_program(nullptr),
      m_lineProgram(nullptr),
      m_modeButton(new QPushButton("Обычный режим", this)),
      m_addLineButton(new QPushButton("Добавить строку", this))
{
    QVBoxLayout* m_toolVLayout = new QVBoxLayout(this);
    m_toolVLayout->addWidget(m_toolBar);

    m_toolBar->addAction(QPixmap("assets/images/playIcon.svg"),
                         "Run", this, &Widget::Compute);


    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    QWidget* wrapWidget = new QWidget(this);
    QVBoxLayout* modeLayout = new QVBoxLayout(wrapWidget);
    modeLayout->addWidget(m_codeEditor);
    modeLayout->addWidget(m_lineEditor);
    modeLayout->addWidget(m_modeButton);
    modeLayout->addWidget(m_addLineButton);
    m_addLineButton->setVisible(false);
    wrapWidget->setLayout(modeLayout);
    m_lineEditor->setVisible(false);

    splitter->addWidget(wrapWidget);
    splitter->addWidget(m_sceneView);
    splitter->setMidLineWidth(2);
    m_sceneView->setMinimumWidth(500);
    m_toolVLayout->addWidget(splitter);

    m_sceneView->AddObject(new OpenglCube({0, 0, 0}, {1, 1, 1},
                                          QColor(255, 255, 255, 100)));

    QMenuBar* menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("Файл");
    menuBar->addMenu(fileMenu);

    QAction* saveAction = new QAction;
    saveAction->setText("Открыть");
    connect(saveAction, &QAction::triggered, this, &Widget::OpenFile);
    fileMenu->addAction(saveAction);

    m_toolVLayout->setMenuBar(menuBar);

    m_modelCalculator->SetAddVoxelFunc([this](VoxelData data){
        m_sceneView->AddObject(new OpenglCube(data.position, data.size, data.color));
    });

    m_execThread = new TaskThread(m_modelCalculator, this);


    StyleLoader::attach("../assets/styles/dark.qss");

    connect(m_modeButton, &QPushButton::clicked, this, &Widget::SwitchMode);
    connect(m_addLineButton, &QPushButton::clicked, m_lineEditor, &LineEditor::addItem);
    connect(m_lineEditor, &LineEditor::runLine, this, &Widget::ComputeLine);
}


Widget::~Widget()
{

}


void Widget::AddVoxel(VoxelData data)
{
    m_sceneView->AddObject(new OpenglCube(data.position, data.size, data.color));
}


void Widget::Compute()
{
    QString source = m_codeEditor->GetActiveText();
    if(!source.isEmpty())
    {
        m_parser.SetText(source.toStdString());

        m_execThread->terminate();
        if(m_program)
            delete m_program;

        m_program = m_parser.GetProgram();
        m_modelCalculator->SetProgram(m_program);

        m_sceneView->ClearObjects();

        m_execThread->start();
    }
}


void Widget::SwitchMode()
{
    if(m_mode == Mode::Common)
    {
        m_codeEditor->setVisible(false);
        m_lineEditor->setVisible(true);
        m_addLineButton->setVisible(true);
        m_mode = Mode::Line;
        m_modeButton->setText("Построчный режим");
    }
    else
    {
        m_codeEditor->setVisible(true);
        m_lineEditor->setVisible(false);
        m_addLineButton->setVisible(false);
        m_mode = Mode::Common;
        m_modeButton->setText("Обычный режим");
    }
}

void Widget::ComputeLine(QString line)
{
    m_lineParser.SetText(line.toStdString());

    Program* lineProg = m_lineParser.GetProgram();
    if(m_lineProgram)
    {
        if(m_lineProgram->MergeProgram(lineProg))
        {
            delete lineProg;
        }
        else
        {
            qDebug()<<"Program merging error:"<<line;
            return;
        }
    }
    else
        m_lineProgram = lineProg;
    if(!m_lineProgram->GetSymbolTable().GetAllArgs().empty())
    {
        m_modelCalculator->SetProgram(m_lineProgram);
        m_execThread->start();
    }
}

void Widget::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open program file"), "../",
                                                    tr("Plan text(*.txt)"));
    if(!fileName.isEmpty())
        m_codeEditor->AddFile(fileName);
}


bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(QKeySequence("F6") == QKeySequence(keyEvent->key()))
        {
            QString fileName = QFileDialog::getOpenFileName(this,
                                                            tr("Stylesheet"), "../style",
                                                            tr("Stylesheet(*.qss)"));
            if(!fileName.isEmpty())
            {
                StyleLoader::attach(fileName);
            }
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}
