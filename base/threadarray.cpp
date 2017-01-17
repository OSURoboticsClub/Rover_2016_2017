#include "threadarray.h"

ThreadArray::ThreadArray(QObject *parent) : QObject(parent)
{
    numThreads = 0;
    clearing = false;
    threadhead = new threadnode;

}

void ThreadArray::push(QThread *m_thread, bool startImmediately = false)
{
    if (!(m_thread->isRunning())){
        threadnode *temp = new threadnode;
        temp = threadhead;

        int i = 0;
        for (i = 0; i < numThreads;i++){
            temp->next = new threadnode;
            temp = temp->next;
        }
        temp->n_thread = m_thread;
        temp->next = NULL;
        temp->order = i + 1;

        if (startImmediately){
            temp->n_thread->start();
        }

        connect(this, SIGNAL(closeThreads()),temp->n_thread, SLOT(stop()));
        numThreads++;
    }

}

void ThreadArray::convertToArray()
{
    struct threadnode *temp = threadhead;
    threadArray = new QThread *[numThreads];
    for (int i = 0;i < numThreads;i++){
        threadArray[i] = temp->n_thread;
        if (temp->next != NULL) {
            temp = temp->next;
        }
    }
}


bool ThreadArray :: clear()
{
    if (!clearing){
        clearing = true;
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
}

ThreadArray::~ThreadArray()
{
    clear();
}
