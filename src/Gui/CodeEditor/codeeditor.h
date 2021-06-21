#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTabWidget>
#include <QVector>

#include "codeeditortab.h"

class CodeEditor: public QTabWidget
{
public:
    CodeEditor(QWidget* parent = nullptr);


    void AddFile(const QString& sourceFile);
    QString GetActiveText();

private slots:
    void closeTab(int index);

private:
    QVector<CodeEditorTab*> m_tabs;
};

#endif // CODEEDITOR_H
