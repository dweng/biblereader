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
#include <QTextEdit>

/**
 * @brief The BibleCommentaryWidget class
 * This widget represent the widget in main window of BR
 */
class BibleCommentaryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BibleCommentaryWidget(QWidget *parent = 0);

signals:

public slots:

private:
    QTreeWidget *contents;
    QTextEdit *content;
};

#endif // BIBLECOMMENTARYWIDGET_H
