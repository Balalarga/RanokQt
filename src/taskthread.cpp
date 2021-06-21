#include "taskthread.h"


TaskThread::TaskThread(ICalculator *task, QObject *parent) :
    QThread(parent),
    m_task(task)
{

}


void TaskThread::run()
{
    m_task->Calculate();
}
