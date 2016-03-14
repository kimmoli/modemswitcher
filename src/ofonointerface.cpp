#include "ofonointerface.h"

ofonoInterface::ofonoInterface(QObject *parent) :
    QObject(parent)
{
    bus = new QDBusConnection(QDBusConnection::systemBus());

    foundModems.clear();
    foundModemPaths.clear();
    emit modemsChanged();
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
    foundModemPaths.clear();
    emit modemsChanged();
}

void ofonoInterface::cbGetModemsOk(QDBusMessage msg)
{
    int i=0;
    QDBusArgument da = qvariant_cast<QDBusArgument> ( msg.arguments().at(0) );
    foundModems.clear();
    foundModemPaths.clear();

    da.beginArray();
    while (!da.atEnd())
    {
        QVariant v = da.asVariant();
        QDBusObjectPath p;
        p = qvariant_cast<QDBusObjectPath>(v);
        qDebug() << i++ << p.path();
        foundModems.append(p.path());
        foundModemPaths.append(p);
    }
    da.endArray();

    emit modemsChanged();
}

void ofonoInterface::setModem(int index)
{
    if (index < 0 || index >= foundModems.count())
        return;

    QDBusMessage call = QDBusMessage::createMethodCall("org.ofono", "/", "org.nemomobile.ofono.ModemManager", "SetEnabledModems");
    QVariantList args;
    QVariantList pl;
    pl << QVariant::fromValue(foundModemPaths.at(index));
    args <<  pl;
    call.setArguments(args);
    bus->call(call);
}
