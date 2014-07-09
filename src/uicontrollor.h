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

#ifndef UICONTROLLOR_H
#define UICONTROLLOR_H

#include <QObject>
#include <QMap>

class UIControllor : public QObject
{
    Q_OBJECT
    Q_ENUMS(PageView)
    Q_PROPERTY(PageView currView READ getCurrView WRITE setCurrView NOTIFY currViewChanged)
    Q_PROPERTY(int conversionId READ conversionId WRITE setConversionId NOTIFY conversionIdChanged)
    Q_PROPERTY(bool isUserInputPhoneNum READ isUserInputPhoneNum WRITE setUserInputPhoneNum NOTIFY isUserInputPhoneNumChanged)
    Q_PROPERTY(QString conversionUserName READ conversionUserName WRITE setConversionUserName NOTIFY conversionUserNameChanged)
    Q_PROPERTY(QString conversionPhoneNum READ conversionPhoneNum WRITE setConversionPhoneNum NOTIFY conversionPhoneNumChanged)
    Q_PROPERTY(QByteArray conversionUserHeadImg READ conversionUserHeadImg WRITE setConversionUserHeadImg NOTIFY conversionUserHeadImgChanged)
    Q_PROPERTY(QString contactListFilter READ contactListFilter WRITE setContactListFilter NOTIFY contactListFilterChanged)

public:
    explicit UIControllor(QObject *parent = 0);

    enum PageView
    {
        NullPageView = 0,
        SmsListView,
        ConversionView,
        ContactListView
    };

    Q_INVOKABLE PageView getCurrView() const;
    Q_INVOKABLE void setCurrView(const PageView &value);

    Q_INVOKABLE int conversionId() const;
    Q_INVOKABLE void setConversionId(int id);

    Q_INVOKABLE bool isUserInputPhoneNum() const;
    Q_INVOKABLE void setUserInputPhoneNum(bool inputed);

    Q_INVOKABLE QString conversionUserName() const;
    Q_INVOKABLE void setConversionUserName(const QString &username);

    Q_INVOKABLE QString conversionPhoneNum() const;
    Q_INVOKABLE void setConversionPhoneNum(const QString &phoneNum);

    Q_INVOKABLE QByteArray conversionUserHeadImg() const;
    Q_INVOKABLE void setConversionUserHeadImg(const QByteArray &headimg);

    Q_INVOKABLE QString contactListFilter() const;

signals:
    void currViewChanged(PageView currView);
    void conversionIdChanged(int id);
    void isUserInputPhoneNumChanged(bool inputed);
    void conversionUserNameChanged(const QString &username);
    void conversionPhoneNumChanged(const QString &phoneNum);
    void conversionUserHeadImgChanged(const QByteArray &headimg);
    void contactListFilterChanged(const QString &filter);

public slots:
    void setContactListFilter(const QString &filter);

private:
    struct ConversionInfo
    {
        ConversionInfo()
            : id(-1), isUserInputPhoneNum(true)
        { }

        int id;
        bool isUserInputPhoneNum;
        QString userName;
        QString phoneNum;
        QByteArray headImg;
    };

    PageView m_currView;
    ConversionInfo m_currConversionInfo;
    QString m_contactListFilter;
};

#endif // UICONTROLLOR_H
