#ifndef BIBLEREADERTOOLTIP_H
#define BIBLEREADERTOOLTIP_H

#include <QFrame>
#include <QString>
#include <QPoint>
#include <QTextBrowser>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QFont>

class BibleReaderToolTip: public QFrame
{
    Q_OBJECT
public:
    void showText(QPoint pos, QString text, int mseconds);
    void setTextFont(QFont font);
    ~BibleReaderToolTip();
    BibleReaderToolTip(QWidget *parent = 0);


private:
    // widgets
    QTextBrowser *tipContentWidget;
    QLabel *helpInfoWidget;
    QString text;
    QTimer *durationTimer;

protected:
    void leaveEvent(QEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *event);
private:
    void createWidgets();
    void destroyWidgets();
    int screenWidth;
    int screenHeight;
    bool shiftPressed;

signals:

private slots:
    void hideTip();
};

#endif // BIBLEREADERTOOLTIP_H
