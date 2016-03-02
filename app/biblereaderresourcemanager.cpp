#include "biblereaderresourcemanager.h"

BibleReaderResourceManager::BibleReaderResourceManager(QObject *parent) : QObject(parent)
{
    downloader = NULL;
}

BibleReaderResourceManager::~BibleReaderResourceManager()
{
    if (downloader) {
        delete downloader;
    }
}

void BibleReaderResourceManager::addResourceUrl(QString key, QString url)
{
    if (!resourceUrls.contains(key)) {
        resourceUrls.insert(key, QUrl(url));
    }
}

void BibleReaderResourceManager::removeResourceUrl(QString key)
{
    if (resourceUrls.contains(key)) {
        resourceUrls.remove(key);
    }
}

QList<BRResource> BibleReaderResourceManager::getResources(QString key)
{
    if (resources.contains(key)) {
        return resources.value(key);
    } else {
        return QList<BRResource>();
    }
}

QMap<QString, QUrl> BibleReaderResourceManager::getResourceUrls() const
{
    return resourceUrls;
}

void BibleReaderResourceManager::refresh(QString resKey)
{
    if (resourceUrls.contains(resKey)) {
        QUrl url = resourceUrls.value(resKey);
        downloader = new BibleReaderDownloader(resKey, url, this);
        connect(downloader, SIGNAL(finished(QString)), this, SLOT(gotResources(QString)));
        downloader->start();
    }
}

void BibleReaderResourceManager::refreshAll()
{
    QMap<QString, QUrl>::const_iterator iter = resourceUrls.constBegin();

    while (iter != resourceUrls.constEnd()) {
        refresh(iter.key());
        iter++;
    }
}

void BibleReaderResourceManager::gotResources(QString key)
{
    resources[key] = downloader->getResources();
}


