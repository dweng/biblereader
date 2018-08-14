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

QList<BRResource> BibleReaderResourceManager::getResources()
{
    return resources;
}

QUrl BibleReaderResourceManager::getResourceUrl() const
{
    return resourceUrl;
}

void BibleReaderResourceManager::setResourceUrl(const QUrl &value)
{
    resourceUrl = value;
}

void BibleReaderResourceManager::refresh()
{
    downloader = new BibleReaderDownloader(resourceUrl, this);
    connect(downloader, SIGNAL(finished()), this, SLOT(gotResource()));
    downloader->start();
}

void BibleReaderResourceManager::gotResource()
{
    this->resources = downloader->getResources();

    emit refreshed();
}


