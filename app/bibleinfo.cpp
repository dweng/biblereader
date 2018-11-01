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
#include "bibleinfo.h"

BibleInfo::BibleInfo()
{
    version = "";
    language = "";
    hasSN = false;
    chapterString = "";
    description = "";
    rtl = false;
    dataPath = "";
    copyright = "";
}
QString BibleInfo::getVersion() const
{
    return version;
}

void BibleInfo::setVersion(const QString &value)
{
    version = value;
}
QString BibleInfo::getLanguage() const
{
    return language;
}

void BibleInfo::setLanguage(const QString &value)
{
    language = value;
}
bool BibleInfo::getHasSN() const
{
    return hasSN;
}

void BibleInfo::setHasSN(bool value)
{
    hasSN = value;
}
QString BibleInfo::getChapterString() const
{
    return chapterString;
}

void BibleInfo::setChapterString(const QString &value)
{
    chapterString = value;
}
bool BibleInfo::getRtl() const
{
    return rtl;
}

void BibleInfo::setRtl(bool value)
{
    rtl = value;
}
QString BibleInfo::getDataPath() const
{
    return dataPath;
}

void BibleInfo::setDataPath(const QString &value)
{
    dataPath = value;
}
QString BibleInfo::getDescription() const
{
    return description;
}

void BibleInfo::setDescription(const QString &value)
{
    description = value;
}
QString BibleInfo::getCopyright() const
{
    return copyright;
}

void BibleInfo::setCopyright(const QString &value)
{
    copyright = value;
}
QString BibleInfo::getFullname() const
{
    return fullname;
}

void BibleInfo::setFullname(const QString &value)
{
    fullname = value;
}

int BibleInfo::getNumber() const
{
    return number;
}

void BibleInfo::setNumber(int value)
{
    number = value;
}


