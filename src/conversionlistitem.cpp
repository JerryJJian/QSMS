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

#include "conversionlistitem.h"
#include <QDebug>

ConversionListItem::ConversionListItem(QObject *parent)
    : ListItem(parent)
{
    initRoles();
}

ConversionListItem::ConversionListItem(const ConversionListItem::Data &data, QObject *parent) :
    ListItem(parent), m_data(data)
{
    initRoles();
}

QString ConversionListItem::id() const
{
    return data(IDRole).toString();
}

bool ConversionListItem::selected() const
{
    return data(SelectedRole).toBool();
}

void ConversionListItem::setSelected(bool selected)
{
    setData(SelectedRole, selected);
}

QVariant ConversionListItem::data(int role) const
{
    switch (role)
    {
    case IDRole:                return QVariant::fromValue(m_data.id);
    case DateRole:              return QVariant::fromValue(m_data.date);
    case TimeRole:              return QVariant::fromValue(m_data.time);
    case MsgCountRole:          return QVariant::fromValue(m_data.msgCount);
    case RecipientIDRole:       return QVariant::fromValue(m_data.recipIDs);
    case SnippetRole:           return QVariant::fromValue(m_data.snippet);
    case SnippetCSRole:         return QVariant::fromValue(m_data.snippetCS);
    case ReadRole:              return QVariant::fromValue(m_data.read);
    case ErrorRole:             return QVariant::fromValue(m_data.error);
    case HasAttachmentRole:     return QVariant::fromValue(m_data.hasAttach);
    case SelectedRole:          return QVariant::fromValue(m_data.selected);
    case UserDisplayNameRole:   return QVariant::fromValue(m_data.userName);
    case PhoneNumberRole:       return QVariant::fromValue(m_data.phoneNum);
    case ContactIdRole:         return QVariant::fromValue(m_data.contactId);
    case UserPhotoIdRole:       return QVariant::fromValue(m_data.photoId);
    case UserPhotoRole:         return QVariant::fromValue(m_data.photo);
    }

    return QVariant();
}

bool ConversionListItem::setData(int role, const QVariant &value)
{
    bool edited = false;
    switch (role)
    {
    case ReadRole:
        m_data.read = value.toInt();
        edited = true;
        break;

    case SelectedRole:
        m_data.selected = value.toBool();
        edited = true;
        break;

    case UserDisplayNameRole:
        m_data.userName = value.toString();
        edited = true;
        break;

    case PhoneNumberRole:
        m_data.phoneNum = value.toString();
        edited = true;
        break;

    case ContactIdRole:
        m_data.contactId = value.toInt();
        edited = true;
        break;

    case UserPhotoIdRole:
        m_data.photoId = value.toInt();
        edited = true;
        break;

    case UserPhotoRole:
        m_data.photo = value.toByteArray();
        edited = true;
        break;

    default: break;
    }

    if (edited)
        emit dataChanged();

    return edited;
}

QHash<int, QByteArray> ConversionListItem::roleNames() const
{
    return m_roles;
}

void ConversionListItem::initRoles()
{
    m_roles.insert(IDRole,              QByteArray("id"));
    m_roles.insert(DateRole,            QByteArray("date"));
    m_roles.insert(TimeRole,            QByteArray("time"));
    m_roles.insert(MsgCountRole,        QByteArray("msgCount"));
    m_roles.insert(RecipientIDRole,     QByteArray("recipientId"));
    m_roles.insert(SnippetRole,         QByteArray("snippet"));
    m_roles.insert(SnippetCSRole,       QByteArray("snippetCS"));
    m_roles.insert(ReadRole,            QByteArray("read"));
    m_roles.insert(ErrorRole,           QByteArray("error"));
    m_roles.insert(HasAttachmentRole,   QByteArray("hasAttach"));
    m_roles.insert(SelectedRole,        QByteArray("selected"));
    m_roles.insert(UserDisplayNameRole, QByteArray("displayname"));
    m_roles.insert(PhoneNumberRole,     QByteArray("phoneNum"));
    m_roles.insert(ContactIdRole,       QByteArray("contactId"));
    m_roles.insert(UserPhotoIdRole,     QByteArray("photoId"));
    m_roles.insert(UserPhotoRole,       QByteArray("photo"));
}
