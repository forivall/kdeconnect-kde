/**
 * Copyright 2013 Albert Vaca <albertvaka@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QDBusConnection>
#include <QString>
#include <qvector.h>
#include "devicelinks/devicelink.h"

class DeviceLink;

class Device : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "kdeconnect.device")

public:

    //Device known from KConfig, we trust it but we need to wait for a incoming devicelink to communicate
    Device(const QString& id, const QString& name);

    //Device known via a presentation package sent to us via a devicelink, we know everything but we don't trust it yet
    Device(const QString& id, const QString& name, DeviceLink* dl);

    //Device known via discovery, we know nothing and have to ask for a presentation package
    //(not supported yet, do we need it or we can rely on the device presenging itself?)
    //Device(const QString& id, DeviceLink* dl);

    void addLink(DeviceLink*);
    void removeLink(DeviceLink*);

    bool sendPackage(const NetworkPackage& np);

Q_SIGNALS:
    void receivedPackage(const NetworkPackage& np);

public Q_SLOTS:
    Q_SCRIPTABLE QString id() const{ return m_deviceId; }
    Q_SCRIPTABLE QString name() const { return m_deviceName; }
    Q_SCRIPTABLE bool paired() const { return m_paired; }
    Q_SCRIPTABLE bool reachable() const { return !m_deviceLinks.empty(); }
    Q_SCRIPTABLE void setPair(bool b);
    Q_SCRIPTABLE void sendPing();

private Q_SLOTS:
    void privateReceivedPackage(const NetworkPackage& np);

private:
    bool m_paired;
    QString m_deviceId;
    QString m_deviceName;
    QVector<DeviceLink*> m_deviceLinks;
    bool m_knownIdentiy;


};

#endif // DEVICE_H