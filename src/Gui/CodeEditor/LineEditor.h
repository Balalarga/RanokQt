#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include <QListView>
#include <QPushButton>
#include <QTableWidget>
#include <QStringListModel>

class LineEditor : public QTableWidget
{
    Q_OBJECT
public:
    LineEditor(QWidget *parent = nullptr);

    bool eventFilter(QObject *obj, QEvent *evt) override;

protected:
    void resizeEvent(QResizeEvent*) override;

signals:
    void runLine(QString line);


public slots:
    void addItem();

};

#endif // LINEEDITOR_H
