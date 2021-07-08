#include "StyleLoader.h"
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include <QDebug>

void StyleLoader::attach(const QString &filename)
{
        StyleLoader * loader = new StyleLoader(qApp, filename);
        loader->setAppStyleSheet();
}

void StyleLoader::setAppStyleSheet()
{
    QFile file(m_filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open stylesheet file " << m_filename;
        return;
    }
    QString stylesheet = QString::fromUtf8(file.readAll());
    qApp->setStyleSheet(stylesheet);
}

QString StyleLoader::defaultStyleFile()
{
    return QApplication::applicationDirPath() + "/style.qss";
}

StyleLoader::StyleLoader(QObject *parent, const QString& filename):
    QObject(parent),
    m_filename(filename)
{

}
