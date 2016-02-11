/**
 * [1 Timothy 3:16-17 ESV]
 * All Scripture is breathed out by God and profitable
 * for teaching, for reproof, for correction, and for training in righteousness,
 * that the man of God may be complete, equipped for every good work.
 *
 * BibleReader is an simple application for you to study God's Word.
 * The main user of this application maybe the pastor who's main language
 * is Chinese.
 *
 * You can use it totally free, and if you want to get source code of
 * this application, please email me at dweng123@gmail to get the source
 * code.
 */
#ifndef BIBLECOMMENTARYWIDGET_H
#define BIBLECOMMENTARYWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTextEdit>
#include <QSplitter>
#include "biblereadercore.h"

/**
 * @brief The BibleCommentaryWidget class
 * This widget represent the widget in main window of BR
 */
class BibleCommentaryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BibleCommentaryWidget(BibleReaderCore *brc, QString cn, QWidget *parent = 0);
    ~BibleCommentaryWidget();
    QString getCmtFullName() const;
    void setCmtFullName(const QString &value);

    QString getCmtTitle() const;
    void setCmtTitle(const QString &value);

signals:

public slots:

private slots:
    QString changeChapterCmt(int book, int chapter, int verse);
    QString changeChapterCmt(QTreeWidgetItem *current, QTreeWidgetItem *previous);
private:
    /**
     * @brief section, commentary contents
     */
    QTreeWidget *section;

    /**
     * @brief commentary content
     */
    QTextEdit *content;

    /**
     * @brief Splitter widget for dictionary widget
     */
    QSplitter *dictSplitter;

    /**
     * @brief Left side of widget container.
     */
    QWidget *leftContainer;

    /**
     * @brief bible reader core instance
     */
    BibleReaderCore *brCore;

    /**
     * @brief commentary name
     */
    QString cmtName;

    /**
     * @brief cmtFullName
     */
    QString cmtFullName;

    /**
     * @brief cmtTitle
     */
    QString cmtTitle;
private:
    void createWidgets();
    void destroyWidgets();
};

#endif // BIBLECOMMENTARYWIDGET_H
