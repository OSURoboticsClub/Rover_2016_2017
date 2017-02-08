#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtSerialPort/QSerialPort>
#include <QApplication>
#include <QtQuickWidgets/QQuickWidget>
#include <QTextStream>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>

#include "mainwindow.h"
#include "serial/serialhandler.h"

#define SerialHandlerInstance Singleton<SerialHandler>::instance()

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

//    QQmlEngine engine;
//    QQmlComponent component(&engine,QUrl(QStringLiteral("qrc:/panel.qml")));
//    QObject *object = component.create();

 //   QQuickItem *item = qobject_cast<QQuickItem*>(object);
//    item->setWidth(500);

    QQuickView view(QUrl(QStringLiteral("qrc:/panel.qml")));
    QObject *item = view.rootObject();

    MainWindow w;

    QObject::connect(item, SIGNAL(debugTest(QString)), &w, SLOT(printFromQml(QString)));
    QObject::connect(item, SIGNAL(closeMainWindow()), &w, SLOT(close()));
    QObject::connect(item, SIGNAL(closeMainWindow()), item, SLOT(deleteLater()));

    //w.show();
    view.show();

       qDebug() << "main";

    return app.exec();


}
