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

#ifndef CONVERSIONLISTITEM_H
#define CONVERSIONLISTITEM_H

#include "listmodel.h"

class ConversionListItem : public ListItem
{
    Q_OBJECT
public:
    // "_id", "date", "message_count", "recipient_ids",
    // "snippet", "snippet_cs", "read", "error", "has_attachment"
    enum Roles
    {
        IDRole = Qt::UserRole + 1,
        DateRole,
        TimeRole,
        MsgCountRole,
        RecipientIDRole,
        SnippetRole,
        SnippetCSRole,
        ReadRole,
        ErrorRole,
        HasAttachmentRole,
        SelectedRole,
        // contact
        UserDisplayNameRole,
        PhoneNumberRole,
        ContactIdRole,
        UserPhotoIdRole,
        UserPhotoRole
    };

    struct Data
    {
        Data() { }
        Data( int _id, const QString &_date, const QString &_time, int _msgCount, const QString & _recipIDs,
              const QString & _snippet, int _snippetCS, int _read, int _error, int _hasAttach)
            : id(_id), date(_date), time(_time), msgCount(_msgCount), recipIDs(_recipIDs), snippet(_snippet),
              snippetCS(_snippetCS), read(_read), error(_error), hasAttach(_hasAttach), selected(false),
              userName(""), phoneNum(""), contactId(-1), photoId(-1)
        { }

        Data(const Data &other)
            : id(other.id), date(other.date), time(other.time), msgCount(other.msgCount),
              recipIDs(other.recipIDs), snippet(other.snippet), snippetCS(other.snippetCS),
              read(other.read), error(other.error), hasAttach(other.hasAttach),
              selected(other.selected), userName(other.userName), phoneNum(other.phoneNum),
              contactId(other.contactId), photoId(other.photoId), photo(other.photo)
        { }

        Data &operator =(const Data &other)
        {
            id = other.id;
            date = other.date;
            time = other.time;
            msgCount = other.msgCount;
            recipIDs = other.recipIDs;
            snippet = other.snippet;
            snippetCS = other.snippetCS;
            read = other.read;
            error = other.error;
            hasAttach = other.hasAttach;
            selected = other.selected;
            userName = other.userName;
            phoneNum = other.phoneNum;
            contactId = other.contactId;
            photoId = other.photoId;
            photo = other.photo;

            return *this;
        }

        int id;
        QString date;
        QString time;
        int msgCount;
        QString recipIDs;
        QString snippet;
        int snippetCS;
        int read;
        int error;
        int hasAttach;
        bool selected;
        QString userName;
        QString phoneNum;
        int contactId;
        int photoId;
        QByteArray photo;
    };


    explicit ConversionListItem(QObject *parent = 0);
    explicit ConversionListItem(const ConversionListItem::Data &data, QObject *parent = 0);

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

#endif // CONVERSIONLISTITEM_H
