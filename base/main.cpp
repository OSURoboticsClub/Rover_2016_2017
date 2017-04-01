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

    QQuickView view;
    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    view.show();

    QObject *rootView = view.rootObject();
    Backend b(rootView);

    return app.exec();
}
