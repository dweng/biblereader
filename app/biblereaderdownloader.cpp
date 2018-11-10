#include <QEventLoop>
#include "biblereaderdownloader.h"
#include "Logger.h"

BibleReaderDownloader::BibleReaderDownloader(QUrl &url, QObject *parent) :
    QObject(parent), url(url)
{
    data.clear();
    isFinished = false;
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

QUrl BibleReaderDownloader::getUrl() const
{
    return url;
}

void BibleReaderDownloader::setUrl(const QUrl &value)
{
    url = value;
}

bool BibleReaderDownloader::getIsFinished() const
{
    return isFinished;
}

void BibleReaderDownloader::start(int block)
{
    data.clear();
    if (block == 1) {
        QEventLoop el;
        reply = manager->get(QNetworkRequest(url));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(getErrorString(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64)));
        connect(manager, SIGNAL(finished(QNetworkReply*)), &el, SLOT(quit()));
        el.exec();
    } else {
        reply = manager->get(QNetworkRequest(url));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(getErrorString(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64, qint64)));
    }
}

void BibleReaderDownloader::replyFinished(QNetworkReply *reply)
{
    data = reply->readAll();
    isFinished = true;

    emit finished();
}

void BibleReaderDownloader::getErrorString(QNetworkReply::NetworkError code)
{
    errorString = "";

    emit error(errorString);
}



