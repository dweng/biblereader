#include <QDir>
#include <QFile>
#include <QDomDocument>
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

bool BibleReaderResourceManager::installRes(BRResource resource, BibleReaderCore *brCore)
{
    QString baseUrl = "http://files.biblereader.cn/resources/";

    QString folderName = "";
    QDir tempDir;
    bool ret = true;
    switch (resource.type) {
    case Bible:
        // brCore->removeBibleVersion(resource.shortName);
        folderName.append(brCore->getConfigurator()->getBiblePathBase()).
                append(resource.shortName).append(QDir::separator());

        downloader->setUrl(QUrl(baseUrl.append(resource.url)));
        downloader->start();
        while (downloader->getIsFinished()) {
            // create folder
            tempDir.setPath(brCore->getConfigurator()->getBiblePathBase());
            tempDir.mkdir(resource.shortName);
            // save file
            QFile tmpFile(resource.shortName.append(".BIB"));
            if (tmpFile.open(QIODevice::WriteOnly)) {
                tmpFile.write(downloader->getData());
                tmpFile.close();
            } else {
                ret = false;
            }

            break;
        }
        LOG_INFO() << folderName << ret;
        break;

    case Dict:

        break;
    default:
        break;

    }
    return ret;
}

bool BibleReaderResourceManager::updateRes(BRResource resource)
{

    return true;
}

void BibleReaderResourceManager::refresh()
{
    downloader = new BibleReaderDownloader(resourceUrl, this);
    connect(downloader, SIGNAL(finished()), this, SLOT(gotResources()));
    downloader->start();
}

void BibleReaderResourceManager::gotResources()
{
    QString replyString = downloader->getData();
    parseResources(replyString);

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

QString BibleReaderResourceManager::type2str(ResourceType type)
{
    QString typeStr;
    switch (type) {
    case Book:
        typeStr = tr("Book");
        break;

    case Bible:
        typeStr = tr("Bible");
        break;

    case Commentary:
        typeStr = tr("Commentary");
        break;

    case Dict:
        typeStr = tr("Dict");
        break;

    case Map:
        typeStr = tr("Map");
        break;
    default:
        typeStr = tr("Unknown");
        break;

    }

    return typeStr;
}

void BibleReaderResourceManager::parseResources(QString resString)
{
    // delete all resource item
    resources.clear();

    if (!resString.isEmpty()) {
        QDomDocument doc;
        QString error;
        int row = 0, column = 0;

        if (doc.setContent(resString, &error, &row, &column)) {
            QDomElement root = doc.documentElement();

            // bibles
            QDomNode bibleRoot = root.firstChild();
            QDomNodeList bibles = bibleRoot.childNodes();

            for (int i = 0; i < bibles.count(); i++) {
                QDomElement bible = bibles.at(i).toElement();

                BRResource res;

                res.longName = bible.attribute("longname");
                res.shortName = bible.attribute("shortname");
                res.size = bible.attribute("size").toInt();
                res.version = bible.attribute("version").toInt();
                res.url = bible.attribute("url");
                res.type = Bible;
                res.typeStr = type2str(Bible);
                res.isinstalled = false;
                res.isupdated = false;

                resources.push_back(res);
            }

            // dicts
            QDomNode dictRoot = bibleRoot.nextSibling();
            QDomNodeList dicts = dictRoot.childNodes();

            for (int i = 0; i < dicts.count(); i++) {
                QDomElement dict = dicts.at(i).toElement();

                BRResource res;

                res.longName = dict.attribute("longname");
                res.shortName = dict.attribute("shortname");
                res.size = dict.attribute("size").toInt();
                res.version = dict.attribute("version").toInt();
                res.url = dict.attribute("url");
                res.type = Dict;
                res.typeStr = type2str(Dict);
                res.isinstalled = false;
                res.isupdated = false;

                resources.push_back(res);
            }

        } else {
            LOG_INFO() << resString;
        }
    }
}
