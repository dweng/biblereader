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
#ifndef BIBLEDICTINFO_H
#define BIBLEDICTINFO_H

#include <QString>

/**
 * @brief The BibleDictInfo class
 * this class represents information of a bible dict
 *
 * @version 1.0
 */
class BibleDictInfo
{
public:
    BibleDictInfo();
    BibleDictInfo(QString& n, QString& d);

    void setName(const QString& value);
    QString getName();

    void setDescription(QString& value);
    QString getDescription();

    QString getShortname() const;
    void setShortname(const QString &value);

    QString getCopyright() const;
    void setCopyright(const QString &value);

    int getVersion() const;
    void setVersion(int value);

    QString getLang() const;
    void setLang(const QString &value);

    QString getFullname() const;
    void setFullname(const QString &value);

private:
    QString name;
    QString description;
    QString shortname;
    QString fullname;
    QString copyright;
    int version;
    QString lang;
};

#endif // BIBLEDICTINFO_H
