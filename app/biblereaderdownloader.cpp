#include "biblereaderdownloader.h"
#include "Logger.h"

BibleReaderDownloader::BibleReaderDownloader(QUrl &url, QObject *parent) :
    QObject(parent), url(url)
{
    data.clear();
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

}

BibleReaderDownloader::~BibleReaderDownloader()
{
    if (manager) {
        delete manager;
    }
}

QByteArray BibleReaderDownloader::getData() const
{
    return data;
}

void BibleReaderDownloader::start()
{
    data.clear();
    reply = manager->get(QNetworkRequest(url));

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SIGNAL(getErrorString(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64)));
}

void BibleReaderDownloader::replyFinished(QNetworkReply *reply)
{
    data = reply->readAll();
    reply->deleteLater();
    emit finished();
}

void BibleReaderDownloader::getErrorString(QNetworkReply::NetworkError code)
{
    errorString = "";

    emit error(errorString);
}



