#include "bibledicttabwidget.h"

BibleDictTabWidget::BibleDictTabWidget(BibleReaderCore *brc, QWidget *parent)
    : QTabWidget(parent)
{
    brCore = brc;

    dicts = brCore->getAllDictionarys();
    for (int i = 0; i < dicts.size(); i++) {
        BibleDictionaryWidget *dictWidget =
                new BibleDictionaryWidget(brCore,
                                          brCore->getCurrentDict(),
                                          this);
        dictWidgets.push_back(dictWidget);
        addTab(dictWidget, dicts[i].getFullname());
    }
}

BibleDictTabWidget::~BibleDictTabWidget()
{

}

