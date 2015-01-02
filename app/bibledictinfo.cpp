#include "bibledictinfo.h"

BibleDictInfo::BibleDictInfo()
{
}

BibleDictInfo::BibleDictInfo(QString& n, QString& d)
{
    name = n;
    description = d;
}

void BibleDictInfo::setName(QString &value)
{
    name = value;
}

QString BibleDictInfo::getName()
{
    return name;
}

void BibleDictInfo::setDescription(QString &value)
{
    description = value;
}

QString BibleDictInfo::getDescription()
{
    return description;
}
