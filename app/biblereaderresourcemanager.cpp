#include <QDir>
#include "biblereaderresourcemanager.h"
#include "biblereadercore.h"
#include <Logger.h>

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

bool BibleReaderResourceManager::removeRes(BRResource resource, BibleReaderCore *brCore)
{
    QString folderName = "";
    QDir tempDir;
    bool ret = true;
    switch (resource.type) {
    case Bible:
        brCore->removeBibleVersion(resource.shortName);
        folderName.append(brCore->getConfigurator()->getBiblePathBase()).
                append(resource.shortName).append(QDir::separator());

        ret = deleteDirectory(folderName);
        LOG_INFO() << folderName << ret;
        break;

    case Dict:

        break;
    default:
        break;

    }
    return ret;
}

bool BibleReaderResourceManager::installRes(BRResource resource)
{
    return true;
}

bool BibleReaderResourceManager::updateRes(BRResource resource)
{
    return true;
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


bool BibleReaderResourceManager::deleteDirectory(const QString &path)
{
    if (path.isEmpty())
        return false;

    QDir dir(path);
    if(!dir.exists())
        return true;

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        LOG_INFO() << fi.absoluteFilePath();
        if (fi.isFile())
            LOG_INFO() << fi.dir().remove(fi.fileName());
        else
            LOG_INFO() << deleteDirectory(fi.absoluteFilePath());
    }
    return dir.rmpath(dir.absolutePath());
}
