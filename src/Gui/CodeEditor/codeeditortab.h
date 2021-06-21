#ifndef CODEEDITORTAB_H
#define CODEEDITORTAB_H

#include <QPlainTextEdit>
#include "highlighter.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

class CodeEditorTab : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditorTab(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    bool SetFile(const QString& filepath);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
    Highlighter* highlighter;
    QString m_filepath;
    QString m_name;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditorTab *editor) : QWidget(editor), codeEditor(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditorTab *codeEditor;
};

#endif
