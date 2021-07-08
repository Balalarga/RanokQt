#include "CodeEditor.h"

#include <QMessageBox>
#include <QScrollBar>
#include <QDebug>

CodeEditor::CodeEditor(QWidget *parent):
    QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this, &QTabWidget::tabCloseRequested,
            this, &CodeEditor::closeTab);
}


void CodeEditor::AddFile(const QString &sourceFile)
{
    QFile file(sourceFile);
    if(file.open(QIODevice::ReadOnly))
    {
        auto tab = new CodeEditorTab(this);
//        connect(data.tab, &CodeEditorTab::textChanged, this, &CodeEditor::tabTextChanged);
        tab->setPlainText(file.readAll());
        tab->verticalScrollBar()->setSliderPosition(0);

//        data.filepath = sourceFile;
        auto splitFilepath = sourceFile.split('/');
        if(splitFilepath.empty())
            splitFilepath = sourceFile.split('\\');
//        data.filename = splitFilepath.last();


        addTab(tab, "tab");
        setCurrentIndex(addTab(tab, "tab"));


        m_tabs.push_back(tab);
    }
    else
        QMessageBox::critical(this, "Ошибка", "Невозможно открыть файл");
}

QString CodeEditor::GetActiveText()
{
    int id = currentIndex();
    if(id > -1)
    {
        return m_tabs.at(id)->document()->toPlainText();
    }
    return "";
}

void CodeEditor::keyEvent(QKeyEvent *e)
{

}

void CodeEditor::closeTab(int index)
{
    auto tab = m_tabs[index];
    tab->deleteLater();
    m_tabs.erase(m_tabs.begin()+index);
}
