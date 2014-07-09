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

#include "imageview.h"
#include <QtGui/QPainter>

ImageView::ImageView(QQuickPaintedItem *parent) :
    QQuickPaintedItem(parent),
    m_isValid(false)
{
    setFlag(ItemHasContents, true);
    setRenderTarget(QQuickPaintedItem::Image);
    setAntialiasing(true);
}

void ImageView::paint(QPainter *painter)
{
    if (!m_isValid)
        return ;

    painter->drawPixmap(contentsBoundingRect().toRect(), m_pixmap.scaled(contentsBoundingRect().toRect().size()));
}

QByteArray ImageView::getSource() const
{
    return m_source;
}

void ImageView::setSource(const QByteArray &source)
{
    if (m_source == source)
        return ;

    m_source = source;

    m_pixmap.loadFromData(QByteArray::fromBase64(m_source));
    setValid(!m_pixmap.isNull());

    update();
    emit sourceChanged(m_source);
}

bool ImageView::getValid() const
{
    return m_isValid;
}

void ImageView::setValid(bool valid)
{
    if (m_isValid == valid)
        return ;

    m_isValid = valid;
    emit validChanged(m_isValid);
}

