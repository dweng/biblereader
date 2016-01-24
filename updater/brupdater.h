#ifndef BRUPDATER_H
#define BRUPDATER_H

#include <QCoreApplication>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "brupdaterdata.h"

class BRUpdater : public QCoreApplication
{
    Q_OBJECT
public:
    explicit BRUpdater(int argc, char *argv[]);
    ~BRUpdater();

signals:

public slots:
    QString replyFinished(QNetworkReply *reply);

private:
    void parse();

private:
    /**
     * @brief server
     * updater server url
     */
    QString server;

    QString replyString;

    QNetworkAccessManager *manager;

    BRUpdaterData *data;
};

#endif // BRUPDATER_H
