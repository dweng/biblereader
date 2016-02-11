#ifndef BIBLEREADERDONATIONDLG_H
#define BIBLEREADERDONATIONDLG_H

#include <QDialog>
#include <QTextBrowser>
#include <QMouseEvent>
#include <QDebug>

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;

QT_END_NAMESPACE

class DonationBrowser: public QTextBrowser {
public:
    DonationBrowser(QWidget *parent): QTextBrowser(parent) {
        setOpenExternalLinks(true);
    }

protected:
    void mouseMoveEvent(QMouseEvent *e){
        QTextCursor textCursor = cursorForPosition(e->pos());
        textCursor.select(QTextCursor::WordUnderCursor);

        QString href = textCursor.charFormat().anchorHref();
        if (!href.isEmpty())
            qDebug()<< href;
        QTextBrowser::mouseMoveEvent(e);
    }
};

class BibleReaderDonationDlg : public QDialog
{
    Q_OBJECT
public:
    explicit BibleReaderDonationDlg(QWidget *parent = 0);
    ~BibleReaderDonationDlg();

signals:

public slots:

private:
    DonationBrowser *donation;
    QString readDonation(QString path);
};

#endif // BIBLEREADERDONATIONDLG_H
