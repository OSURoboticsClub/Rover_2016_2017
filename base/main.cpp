#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtSerialPort/QSerialPort>
#include <QApplication>
#include <QtQuickWidgets/QQuickWidget>

#include "gamepadmonitor.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.connectSerial();
    GamepadMonitor monitor;
    w.show();
    return app.exec();

    /*
    QSerialPort serial;

    //QQmlApplicationEngine engine;
    //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    */

}
