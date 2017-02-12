#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtSerialPort/QSerialPort>
#include <QApplication>
#include <QtQuickWidgets/QQuickWidget>
#include <QTextStream>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QtWebEngine>

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
    QtWebEngine::initialize();

    QQuickView view;
    view.setSource(QUrl("qrc:/main.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);

//    QObject *item = view.rootObject();



    //QObject::connect(item, SIGNAL(closeMainWindow()), &w, SLOT(close()));
    //QObject::connect(item, SIGNAL(closeMainWindow()), item, SLOT(deleteLater()));

    //w.show();
    view.show();

    QObject *object = view.rootObject();
    MainWindow w(object);

    //   qDebug() << "main";

    return app.exec();
}
