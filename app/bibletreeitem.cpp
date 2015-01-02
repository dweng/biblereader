#include "bibletreeitem.h"


BibleTreeItem::BibleTreeItem(const QList<QVariant> &data, BibleTreeItem *parent)
{
    itemData = data;
    parentItem = parent;
}

BibleTreeItem::~BibleTreeItem()
{
    qDeleteAll(childItems);
}

void BibleTreeItem::appendChild(BibleTreeItem *child)
{
    childItems.append(child);
}

BibleTreeItem *BibleTreeItem::child(int row)
{
    return childItems.value(row);
}

int BibleTreeItem::childCount() const
{
    return childItems.count();
}

int BibleTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant BibleTreeItem::data(int column) const
{
    return itemData.value(column);
}

int BibleTreeItem::row() const
{
    if (parentItem) {
        return parentItem->childItems.indexOf(const_cast<BibleTreeItem*>(this));
    }

    return 0;
}

BibleTreeItem *BibleTreeItem::parent()
{
    return parentItem;
}










