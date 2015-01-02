#ifndef BIBLETREEMODEL_H
#define BIBLETREEMODEL_H

#include <QAbstractItemModel>
#include "bibletreeitem.h"
#include "bibletextdao.h"

class BibleTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit BibleTreeModel(QObject *parent = 0);
    ~BibleTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    // build model data
    void setupModelData(BibleTreeItem *parent);

    // update model data
    void updateData();

signals:

public slots:

private:
    BibleTreeItem *rootItem;
    BibleTextDAO *btDAO;
};

#endif // BIBLETREEMODEL_H
