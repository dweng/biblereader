#ifndef BIBLEREADERRESOURCEMANAGER_H
#define BIBLEREADERRESOURCEMANAGER_H

#include <QObject>
#include <QUrl>
#include <QMap>
#include <QList>

#include "biblereaderdownloader.h"

class BibleReaderCore;

class BibleReaderResourceManager : public QObject
{
    Q_OBJECT
public:
    explicit BibleReaderResourceManager(QObject *parent = 0);
    ~BibleReaderResourceManager();

    QList<BRResource> getResources();

    QUrl getResourceUrl() const;
    void setResourceUrl(const QUrl &value);

    bool removeRes(BRResource resource, BibleReaderCore *brCore);
    bool installRes(BRResource resource);
    bool updateRes(BRResource resource);

private:
    BibleReaderDownloader *downloader;
    QUrl resourceUrl;
    QList<BRResource> resources;

signals:
    void refreshed();

public slots:
    void refresh();

private slots:
    void gotResource();

private:
    bool deleteDirectory(const QString &path);
};
#endif // BIBLEREADERRESOURCEMANAGER_H
