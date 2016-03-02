#ifndef BIBLEREADERRESOURCEMANAGER_H
#define BIBLEREADERRESOURCEMANAGER_H

#include <QObject>
#include <QUrl>
#include <QMap>
#include <QList>
#include "biblereaderdownloader.h"

class BibleReaderResourceManager : public QObject
{
    Q_OBJECT
public:
    explicit BibleReaderResourceManager(QObject *parent = 0);
    ~BibleReaderResourceManager();

    void addResourceUrl(QString key, QString url);
    void removeResourceUrl(QString key);

    QList<BRResource> getResources(QString key);

    QMap<QString, QUrl> getResourceUrls() const;

private:
    QMap<QString, QUrl> resourceUrls;
    BibleReaderDownloader *downloader;
    QMap< QString, QList<BRResource> > resources;

signals:
    void allRefreshed();
    void refreshed(QString resKey);

public slots:
    void refresh(QString resKey);
    void refreshAll();

private slots:
    void gotResources(QString key);
};

#endif // BIBLEREADERRESOURCEMANAGER_H
