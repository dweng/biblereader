#ifndef BIBLEREADERDOWNLOADER_H
#define BIBLEREADERDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>

enum ResourceType {
    Bible,
    Commentary,
    Dict,
    Map,
    Book
};

typedef struct _BRResource {
    QString shortName;
    QString description;
    QString url;
    ResourceType type;
    QString typeStr;
    QString longName;
    int size;
    int version;

    bool isinstalled;
    bool isupdated;
} BRResource;

class BibleReaderDownloader : public QObject
{
    Q_OBJECT
public:
    explicit BibleReaderDownloader(QUrl &url, QObject *parent = 0);
    ~BibleReaderDownloader();

    QList<BRResource> getResources() const;

private:
    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

    QList<BRResource> resources;

signals:
    void finished();
    //void error(QString errorText);
    //void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

public slots:
    void start();

private slots:
    virtual void replyFinished(QNetworkReply *reply);

private:
    QString type2str(ResourceType type);
};

#endif // BIBLEREADERDOWNLOADER_H
