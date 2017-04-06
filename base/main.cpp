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


    QObject *rootView = view.rootObjects()[0];
    Backend b(rootView);




    return app.exec();
}
