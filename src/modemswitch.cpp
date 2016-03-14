
#include <sailfishapp.h>
#include <QtQml>
#include <QScopedPointer>
#include <QQuickView>
#include <QQmlEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QCoreApplication>
#include "ofonointerface.h"

int main(int argc, char *argv[])
{

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    ofonoInterface *o = new ofonoInterface();

    view->rootContext()->setContextProperty("Ofono", o);

    view->setSource(SailfishApp::pathTo("qml/modemswitch.qml"));
    view->show();

    return app->exec();
}

