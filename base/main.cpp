#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine>
#include <QQuickStyle>

#include "messagehandler.h"
#include "backend.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //qInstallMessageHandler(myMessageHandler);

    QtWebEngine::initialize();

    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine view(QUrl("qrc:/qml/main.qml"));

    //QObject *rootView = view.rootObject();
    //Backend b(rootView);

    //this is irratating because it always throws a qdebug message when connecting the view to deleteLater(), but this is an actual bug with qt and neccessary to properly close out all of the threads
    //QObject::connect(rootView, SIGNAL(closeMainWindow()), &b, SLOT(close()));
    //QObject::connect(rootView, SIGNAL(closeMainWindow()), item, SLOT(deleteLater()));

    return app.exec();
}
