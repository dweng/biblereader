#include "brupdaterdata.h"

BRUpdaterData::BRUpdaterData()
{

}

BRUpdaterData::~BRUpdaterData()
{

}
int BRUpdaterData::getMajorVersion() const
{
    return majorVersion;
}

void BRUpdaterData::setMajorVersion(int value)
{
    majorVersion = value;
}
int BRUpdaterData::getMinorVersion() const
{
    return minorVersion;
}

void BRUpdaterData::setMinorVersion(int value)
{
    minorVersion = value;
}
int BRUpdaterData::getTailVersion() const
{
    return tailVersion;
}

void BRUpdaterData::setTailVersion(int value)
{
    tailVersion = value;
}
QString BRUpdaterData::getNewVersionDownloadUrl() const
{
    return newVersionDownloadUrl;
}

void BRUpdaterData::setNewVersionDownloadUrl(const QString &value)
{
    newVersionDownloadUrl = value;
}





