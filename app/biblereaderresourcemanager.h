#ifndef BIBLEREADERRESOURCEMANAGER_H
#define BIBLEREADERRESOURCEMANAGER_H

#include <QObject>
#include <QUrl>
#include <QMap>
#include <QList>

#include "biblereaderdownloader.h"

class BibleReaderCore;

/**
 * @brief The ResourceType enum
 * Bible Resource enum
 */
enum ResourceType {
    Bible,
    Commentary,
    Dict,
    Map,
    Book
};

/**
 * @brief Bible Resource structure
 */
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
    bool installRes(BRResource resource, BibleReaderCore *brCore);
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
    void gotResources();

private:
    bool deleteDirectory(const QString &path);
    QString type2str(ResourceType type);
    void parseResources(QString resString);
};
#endif // BIBLEREADERRESOURCEMANAGER_H
