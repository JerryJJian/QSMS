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

#ifndef SMSCLIENT_H
#define SMSCLIENT_H

#include <QObject>
#include <QAndroidJniObject>
#include <jni.h>

class ListItem;

void QtSmsClient_jniOnMsgReceived(JNIEnv *, jobject, jlong, jlong);
void QtSmsClient_jniOnMsgSendOut(JNIEnv *, jobject, jlong, jlong, jboolean);
void QtSmsClient_jniOnMsgDelivered(JNIEnv *, jobject, jlong, jlong);
void QtSmsClient_jniOnMsgConversionCreated(JNIEnv *, jobject, jlong, jlong);

class SMSClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int conversionId READ conversionId WRITE setConversionId NOTIFY conversionIdChanged)
public:
    explicit SMSClient(QObject *parent = 0);

    Q_INVOKABLE int conversionId() const;

signals:
    void conversionIdChanged(int id);
//    void updateConversionData();

    void conversionListUpdated(const QList<ListItem *> &items);
    void conversionListAppended(const QList<ListItem *> &items);

    void conversionDataUpdated(const QList<ListItem *> &items);
    void conversionDataPrepended(const QList<ListItem *> &items);

    void contactListUpdate(const QList<ListItem *> &items);

    void appendMsgToConversion(ListItem *item);

    void conversionCreated(int threadid);

    void newMsgReceived(int threadid, const QString &title, const QString &text);
    void reloadConversionList();

public slots:
    void setConversionId(int id);
    void sendMessage(int threadid, const QString &phoneNum, const QString &msg);
    void readContactList();
    void readConversionList(int from = 0, int count = 30);
    void readConversionData(int threadid, int from = 0, int count = 30);
    void updateMessageAsReaded(int threadid);
    void hideClient();
    void deleteSmsByConversionId(const QStringList &idlist);
    void deleteSmsById(const QStringList &idlist);

private slots:
    void updateCurrConversionData();

protected:
    friend void QtSmsClient_jniOnMsgReceived(JNIEnv *, jobject, jlong, jlong);
    friend void QtSmsClient_jniOnMsgSendOut(JNIEnv *, jobject, jlong, jlong, jboolean);
    friend void QtSmsClient_jniOnMsgDelivered(JNIEnv *, jobject, jlong, jlong);
    friend void QtSmsClient_jniOnMsgConversionCreated(JNIEnv *, jobject, jlong, jlong);

    void onMsgReceived(jlong threadid);
    void onMsgSendOut(jlong msgid, jboolean isSuccess);
    void onMsgDelivered(jlong msgid);
    void onMsgConversionCreated(jlong threadid);

private:
    int findConversionDataByAddress(const QString &address);

private:
    QAndroidJniObject m_smsClient;
    int m_conversionId;
    bool m_needNotifyMsg;
};

#endif // SMSCLIENT_H
