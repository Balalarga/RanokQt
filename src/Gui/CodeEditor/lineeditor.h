#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include <QListView>
#include <QTableView>
#include <QPushButton>
#include <QStringListModel>

class LineEditor : public QTableView
{
    Q_OBJECT
public:
    LineEditor(QWidget *parent = nullptr);
    QStringList getLines();


signals:
    void runLine(QString line);


public slots:
    void addItem();


protected:
    void resizeEvent(QResizeEvent*);


private:
    QStringListModel* m_model;
};

#endif // LINEEDITOR_H
