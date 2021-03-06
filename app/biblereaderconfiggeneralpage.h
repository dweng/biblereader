#ifndef BIBLEREADERCONFIGGENERALPAGE_H
#define BIBLEREADERCONFIGGENERALPAGE_H

#include <QWidget>
#include "biblereaderconfigurator.h"

QT_BEGIN_NAMESPACE

class QColor;
class QPushButton;
class QFontComboBox;
class QComboBox;

QT_END_NAMESPACE

class BibleReaderConfigGeneralPage : public QWidget
{
    Q_OBJECT
public:
    explicit BibleReaderConfigGeneralPage(BibleReaderConfigurator *cfg, QWidget *parent = 0);
    ~BibleReaderConfigGeneralPage();

    QColor getSelectedVerseBG() const;
    void setSelectedVerseBG(const QColor &value);

    QColor getPrjBG() const;
    void setPrjBG(const QColor &value);

    QColor getPrjFG() const;
    void setPrjFG(const QColor &value);

signals:

private slots:
    void SVBGBtnClicked();
    void prjBGBtnClicked();
    void prjFGBtnClicked();
    void autoCheckBoxClicked(bool checked);
    void btFontFamilyChanged(QFont f);
    void btFontSizeChanged(QString size);

private:
    QColor selectedVerseBG;
    QColor prjBG;
    QColor prjFG;

    QString colorQSS;

    QPushButton *selectedVerseBGBtn;
    QPushButton *projectDlgBGBtn;
    QPushButton *projectDlgFGBtn;

    QFontComboBox *fontCB;
    QComboBox *fontSizeCB;

    BibleReaderConfigurator *configurator;
};

#endif // BIBLEREADERCONFIGGENERALPAGE_H
