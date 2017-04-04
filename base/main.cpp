#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine>

#include "messagehandler.h"
#include "backend.h"
#include "qfi_ADI.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //qInstallMessageHandler(myMessageHandler);
    //qmlRegisterType<qfi_ADI>("Widgets", 1, 0, "AttitudeIndicator");

    QtWebEngine::initialize();

    QQmlApplicationEngine view(QUrl("qrc:/qml/main.qml"));

    //view.setSource();
    //view.show();

    //QObject *rootView = view.rootObject();
    //Backend b(rootView);

    return app.exec();
}
