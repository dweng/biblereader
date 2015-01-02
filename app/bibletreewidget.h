/**
 * [1 Timothy 3:16-17 ESV]
 * All Scripture is breathed out by God and profitable
 * for teaching, for reproof, for correction, and for training in righteousness,
 * that the man of God may be complete, equipped for every good work.
 *
 * BibleReader is an simple application for you to study God's Word.
 * The main user of this application maybe the pastor who's main language
 * is Chinese Simplified.
 *
 * You can use it totally free, and if you want to get source code of
 * this application, please email me at dweng123@gmail to get the source
 * code.
 *
 */
#ifndef BIBLETREEWIDGET_H
#define BIBLETREEWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QList>
#include "biblereadercore.h"
/**
 * @brief The BibleTreeWidget class
 * This widget will show bible books as a tree widget.
 *
 * @version 1.0
 */
class BibleTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param brc bibleReaderCore instance
     * @param parent parent widget
     */
    explicit BibleTreeWidget(BibleReaderCore *brc, QWidget *parent = 0);
    ~BibleTreeWidget();

    /**
     * @brief Rebuild bible tree widget items.
     */
    void rebuildBibleTreeData();

signals:

public slots:
    void onBibleVersionChanged(QString version);

private:
    /**
     * @brief BibleReaderCore instance
     */
    BibleReaderCore *brCore;

    /**
     * @brief bible tree widget item list, only contains books
     */
    QList<QTreeWidgetItem*> bookItems;

private:
    /**
     * @brief build bible tree widget data.
     */
    void buildBibleTreeData();
    /**
     * @brief destroy book items
     */
    void destroyBookItems();
};

#endif // BIBLETREEWIDGET_H
