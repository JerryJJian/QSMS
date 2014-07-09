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
import UIControllor 1.0
import ImageViews 1.0

Item {
    id: root
    width: 100
    height: 62

    property int pageId: PageView.SmsListView
    property bool needUpdating: false

    signal showUserInfo(string phoneNum)
    signal showConversion(int threadid, string username, string phonenum, string headimg)
    signal fetchMore()
    signal deleteConversion()
    focus: true

    Keys.onBackPressed: {
        if (ConversionListModel.selectedMode) {
            ConversionListModel.cancelSlectectMode()
            return ;
        }

        SMSClient.hideClient()
        event.accepted = true
    }

    Component.onCompleted: {
        SMSClient.readConversionList()
    }

    onFetchMore: {
        needUpdating = false
        SMSClient.readConversionList(ConversionListModel.count)
    }

    onDeleteConversion: {
        SMSClient.deleteSmsByConversionId(ConversionListModel.selectedItems())
        ConversionListModel.removeSelectedItems()
    }

    Item {
        id: titleBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 10 * mainWindow.pixelPerMM
        z: smsList.z + 1

        property alias title: titleLabel.text

        Image {
            id: iconImage
            anchors.left: parent.left; anchors.leftMargin: 2.4 * mainWindow.pixelPerMM
            anchors.verticalCenter: parent.verticalCenter
            source: "./images/icon.png"
            width: 6 * mainWindow.pixelPerMM
            height: width
        }

        Text {
            id: titleLabel
            anchors.left: iconImage.right; anchors.leftMargin: 2.4 * mainWindow.pixelPerMM
            anchors.verticalCenter: parent.verticalCenter
            width: Text.paintedWidth
            height: Text.paintedHeight
            font.pixelSize: 3.5 * mainWindow.pixelPerMM
            text: qsTr("Message")
        }

        Image {
            id: delMsgsBtn
            width: 7 * mainWindow.pixelPerMM; height: width
            anchors.centerIn: delMsgsBtnMA
//            anchors.verticalCenter: parent.verticalCenter
//            anchors.right: parent.right; anchors.rightMargin: 3 * mainWindow.pixelPerMM
            source: "./images/delete.png"
            scale: delMsgsBtnMA.pressed ? 1.2 : 1
            Behavior on scale { NumberAnimation { easing.type: Easing.OutBack; duration: 200 } }
            visible: ConversionListModel.selectedMode
        }

        MouseArea {
            id: delMsgsBtnMA
            anchors.right: newMsgBtnMA.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 10 * mainWindow.pixelPerMM
            visible: delMsgsBtn.visible
            onClicked: root.deleteConversion()
        }

        Image {
            id: newMsgBtn
            width: 5 * mainWindow.pixelPerMM; height: width
            anchors.centerIn: newMsgBtnMA
//            anchors.verticalCenter: parent.verticalCenter
//            anchors.right: parent.right; anchors.rightMargin: 3 * mainWindow.pixelPerMM
            source: "./images/message.png"
            scale: newMsgBtnMA.pressed ? 1.2 : 1
            Behavior on scale { NumberAnimation { easing.type: Easing.OutBack; duration: 200 } }
        }

        MouseArea {
            id: newMsgBtnMA
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 10 * mainWindow.pixelPerMM
            onClicked: root.showConversion(-1, "", "", "")
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.bottom
            opacity: 0.5
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#CBCBCB" }
                GradientStop { position: 1; color: "white" }
            }

            height: 0.5 * mainWindow.pixelPerMM
        }
    }

    ListView {
        id: smsList
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true

        section.property: "date"
        section.criteria: ViewSection.FullString
        section.delegate: Item { width: smsList.width; height: 2.4 * mainWindow.pixelPerMM }

        model: ConversionListModel

        delegate: Rectangle {
            id: delegateRect
            x: 2.4 * mainWindow.pixelPerMM
            width: smsList.width - 4.8 * mainWindow.pixelPerMM
            height: 14 * mainWindow.pixelPerMM
            color: itemMouseArea.pressed ? "#83C8EF" : "white"

            MouseArea {
                id: itemMouseArea
                anchors.fill: parent

                onReleased: {
                    if (ConversionListModel.selectedMode) {
                        selected = !selected
                    }
                    else {
                        if (read === 0) {
                            read = 1
                            SMSClient.updateMessageAsReaded(id)
                        }
                        root.showConversion(id, displayname, phoneNum, photo)
                    }
                }
            }

            Rectangle {
                id: headImage
                x: 2.4 * mainWindow.pixelPerMM;
                anchors.verticalCenter: parent.verticalCenter
                width: 10 * mainWindow.pixelPerMM; height: width
                border.color: "#E0E0E0"
                border.width: 1
                radius: 3

                Image {
                    width: headImage.width - 2
                    height: headImage.height - 2
                    anchors.centerIn: headImage
                    source: "./images/userhead.png"
                    visible: !userHeadImg.visible
                    opacity: 0.3
                }

                ImageView {
                    id: userHeadImg
                    width: headImage.width - 2
                    height: headImage.height - 2
                    anchors.centerIn: headImage
                    source: photo
                    visible: photoId > 0
                }
            }

            Rectangle {
                id: unreadFlag
                width: height
                height: latestMsg.font.pixelSize
                radius: width / 2
                color: "#41B1F0"
                border.width: 1
                border.color: "#59ADD9"
                visible: read === 0
                anchors.verticalCenter: userName.verticalCenter
                anchors.left: headImage.right; anchors.leftMargin: 2.4 * mainWindow.pixelPerMM
            }

            Text {
                id: userName
                text: displayname === "" ? phoneNum : displayname
                height: Text.paintedHeight
                anchors.left: unreadFlag.visible ? unreadFlag.right : headImage.right
                anchors.leftMargin: (unreadFlag.visible ? 1 : 2.4) * mainWindow.pixelPerMM
                anchors.right: msgTime.left; anchors.rightMargin: 2.4 * mainWindow.pixelPerMM
                anchors.top: parent.top; anchors.topMargin: 0.8 * mainWindow.pixelPerMM
                color: "black"
                maximumLineCount: 1
//                font.bold: true
                font.pixelSize: 3.5 * mainWindow.pixelPerMM
            }

            Text {
                id: latestMsg
                text: snippet
                anchors.left: userName.left
                anchors.top: userName.bottom; anchors.topMargin: 0.6 * mainWindow.pixelPerMM
                anchors.bottom: parent.bottom; anchors.bottomMargin: 0.6 * mainWindow.pixelPerMM
                anchors.right: selectedMouseArea.left
                maximumLineCount: 2
                lineHeightMode: Text.FixedHeight
                lineHeight: 3.5 * mainWindow.pixelPerMM
                font.pixelSize: 3 * mainWindow.pixelPerMM
                color: "#7F7F7F"
                wrapMode: Text.WrapAnywhere
            }

            Text {
                id: msgTime
                width: Text.paintedWidth
                height: Text.paintedHeight
                anchors.right: parent.right; anchors.rightMargin: 2.4 * mainWindow.pixelPerMM
                anchors.verticalCenter: userName.verticalCenter
                color: "#7F7F7F"
                font.pixelSize: 2.4 * mainWindow.pixelPerMM
                text: date // + " " + time
            }

            Rectangle {
                id: selectedFlag
                anchors.verticalCenter: latestMsg.verticalCenter
                anchors.horizontalCenter: selectedMouseArea.horizontalCenter
                width: ConversionListModel.selectedMode ? (3 * mainWindow.pixelPerMM) : mainWindow.pixelPerMM
                height: width
                radius: width / 2
                color: ConversionListModel.selectedMode ? "#F2F2F2" : border.color
                border.color: "#B9B9B9"
                border.width: 1

                Image {
                    anchors.fill: parent
                    source: "./images/checked.png"
                    visible: selected
                }
            }

            Rectangle {
                width: delegateRect.width
                anchors.bottom: parent.bottom
                height: 1
                color: "#E2E2E2"
            }

            MouseArea {
                id: selectedMouseArea
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: height
                onClicked: {
                    selected = !selected
                }
            }
        }

        header: Item { height: 2.4 * mainWindow.pixelPerMM }
        footer: Item { height: 2.4 * mainWindow.pixelPerMM }

        Rectangle {
            id: scrollBar
            width: 0.5 * mainWindow.pixelPerMM
            height: smsList.height * smsList.height / smsList.contentHeight
            anchors.right: parent.right; anchors.rightMargin: 0.5 * mainWindow.pixelPerMM
            y: smsList.contentY * (smsList.height - scrollBar.height) / (smsList.contentHeight - smsList.height)
            color: "#A4A4A4"
            visible: scrollBar.height < smsList.height
            opacity: 0
        }

        onMovementStarted: {
            if (!visible) return ;
            scrollbarFadeoutAnimation.stop()
            scrollBar.opacity = 1
        }

        onMovementEnded: {
            scrollbarFadeoutAnimation.start()
        }

        onAtYEndChanged: {
            if (atYEnd /*&& ConversionListModel.count > 0*/) {
                if (flicking || dragging)
                    needUpdating = true
            }
        }

        onFlickingChanged: {
            if (!flicking && needUpdating) root.fetchMore()
        }

        onDraggingChanged: {
            if (!dragging && needUpdating) root.fetchMore()
        }

        SequentialAnimation　{
            id: scrollbarFadeoutAnimation
            PauseAnimation { duration: 1000 }
            PropertyAnimation {
                target: scrollBar
                easing.type: Easing.InCirc
                properties: "opacity"
                from:　1
                to: 0
                duration: 1000
            }
        }

    }


}
