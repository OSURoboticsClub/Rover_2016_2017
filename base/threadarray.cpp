#include "threadarray.h"

ThreadArray::ThreadArray(QObject *parent) : QObject(parent)
{
    numThreads = 0;
    clearing = false;
    threadhead = NULL;

}


void ThreadArray::push(QThread *m_thread, bool startImmediately)
{
    if (!(m_thread->isRunning())){
        qDebug() << 1;
        threadnode *temp = new threadnode;
        temp->n_thread = m_thread;
        temp->next = new threadnode;
        temp->nextBlank = true;
        qDebug() << 2;
        if (threadhead == NULL) {

            threadhead = temp;

        }
        else {
            threadnode* last = threadhead;
            for(int i=0;i<numThreads;i++) {
                last = last->next;
            }
            qDebug() << "here";
            last->next = temp;
            last->nextBlank = false;
            qDebug() << "there";
        }

        if (startImmediately){
            m_thread->start();
        }
        qDebug() << 3;
        connect(this, SIGNAL(closeThreads()), m_thread, SLOT(stop()));
        numThreads++;

    }
}

void ThreadArray::convertToArray()
{
    struct threadnode *temp = threadhead;
    threadArray = new QThread *[numThreads];
    for (int i = 0;i < numThreads;i++){
        threadArray[i] = temp->n_thread;
        if (!temp->nextBlank) {
            temp = temp->next;
        }
    }
}


bool ThreadArray :: clear()
{
    if (!clearing){
        qDebug() << 'a';
        clearing = true;
        emit closeThreads();
        bool allThreadsKilled = false;
        int numThreadsRunning = 0;
        bool terminateHasRun = false;

        QTime startTime;
        startTime.start();

        convertToArray();
        qDebug() << 'b';
        while (!allThreadsKilled)
        {
            qDebug() << 'c';
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
            qDebug() << 'd';
        }
        qDebug() << 'e';
        for (int i = 0; i < numThreads; i++)
        {
            qDebug() << numThreads;
            qDebug() << numThreadsRunning;
            qDebug() << "here";
            delete threadArray[i];
            qDebug() << "there";
        }
        delete threadArray;

        qDebug() << 'f';

        return true;
    }
}

ThreadArray::~ThreadArray()
{
    clear();
}
