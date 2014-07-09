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

#include "smsclient.h"
#include <QDebug>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras>
#include <QtAndroid>
#include <jni.h>
#include "conversionlistitem.h"
#include "conversiondatalistitem.h"
#include "contactlistitem.h"

SMSClient::SMSClient(QObject *parent) :
    QObject(parent), m_conversionId(-1), m_needNotifyMsg(false)
{
    m_smsClient = QAndroidJniObject("org/qtproject/example/QSMS/sms/SMSClient");
    m_smsClient.setStaticField<jlong>("org/qtproject/example/QSMS/sms/SMSClient",
                                             "qtObject", reinterpret_cast<long>(this));

    QAndroidJniObject::callStaticMethod<void>("org/qtproject/example/QSMS/sms/SMSClient",
                                              "setActivity",
                                              "(Landroid/app/Activity;)V",
                                              QtAndroid::androidActivity().object<jobject>());

    connect(this, SIGNAL(reloadConversionList()), this, SLOT(readConversionList()), Qt::QueuedConnection);
}

int SMSClient::conversionId() const
{
    return m_conversionId;
}

void SMSClient::setConversionId(int id)
{
    if (m_conversionId == id)
        return ;

    m_conversionId = id;
    emit conversionIdChanged(m_conversionId);
}

void SMSClient::sendMessage(int threadid, const QString &phoneNum, const QString &msg)
{
    QString currYear(QDate::currentDate().toString(QLatin1String("yyyy-")));
    QAndroidJniObject phoneNumber = QAndroidJniObject::fromString(phoneNum);
    QAndroidJniObject msgText = QAndroidJniObject::fromString(msg);
    QAndroidJniObject sendedMsg = m_smsClient.callObjectMethod("sendMessage",
                                                               "(JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;",
                                                               (jlong)threadid,
                                                               phoneNumber.object<jstring> (),
                                                               msgText.object<jstring> () );

    QStringList datalist(sendedMsg.toString().split(QLatin1String("\n"), QString::SkipEmptyParts));

    if (datalist.size() >= 1)
    {
        QStringList datas(datalist.at(0).split(QLatin1String(",")));
        if (datas.size() < 9 || datas.value(0, "-1").toInt() < 0) return ;
        // "_id", "address", "person", "body", "date", "type", "protocol", "read", "status"
    //        int _id, const QString &_address, const QString &_person, const QString &_body,
    //                      const QString &_date, const QString &_time, int _read, int _protocol, int _status
        ListItem *item = new ConversionDataListItem(ConversionDataListItem::Data(datas.value(0).toInt(),
                                                                               datas.value(1),
                                                                               datas.value(2),
                                                                               QString::fromUtf8(QByteArray::fromBase64(datas.value(3).toUtf8())),
                                                                               datas.value(4).section(QLatin1String(" "), 0, 0).replace(currYear, ""),
                                                                               datas.value(4).section(QLatin1String(" "), 1, 1),
                                                                               datas.value(5).toInt(),
                                                                               datas.value(7).toInt(),
                                                                               datas.value(6).toInt(),
                                                                               datas.value(8).toInt()));

        emit appendMsgToConversion(item);
        emit reloadConversionList();
    }
}

void SMSClient::readContactList()
{
    QAndroidJniObject conversionData = QAndroidJniObject::callStaticObjectMethod("org/qtproject/example/QSMS/sms/SMSClient",
                                              "readContactList", "()Ljava/lang/String;");

    QStringList contactList(conversionData.toString().split(QLatin1String("\n"), QString::SkipEmptyParts));

    QList<ListItem *> contactItems;
    foreach (const QString &item, contactList)
    {
        QStringList datas(item.split(QLatin1String(",")));

        if (datas.size() < 4 || datas.value(0).toInt() < 0) continue;

        // contact id, user name, phone num, photo id
        ListItem *it = new ContactListItem(ContactListItem::Data(datas.value(0).toInt(),
                                                                 datas.value(1),
                                                                 datas.value(2),
                                                                 datas.value(3).toInt(),
                                                                 datas.value(4).toUtf8()));
        contactItems.append(it);
    }

    if (contactItems.count() > 0)
        emit contactListUpdate(contactItems);
}


void SMSClient::readConversionList(int from, int count)
{
    QStringList smsList;
    int read = 1;
    int cnter = 0;

    do {
        QAndroidJniObject conversionDataList = QAndroidJniObject::callStaticObjectMethod("org/qtproject/example/QSMS/sms/SMSClient",
                                                  "readConversionList", "(II)Ljava/lang/String;", from, count);

        smsList = conversionDataList.toString().split(QLatin1String("\n"), QString::SkipEmptyParts);

        QStringList datas(smsList.value(0, "").split(QLatin1String(",")));
        read = datas.value(6, "1").toInt();
    }
    while (m_needNotifyMsg && read != 0 && cnter++ < 2);

    QList<ListItem *> conversionList;
    QStringList userList;
    QHash<QString, ListItem *> conversionMap;
    QString currYear(QDate::currentDate().toString(QLatin1String("yyyy-")));

    // check unread msg
//    bool checkLatestMsg = (from == 0 && m_conversionId >= 0 && smsList.size() > 0);

    foreach (const QString &item, smsList)
    {
        QStringList datas(item.split(QLatin1String(",")));

        if (datas.size() < 9 || datas.value(0, "-1").toInt() < 0) continue;

//        "_id", "date", "message_count", "recipient_ids",
//        "snippet", "snippet_cs", "read", "error", "has_attachment"

        userList.append(datas.value(3).split(QLatin1String(" "), QString::SkipEmptyParts));
        ListItem *it = new ConversionListItem(ConversionListItem::Data(datas.value(0).toInt(),
                                                                       datas.value(1).section(QLatin1String(" "), 0, 0).replace(currYear, ""),
                                                                       datas.value(1).section(QLatin1String(" "), 1, 1),
                                                                       datas.value(2).toInt(),
                                                                       datas.value(3),
                                                                       QString::fromUtf8(QByteArray::fromBase64(datas.value(4).toUtf8())),
                                                                       datas.value(5).toInt(),
                                                                       datas.value(6).toInt(),
                                                                       datas.value(7).toInt(),
                                                                       datas.value(8).toInt()));

        conversionList.append(it);
        conversionMap.insert(datas.value(3), it);
    }

    if (userList.size() > 0)
    {
        QAndroidJniObject userIds = QAndroidJniObject::fromString(userList.join(QLatin1String(",")));
        QAndroidJniObject userInfos = QAndroidJniObject::callStaticObjectMethod("org/qtproject/example/QSMS/sms/SMSClient",
                                                                                "readContactInfo",
                                                                                "(Ljava/lang/String;)Ljava/lang/String;",
                                                                                userIds.object<jstring> ());

        QStringList contactList(userInfos.toString().split(QLatin1String("\n"), QString::SkipEmptyParts));

//        QHash<QString, ListItem *> userHeadMap;

        foreach (const QString &contact, contactList)
        {
            // thread id, contact id, user name, phone num, photo id
            QStringList user(contact.split(QLatin1String(",")));

            if (user.size() < 5 || user.value(0, "-1").toInt() < 0)
                continue ;


            QString recipient_id(user.value(0));

            ListItem *it = conversionMap.value(recipient_id, NULL);

            if (it == NULL)
            {
                foreach (const QString &key, conversionMap.keys())
                {
                    if (key.contains(QLatin1String(" "))
                            && key.split(QLatin1String(" ")).contains(recipient_id))
                    {
                        it = conversionMap.value(key, NULL);
                    }
                }
            }

            if (it == NULL) continue ;

            if (!user.value(2).isEmpty())
            {
                it->setData(ConversionListItem::UserDisplayNameRole,    QVariant::fromValue(user.value(2)));
                it->setData(ConversionListItem::PhoneNumberRole,        QVariant::fromValue(user.value(3)));
                it->setData(ConversionListItem::ContactIdRole,          QVariant::fromValue(user.value(1)));
                it->setData(ConversionListItem::UserPhotoIdRole,        QVariant::fromValue(user.value(4)));
                it->setData(ConversionListItem::UserPhotoRole,          QVariant::fromValue(user.value(5).toUtf8()));

//                if (!user.value(4).isEmpty())
//                {
//                    userHeadMap.insert(user.value(4), it);
//                }
            }
            else if (it->data(ConversionListItem::PhoneNumberRole).toString().isEmpty())
            {
                it->setData(ConversionListItem::PhoneNumberRole, QVariant::fromValue(user.value(3).remove("+86")));
            }
        }

//        if (!userHeadMap.isEmpty())
//        {
//            QStringList photoList(userHeadMap.keys());
//            QAndroidJniObject photoIds = QAndroidJniObject::fromString(photoList.join(QLatin1String(",")));
//            QAndroidJniObject userHeads = QAndroidJniObject::callStaticObjectMethod("org/qtproject/example/QSMS/sms/SMSClient",
//                                                                                    "readContactPhoto",
//                                                                                    "(Ljava/lang/String;)Ljava/lang/String;",
//                                                                                    photoIds.object<jstring> ());

//             QStringList userHeadList(userHeads.toString().split(QLatin1String("\n"), QString::SkipEmptyParts));

//             foreach (const QString &userItem, userHeadList)
//             {
//                 // photo id, photo data
//                 QStringList photos(userItem.split(QLatin1String(",")));

//                 if (photos.size() < 2 || photos.value(0).isEmpty())
//                     continue ;

//                 qDebug() << "#" << photos.value(0) << ":" << photos.value(1);
//             }
//        }

    }

    if (conversionList.size() > 0)
    {
        if (m_needNotifyMsg)
        {
            m_needNotifyMsg = false;

            ListItem *item = conversionList.at(0);

            if (item != NULL
                    && item->data(ConversionListItem::ReadRole).toInt() == 0)
            {
                if (item->data(ConversionListItem::IDRole).toInt() == m_conversionId)
                {
//                    emit appendMsgToConversion(item);
                    readConversionData(m_conversionId);
                }
                else
                {
                    QString username(item->data(ConversionListItem::UserDisplayNameRole).toString());
                    QString phonenum(item->data(ConversionListItem::PhoneNumberRole).toString());
                    QString msgcontent(item->data(ConversionListItem::SnippetRole).toString());

                    emit newMsgReceived(item->data(ConversionListItem::IDRole).toInt(),
                                        (username.isEmpty() ? phonenum : username), msgcontent);
                }

            }
        }

        if (from == 0)
            emit conversionListUpdated(conversionList);
        else
            emit conversionListAppended(conversionList);
    }
}

void SMSClient::readConversionData(int threadid, int from, int count)
{
    if (threadid < 0)
        return ;

    QAndroidJniObject conversionData = QAndroidJniObject::callStaticObjectMethod("org/qtproject/example/QSMS/sms/SMSClient",
                                              "readConversionData",
                                              "(III)Ljava/lang/String;", threadid, from, count);

    QStringList smsList(conversionData.toString().split(QLatin1String("\n"), QString::SkipEmptyParts));

    QString currYear(QDate::currentDate().toString(QLatin1String("yyyy-")));
    QList<ListItem *> conversionItems;
    foreach (const QString &item, smsList)
    {
        QStringList datas(item.split(QLatin1String(",")));

        if (datas.size() < 9 || datas.value(0).toInt() < 0) continue;

        // "_id", "address", "person", "body", "date", "type", "protocol", "read", "status"
//        int _id, const QString &_address, const QString &_person, const QString &_body,
//                      const QString &_date, const QString &_time, int _read, int _protocol, int _status
        ListItem *it = new ConversionDataListItem(ConversionDataListItem::Data(datas.value(0).toInt(),
                                                                               datas.value(1),
                                                                               datas.value(2),
                                                                               QString::fromUtf8(QByteArray::fromBase64(datas.value(3).toUtf8())),
                                                                               datas.value(4).section(QLatin1String(" "), 0, 0).replace(currYear, ""),
                                                                               datas.value(4).section(QLatin1String(" "), 1, 1),
                                                                               datas.value(5).toInt(),
                                                                               datas.value(7).toInt(),
                                                                               datas.value(6).toInt(),
                                                                               datas.value(8).toInt()));
        conversionItems.prepend(it);
    }

    if (conversionItems.count() > 0)
    {
        if (from == 0)
            emit conversionDataUpdated(conversionItems);
        else
            emit conversionDataPrepended(conversionItems);
    }
}

void SMSClient::updateMessageAsReaded(int threadid)
{
    if (threadid < 0)
        return ;

    QAndroidJniObject::callStaticMethod<void>("org/qtproject/example/QSMS/sms/SMSClient",
                                              "updateMessageAsReaded", "(I)V", threadid);
}

void SMSClient::hideClient()
{
    QAndroidJniObject::callStaticMethod<void>("org/qtproject/example/QSMS/sms/SMSClient", "hideClient");
    //"org/qtproject/qt5/android/bindings/QtActivity", "hideClient");
}

void SMSClient::deleteSmsByConversionId(const QStringList &idlist)
{
    QAndroidJniObject idobj = QAndroidJniObject::fromString(idlist.join(QLatin1String(",")));
    QAndroidJniObject::callStaticMethod<void>("org/qtproject/example/QSMS/sms/SMSClient",
                                              "deleteSmsByConversionId", "(Ljava/lang/String;)V",
                                              idobj.object<jstring> ());
}

void SMSClient::deleteSmsById(const QStringList &idlist)
{
    QAndroidJniObject idobj = QAndroidJniObject::fromString(idlist.join(QLatin1String(",")));
    QAndroidJniObject::callStaticMethod<void>("org/qtproject/example/QSMS/sms/SMSClient",
                                              "deleteSmsById", "(Ljava/lang/String;)V",
                                              idobj.object<jstring> ());
}

void SMSClient::updateCurrConversionData()
{
    readConversionData(m_conversionId);
    updateMessageAsReaded(m_conversionId);
}

void SMSClient::onMsgReceived(jlong threadid)
{
    qDebug() << Q_FUNC_INFO << ">" << threadid;
    m_needNotifyMsg = true;
    emit reloadConversionList();
}

void SMSClient::onMsgSendOut(jlong msgid, jboolean isSuccess)
{
    qDebug() << Q_FUNC_INFO << ">" << msgid << ":" << isSuccess;
}

void SMSClient::onMsgDelivered(jlong msgid)
{
    qDebug() << Q_FUNC_INFO << ">" << msgid;
}

void SMSClient::onMsgConversionCreated(jlong threadid)
{
    qDebug() << Q_FUNC_INFO << ">" << threadid;
    emit conversionCreated(threadid);
}
