#ifndef THREADARRAY_H
#define THREADARRAY_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <QList>

#include "serial/serialhandler.h"
#include "inputs/controllerhandler.h"

class ThreadArray : public QObject {
    Q_OBJECT
public:
    void push(QThread *m_thread, bool startImmediately);
    bool clear();

    void m_push(QThread *m_thread, bool startImmediately);
    bool m_clear();
    ThreadArray();
    ~ThreadArray();
private:
    int numThreads;
    struct threadnode {
        QThread *n_thread;
        threadnode *next;
        bool _running;
        bool nextBlank;
    };
    threadnode *threadhead;
    //this is so that I don't have to rewrite the clear function
    QThread **threadArray;
    void convertToArray();
    bool clearing;

    QList<QThread *> threadList;

signals:
    void closeThreads();
};

#endif
