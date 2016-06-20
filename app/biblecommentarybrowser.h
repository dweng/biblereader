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
#ifndef BIBLECOMMENTARYBROWSER_H
#define BIBLECOMMENTARYBROWSER_H

#include <QTextBrowser>
#include <QPoint>
#include "biblereadercore.h"
#include "biblereadertooltip.h"

/**
 * @brief The BibleCommentaryBrowser class
 * 圣经注释浏览器控件
 */
class BibleCommentaryBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    BibleCommentaryBrowser(BibleReaderCore *brc, QWidget *parent = 0);
    virtual ~BibleCommentaryBrowser();

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private:
    BibleReaderCore *brCore;
    BibleReaderToolTip *tooltip;
    int screenWidth;
    int screenHeight;
    bool shiftPressed;

private slots:
    void setCurrentBCV(QUrl url);

private:
    void showToolTip(QString text, QPoint pos);
};


#endif // BIBLECOMMENTARYBROWSER_H
