#ifndef THREADARRAY_H
#define THREADARRAY_H

#include <QDebug>
#include <QObject>
#include <QThread>
#include <QTime>
#include "serial/serialhandler.h"
#include "inputs/controllerhandler.h"

class ThreadArray : public QObject {
    Q_OBJECT
public:
    void push(QThread *m_thread, bool startImmediately);
    bool clear();
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
signals:
    void closeThreads();
};

#endif
