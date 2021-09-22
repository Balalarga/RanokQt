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
        auto splitFilepath = sourceFile.split('/');
        if(splitFilepath.empty())
            splitFilepath = sourceFile.split('\\');
        QString fileName = splitFilepath.last();

        auto tab = new CodeEditorTab(this);
        tab->setPlainText(file.readAll());
        tab->verticalScrollBar()->setSliderPosition(0);
        m_tabs.push_back(tab);

        int newTabId = addTab(tab, fileName);
        setCurrentIndex(newTabId);
    }
    else
        QMessageBox::about(this, "Ошибка", "Невозможно открыть файл");
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
