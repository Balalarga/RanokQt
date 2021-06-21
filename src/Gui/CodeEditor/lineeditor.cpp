#include "lineeditor.h"

#include <QStyle>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>


LineEditor::LineEditor(QWidget *parent):
    QTableView(parent),
    m_model(new QStringListModel(QStringList()<<"// Start code here", this))
{
    setModel(m_model);
    horizontalHeader()->hide();
    m_model->insertColumn(1);
    QString stylesheet = R"(
QListView
{
    background-color: #505050;
}
QItemDelegate
{
    background-color: #202020;
    color: #f2f2f2;
}
)";
    setStyleSheet(stylesheet);
}

QStringList LineEditor::getLines()
{
    return m_model->stringList();
}

void LineEditor::addItem()
{
    if(m_model->insertRow(m_model->rowCount()))
    {

    }
}

void LineEditor::resizeEvent(QResizeEvent *)
{
    if(width()*0.2 < 50)
    {
        setColumnWidth(0, width()*0.8);
        setColumnWidth(1, width()*0.2);
    }
    else
    {
        setColumnWidth(0, width()-50);
        setColumnWidth(1, 50);
    }
}
