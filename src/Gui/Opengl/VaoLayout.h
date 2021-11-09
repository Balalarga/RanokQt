#ifndef SHADERLAYOUT_H
#define SHADERLAYOUT_H

#include <QObject>
#include <QGL>

class VaoLayout : public QObject
{
    Q_OBJECT
public:
    explicit VaoLayout(QObject *parent = nullptr);

    template<class T>
    void Add(int count)
    {

    }

signals:


private:

};

#endif // SHADERLAYOUT_H
