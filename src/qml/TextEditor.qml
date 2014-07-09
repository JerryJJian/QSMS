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

FocusScope {
    id: root
    property real maxHeight: 150
    property alias text: msgEditor.text

    width: 240
    height: Math.min(Math.max(10 * mainWindow.pixelPerMM, msgEditor.paintedHeight), maxHeight)
    clip: true
    focus: msgEditor.activeFocus

    Flickable {
        id: msgInputArea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top; anchors.topMargin: mainWindow.pixelPerMM
        anchors.bottom: parent.bottom; anchors.bottomMargin: 2 * mainWindow.pixelPerMM
        contentWidth: width
        contentHeight: Math.max(msgEditor.paintedHeight, height)
        clip: true

        TextEdit {
            id: msgEditor
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: Text.paintedHeight
            textMargin: 3 * mainWindow.pixelPerMM
            font.pixelSize: 3.5 * mainWindow.pixelPerMM
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            selectByMouse: false
            activeFocusOnPress: true

            cursorVisible: activeFocus
            cursorDelegate: Item {
                width: handDragger.width
                height: cursorLine.height + handDragger.height
                clip: false
                opacity: 0.3

                Rectangle {
                    id: cursorLine
                    width: 1
                    height: msgEditor.font.pixelSize
                    color: "blue"
                }

                Rectangle {
                    id: handDragger
                    anchors.horizontalCenter: cursorLine.horizontalCenter
                    anchors.top: cursorLine.bottom
                    width: msgEditor.font.pixelSize; height: width
                    radius: width / 2
                    color: "blue"
                }
            }

            onCursorRectangleChanged: msgInputArea.ensureVisible(cursorRectangle)
        }

        Text {
            anchors.left: msgEditor.left; anchors.leftMargin: font.pixelSize
            anchors.verticalCenter: parent.verticalCenter
            height: Text.paintedHeight
            width: Text.paintedWidth
            font.pixelSize: 0
            text: qsTr("Input SMS")
            color: "#D0D0D0"
            visible: msgEditor.text === ""
        }

        function ensureVisible(r)
        {
            if (contentX >= r.x)
                contentX = r.x;
            else if (contentX + width <= r.x + r.width)
                contentX = r.x + r.width - width;

            if (contentY >= r.y)
                contentY = r.y;
            else if (contentY + height <= r.y + r.height)
                contentY = r.y + r.height - height;
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

    Rectangle {
        id: textScrollBar
        width: 0.5 * mainWindow.pixelPerMM
        height: msgInputArea.height * msgInputArea.height / msgInputArea.contentHeight
        anchors.right: parent.right; anchors.rightMargin: 0.5 * mainWindow.pixelPerMM
        y: msgInputArea.topMargin + msgInputArea.contentY * (msgInputArea.height - textScrollBar.height)
           / (msgInputArea.contentHeight - msgInputArea.height)
        color: "#A4A4A4"
        visible: root.maxHeight < msgInputArea.height
    }
}
