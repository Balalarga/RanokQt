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
    auto tab = new CodeEditorTab(this);
    if(!tab->SetFile(sourceFile))
    {
        QMessageBox::about(this, "Ошибка", "Невозможно открыть файл");
        delete tab;
        return;
    }
    tab->verticalScrollBar()->setSliderPosition(0);
    m_tabs.push_back(tab);
    int newTabId = addTab(tab, tab->GetFileName());
    setCurrentIndex(newTabId);
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

QString CodeEditor::GetActiveFile()
{
    int id = currentIndex();
    if(id > -1)
    {
        return m_tabs.at(id)->GetFileName();
    }
    return "";
}

QString CodeEditor::GetActiveFilepath()
{
    int id = currentIndex();
    if(id > -1)
    {
        return m_tabs.at(id)->GetFilepath();
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
