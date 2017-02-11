#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtSerialPort/QSerialPort>
#include <QApplication>
#include <QtQuickWidgets/QQuickWidget>
#include <QTextStream>

#include "messagehandler.h"
#include "mainwindow.h"
#include "serial/serialhandler.h"

#define SerialHandlerInstance Singleton<SerialHandler>::instance()

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    qInstallMessageHandler(myMessageHandler);
    MainWindow w;
    w.show();
    return app.exec();

}
