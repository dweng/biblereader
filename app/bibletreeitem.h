#ifndef BIBLETREEITEM_H
#define BIBLETREEITEM_H

#include <QList>
#include <QVariant>

class BibleTreeItem
{
public:
    BibleTreeItem(const QList<QVariant> &data,BibleTreeItem *parent=0);
    ~BibleTreeItem();
    void appendChild(BibleTreeItem *child);
    BibleTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    BibleTreeItem *parent();

 private:
     BibleTreeItem *parentItem;  // parent node
     QList<BibleTreeItem*> childItems; // children nodes
     QList<QVariant> itemData; //children nodes' data
};

#endif // BIBLETREEITEM_H
