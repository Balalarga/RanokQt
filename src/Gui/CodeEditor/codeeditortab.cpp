#include "codeeditortab.h"

#include <QPainter>
#include <QTextBlock>


CodeEditorTab::CodeEditorTab(QWidget *parent):
    QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, &CodeEditorTab::blockCountChanged,
            this, &CodeEditorTab::updateLineNumberAreaWidth);
    connect(this, &CodeEditorTab::updateRequest,
            this, &CodeEditorTab::updateLineNumberArea);
//    connect(this, &CodeEditorTab::cursorPositionChanged,
//            this, &CodeEditorTab::highlightCurrentLine);


    updateLineNumberAreaWidth(0);
//    highlightCurrentLine();


    highlighter = new Highlighter(document());

    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
}


int CodeEditorTab::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

bool CodeEditorTab::SetFile(const QString &filepath)
{
    QFile file(filepath);
    if(file.open(QIODevice::ReadOnly))
    {
        setPlainText(file.readAll());
        m_filepath = filepath;
        m_name = filepath;
        auto splitFilepath = filepath.split('/');
        if(splitFilepath.empty())
            splitFilepath = filepath.split('\\');
        m_name = splitFilepath.last();
        return true;
    }
    return false;
}

void CodeEditorTab::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditorTab::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditorTab::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
                                      lineNumberAreaWidth(),cr.height()));
}

void CodeEditorTab::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


void CodeEditorTab::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());


    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}


