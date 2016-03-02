#include "bibledicttabwidget.h"

BibleDictTabWidget::BibleDictTabWidget(BibleReaderCore *brc, QWidget *parent)
    : QTabWidget(parent)
{
    brCore = brc;
    connect(brCore, SIGNAL(showDictItem(QString,QString)), this, SLOT(showDictItem(QString,QString)));

    dicts = brCore->getAllDictionarys();
    for (int i = 0; i < dicts.size(); i++) {
        BibleDictionaryWidget *dictWidget =
                new BibleDictionaryWidget(brCore,
                                          dicts[i].getName(),
                                          this);
        dictWidgets.push_back(dictWidget);
        addTab(dictWidget, dicts[i].getFullname());
    }

    for (int i = 0; i < dicts.size(); i++) {
        if (dicts[i].getName() == brc->getCurrentDict()) {
            setCurrentIndex(i);
            break;
        }
    }

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(updateCurrentDict(int)));
}

BibleDictTabWidget::~BibleDictTabWidget()
{

}

void BibleDictTabWidget::updateCurrentDict(int index)
{
    QString dictname = dictWidgets[index]->getDictName();
    brCore->setCurrentDict(dictname);
    brCore->getConfigurator()->setDefaultDict(dictname);
}

void BibleDictTabWidget::showDictItem(QString dictName, QString itemName)
{
    for (int i = 0; i < dicts.size(); i++) {
        if (dicts[i].getName() == dictName) {
            setCurrentIndex(i);
            dictWidgets[i]->showExplaination(itemName);
            break;
        }
    }
}

