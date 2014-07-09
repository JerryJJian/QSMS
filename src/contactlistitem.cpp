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

#include "contactlistitem.h"

ContactListItem::ContactListItem(QObject *parent) :
    ListItem(parent)
{
    initRoles();
}

ContactListItem::ContactListItem(const ContactListItem::Data &data, QObject *parent)
    : ListItem(parent), m_data(data)
{
    initRoles();
}


QString ContactListItem::id() const
{
    return data(IDRole).toString();
}

bool ContactListItem::selected() const
{
    return data(SelectedRole).toBool();
}

void ContactListItem::setSelected(bool selected)
{
    setData(SelectedRole, selected);
}

QVariant ContactListItem::data(int role) const
{
    switch (role)
    {
    case IDRole:                return QVariant::fromValue(m_data.id);
    case UserDisplayNameRole:   return QVariant::fromValue(m_data.userName);
    case PhoneNumberRole:       return QVariant::fromValue(m_data.phoneNum);
    case UserPhotoIdRole:       return QVariant::fromValue(m_data.photoId);
    case UserPhotoRole:         return QVariant::fromValue(m_data.photo);
    case SelectedRole:          return QVariant::fromValue(m_data.selected);
    }

    return QVariant();
}

bool ContactListItem::setData(int role, const QVariant &value)
{
    if (role == SelectedRole)
    {
        m_data.selected = value.toBool();
        emit dataChanged();
        return true;
    }
    else if (role == UserPhotoRole)
    {
        m_data.photo = value.toByteArray();
        emit dataChanged();
        return true;
    }

    return false;
}

QHash<int, QByteArray> ContactListItem::roleNames() const
{
    return m_roles;
}

void ContactListItem::initRoles()
{
    m_roles.insert(IDRole,              QByteArray("id"));
    m_roles.insert(SelectedRole,        QByteArray("selected"));
    m_roles.insert(UserDisplayNameRole, QByteArray("displayname"));
    m_roles.insert(PhoneNumberRole,     QByteArray("phoneNum"));
    m_roles.insert(UserPhotoIdRole,     QByteArray("photoId"));
    m_roles.insert(UserPhotoRole,       QByteArray("photo"));
}
