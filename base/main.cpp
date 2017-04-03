#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine>

#include "messagehandler.h"
#include "backend.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //qInstallMessageHandler(myMessageHandler);
    QtWebEngine::initialize();

    QQmlApplicationEngine view(QUrl("qrc:/qml/main.qml"));
    //view.setSource();
    //view.show();

    //QObject *rootView = view.rootObject();
    //Backend b(rootView);

    return app.exec();
}
