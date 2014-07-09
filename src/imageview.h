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

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QtQuick/QQuickPaintedItem>
#include <QPixmap>
#include <QPainter>

class ImageView : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QByteArray source READ getSource WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool isvalid READ getValid WRITE setValid NOTIFY validChanged)
public:
    explicit ImageView(QQuickPaintedItem *parent = 0);
    

public:
    void paint(QPainter *painter);

    Q_INVOKABLE QByteArray getSource() const;
    Q_INVOKABLE bool getValid() const;


signals:
    void sourceChanged(const QByteArray &source);
    void validChanged(bool valid);

public slots:
    void setSource(const QByteArray &source);
    void setValid(bool valid);

private:
    QByteArray m_source;
    bool m_isValid;
    QPixmap m_pixmap;
};

#endif // IMAGEVIEW_H
