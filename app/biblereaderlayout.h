#ifndef BIBLEREADERLAYOUT_H
#define BIBLEREADERLAYOUT_H

#include <QObject>
#include <QAction>

class BibleReaderLayout : public QObject
{
    Q_OBJECT
public:
    explicit BibleReaderLayout(QObject *parent = 0);

signals:

public slots:

private:
    QAction *act;


};

#endif // BIBLEREADERLAYOUT_H
