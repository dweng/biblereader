#include "biblereaderdownloader.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "Logger.h"

BibleReaderDownloader::BibleReaderDownloader(QString &key, QUrl &url, QObject *parent) :
    QObject(parent), url(url), key(key)
{

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

}

BibleReaderDownloader::~BibleReaderDownloader()
{
    if (manager) {
        delete manager;
    }
}

QList<BRResource> BibleReaderDownloader::getResources() const
{
    return resources;
}

void BibleReaderDownloader::start()
{
    reply = manager->get(QNetworkRequest(url));
}

void BibleReaderDownloader::replyFinished(QNetworkReply *reply)
{
    // delete all resource item
    resources.clear();

    //
    QString replyString = reply->readAll();
    if (!replyString.isEmpty()) {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(replyString.toUtf8(), &error);

        if (error.error == QJsonParseError::NoError) {
            if (doc.isObject()) {
                QJsonObject object = doc.object();
                QJsonArray resArray = object.value("BRResource").toArray();

                for (int i=0; i < resArray.size(); i++) {
                    BRResource res;
                    QJsonObject tmp = resArray[i].toObject();
                    res.description = tmp.value("description").toString();
                    res.key = tmp.value("key").toString();
                    res.size = tmp.value("size").toInt();
                    res.type = (ResourceType)tmp.value("type").toInt();
                    res.typeStr = type2str(res.type);
                    res.url = tmp.value("url").toString();
                    res.version = tmp.value("version").toInt();

                    resources.push_back(res);
                }
            }
        } else {
            LOG_INFO() << replyString;
        }
    }
    reply->deleteLater();

    emit finished(key);
}

QString BibleReaderDownloader::type2str(ResourceType type)
{
    QString typeStr;
    switch (type) {
    case Book:
        typeStr = tr("Book");
        break;

    case Bible:
        typeStr = tr("Bible");
        break;

    case Commentary:
        typeStr = tr("Commentary");
        break;

    case Dict:
        typeStr = tr("Dict");
        break;

    case Map:
        typeStr = tr("Map");
        break;
    default:
        typeStr = tr("Unknown");
        break;

    }

    return typeStr;
}

