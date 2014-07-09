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

#include "conversiondatalistitem.h"

ConversionDataListItem::ConversionDataListItem(QObject *parent)
    : ListItem(parent)
{
    initRoles();
}

ConversionDataListItem::ConversionDataListItem(const ConversionDataListItem::Data &data, QObject *parent) :
    ListItem(parent), m_data(data)
{
    initRoles();
}

QString ConversionDataListItem::id() const
{
    return data(IDRole).toString();
}

bool ConversionDataListItem::selected() const
{
    return data(SelectedRole).toBool();
}

void ConversionDataListItem::setSelected(bool selected)
{
    setData(SelectedRole, selected);
}

//    "_id", "address", "person", "body", "date", "type", "protocol", "read", "status"
QVariant ConversionDataListItem::data(int role) const
{
    switch (role)
    {
    case IDRole:            return QVariant::fromValue(m_data.id);
    case DateRole:          return QVariant::fromValue(m_data.date);
    case TimeRole:          return QVariant::fromValue(m_data.time);
    case AddressRole:       return QVariant::fromValue(m_data.address);
    case PersonRole:        return QVariant::fromValue(m_data.person);
    case BodyRole:          return QVariant::fromValue(m_data.body);
    case TypeRole:          return QVariant::fromValue(m_data.type);
    case ReadRole:          return QVariant::fromValue(m_data.read);
    case ProtocolRole:      return QVariant::fromValue(m_data.protocol);
    case StatusRole:        return QVariant::fromValue(m_data.status);
    case SelectedRole:      return QVariant::fromValue(m_data.selected);
    }

    return QVariant();
}

bool ConversionDataListItem::setData(int role, const QVariant &value)
{
    if (role == SelectedRole)
    {
        m_data.selected = value.toBool();
        emit dataChanged();
        return true;
    }

    return false;
}

QHash<int, QByteArray> ConversionDataListItem::roleNames() const
{
    return m_roles;
}

void ConversionDataListItem::initRoles()
{
    m_roles.insert(IDRole,              QByteArray("id"));
    m_roles.insert(DateRole,            QByteArray("date"));
    m_roles.insert(TimeRole,            QByteArray("time"));
    m_roles.insert(AddressRole,         QByteArray("address"));
    m_roles.insert(PersonRole,          QByteArray("person"));
    m_roles.insert(BodyRole,            QByteArray("body"));
    m_roles.insert(TypeRole,            QByteArray("type"));
    m_roles.insert(ReadRole,            QByteArray("read"));
    m_roles.insert(ProtocolRole,        QByteArray("protocol"));
    m_roles.insert(StatusRole,          QByteArray("status"));
    m_roles.insert(SelectedRole,        QByteArray("selected"));
}
