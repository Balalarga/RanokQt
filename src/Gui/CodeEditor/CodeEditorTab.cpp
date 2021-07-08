#include "CodeEditorTab.h"

#include <QPainter>
#include <QTextBlock>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QStringListModel>


CodeEditorTab::CodeEditorTab(QWidget *parent):
    QTextEdit(parent)
{
    _completer = new QCompleter(this);
    _completer->setModel(ModelFromFile(":/assets/language/ranok_words.txt"));
    _completer->setCaseSensitivity(Qt::CaseInsensitive);
    _completer->setCompletionMode(QCompleter::PopupCompletion);
    _completer->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(_completer, QOverload<const QString &>::of(&QCompleter::activated),
                     this, &CodeEditorTab::insertCompletion);

    highlighter = new Highlighter(document());

    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
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

void CodeEditorTab::insertCompletion(const QString &completion)
{
    QTextCursor tc = textCursor();
    int extra = completion.length() - _completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString CodeEditorTab::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

QAbstractItemModel *CodeEditorTab::ModelFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(_completer);

    QStringList words;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << QString::fromUtf8(line.trimmed());
    }
    return new QStringListModel(words, _completer);
}

void CodeEditorTab::keyPressEvent(QKeyEvent *e)
{
    if (_completer && _completer->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    }

    const bool isShortcut = (e->modifiers().testFlag(Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!_completer || !isShortcut) // do not process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers().testFlag(Qt::ControlModifier) ||
            e->modifiers().testFlag(Qt::ShiftModifier);
    if (!_completer || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    const bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1
                        || eow.contains(e->text().right(1)))) {
        _completer->popup()->hide();
        return;
    }

    if (completionPrefix != _completer->completionPrefix()) {
        _completer->setCompletionPrefix(completionPrefix);
        _completer->popup()->setCurrentIndex(_completer->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(_completer->popup()->sizeHintForColumn(0)
                + _completer->popup()->verticalScrollBar()->sizeHint().width());
    _completer->complete(cr); // popup it up!
}

void CodeEditorTab::focusInEvent(QFocusEvent *e)
{
    if (_completer)
        _completer->setWidget(this);
    QTextEdit::focusInEvent(e);
}
