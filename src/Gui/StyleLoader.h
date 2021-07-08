#ifndef STYLELOADER_H
#define STYLELOADER_H

#include <QObject>
#include <QKeySequence>

class StyleLoader: public QObject
{
    Q_OBJECT
public:
    static void attach(const QString& filename = defaultStyleFile());

private:
    StyleLoader(QObject * parent, const QString& filename);
    void setAppStyleSheet();
    static QString defaultStyleFile();
    QString m_filename;

};
#endif // STYLELOADER_H
