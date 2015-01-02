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
#include "biblereaderapp.h"
#include <QTextCodec>

// for logging
#include "Logger.h"
#include "FileAppender.h"

BibleReaderApp::BibleReaderApp(int argc, char **argv):
    QApplication(argc, argv)
{
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    // logging
    FileAppender* fileAppender = new FileAppender("debug.log");
    fileAppender->setFormat("[%{type:-7}] <%{Function}> %{message}\n");
    logger->registerAppender(fileAppender);
    LOG_INFO("Bible Reader started!");

    LOG_INFO() << "Loading translations...";
    LOG_INFO() << "Loading ZH_cn:" << translator.load("./translations/ZH_cn.qm");;
    installTranslator(&translator);

    // init biblereadercore
    initBibleReaderCore();
    w = new BibleReaderMainWindow(brCore);
    w->show();
}

BibleReaderApp::~BibleReaderApp()
{
    if (w) {
        delete w;
    }

    deinitBibleReaderCore();
}

bool BibleReaderApp::initBibleReaderCore()
{
    LOG_DEBUG("Bible Reader Core Initing...");
    this->brCore = new BibleReaderCore();
    LOG_DEBUG("Bible Reader Core inited.");
    return true;
}

bool BibleReaderApp::deinitBibleReaderCore()
{
    LOG_DEBUG("Bible Reader Core deiniting...");
    delete brCore;
    LOG_DEBUG("Bible Reader Core deinited.");
    return true;
}
