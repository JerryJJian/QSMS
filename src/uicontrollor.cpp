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

#include "uicontrollor.h"
#include <QDebug>

UIControllor::UIControllor(QObject *parent) :
    QObject(parent),
    m_currView(NullPageView)
{
}

UIControllor::PageView UIControllor::getCurrView() const
{
    return m_currView;
}

void UIControllor::setCurrView(const PageView &value)
{
    if (m_currView == value)
        return ;

    m_currView = value;
    emit currViewChanged(m_currView);
}

int UIControllor::conversionId() const
{
    return m_currConversionInfo.id;
}

void UIControllor::setConversionId(int id)
{
    if (m_currConversionInfo.id == id)
        return ;

    m_currConversionInfo.id = id;
    emit conversionIdChanged(m_currConversionInfo.id);
}

bool UIControllor::isUserInputPhoneNum() const
{
    return m_currConversionInfo.isUserInputPhoneNum;
}

void UIControllor::setUserInputPhoneNum(bool inputed)
{
    if (m_currConversionInfo.isUserInputPhoneNum == inputed)
        return ;

    m_currConversionInfo.isUserInputPhoneNum = inputed;
    emit isUserInputPhoneNumChanged(m_currConversionInfo.isUserInputPhoneNum);
}

QString UIControllor::conversionUserName() const
{
    return m_currConversionInfo.userName;
}

void UIControllor::setConversionUserName(const QString &username)
{
    if (m_currConversionInfo.userName == username)
        return ;

    m_currConversionInfo.userName = username;
    emit conversionUserNameChanged(username);
}

QString UIControllor::conversionPhoneNum() const
{
    return m_currConversionInfo.phoneNum;
}

void UIControllor::setConversionPhoneNum(const QString &phoneNum)
{
    if (m_currConversionInfo.phoneNum == phoneNum)
        return ;

    m_currConversionInfo.phoneNum = phoneNum;
    emit conversionPhoneNumChanged(phoneNum);
}

QByteArray UIControllor::conversionUserHeadImg() const
{
    return m_currConversionInfo.headImg;
}

void UIControllor::setConversionUserHeadImg(const QByteArray &headimg)
{
    if (m_currConversionInfo.headImg == headimg)
        return ;

    m_currConversionInfo.headImg = headimg;
    emit conversionUserHeadImgChanged(m_currConversionInfo.headImg);
}

QString UIControllor::contactListFilter() const
{
    return m_contactListFilter;
}

void UIControllor::setContactListFilter(const QString &filter)
{
    QString filterText(filter.trimmed());
    if (m_contactListFilter.compare(filterText, Qt::CaseInsensitive) == 0)
        return ;

    m_contactListFilter = filterText;
    emit contactListFilterChanged(m_contactListFilter);
}

