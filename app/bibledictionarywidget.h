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
#ifndef BIBLEDICTIONARYWIDGET_H
#define BIBLEDICTIONARYWIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include <QListWidget>
#include <QComboBox>
#include <QSplitter>

#include "biblereadercore.h"
#include "bibledictbrowser.h"
#include "Logger.h"

/**
 * @brief The BibleDictionaryWidget class
 * This class represents dictionary widget of Bible Reader
 *
 * @version 1.0
 */
class BibleDictionaryWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Bible Dictionary widget
     * @param parent parent widget
     */
    explicit BibleDictionaryWidget(BibleReaderCore *brc, QString dn, QWidget *parent = 0);
    
    /**
     * @brief Destructor.
     */
    ~BibleDictionaryWidget();
    QString getDictName() const;
    void setDictName(const QString &value);

signals:

private slots:
    void showExplaination(QListWidgetItem *, QListWidgetItem *);
    void showExplaination(int index);

public slots:
    void showExplaination(QString itemName);

private:
    /**
     * @brief Create widgets in this widgets.
     */
    void createWidgets();

    /**
     * @brief Destory widgets.
     */
    void destoryWidgets();

    /**
     * @brief compileExplaination
     * @param exp QString
     * @return compiled exp
     */
    QString compileExplaination(QString exp);
private:
    /**
     * @brief bible reader core instance.
     */
    BibleReaderCore *brCore;

    /**
     * @brief Show word explaination area.
     */
    BibleDictBrowser *dictShowExplaination;

    /**
     * @brief Dictionary words combo box
     */
    QComboBox *dictWordsCombo;

    /**
     * @brief Dictionary words list widget
     */
    QListWidget *dictWordsList;

    /**
     * @brief Splitter widget for dictionary widget
     */
    QSplitter *dictSplitter;

    /**
     * @brief Left side of widget container.
     */
    QWidget *leftContainer;

    /**
     * @brief Dictionary cache.
     */
    QMap<QString, QString> wordsList;

    /**
     * @brief Dictionary name.
     */
    QString dictName;

    /**
     * @brief This dictionary is current active or not.
     */
    bool isCurrent;

    /**
     * @brief This dictionary is default active or not.
     */
    bool isDefault;
};

#endif // BIBLEDICTIONARYWIDGET_H
