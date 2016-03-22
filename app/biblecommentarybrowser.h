#ifndef BIBLECOMMENTARYBROWSER_H
#define BIBLECOMMENTARYBROWSER_H

#include <QTextBrowser>
#include <QPoint>
#include "biblereadercore.h"
#include "biblereadertooltip.h"

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
