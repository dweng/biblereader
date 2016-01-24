#ifndef BRUPDATER_H
#define BRUPDATER_H

#include <QObject>

class BRUpdater : public QObject
{
    Q_OBJECT
public:
    explicit BRUpdater(QObject *parent = 0);
    ~BRUpdater();

signals:

public slots:
};

#endif // BRUPDATER_H
