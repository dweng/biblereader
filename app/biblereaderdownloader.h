#ifndef BIBLEREADERDOWNLOADER_H
#define BIBLEREADERDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>

class BibleReaderDownloader : public QObject
{
    Q_OBJECT
public:
    explicit BibleReaderDownloader(QUrl &url, QObject *parent = 0);
    ~BibleReaderDownloader();

    QByteArray getData() const;

private:
    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QByteArray data;
    QString errorString;

signals:
    void finished();
    void error(QString errorText);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

public slots:
    void start();

private slots:
    virtual void replyFinished(QNetworkReply *reply);
    void getErrorString(QNetworkReply::NetworkError code);

};

#endif // BIBLEREADERDOWNLOADER_H
