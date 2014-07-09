/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtAndroidExtras module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

package org.qtproject.example.QSMS.notification;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.util.Log;
import android.app.Activity;
import org.qtproject.example.QSMS.R;
import android.content.Intent;
import android.content.IntentFilter;
import android.app.PendingIntent;
import android.content.ComponentName;

public class NotificationClient
{
    private static NotificationManager m_notificationManager;
    private static Notification.Builder m_builder;
    private static Activity m_activity = null;

    public static void setActivity(Activity activity)
    {
        m_activity = activity;
    }

    public static void notify(int id, String title, String text)
    {
        if (m_notificationManager == null) {
            m_notificationManager = (NotificationManager)m_activity.getSystemService(Context.NOTIFICATION_SERVICE);
            m_builder = new Notification.Builder(m_activity);
            m_builder.setSmallIcon(R.drawable.icon);
        }

        m_builder.setWhen(System.currentTimeMillis());
        m_builder.setAutoCancel(true);

        Notification notifier = m_builder.getNotification();
        notifier.defaults = Notification.DEFAULT_ALL;

        Intent appIntent = new Intent(Intent.ACTION_MAIN);
        appIntent.addCategory(Intent.CATEGORY_LAUNCHER);
//        appIntent.setComponent(new ComponentName(m_activity.getPackageName(), m_activity.getPackageName() + "." + m_activity.getLocalClassName()));
        appIntent.setComponent(new ComponentName("org.qtproject.example.QSMS", "org.qtproject.qt5.android.bindings.QtActivity"));
        appIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK| Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
        PendingIntent contentIntent = PendingIntent.getActivity(m_activity, 0, appIntent, PendingIntent.FLAG_UPDATE_CURRENT);

        notifier.setLatestEventInfo(m_activity, title, text, contentIntent);
        m_notificationManager.notify(id, notifier);

    }

}
