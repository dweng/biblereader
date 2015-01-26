/**
 * [1 Timothy 3:16-17 ESV]
 * All Scripture is breathed out by God and profitable
 * for teaching, for reproof, for correction, and for training in righteousness,
 * that the man of God may be complete, equipped for every good work.
 *
 * BibleReader is an simple application for you to study God's Word.
 * The main user of this application maybe the pastor who's main language
 * is Chinese.
 *
 * You can use it totally free, and if you want to get source code of
 * this application, please email me at dweng123@gmail to get the source
 * code.
 */
#include "bibledictinfo.h"

BibleDictInfo::BibleDictInfo()
{
}

BibleDictInfo::BibleDictInfo(QString& n, QString& d)
{
    name = n;
    description = d;
}

void BibleDictInfo::setName(const QString &value)
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
QString BibleDictInfo::getShortname() const
{
    return shortname;
}

void BibleDictInfo::setShortname(const QString &value)
{
    shortname = value;
}

QString BibleDictInfo::getCopyright() const
{
    return copyright;
}

void BibleDictInfo::setCopyright(const QString &value)
{
    copyright = value;
}
int BibleDictInfo::getVersion() const
{
    return version;
}

void BibleDictInfo::setVersion(int value)
{
    version = value;
}
QString BibleDictInfo::getLang() const
{
    return lang;
}

void BibleDictInfo::setLang(const QString &value)
{
    lang = value;
}
QString BibleDictInfo::getFullname() const
{
    return fullname;
}

void BibleDictInfo::setFullname(const QString &value)
{
    fullname = value;
}






