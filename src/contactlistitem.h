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

#ifndef CONTACTLISTITEM_H
#define CONTACTLISTITEM_H

#include "listmodel.h"

class ContactListItem : public ListItem
{
    Q_OBJECT
public:
    // contact id, user name, phone num, photo id
    enum Roles
    {
        IDRole = Qt::UserRole + 1,
        UserDisplayNameRole,
        PhoneNumberRole,
        UserPhotoIdRole,
        UserPhotoRole,
        SelectedRole
    };

    struct Data
    {
        Data() { }
        Data( int _id, const QString &_userName, const QString &_phoneNum, int _photoId, const QByteArray &_photo)
            : id(_id), userName(_userName), phoneNum(_phoneNum), photoId(_photoId), photo(_photo), selected(false)
        { }

        Data(const Data &other)
            : id(other.id), userName(other.userName), phoneNum(other.phoneNum),
              photoId(other.photoId), photo(other.photo), selected(other.selected)
        { }

        Data &operator =(const Data &other)
        {
            id = other.id;
            userName = other.userName;
            phoneNum = other.phoneNum;
            photoId = other.photoId;
            photo = other.photo;
            selected = other.selected;

            return *this;
        }

        int id;
        QString userName;
        QString phoneNum;
        int photoId;
        QByteArray photo;
        bool selected;
    };


    explicit ContactListItem(QObject *parent = 0);
    explicit ContactListItem(const ContactListItem::Data &data, QObject *parent = 0);

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

#endif // CONTACTLISTITEM_H
