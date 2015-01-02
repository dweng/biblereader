#include "bibletreemodel.h"
#include <QIcon>
#include <QDebug>

BibleTreeModel::BibleTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootItem = NULL;
    //btDAO = new BibleTextDAO("LZZ");

    updateData();
}

BibleTreeModel::~BibleTreeModel()
{
    if (rootItem) {
        delete rootItem;
    }

    delete btDAO;
}

QVariant BibleTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // add icon
    if(role==Qt::DecorationRole && index.column()==0)
            return QIcon("images/fold.png");

    // shou node data
    if(role==Qt::DisplayRole)
    {
        BibleTreeItem *item = static_cast<BibleTreeItem*>(index.internalPointer());
         return item->data(index.column());
    }

    return QVariant();
}

Qt::ItemFlags BibleTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QVariant BibleTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex BibleTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row,column,parent))
        return QModelIndex();

    BibleTreeItem *parentItem;
    if(!parent.isValid())
    {
        parentItem=rootItem;
    }else
    {
        parentItem=static_cast<BibleTreeItem*>(parent.internalPointer());
    }

    BibleTreeItem *childItem=parentItem->child(row);
    if(childItem) {
        return createIndex(row,column,childItem); // expand tree, build index for children nodes
    } else
        return QModelIndex();
}

QModelIndex BibleTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    BibleTreeItem *childItem=static_cast<BibleTreeItem*>(index.internalPointer());
    BibleTreeItem *parentItem=childItem->parent();

    // 顶层节点,直接返回空索引
    if(parentItem==rootItem)
        return QModelIndex();

    // 为父结点建立索引
    return createIndex(parentItem->row(),0,parentItem);
}

int BibleTreeModel::rowCount(const QModelIndex &parent) const
{
    BibleTreeItem *parentItem;

    if(!parent.isValid())
        parentItem=rootItem;
    else
        parentItem=static_cast<BibleTreeItem*>(parent.internalPointer());

    return parentItem->childCount(); // 返回父结点下子结点数目
}

int BibleTreeModel::columnCount(const QModelIndex &parent) const
{
    /*
    BibleTreeItem *parentItem;
    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<BibleTreeItem*>(parent.internalPointer());
    }
    */
    return rootItem->columnCount();
}

void BibleTreeModel::setupModelData(BibleTreeItem *parent)
{
    // get all bible books
    QList<BibleBook> books = btDAO->getAllBooks();
    for(int i=0;i<books.count();i++)
    {
        QList<QVariant> datas;
        BibleBook book = books.value(i);
        datas<<book.getLongName()<<book.getBookNumber();

        //
        BibleTreeItem *bookItem = new BibleTreeItem(datas,parent);
        parent->appendChild(bookItem);

        // get all chapters for one book
        QList<int> chapters = btDAO->getChaptersCountOfOneBook(book.getBookNumber());
        for(int j=0;j<chapters.count();j++)
        {
            QList<QVariant> ds;
            int versesCount = chapters.value(j);
            ds<<book.getLongName()<<book.getBookNumber()<<j+1;
            BibleTreeItem *chapterItem = new BibleTreeItem(ds,bookItem);
            // add verses to chapter
            for (int k=0; k<versesCount;k++)
            {
                QList<QVariant> ds2;
                ds2<<book.getLongName()<<book.getBookNumber()<<j+1<<k+1;
                BibleTreeItem *verseItem = new BibleTreeItem(ds2, chapterItem);
                chapterItem->appendChild(verseItem);
            }
            bookItem->appendChild(chapterItem);
        }
    }
}

void BibleTreeModel::updateData()
{
    // drop old model data
    if(rootItem)
    {
        delete rootItem;
        rootItem = NULL;
    }

    QList<QVariant> rootData;
    rootData<<tr("Book")<<tr("Book Number")<<tr("Chapter")<<tr("Verse");

    rootItem = new BibleTreeItem(rootData);
    setupModelData(rootItem);

    // refresh model
    this->beginResetModel();
    this->endResetModel();
}
