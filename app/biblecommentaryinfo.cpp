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
#include "biblecommentaryinfo.h"

BibleCommentary::BibleCommentaryInfo()
{
}

QString BibleCommentaryInfo::getName() const
{
    return name;
}

void BibleCommentaryInfo::setName(const QString &value)
{
    name = value;
}
QString BibleCommentaryInfo::getDescription() const
{
    return description;
}

void BibleCommentaryInfo::setDescription(const QString &value)
{
    description = value;
}
QString BibleCommentaryInfo::getShortname() const
{
    return shortname;
}

void BibleCommentaryInfo::setShortname(const QString &value)
{
    shortname = value;
}
QString BibleCommentaryInfo::getFullname() const
{
    return fullname;
}

void BibleCommentaryInfo::setFullname(const QString &value)
{
    fullname = value;
}
QString BibleCommentaryInfo::getCopyright() const
{
    return copyright;
}

void BibleCommentaryInfo::setCopyright(const QString &value)
{
    copyright = value;
}
int BibleCommentaryInfo::getVersion() const
{
    return version;
}

void BibleCommentaryInfo::setVersion(int value)
{
    version = value;
}
QString BibleCommentaryInfo::getLang() const
{
    return lang;
}

void BibleCommentaryInfo::setLang(const QString &value)
{
    lang = value;
}






