#include "ofonointerface.h"

ofonoInterface::ofonoInterface(QObject *parent) :
    QObject(parent)
{
    bus = new QDBusConnection(QDBusConnection::systemBus());

    foundModems.clear();
    emit modemsChanged();

//    QVariantList args;
//    args << QString("hello");
//    call.setArguments(args);

}

void ofonoInterface::go()
{
    getModems();
}


void ofonoInterface::getModems()
{
    QDBusMessage call = QDBusMessage::createMethodCall("org.ofono", "/", "org.nemomobile.ofono.ModemManager", "GetAvailableModems");
    bus->callWithCallback(call, this, SLOT(cbGetModemsOk(QDBusMessage)), SLOT(cbGetModemsError(QDBusError)));
}

void ofonoInterface::cbGetModemsError(QDBusError err)
{
    qDebug() << "ERROR" << QDBusError::errorString(err.type());
    foundModems.clear();
    emit modemsChanged();
}

void ofonoInterface::cbGetModemsOk(QDBusMessage msg)
{
    int i=0;
    QDBusArgument da = qvariant_cast<QDBusArgument> ( msg.arguments().at(0) );
    foundModems.clear();

    da.beginArray();
    while (!da.atEnd())
    {
        QVariant v = da.asVariant();
        QDBusObjectPath p;
        p = qvariant_cast<QDBusObjectPath>(v);
        qDebug() << i++ << p.path();
        foundModems.append(p.path());
    }
    da.endArray();

    emit modemsChanged();
}
