#include "lineeditor.h"

#include <QStyle>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>


LineEditor::LineEditor(QWidget *parent):
    QTableWidget(parent),
    m_model(new QStringListModel(QStringList()<<"// Start code here", this))
{
    setColumnCount(2);
    horizontalHeader()->hide();
    horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    horizontalHeader()[0].setMaximumWidth(30);

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
    int row = rowCount();
    insertRow(row);
    setItem(row, 0, new QTableWidgetItem(""));
    QPushButton* btn = new QPushButton("+");
    btn->setMaximumWidth(30);
    setCellWidget(row, 1, btn);
    connect(btn, &QPushButton::clicked, this, [this, row](){ emit runLine(this->item(row, 0)->text()); });
}

void LineEditor::resizeEvent(QResizeEvent *)
{

}
