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
#ifndef BIBLETEXTCOMPAREBROWSER_H
#define BIBLETEXTCOMPAREBROWSER_H

#include <QTextEdit>
#include "biblereadercore.h"

class BibleTextCompareBrowser : public QTextEdit
{
    Q_OBJECT
public:
    BibleTextCompareBrowser(BibleReaderCore *brc, QWidget *parent = 0);
    ~BibleTextCompareBrowser();

public slots:
    void showComparedBibleText();

protected:

private:
    BibleReaderCore *brCore;
    // bible text font and size
    QString btFontFamily;
    double btFontSize;
};

#endif // BIBLETEXTCOMPAREBROWSER_H
