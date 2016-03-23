#include "biblecommentarytabwidget.h"

BibleCommentaryTabWidget::BibleCommentaryTabWidget(BibleReaderCore *brc,
                                                   QWidget *parent)
:QTabWidget(parent)
{
    brCore = brc;
    QList<BibleCommentaryInfo> allcmts = brc->getAllCommentarys();

    for (int i = 0; i < allcmts.size(); i++) {
        BibleCommentaryInfo tempinfo = allcmts[i];
        BibleCommentaryWidget *newCmtWidget = new
                BibleCommentaryWidget(brc, tempinfo.getShortname(), this);
        bcWidgets.push_back(newCmtWidget);
        addTab(newCmtWidget, QIcon(QString(":/img/assets/images/report.png")),tempinfo.getFullname());
    }

    for (int i = 0; i < tabBar()->count(); i++) {
        tabBar()->setTabTextColor(i, QColor("green"));
    }

    for (int i = 0; i < allcmts.size(); i++) {
        if (allcmts[i].getShortname() == brc->getCurrentCommentary()) {
            setCurrentIndex(i);
            break;
        }
    }

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(updateCurrentCommentary(int)));
}

BibleCommentaryTabWidget::~BibleCommentaryTabWidget()
{
    if (!bcWidgets.empty()) {
        bcWidgets.clear();
    }
}

void BibleCommentaryTabWidget::updateCurrentCommentary(int index)
{
    QString cmmtname = bcWidgets[index]->getCmtName();
    brCore->setCurrentCommentary(cmmtname);
    brCore->getConfigurator()->setDefaultCommentary(cmmtname);
}


