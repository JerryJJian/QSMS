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
import QtQuick.Controls 1.1
import UIControllor 1.0
import ImageViews 1.0

Item {
    id: root
    width: 100
    height: 62
    property int pageId: PageView.ConversionView

    property int conversionId: UIControllor.conversionId
    property string userDisplayName: UIControllor.conversionUserName
    property string userPhoneNum: UIControllor.conversionPhoneNum
    property string userHeadImage: UIControllor.conversionUserHeadImg

    property bool needUpdating: false

    signal fetchMore();
    signal goback();
    signal sendMsg(string phone, string text)
    signal showContactList()
    signal deleteMsg()

    Component.onCompleted: UIControllor.isUserInputPhoneNum = true

    onConversionIdChanged: {
        needUpdating = false
        SMSClient.conversionId = conversionId
        if (conversionId < 0)
            ConversionDataListModel.clear()
        else
            SMSClient.readConversionData(conversionId)
    }

    onFetchMore: {
        needUpdating = false
        SMSClient.readConversionData(conversionId, ConversionDataListModel.count)
    }

    onSendMsg: {
        SMSClient.sendMessage(root.conversionId, phone, text)
    }

    onDeleteMsg: {
        SMSClient.deleteSmsById(ConversionDataListModel.selectedItems())
        ConversionDataListModel.removeSelectedItems()
    }

    Connections {
        target: ConversionDataListModel
        onDataModelAppended: conversionList.positionViewAtEnd()
        onDataModelPrepended: conversionList.positionViewAtIndex(ConversionDataListModel.effectCount, ListView.Beginning)
    }

    Connections {
        target: SMSClient
        onConversionCreated: {
            if (UIControllor.currView !== root.pageId) return ;
            if (UIControllor.conversionId === threadid) return ;
            if (UIControllor.conversionId < 0) UIControllor.conversionId = threadid;
        }
    }

    Keys.onBackPressed: {
        if (ConversionDataListModel.selectedMode) {
            ConversionDataListModel.cancelSlectectMode()
            return ;
        }

        root.goback()
    }

    Item {
        id: titleBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 10 * mainWindow.pixelPerMM
        z: conversionList.z + 1

        states: [
            State {
                name: "newmsg"
                when: root.conversionId < 0
                PropertyChanges { target: userInfo; visible: false; }
                PropertyChanges { target: titleItem; visible: true; }
            },
            State {
                name: "user"
                when: root.userDisplayName !== ""
                PropertyChanges { target: userInfo; visible: true; }
                PropertyChanges { target: userName; text: userDisplayName; color: "black"; anchors.topMargin: 0.2 * mainWindow.pixelPerMM; }
                PropertyChanges { target: phoneNum; visible: true; }
                PropertyChanges { target: titleItem; visible: false; }
            },
            State {
                name: "phone"
                when: root.userDisplayName === ""
                PropertyChanges { target: userInfo; visible: true; }
                PropertyChanges { target: userName; text: userPhoneNum; color: phoneNum.color; anchors.topMargin: 2 * mainWindow.pixelPerMM; }
                PropertyChanges { target: phoneNum; visible: false; }
                PropertyChanges { target: titleItem; visible: false; }
            }
        ]

        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: userInfo.right
            visible: userMouseArea.pressed
        }

        Item {
            id: titleItem
            anchors.fill: parent
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
                text: qsTr("New Message")
            }
        }

        Item {
            id: userInfo
            anchors.top: parent.top; anchors.topMargin: mainWindow.pixelPerMM
            anchors.left: parent.left
            anchors.bottom: parent.bottom; anchors.bottomMargin: mainWindow.pixelPerMM
            width: headImageRect.width + 8 * mainWindow.pixelPerMM + Math.max(userName.width, phoneNum.width)

            Rectangle {
                id: headImageRect
                anchors.left: parent.left; anchors.leftMargin: 2.4 * mainWindow.pixelPerMM;
                anchors.verticalCenter: parent.verticalCenter
                width: 7 * mainWindow.pixelPerMM; height: width
                border.color: "#E0E0E0"
                border.width: 1
                radius: 3

                Image {
                    id: headImage
                    width: headImageRect.width - 2
                    height: headImageRect.height - 2
                    anchors.centerIn: headImageRect
                    source: "./images/userhead.png"
                    visible: !userHeadImg.visible
                    opacity: 0.3
                }

                ImageView {
                    id: userHeadImg
                    width: headImageRect.width - 2
                    height: headImageRect.height - 2
                    anchors.centerIn: headImageRect
                    source: root.userHeadImage
                    visible: isvalid
                }
            }

            Text {
                id: userName
                text: userDisplayName
                height: Text.paintedHeight
                width: Text.paintedWidth
                anchors.top: parent.top;
                anchors.left: headImageRect.right; anchors.leftMargin: 2.4 * mainWindow.pixelPerMM
                maximumLineCount: 1
//                font.bold: true
                font.pixelSize: 3.5 * mainWindow.pixelPerMM
            }

            Text {
                id: phoneNum
                text: userPhoneNum
                anchors.left: userName.left
                anchors.top: userName.bottom; anchors.topMargin: 0.3 * mainWindow.pixelPerMM
                width: Text.paintedWidth
                height: Text.paintedHeight
                font.pixelSize: 3 * mainWindow.pixelPerMM
                color: "#7F7F7F"
                wrapMode: Text.WrapAnywhere
            }

            Image {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: 2.4 * mainWindow.pixelPerMM; height: width
                source: "./images/dropableflag.png"
            }

            MouseArea {
                id: userMouseArea
                anchors.fill: parent
            }
        }

        Image {
            id: delMsgBtn
            width: 7 * mainWindow.pixelPerMM; height: width
            anchors.centerIn: delMsgBtnMA
            source: "./images/delete.png"
            scale: delMsgBtnMA.pressed ? 1.2 : 1
            Behavior on scale { NumberAnimation { easing.type: Easing.OutBack; duration: 200 } }
            visible: ConversionDataListModel.selectedMode
        }

        MouseArea {
            id: delMsgBtnMA
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 10 * mainWindow.pixelPerMM
            visible: delMsgBtn.visible
            onClicked: root.deleteMsg()
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

    Rectangle {
        id: toolbar
        anchors.left: parent.left;
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: textEditor.height
        z: conversionList.z + 1

        Image {
            id: gobackBtn
            width: 5 * mainWindow.pixelPerMM; height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left; anchors.leftMargin: 2 * mainWindow.pixelPerMM
            source: "./images/goback.png"
            scale: gobackBtnMA.pressed ? 1.2 : 1
            Behavior on scale { NumberAnimation { easing.type: Easing.OutBack; duration: 200 } }
        }

        MouseArea {
            id: gobackBtnMA
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 10 * mainWindow.pixelPerMM
            onClicked: root.goback()
        }

        TextEditor {
            id: textEditor
            anchors.left: gobackBtnMA.right; anchors.leftMargin: 3 * mainWindow.pixelPerMM
            anchors.right: sentoutBtnMA.left; anchors.rightMargin: 3 * mainWindow.pixelPerMM
            anchors.bottom: parent.bottom
            maxHeight: root.height / 3
        }

//        TextArea {
//            id: textEditor
//            anchors.left: gobackBtnMA.right; anchors.leftMargin: 3 * mainWindow.pixelPerMM
//            anchors.right: sentoutBtnMA.left; anchors.rightMargin: 3 * mainWindow.pixelPerMM
//            anchors.bottom: parent.bottom
//            height: Text.paintedHeight
//            font.pixelSize: 3.5 * mainWindow.pixelPerMM
//            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
//            selectByMouse: false
//            activeFocusOnPress: true
//        }

        Image {
            id: sentoutBtn
            width: 5 * mainWindow.pixelPerMM ; height: width
            anchors.centerIn: sentoutBtnMA
//            anchors.verticalCenter: parent.verticalCenter
//            anchors.right: parent.right; anchors.rightMargin: 2.4 * mainWindow.pixelPerMM
            source: "./images/sentout.png"
            opacity: textEditor.text === "" ? 0.4 : (sentoutBtnMA.pressed ? 0.6 : 0.8)
        }

        MouseArea {
            id: sentoutBtnMA
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 10 * mainWindow.pixelPerMM
            onClicked: {
                console.log("onSendMsgButtonClicked: " + userPhoneNum + " > " + textEditor.text)
                if (userPhoneNum === "" || textEditor.text === "") return ;
                root.sendMsg(userPhoneNum, textEditor.text)
                textEditor.text = ""
            }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.top
            opacity: 0.5
            gradient: Gradient {
                GradientStop { position: 1; color: "#CBCBCB" }
                GradientStop { position: 0; color: "white" }
            }

            height: 0.5 * mainWindow.pixelPerMM
        }
    }

    Item {
        id: userSelector
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: visible ? 10 * mainWindow.pixelPerMM : 0
        visible: root.conversionId < 0 || root.userPhoneNum === ""
        z: conversionList.z + 1

        Text {
            id: sendToPersonLabel
            anchors.left: parent.left; anchors.leftMargin: 2.4 * mainWindow.pixelPerMM
            anchors.verticalCenter: parent.verticalCenter
            width: Text.paintedWidth
            height: Text.paintedHeight
            font.pixelSize: 3.5 * mainWindow.pixelPerMM
            text: qsTr("Send to")
            color: "#B2B2B2"
        }

        TextEdit {
            id: sendToAddressEditor
            anchors.left: sendToPersonLabel.right; anchors.leftMargin: 2 * mainWindow.pixelPerMM
            anchors.right: selectPersonBtn.left; anchors.rightMargin: 2 * mainWindow.pixelPerMM
            anchors.top: parent.top; anchors.topMargin: 3 * mainWindow.pixelPerMM
            anchors.bottom: parent.bottom; anchors.bottomMargin: 3 * mainWindow.pixelPerMM
            font.pixelSize: 3.5 * mainWindow.pixelPerMM
            color: "black"
            visible: UIControllor.isUserInputPhoneNum

            onTextChanged: {
                UIControllor.isUserInputPhoneNum = true
                UIControllor.conversionUserName = text
                UIControllor.conversionPhoneNum = text
                UIControllor.conversionUserHeadImg = ""
            }
        }

        Text {
            id: sendToAddressLabel
            anchors.left: sendToPersonLabel.right; anchors.leftMargin: 2 * mainWindow.pixelPerMM
            anchors.right: selectPersonBtn.left; anchors.rightMargin: 2 * mainWindow.pixelPerMM
            anchors.top: parent.top; anchors.topMargin: 3 * mainWindow.pixelPerMM
            anchors.bottom: parent.bottom; anchors.bottomMargin: 3 * mainWindow.pixelPerMM
            visible: !UIControllor.isUserInputPhoneNum
            text: UIControllor.conversionUserName === "" ? UIControllor.conversionPhoneNum : UIControllor.conversionUserName

            MouseArea {
                anchors.right: parent.right; anchors.rightMargin: 10 * mainWindow.pixelPerMM
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: height

                onClicked: {
                    UIControllor.conversionPhoneNum = ""
                    UIControllor.conversionUserName = ""
                    UIControllor.conversionUserHeadImg = ""
                    UIControllor.isUserInputPhoneNum = true
                }

                Image {
                    id: clearFlag
                    width: 5 * mainWindow.pixelPerMM
                    height: width
                    anchors.centerIn: parent

                    opacity: 0.3
                    scale: parent.pressed ? 1.2 : 1
                    source: "./images/clear.png"
                    Behavior on scale { NumberAnimation { duration: 200 } }
                }
            }
        }

        Image {
            id: selectPersonBtn
            width: 7 * mainWindow.pixelPerMM ; height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right; anchors.rightMargin: 2.4 * mainWindow.pixelPerMM
            source: "./images/select_person.png"
            opacity: selectPersonBtnMA.pressed ? 0.8 : 0.5
            scale: selectPersonBtnMA.pressed ? 1.2 : 1
            Behavior on scale { NumberAnimation { easing.type: Easing.OutBack; duration: 200 } }
        }

        MouseArea {
            id: selectPersonBtnMA
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 10 * mainWindow.pixelPerMM
            onClicked: {
                root.showContactList()
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
        id: conversionList
        anchors.top: userSelector.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: toolbar.top
        clip: true
        property int fristVisionIndex: 0

        model: ConversionDataListModel

        delegate: Item {
            id: delegateRect
            width: conversionList.width
            height: datetimeLabel.height + msgLabel.paintedHeight + 11 * mainWindow.pixelPerMM

            states: [
                State {
                    name: "inmsg"
                    when: type === 1
                    PropertyChanges { target: contentRect; anchors.left: delegateRect.left; anchors.leftMargin: 3 * mainWindow.pixelPerMM }
                    PropertyChanges { target: datetimeLabel; anchors.left: msgContent.left; anchors.leftMargin: 2 * mainWindow.pixelPerMM }
                    PropertyChanges { target: directionFlag; source: selected ? "./images/inmsgflag_selected.png" : "./images/inmsgflag.png"; anchors.right: msgContent.left }
                    PropertyChanges { target: msgContent; color: selected ? "#7DC2E9" : "white" }
                    PropertyChanges { target: msgLabel; anchors.left: contentRect.left; anchors.leftMargin: 3 * mainWindow.pixelPerMM }
                },
                State {
                    name: "outmsg"
                    when: type !== 1
                    PropertyChanges { target: contentRect; anchors.right: delegateRect.right; anchors.rightMargin: 3 * mainWindow.pixelPerMM }
                    PropertyChanges { target: datetimeLabel; anchors.right: contentRect.right; anchors.rightMargin: 5 * mainWindow.pixelPerMM }
                    PropertyChanges { target: directionFlag; source: selected ? "./images/outmsgflag_selected.png" : "./images/outmsgflag.png"; anchors.left: msgContent.right }
                    PropertyChanges { target: msgContent; color: selected ? "#7DC2E9" : (status === 128 ? "#F7AF65" : "#D9D9D9") }
                    PropertyChanges { target: msgLabel; anchors.right: contentRect.right; anchors.rightMargin: 5 * mainWindow.pixelPerMM - msgLabel.width + msgLabel.paintedWidth }
                }
            ]

            Item {
                id: contentRect
                width: parent.width - 17 * mainWindow.pixelPerMM
                height: parent.height
                anchors.top: parent.top

                Text {
                    id: datetimeLabel
                    height: Text.paintedHeight
                    width: Text.paintedWidth
                    anchors.top: parent.top; anchors.topMargin: 2.4 * mainWindow.pixelPerMM
                    color: "#B2B2B2"
                    text: date + " " + time
                    font.pixelSize: 3 * mainWindow.pixelPerMM

                    Image {
                        width: 2 * mainWindow.pixelPerMM; height: width
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.right; anchors.leftMargin: 1.2 * mainWindow.pixelPerMM
                        source: "./images/sended_ok.png"
                        visible: type !== 1
                    }
                }

                Rectangle {
                    id: msgContent
                    width: Math.abs(contentRect.width - msgLabel.paintedWidth - 6 * mainWindow.pixelPerMM) < msgLabel.font.pixelSize ? contentRect.width : (msgLabel.paintedWidth + 6 * mainWindow.pixelPerMM)
                    height: msgLabel.paintedHeight + 6 * mainWindow.pixelPerMM
                    anchors.verticalCenter: msgLabel.verticalCenter
                    anchors.left: msgLabel.left;
                    anchors.leftMargin: - 2 * mainWindow.pixelPerMM
                    radius: 5

                    Image {
                        id: directionFlag
                        width: 2 * mainWindow.pixelPerMM; height: width
                        anchors.top: parent.top; anchors.topMargin: 3 * mainWindow.pixelPerMM
                        Behavior on source {
                            NumberAnimation {
                                properties: "opacity"
                                from: 0
                                to: 1
                                duration: 500
                            }
                        }
                    }

                    Behavior on color { ColorAnimation { duration: 200 } }
                }

                Text {
                    id: msgLabel
                    height: Text.paintedHeight
                    width: contentRect.width - 6 * mainWindow.pixelPerMM
                    anchors.top: datetimeLabel.bottom; anchors.topMargin: 5 * mainWindow.pixelPerMM
                    font.pixelSize: 3.5 * mainWindow.pixelPerMM

                    text: body
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                MouseArea {
                    anchors.fill: msgContent
                    onPressAndHold: {
                        selected = !selected
                    }
                    onClicked: {
                        if (ConversionDataListModel.selectedMode)
                            selected = !selected
                    }
                }
            }
        }

        header: Item { height: 2.4 * mainWindow.pixelPerMM }
        footer: Item { height: 2.4 * mainWindow.pixelPerMM }

        onAtYBeginningChanged: {
            if (atYBeginning && (flicking || dragging)  && root.conversionId >= 0) {
                needUpdating = true
            }
        }

        onFlickingChanged: {
            if (!flicking && needUpdating) root.fetchMore()
        }

        onDraggingChanged: {
            if (!dragging && needUpdating) root.fetchMore()
        }

        onMovementStarted: {
            if (!visible) return ;
            scrollbarFadeoutAnimation.stop()
            scrollBar.opacity = 1
        }

        onMovementEnded: {
            scrollbarFadeoutAnimation.start()
        }

        onContentYChanged: {
            fristVisionIndex = indexAt(10, contentY)
        }

        // scroll bar
        Rectangle {
            id: scrollBar
            width: 0.5 * mainWindow.pixelPerMM
            height: conversionList.height * conversionList.height / conversionList.contentHeight
            anchors.right: parent.right; anchors.rightMargin: 0.5 * mainWindow.pixelPerMM
            y: conversionList.headerItem.height + conversionList.height
               *  conversionList.fristVisionIndex / conversionList.model.count
            color: "#A4A4A4"
            visible: scrollBar.height < conversionList.height
            opacity: 0
            clip: true

            Behavior on y { NumberAnimation { duration: 500 } }
        }

        SequentialAnimation {
            id: scrollbarFadeoutAnimation
            PauseAnimation { duration: 1000 }
            PropertyAnimation {
                target: scrollBar
                properties: "opacity"
                from: 1
                to: 0
                duration: 1000
            }
        }
    }


}
