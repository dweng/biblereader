#include "brupdater.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

BRUpdater::BRUpdater(int argc, char *argv[]) : QCoreApplication(argc, argv)
{
    // 1. get update information from server
    server = "http://biblereader.godlove.us/updater.php";

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(server)));

    // 2. compare version information
    // 3. do upgrade if need
}

BRUpdater::~BRUpdater()
{
    if (manager) {
        delete manager;
    }

    if (data) {
        delete data;
    }
}

QString BRUpdater::replyFinished(QNetworkReply *reply)
{
    replyString = reply->readAll();
    parse();
    reply->deleteLater();

    return replyString;
}

void BRUpdater::parse()
{
    if (!replyString.isEmpty()) {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(replyString.toUtf8(), &error);

        if (error.error == QJsonParseError::NoError) {
            if (doc.isObject()) {
                // get update version information
                // and assign all information to data
                qDebug() << doc.toJson();
            }
        } else {
            qDebug() << replyString;
        }
    }
}

