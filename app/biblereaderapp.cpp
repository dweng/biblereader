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
 *
 * \author dweng
 * \version 0.0.5
 * \date 2017-10-31
 */
#include "biblereaderapp.h"
#include <QTextCodec>
#include <QTimer>
#include <QDir>
#include <QFile>

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
    splash = new QSplashScreen; // create a QSplashScreen
    splash->setPixmap(QPixmap(":/img/assets/images/splash.png")); // set the image I would show
    splash->show(); // after show it  close immediately
    splash->raise();
    qApp->processEvents();

    // logging
    QDir dir(QApplication::applicationDirPath());
#ifdef Q_OS_OSX
    dir.cdUp();
    dir.cd("Resources");
#endif
    FileAppender* fileAppender = new FileAppender(dir.absolutePath() + "/debug.log");
    fileAppender->setFormat("[%{time}{yyyy-MM-ddTHH:mm:ss.zzz}] [%{type:-7}] <%{Function}> %{message}\n");
    logger->registerAppender(fileAppender);
    LOG_INFO("Bible Reader started!");

    LOG_INFO() << "Loading translations...";
    splash->showMessage(tr("Loading translations..."), Qt::AlignBottom|Qt::AlignLeft, Qt::blue);
    this->processEvents();
    LOG_INFO() << "Loading ZH_cn:" << translator.load(dir.absolutePath() + "/translations/ZH_cn.qm");
    LOG_INFO() << "Loading Qt ZH_cn:" << qtTranslator.load(dir.absolutePath() + "/translations/qt_zh_CN.qm");
    LOG_INFO() << "loading updater ZH_cn:" << updaterTranslator.load(dir.absolutePath() + "/translations/updater_zh_CN.qm");
    installTranslator(&translator);
    installTranslator(&qtTranslator);
    installTranslator(&updaterTranslator);

    // init biblereadercore
    splash->showMessage(tr("Loading modules..."), Qt::AlignBottom|Qt::AlignLeft, Qt::white);
    this->processEvents();
    this->brCore = new BibleReaderCore();
    connect(brCore, SIGNAL(loadResourceFinished(QString)), this, SLOT(showSplashMessage(QString)));
    connect(brCore, SIGNAL(bibleRemoved(QString)), this, SLOT(reboot()));

    initBibleReaderCore();

    setApplicationName(tr("biblereader"));
    setApplicationVersion("0.0.12");
    setApplicationDisplayName(tr("biblereader"));
    setOrganizationDomain("biblereader.cn");
    setOrganizationName("Bible Reader");
    //setTheme(":qdarkstyle/style.qss");
    //setTheme(":whitestyle/white.qss");
    //setStyleSheet("QMenu[bid=1]:item {color: red;}");

    // Initialize objects
    splash->showMessage(tr("Building layout..."), Qt::AlignBottom|Qt::AlignLeft, Qt::white);
    this->processEvents();
    w = new BibleReaderMainWindow(brCore);
    w->show();

    //QTimer::singleShot(5000, splash, SLOT(close()));
    splash->finish(w);
}

BibleReaderApp::~BibleReaderApp()
{
    if (w) {
        delete w;
    }

    if (splash) {
        delete splash;
    }

    deinitBibleReaderCore();
}

bool BibleReaderApp::initBibleReaderCore()
{
    LOG_DEBUG("Bible Reader Core Initing...");
    //this->brCore = new BibleReaderCore();
    this->brCore->init();
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

void BibleReaderApp::setTheme(QString theme)
{
    QString themeCSS = "";
    QFile themeFile(theme);
    if (themeFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
        themeCSS = themeFile.readAll();
        themeFile.close();
    }

    setStyleSheet(themeCSS);
}

void BibleReaderApp::showSplashMessage(QString msg)
{
    if (splash) {
        splash->showMessage(tr("Loading %1").arg(msg), Qt::AlignBottom|Qt::AlignLeft,
                            Qt::white);
        qApp->processEvents();
    }
}

void BibleReaderApp::reboot()
{
    qApp->exit(773);
}

bool BibleReaderApp::notify(QObject *obj, QEvent *e)
{
    return QApplication::notify(obj,e);
}

bool BibleReaderApp::eventFilter(QObject *obj, QEvent *e)
{
    /*
    switch (e->type()) {
    case QEvent::KeyPress:
            if (obj->objectName() == "tipContent") {
                obj->event(e);
            }
        break;

    default:
        break;
    }
    */

    return QApplication::eventFilter(obj, e);
}
