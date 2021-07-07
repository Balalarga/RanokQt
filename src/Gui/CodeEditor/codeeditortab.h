#ifndef CODEEDITORTAB_H
#define CODEEDITORTAB_H

#include <QPlainTextEdit>
#include <QCompleter>

#include "Highlighter.h"

class CodeEditorTab : public QTextEdit
{
    Q_OBJECT

public:
    CodeEditorTab(QWidget *parent = nullptr);

    bool SetFile(const QString& filepath);


protected:
    void focusInEvent(QFocusEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;


private slots:
    void insertCompletion(const QString& completion);
    QString textUnderCursor() const;
    QAbstractItemModel *ModelFromFile(const QString& fileName);


private:
    QCompleter* _completer;
    Highlighter* highlighter;
    QString m_filepath;
    QString m_name;
};

#endif
