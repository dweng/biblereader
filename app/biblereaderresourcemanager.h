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

    QList<BRResource> getResources();

    QUrl getResourceUrl() const;
    void setResourceUrl(const QUrl &value);

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
};

#endif // BIBLEREADERRESOURCEMANAGER_H
