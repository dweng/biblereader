#include <QObject>
#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include "biblereadertooltip.h"

void BibleReaderToolTip::showText(QPoint pos, QString text, int mseconds)
{
    this->text = text;
    tipContentWidget->setText(text);
    // show
    if ( (pos.y() + 410 > screenHeight) && (pos.x() + 410 > screenWidth)) {
        move(pos.x() - 410, pos.y() - 410);
    } else if (pos.y() + 400 > screenHeight) {
        move(pos.x(), pos.y() - 410);
    } else if(pos.x() + 400 > screenWidth) {
        move(pos.x() - 410, pos.y());
    } else {
        move(pos.x() + 10, pos.y() + 10);
    }
    show();

    connect(durationTimer, SIGNAL(timeout()), this, SLOT(hideTip()));
    durationTimer->start(mseconds);
}

void BibleReaderToolTip::setTextFont(QFont font)
{
    tipContentWidget->setFont(font);
}

BibleReaderToolTip::~BibleReaderToolTip()
{
    durationTimer->stop();
    delete durationTimer;
}

BibleReaderToolTip::BibleReaderToolTip(QWidget *parent): QFrame(parent)
{
    screenWidth = QApplication::desktop()->width();
    screenHeight = QApplication::desktop()->height();
    createWidgets();

    durationTimer = new QTimer(this);
    shiftPressed = false;
}

void BibleReaderToolTip::leaveEvent(QEvent *e)
{
    hide();
    QFrame::leaveEvent(e);
}

void BibleReaderToolTip::mouseMoveEvent(QMouseEvent *e)
{
    QFrame::mouseMoveEvent(e);
}

void BibleReaderToolTip::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Shift) {
        shiftPressed = true;
    }
    QFrame::keyPressEvent(e);
}

void BibleReaderToolTip::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Shift) {
        shiftPressed = false;
    }
    QFrame::keyReleaseEvent(e);
}

void BibleReaderToolTip::createWidgets()
{
    setObjectName("container");
    setAttribute(Qt::WA_ShowWithoutActivating,true);
    setWindowFlags(Qt::ToolTip);
    setWindowTitle(tr("Verses tip window"));
    setFixedSize(400, 400);
    setStyleSheet("QWidget#container { border: 1px solid black; background: white;}");
    tipContentWidget = new QTextBrowser(this);
    tipContentWidget->setObjectName("tipContent");
    tipContentWidget->setStyleSheet("QTextBrowser#tipContent {border: 0px}");
    helpInfoWidget = new QLabel(this);
    helpInfoWidget->setText(tr("Please press [shift] key to hold tip"));
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(tipContentWidget, 9);
    layout->addWidget(helpInfoWidget, 1);

    setLayout(layout);
}

void BibleReaderToolTip::destroyWidgets()
{
    if (tipContentWidget) {
        delete tipContentWidget;
    }

    if (helpInfoWidget) {
        delete helpInfoWidget;
    }
}

void BibleReaderToolTip::hideTip()
{
    hide();
}

