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
    QString GetActiveFile();
    QString GetActiveFilepath();

    inline int GetTabsCount() const { return m_tabs.size(); }
    CodeEditorTab* GetTab(int id);


protected:
    void keyEvent(QKeyEvent* e);


private slots:
    void closeTab(int index);


private:
    QVector<CodeEditorTab*> m_tabs;
};

#endif // CODEEDITOR_H
