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

    property int pageId: PageView.ContactListView
    signal goback();
    signal selectedUser(string username, string phonenum, string headimg)

    Keys.onBackPressed: root.goback()

    Connections {
        target: UIControllor
        onCurrViewChanged: {
            if (currView !== PageView.ContactListView) return ;
            if (ContactListModel.count == 0)
                SMSClient.readContactList()
        }
    }

    Item {
        id: titleBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 10 * mainWindow.pixelPerMM
        z: contactList.z + 1

        Image {
            id: iconImage
            anchors.left: parent.left; anchors.leftMargin: 2.4 * mainWindow.pixelPerMM
            anchors.verticalCenter: parent.verticalCenter
            source: "./images/search.png"
            width: 6 * mainWindow.pixelPerMM
            height: width
            opacity: 0.5
        }

        Item {
            id: searchEditor
            anchors.left: iconImage.right; anchors.leftMargin: 2.4 * mainWindow.pixelPerMM
            anchors.right: parent.right; anchors.rightMargin: 13 * mainWindow.pixelPerMM
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            TextEdit {
                id: textEditor
                anchors.left: parent.left; anchors.leftMargin: 3 * mainWindow.pixelPerMM
                anchors.right: parent.right; anchors.rightMargin: 3 * mainWindow.pixelPerMM
                anchors.verticalCenter: parent.verticalCenter; anchors.verticalCenterOffset: 0.5 * mainWindow.pixelPerMM
                height: Text.paintedHeight
                font.pixelSize: 3.5 * mainWindow.pixelPerMM
                selectByMouse: false
                activeFocusOnPress: true
                text: UIControllor.contactListFilter
                onTextChanged: {
                    UIControllor.contactListFilter = text
                }
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom; anchors.bottomMargin: mainWindow.pixelPerMM
                height: 0.5 * mainWindow.pixelPerMM
                color: "#D0D0D0"

                Rectangle {
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    width: 0.5 * mainWindow.pixelPerMM
                    height: mainWindow.pixelPerMM
                    color: "#D0D0D0"
                }

                Rectangle {
                    anchors.right: parent.right
                    anchors.bottom: parent.top
                    width: 0.5 * mainWindow.pixelPerMM
                    height: mainWindow.pixelPerMM
                    color: "#D0D0D0"
                }
            }
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
        id: contactList
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true

        section.property: "displayname"
        section.criteria: ViewSection.FirstCharacter
        section.delegate: Item { width: contactList.width; height: 2.4 * mainWindow.pixelPerMM }

        model: ContactListProxyModel

        delegate: Rectangle {
            id: delegateRect
            x: 2.4 * mainWindow.pixelPerMM
            width: contactList.width - 4.8 * mainWindow.pixelPerMM
            height: 14 * mainWindow.pixelPerMM
            color: itemMouseArea.pressed ? "#83C8EF" : "white"

            MouseArea {
                id: itemMouseArea
                anchors.fill: parent

                onClicked: {
//                    selected = !selected
                    root.selectedUser(displayname, phoneNum, photo)
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
                    source: "images/userhead.png"
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

            Text {
                id: userName
                text: displayname === "" ? phoneNum : displayname
                width: Text.paintedWidth
                height: Text.paintedHeight
                anchors.left: headImage.right
                anchors.leftMargin: 2.4 * mainWindow.pixelPerMM
                anchors.top: parent.top; anchors.topMargin: 2 * mainWindow.pixelPerMM
                color: "black"
                maximumLineCount: 1
                font.pixelSize: 3.5 * mainWindow.pixelPerMM
            }

            Text {
                id: phoneNumLabel
                text: phoneNum
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

            Rectangle {
                id: selectedFlag
                anchors.verticalCenter: phoneNumLabel.verticalCenter
                anchors.horizontalCenter: selectedMouseArea.horizontalCenter
                width: 3 * mainWindow.pixelPerMM
                height: width
                radius: width / 2
                color: "#F2F2F2"
                border.color: "#B9B9B9"
                border.width: 1
                visible: false

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
            height: contactList.height * contactList.height / contactList.contentHeight
            anchors.right: parent.right; anchors.rightMargin: 0.5 * mainWindow.pixelPerMM
            y: contactList.headerItem.height + contactList.contentY * (contactList.height - scrollBar.height - contactList.headerItem.height - contactList.footerItem.height)
               / (contactList.contentHeight - contactList.height - contactList.headerItem.height - contactList.footerItem.height)
            color: "#A4A4A4"
            visible: scrollBar.height < contactList.height
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

        SequentialAnimation {
            id: scrollbarFadeoutAnimation
            PauseAnimation { duration: 1000 }
            PropertyAnimation {
                target: scrollBar
                easing.type: Easing.InCirc
                properties: "opacity"
                from: 1
                to: 0
                duration: 1000
            }
        }

    }

}
