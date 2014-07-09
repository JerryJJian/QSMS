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

#include "listmodel.h"
#include <QDebug>
#include <QTimer>

ListModel::ListModel(ListItem *prototype, QObject *parent) :
    QAbstractListModel(parent), m_prototype(prototype), m_effectCount(0)
{
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_list.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_list.size())
        return QVariant();

    ListItem *item = m_list.at(index.row());
    if (item == NULL)
        return QVariant();

    return item->data(role);
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.row() < 0 || index.row() >= m_list.size())
        return false;

    ListItem *item = m_list.at(index.row());
    if (item == NULL)
        return false;

    return item->setData(role, value);
}

ListModel::~ListModel()
{
    delete m_prototype;
    clear();
}

void ListModel::updateRows(const QList<ListItem *> &items)
{
    clear();
    appendRows(items);
//    emit dataModelUpdated();
    QTimer::singleShot(0, this, SIGNAL(dataModelUpdated()));
}

void ListModel::prependRows(const QList<ListItem *> &items)
{
    beginResetModel();
    for (int i=items.size() - 1; i>=0; i--)
    {
        ListItem *item = items.at(i);
        if (item == NULL)
            continue;
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        m_list.prepend(item);
    }
    endResetModel();

    setEffectCount(items.size());
    emit dataModelChanged();
    emit countChanged(rowCount());
    QTimer::singleShot(0, this, SIGNAL(dataModelPrepended()));
}

void ListModel::appendRow(ListItem *item)
{
    if (item == NULL)
        return ;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
    m_list.append(item);
    endInsertRows();

    emit dataModelChanged();
    emit countChanged(rowCount());
    setEffectCount(1);
    QTimer::singleShot(0, this, SIGNAL(dataModelAppended()));
}

void ListModel::appendRows(const QList<ListItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+items.size()-1);
    foreach(ListItem *item, items)
    {
        if (item == NULL)
            continue;
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        m_list.append(item);
    }
    endInsertRows();
    emit dataModelChanged();
    emit countChanged(rowCount());
    setEffectCount(items.size());
    QTimer::singleShot(0, this, SIGNAL(dataModelAppended()));
}

void ListModel::insertRow(int row, ListItem *item)
{
    if (item == NULL)
        return ;

    beginInsertRows(QModelIndex(), row, row);
    connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
    m_list.insert(row, item);
    endInsertRows();
    emit dataModelChanged();
    emit countChanged(rowCount());
    setEffectCount(1);
}

void ListModel::handleItemChange()
{
    ListItem *item = qobject_cast<ListItem *> (sender());
    if (item == NULL)
        return ;

    // watch the changing of selected items' number
    bool hasSelectedItems = m_selectedItem.size() > 0 ? true : false;

    int itemRow = m_list.indexOf(item);
    if (item->selected())
        m_selectedItem.insert(item, itemRow);
    else
        m_selectedItem.remove(item);

    if (hasSelectedItems != (m_selectedItem.size() > 0 ? true : false))
        emit selectedModeChanged(m_selectedItem.size() > 0 ? true : false);

    QModelIndex modelIdx = index(itemRow); //indexFromItem(item);
    if(modelIdx.isValid())
        emit dataChanged(modelIdx, modelIdx);
}

ListItem *ListModel::find(const QString &id) const
{
    foreach(ListItem *item, m_list)
    {
        if(item != NULL && item->id() == id)
            return item;
    }

    return 0;
}

QModelIndex ListModel::indexFromItem(const ListItem *item) const
{
    if (item == NULL)
        return QModelIndex();

    for(int row=0; row<m_list.size(); ++row)
    {
        if(m_list.at(row) == item)
            return index(row);
    }

    return QModelIndex();
}

void ListModel::clear()
{
    if (rowCount() <= 0)
        return ;

    beginResetModel();
    qDeleteAll(m_list);
    m_list.clear();
    m_selectedItem.clear();
    endResetModel();
}

void ListModel::cancelSlectectMode()
{
    foreach (ListItem *item, m_selectedItem.keys())
    {
        if (item != NULL)
            item->setSelected(false);
    }

    if (selectedMode())
        m_selectedItem.clear();

    emit selectedModeChanged(false);
}

bool ListModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    if(row < 0 || row >= m_list.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row);
    ListItem *item = m_list.takeAt(row);
    m_selectedItem.remove(item);
    if (item != NULL)
        item->deleteLater();

    endRemoveRows();
    emit dataModelChanged();
    setEffectCount(-1);

    return true;
}

bool ListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    if(row < 0 || (row+count) > m_list.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row+count-1);
    for(int i=0; i<count; ++i)
    {
        ListItem *item = m_list.takeAt(row);
        m_selectedItem.remove(item);
        if (item != NULL)
            item->deleteLater();
    }

    endRemoveRows();
    emit dataModelChanged();
    setEffectCount(-count);

    return true;
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    return m_prototype->roleNames();
}

QString ListModel::data(int index, QString roleName)
{
    QHashIterator<int, QByteArray> it(m_prototype->roleNames());

    while (it.hasNext())
    {
        it.next();
        if (it.value() == roleName)
        {
            int role = it.key();
            return data(this->index(index), role).toString();
        }
    }

    return QString();
}

bool ListModel::selectedMode() const
{
    return m_selectedItem.size() > 0 ? true : false;
}

int ListModel::getEffectCount() const
{
    return m_effectCount;
}

void ListModel::setEffectCount(int count)
{
    if (m_effectCount == count)
        return ;
    m_effectCount = count;
    emit effectCountChanged(m_effectCount);
}

QStringList ListModel::selectedItems()
{
    QStringList ids;

    foreach (ListItem *item, m_selectedItem.keys())
        if (item != NULL) ids.append(item->id());

    return ids;
}

void ListModel::removeSelectedItems()
{
    QList<int> rows = m_selectedItem.values();
    qSort(rows.begin(), rows.end(), qGreater<int>());

    foreach (int row, rows)
        removeRow(row);

    cancelSlectectMode();
}

ListItem *ListModel::takeRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    ListItem *item = m_list.takeAt(row);
    endRemoveRows();

    return item;
}
