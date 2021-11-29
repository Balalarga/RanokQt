#include "BuildSettingsDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QStringListModel>
#include <QFileDialog>
#include <QLineEdit>
#include <QSpinBox>

BuildSettingsDialog::BuildSettingsDialog(QWidget *parent):
    QDialog(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFormLayout* modeLayout = new QFormLayout();
    QLabel* modeLabel = new QLabel("Вид расчета", this);
    _computeMode = new QComboBox(this);
    _computeMode->setEditable(false);
    _computeMode->addItem("Модель");
    _computeMode->addItem("М-образ");
    modeLayout->addRow(modeLabel, _computeMode);
    mainLayout->addLayout(modeLayout, Qt::AlignTop);

    QHBoxLayout* dirLayout = new QHBoxLayout();
    QLabel* dirLabel = new QLabel("Сохранить в: ", this);
    _dirView = new QLineEdit(this);
    _dirView->setText(QDir::currentPath());
    _dirView->setReadOnly(true);
    QPushButton* dirButton = new QPushButton("Открыть", this);
    connect(dirButton, &QPushButton::clicked, this, &BuildSettingsDialog::BrowseFolder);
    dirLayout->addWidget(dirLabel);
    dirLayout->addWidget(_dirView);
    dirLayout->addWidget(dirButton, 0, Qt::AlignRight);
    mainLayout->addLayout(dirLayout, Qt::AlignTop);

    QFormLayout* fileLayout = new QFormLayout();
    QLabel* fileLabel = new QLabel("Имя файла", this);
    _fileName = new QLineEdit(this);
    fileLayout->addRow(fileLabel, _fileName);
    mainLayout->addLayout(fileLayout, Qt::AlignTop);

    QFormLayout* depthLayout = new QFormLayout();
    QLabel* depthLabel = new QLabel("Глубина рекурсии", this);
    _depth = new QSpinBox(this);
    _depth->setRange(1, 15);
    _depth->setValue(5);
    depthLayout->addRow(depthLabel, _depth);
    mainLayout->addLayout(depthLayout, Qt::AlignTop);

    QFormLayout* memoryLayout = new QFormLayout();
    QLabel* memoryLabel = new QLabel("Используемая память(МБ)", this);
    _memorySize = new QSpinBox(this);
    _memorySize->setRange(128, 4096);
    memoryLayout->addRow(memoryLabel, _memorySize);
    mainLayout->addLayout(memoryLayout, Qt::AlignTop);

    QPushButton* okButton = new QPushButton("Готово", this);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(okButton, 0, Qt::AlignBottom | Qt::AlignRight);
}

void BuildSettingsDialog::BrowseFolder()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                 tr("Find Files"), QDir::currentPath());
    if(!dirPath.isEmpty())
    {
        _dirView->setText(dirPath);
    }
}

QString BuildSettingsDialog::rootDir() const
{
    return _dirView->text()+"/";
}

void BuildSettingsDialog::setRootDir(const QString &newRootDir)
{
    _dirView->setText(newRootDir);
}

QString BuildSettingsDialog::fileName() const
{
    return _fileName->text();
}

void BuildSettingsDialog::setFileName(const QString &newFileName)
{
    _fileName->setText(newFileName);
}

QString BuildSettingsDialog::computeMode() const
{
    return _computeMode->currentText();
}

void BuildSettingsDialog::setComputeMode(const QString &newComputeMode)
{
    _computeMode->setCurrentText(newComputeMode);
}

int BuildSettingsDialog::depth() const
{
    return _depth->value();
}

void BuildSettingsDialog::setDepth(int newDepth)
{
    _depth->setValue(newDepth);
}

int BuildSettingsDialog::memorySize() const
{
    return _memorySize->value();
}

void BuildSettingsDialog::setMemorySize(int newMemorySize)
{
    _memorySize->setValue(newMemorySize);
}
