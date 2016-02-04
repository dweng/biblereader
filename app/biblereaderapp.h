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
#ifndef BIBLEREADERAPP_H
#define BIBLEREADERAPP_H

#include "biblereadercore.h"
#include "biblereadermainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QSplashScreen>

/**
 * @brief The BibleReaderApp class
 * This class is the BibleReader application class
 *
 * @version 1.0
 */
class BibleReaderApp : public QApplication
{
    Q_OBJECT
public:
    /**
     * @brief BibleReaderApp constructor
     * @param argc argc from user
     * @param argv argv from user
     */
    explicit BibleReaderApp(int argc, char **argv);
    ~BibleReaderApp();

signals:

public slots:

private:
    bool initBibleReaderCore();
    bool deinitBibleReaderCore();

private:
    QTranslator translator;
    QTranslator qtTranslator;

    BibleReaderMainWindow *w;

    BibleReaderCore *brCore;

    QSplashScreen *startScreen;
};

#endif // BIBLEREADERAPP_H
