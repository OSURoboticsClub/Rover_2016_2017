#include "threadarray.h"

ThreadArray::ThreadArray(QObject *parent) : QObject(parent)
{
    numThreads = 0;
    threadhead = new threadnode;

}

void ThreadArray::push(QThread *m_thread)
{
    threadnode *temp = threadhead;
    qDebug() << "1";
    for (int i = 0; i < numThreads;i++){
        qDebug() << "7";
        temp = temp->next;
    }
    qDebug() << numThreads;
    if (numThreads > 0){
        qDebug() << "9";
        temp->next = new threadnode;
        qDebug() << "10";
        temp = temp->next;
        qDebug() << "11";
    }
    qDebug() << "12";
    //temp->next = 0;
    qDebug() << "2";
    temp->n_thread = m_thread;
    qDebug() << "3";
    m_thread->start();
    qDebug() << "4";
    connect(this, SIGNAL(closeThreads()),m_thread, SLOT(stop()));
    qDebug() << "5";
    numThreads++;
    qDebug() << "6";

}

void ThreadArray::convertToArray()
{
    struct threadnode *temp = threadhead;
    threadArray = new QThread *[numThreads];
    for (int i = 0;i < numThreads;i++){
        threadArray[i] = temp->n_thread;
        temp = temp->next;
    }
}

void ThreadArray::deleteThreadNodes()
{

}

bool ThreadArray :: clear()
{
    emit closeThreads();
    bool allThreadsKilled = false;
    int numThreadsRunning = 0;
    bool terminateHasRun = false;

    QTime startTime;
    startTime.start();

    convertToArray();

    while (!allThreadsKilled)
    {
        for (int i = 0;i < numThreads;i++)
        {
            if (threadArray[i]->isRunning())
            {
                numThreadsRunning++;
            }
        }
        if (numThreadsRunning > 0)
        {
            numThreadsRunning = 0;

            if (!terminateHasRun)
            {
                if (startTime.elapsed() > 8000) //terminate time is 8 seconds. Change to appropriate amount later
                {
                    for (int k = 0;k < numThreads;k++)
                    {
                        if (threadArray[k]->isRunning())
                        {
                            qDebug() << "Terminating thread: " + k;
                            threadArray[k]->terminate();
                        }
                    }
                    terminateHasRun = true;
                    allThreadsKilled = true;
                }
            }
        }

        else
        {
            allThreadsKilled = true;
        }
    }

    for (int i = 0; i < numThreads; i++)
    {
        delete threadArray[i];
    }
    delete threadArray;


    return true;

}

ThreadArray::~ThreadArray()
{
    clear();
}
