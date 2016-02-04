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
#include <QTimer>
// for logging
#include "Logger.h"
#include "FileAppender.h"

BibleReaderApp::BibleReaderApp(int argc, char **argv):
    QApplication(argc, argv)
{
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

#ifdef Q_OS_LINUX
    setFont(QFont(QString("Noto Sans CJK SC"), 10));
#endif
    QSplashScreen *splash = new QSplashScreen; // create a QSplashScreen
    splash->setPixmap(QPixmap(":/img/assets/images/splash.png")); // set the image I would show
    splash->show(); // after show it  close immediately
    splash->raise();
    qApp->processEvents();

    // logging
    QString appDirPath = QApplication::applicationDirPath();
    FileAppender* fileAppender = new FileAppender(appDirPath + "/debug.log");
    fileAppender->setFormat("[%{time}{yyyy-MM-ddTHH:mm:ss.zzz}] [%{type:-7}] <%{Function}> %{message}\n");
    logger->registerAppender(fileAppender);
    LOG_INFO("Bible Reader started!");

    LOG_INFO() << "Loading translations...";
    splash->showMessage(tr("Loading translations..."), Qt::AlignBottom|Qt::AlignLeft, Qt::blue);
    LOG_INFO() << "Loading ZH_cn:" << translator.load(appDirPath + "/translations/ZH_cn.qm");
    LOG_INFO() << "Loading Qt ZH_cn:" << qtTranslator.load(appDirPath + "/translations/qt_zh_CN.qm");
    installTranslator(&translator);
    installTranslator(&qtTranslator);

    // init biblereadercore
    splash->showMessage(tr("Loading modules..."), Qt::AlignBottom|Qt::AlignLeft, Qt::white);
    initBibleReaderCore();

    // Initialize objects
    w = new BibleReaderMainWindow(brCore);
    //w->show();

    QTimer::singleShot(2000, splash, SLOT(close()));
    splash->finish(w);
    delete splash;
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
