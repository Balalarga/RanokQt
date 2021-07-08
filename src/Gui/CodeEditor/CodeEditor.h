#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTabWidget>
#include <QVector>

#include "CodeEditorTab.h"

class CodeEditor: public QTabWidget
{
public:
    CodeEditor(QWidget* parent = nullptr);


    void AddFile(const QString& sourceFile);
    QString GetActiveText();

protected:
    void keyEvent(QKeyEvent* e);

private slots:
    void closeTab(int index);

private:
    QVector<CodeEditorTab*> m_tabs;
};

#endif // CODEEDITOR_H
