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

import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import UIControllor 1.0

Window {
    id: mainWindow
    width: 1080
    height: 800
    visible: true
    property int pageId: PageView.NullPageView

    property real pixelPerMM: Screen.logicalPixelDensity * 1.7

    Rectangle {
        id: mainViewBg
        anchors.fill: parent
        color: "#F2F2F2"
    }

    property Component smsListView: SmsListView {
        id: smsListView
        width: parent.width; height: parent.height

        onShowConversion: {
            stackView.push({ item: conversionView })
            stackView.currentItem.forceActiveFocus()
            UIControllor.conversionId = threadid
            UIControllor.conversionUserName = username
            UIControllor.conversionPhoneNum = phonenum
            UIControllor.conversionUserHeadImg = headimg
        }
    }

    property Component conversionView: ConversionView {
        id: conversionView
        width: parent.width; height: parent.height

        onGoback: {
            stackView.pop()
        }

        onShowContactList: {
            stackView.push({ item: contactListView })
        }
    }

    property Component contactListView: ContactListView {
        id: contactListView
        width: parent.width; height: parent.height

        onGoback: {
            stackView.pop()
        }

        onSelectedUser: {
            stackView.pop()
            UIControllor.isUserInputPhoneNum = false
            UIControllor.conversionUserName = username
            UIControllor.conversionPhoneNum = phonenum
            UIControllor.conversionUserHeadImg = headimg
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: smsListView
        onCurrentItemChanged: {
            currentItem.forceActiveFocus();
            UIControllor.currView = currentItem.pageId
        }
    }
}
