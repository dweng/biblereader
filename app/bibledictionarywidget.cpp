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
#include <QSplitter>
#include <QGridLayout>
#include "bibledictionarywidget.h"

BibleDictionaryWidget::BibleDictionaryWidget(BibleReaderCore *brc, QString dn, QWidget *parent) :
    QWidget(parent)
{
    brCore = brc;
    dictName = dn;

    if (brCore->getCurrentDict() == dictName) {
        isCurrent = true;
    }

    if (wordsList.empty()) {
        wordsList = brCore->getAllWordsAndExplainations(dictName);
    }

    createWidgets();
}

BibleDictionaryWidget::~BibleDictionaryWidget()
{
    destoryWidgets();
}

void BibleDictionaryWidget::createWidgets()
{
    dictShowExplaination = new QTextEdit(this);
    dictShowExplaination->setReadOnly(true);

    dictWordsCombo = new QComboBox(this);
    dictWordsCombo->setEditable(true);
    dictWordsCombo->addItems(wordsList.keys());
    connect(dictWordsCombo,
            SIGNAL(activated(int)),
            this, SLOT(showExplaination(int)));

    dictWordsList = new QListWidget(this);
    dictWordsList->addItems(wordsList.keys());
    connect(dictWordsList,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this,
            SLOT(showExplaination(QListWidgetItem*,QListWidgetItem*)));

    leftContainer = new QWidget(this);
    leftContainer->setMaximumWidth(100);
    QVBoxLayout *vLayout = new QVBoxLayout(leftContainer);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(dictWordsCombo);
    vLayout->addWidget(dictWordsList);
    leftContainer->setLayout(vLayout);

    dictSplitter = new QSplitter(Qt::Horizontal, this);
    dictSplitter->addWidget(leftContainer);
    dictSplitter->addWidget(dictShowExplaination);

    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->addWidget(dictSplitter);
}

void BibleDictionaryWidget::destoryWidgets()
{
    delete dictShowExplaination;
    delete dictWordsCombo;
    delete dictWordsList;
}
QString BibleDictionaryWidget::getDictName() const
{
    return dictName;
}

void BibleDictionaryWidget::setDictName(const QString &value)
{
    dictName = value;
}



void BibleDictionaryWidget::showExplaination(QListWidgetItem* current,
                                             QListWidgetItem* previous)
{
    QString word;
    if (current) {
        word = current->data(0).toString();
    } else {
        word = previous->data(0).toString();
    }
    LOG_INFO() << "get explaination for:" << word;
    QString explaination = wordsList.value(word);
    LOG_INFO() << explaination;
    dictShowExplaination->setText(
                explaination.replace(QString("\\r\\n"), QString("\n")));
}

void BibleDictionaryWidget::showExplaination(int index)
{
    LOG_DEBUG() << "Index: " << index;
    QString word = dictWordsCombo->itemText(index);
    LOG_INFO() << "get explaination for:" << word;
    QString explaination = wordsList.value(word);
    LOG_INFO() << explaination;
    dictShowExplaination->setText(
                explaination.replace(QString("\\r\\n"), QString("\n")));
}

void BibleDictionaryWidget::showExplaination(QString itemName)
{
    int index;
    if ((index = dictWordsCombo->findText(itemName)) != -1) {
        dictWordsCombo->setCurrentIndex(index);
        QString explaination = wordsList.value(itemName);
        dictShowExplaination->setText(
                    explaination.replace(QString("\\r\\n"), QString("\n")));
    }
}
