#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtSerialPort/QSerialPort>
#include <QApplication>
#include <QtQuickWidgets/QQuickWidget>
#include <QTextStream>

#include "mainwindow.h"
#include "serial/serialhandler.h"

#define SerialHandlerInstance Singleton<SerialHandler>::instance()

void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
    break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
    break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
    break;
    }
    QFile outFile("log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    qInstallMessageHandler(myMessageHandler);
    MainWindow w;
    w.show();
    return app.exec();

}
