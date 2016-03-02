#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFormLayout>
#include <QColorDialog>
#include <QFontComboBox>
#include <QComboBox>
#include <QFontDatabase>
#include <QList>
#include <QStringList>

#include "biblereaderconfiggeneralpage.h"

BibleReaderConfigGeneralPage::BibleReaderConfigGeneralPage(
        BibleReaderConfigurator *cfg,QWidget *parent) : QWidget(parent)
{
    configurator = cfg;

    colorQSS = QString("background-color: %1");
    QGroupBox *colorGroup = new QGroupBox(tr("Font and Color"));

    QFormLayout *colorLayout = new QFormLayout(this);
    selectedVerseBGBtn = new QPushButton(tr(""));
    selectedVerseBGBtn->setStyleSheet(colorQSS.arg(configurator->getSelectedVerseBG().name()));
    selectedVerseBGBtn->setFixedSize(50, 25);
    projectDlgBGBtn = new QPushButton(tr(""));
    projectDlgBGBtn->setStyleSheet(colorQSS.arg(configurator->getProjectDlgBG().name()));
    projectDlgBGBtn->setFixedSize(50,25);
    projectDlgFGBtn = new QPushButton(tr(""));
    projectDlgFGBtn->setStyleSheet(colorQSS.arg(configurator->getProjectDlgFG().name()));
    projectDlgFGBtn->setFixedSize(50,25);

    // font
    fontCB = new QFontComboBox(this);
    fontCB->setCurrentFont(QFont(cfg->getBibleTextFontFamily()));

    fontSizeCB = new QComboBox(this);
    QStringList fontSizesStringList;
    QList<int> fontSizes = QFontDatabase::standardSizes();
    for (int i = 0; i < fontSizes.size(); i++) {
        fontSizesStringList << QString::number(fontSizes[i]);
    }
    fontSizeCB->addItems(fontSizesStringList);
    fontSizeCB->setCurrentText(QString::number((int)cfg->getBibleTextFontSize()));

    colorLayout->addRow(tr("Selected verse background"), selectedVerseBGBtn);
    colorLayout->addRow(tr("Project background"), projectDlgBGBtn);
    colorLayout->addRow(tr("Project foreground"), projectDlgFGBtn);
    colorLayout->addRow(tr("Bible Text Font"), fontCB);
    colorLayout->addRow(tr("Bible Text Font size"), fontSizeCB);
    colorGroup->setLayout(colorLayout);
    connect(selectedVerseBGBtn, SIGNAL(clicked(bool)), this, SLOT(SVBGBtnClicked()));
    connect(projectDlgBGBtn, SIGNAL(clicked(bool)), this, SLOT(prjBGBtnClicked()));
    connect(projectDlgFGBtn, SIGNAL(clicked(bool)), this, SLOT(prjFGBtnClicked()));
    connect(fontCB, SIGNAL(currentFontChanged(QFont)), this, SLOT(btFontFamilyChanged(QFont)));
    connect(fontSizeCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(btFontSizeChanged(QString)));

    QGroupBox *updateGroup = new QGroupBox(tr("Update"));
    QCheckBox *autoUpdateCheckBox = new QCheckBox(tr("Auto update"));
    if (configurator->getIsAutoUpdate()) {
        autoUpdateCheckBox->setChecked(true);
    }
    connect(autoUpdateCheckBox, SIGNAL(toggled(bool)), this, SLOT(autoCheckBoxClicked(bool)));

    QVBoxLayout *updateLayout = new QVBoxLayout;
    updateLayout->addWidget(autoUpdateCheckBox);
    updateGroup->setLayout(updateLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(colorGroup);
    mainLayout->addWidget(updateGroup);
    mainLayout->addSpacing(12);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

BibleReaderConfigGeneralPage::~BibleReaderConfigGeneralPage()
{
    delete selectedVerseBGBtn;
    delete projectDlgBGBtn;
    delete projectDlgFGBtn;
}

QColor BibleReaderConfigGeneralPage::getSelectedVerseBG() const
{
    return selectedVerseBG;
}

void BibleReaderConfigGeneralPage::setSelectedVerseBG(const QColor &value)
{
    selectedVerseBG = value;
}

QColor BibleReaderConfigGeneralPage::getPrjBG() const
{
    return prjBG;
}

void BibleReaderConfigGeneralPage::setPrjBG(const QColor &value)
{
    prjBG = value;
}

QColor BibleReaderConfigGeneralPage::getPrjFG() const
{
    return prjFG;
}

void BibleReaderConfigGeneralPage::setPrjFG(const QColor &value)
{
    prjFG = value;
}

void BibleReaderConfigGeneralPage::SVBGBtnClicked()
{
    const QColor color = QColorDialog::getColor(Qt::green, this, tr("Select color"));

    if (color.isValid()) {
        selectedVerseBG = color;
        selectedVerseBGBtn->setStyleSheet(colorQSS.arg(selectedVerseBG.name()));

        configurator->setSelectedVerseBG(color);
    }
}

void BibleReaderConfigGeneralPage::prjBGBtnClicked()
{
    const QColor color = QColorDialog::getColor(Qt::green, this, tr("Select color"));

    if (color.isValid()) {
        prjBG = color;
        projectDlgBGBtn->setStyleSheet(colorQSS.arg(prjBG.name()));

        configurator->setProjectDlgBG(color);
    }
}

void BibleReaderConfigGeneralPage::prjFGBtnClicked()
{
    const QColor color = QColorDialog::getColor(Qt::green, this, tr("Select color"));

    if (color.isValid()) {
        prjFG = color;
        projectDlgFGBtn->setStyleSheet(colorQSS.arg(prjFG.name()));

        configurator->setProjectDlgFG(color);
    }
}

void BibleReaderConfigGeneralPage::autoCheckBoxClicked(bool checked)
{
    if (checked) {
        configurator->setIsAutoUpdate(true);
    } else {
        configurator->setIsAutoUpdate(false);
    }
}

void BibleReaderConfigGeneralPage::btFontFamilyChanged(QFont f)
{
    configurator->setBibleTextFontFamily(f.family());
}

void BibleReaderConfigGeneralPage::btFontSizeChanged(QString size)
{
    configurator->setBibleTextFontSize(size.toDouble());
}

