#include "biblecommentarytabwidget.h"

BibleCommentaryTabWidget::BibleCommentaryTabWidget(BibleReaderCore *brc,
                                                   QWidget *parent)
:QTabWidget(parent)
{
    brCore = brc;
    QList<BibleCommentaryInfo> allcmts = brc->getAllCommentarys();

    for (int i = 0; i < allcmts.size(); i++) {
        BibleCommentaryInfo tempinfo = allcmts[i];
        BibleCommentaryWidget *newCmtWidget = new BibleCommentaryWidget(brc, this);
        bcWidgets.push_back(newCmtWidget);
        addTab(newCmtWidget, tempinfo.getFullname());
    }

    for (int i = 0; i < allcmts.size(); i++) {
        if (allcmts[i].getName() == brc->getCurrentCommentary()) {
            setCurrentIndex(i);
            break;
        }
    }
}

BibleCommentaryTabWidget::~BibleCommentaryTabWidget()
{
    if (!bcWidgets.empty()) {
        bcWidgets.clear();
    }
}


