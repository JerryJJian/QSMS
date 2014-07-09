/****************************************************************************
**
** Copyright (C) 2014 Chen Jian <Jerry.C@foxmail.com>
** All rights reserved.
** Contact: Chen Jian <Jerry.C@foxmail.com>
**
** If you want use this file in the commercial usage, please MUST contact to me.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Chen Jian, email: Jerry.C@foxmail.com
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CONVERSIONDATALISTITEM_H
#define CONVERSIONDATALISTITEM_H

#include "listmodel.h"

class ConversionDataListItem : public ListItem
{
    Q_OBJECT
public:
//    "_id", "address", "person", "body", "date", "type", "protocol", "read", "status"
    enum Roles
    {
        IDRole = Qt::UserRole + 1,
        AddressRole,
        PersonRole,
        BodyRole,
        DateRole,
        TimeRole,
        TypeRole,
        ProtocolRole,
        ReadRole,
        StatusRole,
        SelectedRole
    };

    struct Data
    {
        Data() { }
        Data( int _id, const QString &_address, const QString &_person,
              const QString &_body, const QString &_date, const QString &_time,
              int _type, int _read, int _protocol, int _status)
            : id(_id), address(_address), person(_person), body(_body),
              date(_date), time(_time), type(_type), read(_read),
              protocol(_protocol), status(_status), selected(false)
        { }

        Data(const Data &other)
            : id(other.id), address(other.address), person(other.person), body(other.body),
              date(other.date), time(other.time), type(other.type), read(other.read),
              protocol(other.protocol), status(other.status), selected(other.selected)
        { }

        Data &operator =(const Data &other)
        {
            id = other.id;
            address = other.address;
            person = other.person;
            body = other.body;
            date = other.date;
            time = other.time;
            type = other.type;
            read = other.read;
            protocol = other.protocol;
            status = other.status;
            selected = other.selected;

            return *this;
        }

        int id;
        QString address;
        QString person;
        QString body;
        QString date;
        QString time;
        int type;
        int read;
        int protocol;
        int status;
        bool selected;
    };


    explicit ConversionDataListItem(QObject *parent = 0);
    explicit ConversionDataListItem(const ConversionDataListItem::Data &data, QObject *parent = 0);

    virtual QString id() const;
    virtual bool selected() const;
    virtual void setSelected(bool selected);
    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);
    virtual QHash<int, QByteArray> roleNames() const;

private:
    void initRoles();
    QHash<int, QByteArray> m_roles;
    Data m_data;
};

#endif // CONVERSIONDATALISTITEM_H
