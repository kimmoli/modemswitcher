#ifndef OFONOINTERFACE_H
#define OFONOINTERFACE_H

#include <QObject>
#include <QtDBus/QtDBus>


class ofonoInterface : public QObject
{
    Q_OBJECT
public:
    explicit ofonoInterface(QObject *parent = 0);

    Q_INVOKABLE void go();
    Q_PROPERTY(QStringList modems READ _modems NOTIFY modemsChanged)

    QStringList _modems() { return foundModems; }

signals:
    void modemsChanged();

public slots:
    void cbGetModemsError(QDBusError err);
    void cbGetModemsOk(QDBusMessage msg);


private:
    void getModems();
    QDBusConnection *bus;

    QStringList foundModems;

};

#endif // OFONOINTERFACE_H
