#include "lineeditor.h"

#include <QStyle>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>


LineEditor::LineEditor(QWidget *parent):
    QTableWidget(parent)
{
    setColumnCount(2);
    horizontalHeader()->hide();
    horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

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


void LineEditor::addItem()
{
    int row = rowCount();
    insertRow(row);
    setItem(row, 0, new QTableWidgetItem(""));
    QPushButton* btn = new QPushButton(QPixmap("assets/images/playIcon.svg"), "");
    btn->setMaximumWidth(30);
    setCellWidget(row, 1, btn);
    connect(btn, &QPushButton::clicked, this, [this, row](){ emit runLine(this->item(row, 0)->text()); });
}

bool LineEditor::eventFilter(QObject *obj, QEvent *evt)
{
    if (evt->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
        if (keyEvent->key() == Qt::Key_Enter)
        {
            emit runLine(this->item(rowCount()-1, 0)->text());
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return QObject::eventFilter(obj, evt);
    }
}

void LineEditor::resizeEvent(QResizeEvent *)
{
    setColumnWidth(0, width()-70);
    setColumnWidth(1, 50);
}
