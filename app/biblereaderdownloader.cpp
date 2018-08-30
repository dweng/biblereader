#include "biblereaderdownloader.h"
#include <QDomDocument>
#include "Logger.h"

BibleReaderDownloader::BibleReaderDownloader(QUrl &url, QObject *parent) :
    QObject(parent), url(url)
{
    resources.clear();
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
        QDomDocument doc;
        QString error;
        int row = 0, column = 0;

        if (doc.setContent(replyString, &error, &row, &column)) {
            QDomElement root = doc.documentElement();

            // bibles
            QDomNode bibleRoot = root.firstChild();
            QDomNodeList bibles = bibleRoot.childNodes();

            for (int i = 0; i < bibles.count(); i++) {
                QDomElement bible = bibles.at(i).toElement();

                BRResource res;

                res.longName = bible.attribute("longname");
                res.shortName = bible.attribute("shortname");
                res.size = bible.attribute("size").toInt();
                res.version = bible.attribute("version").toInt();
                res.url = bible.attribute("url");
                res.type = Bible;
                res.typeStr = type2str(Bible);

                resources.push_back(res);
            }

            // dicts
            QDomNode dictRoot = bibleRoot.nextSibling();
            QDomNodeList dicts = dictRoot.childNodes();

            for (int i = 0; i < dicts.count(); i++) {
                QDomElement dict = dicts.at(i).toElement();

                BRResource res;

                res.longName = dict.attribute("longname");
                res.shortName = dict.attribute("shortname");
                res.size = dict.attribute("size").toInt();
                res.version = dict.attribute("version").toInt();
                res.url = dict.attribute("url");
                res.type = Dict;
                res.typeStr = type2str(Dict);

                resources.push_back(res);
            }

        } else {
            LOG_INFO() << replyString;
        }
    }
    reply->deleteLater();

    emit finished();
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



