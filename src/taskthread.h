#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include <QThread>
#include "Calculators/ICalculator.h"

class TaskThread : public QThread
{
    Q_OBJECT
public:
    TaskThread(ICalculator* task, QObject *parent = nullptr);


protected:
    void run() override;


private:
    ICalculator* m_task;


};

#endif // TASKTHREAD_H
