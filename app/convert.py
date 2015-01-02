# -*- coding: utf-8 -*-

import sqlite3
import os
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

try:
    import xml.etree.cElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET

# Book for a Bible Book
class Book:
    longName = ''
    shortName = ''
    bookID = 0
    chapterCount = 0

    def __init__(longName, shortName, bookID, chapterCount):
        self.longName = longName
        self.shortName = shortName
        self.bookID = bookID
        self.chapterCount = chapterCount

# SBL greek new testament
def convertSBL(dbpath, sblXML, bookChapterCounts):
    db = sqlite3.connect(dbpath)
    # 1. read SBL bible text from text file
    bookID = 470
    bibleTree = ET.ElementTree(file = sblXML)
    j = 39
    for element in bibleTree.iter(tag = 'book'):
        bShortName = element.attrib['id']
        bLongName = element.find('title').text
        sql = 'insert into books values("%s", %i, "%s", "%s", %i)' % ('#112233', bookID, bShortName, bLongName, bookChapterCounts[j])
        print sql
        db.execute(sql)
        db.commit()

        for pEl in element.iter('p'):
            newVerse = False
            newVerseText = ''
            chapterID = 0
            verseID = 0
            for el in pEl:
                if (el.tag == 'verse-number'):
                    if (newVerseText != '' and chapterID != 0 and verseID != 0):
                        sql = 'insert into verses values(%i, %s, %s, "%s")' % (bookID, chapterID, verseID, newVerseText)
                        print sql
                        db.execute(sql)
                        db.commit()
                    newVerse = True
                    newVerseText = ''
                    chapter = el.attrib['id'].split(' ')[-1].split(':')
                    chapterID = chapter[0]
                    verseID = chapter[1]
                elif (newVerse == True and el.tag != 'verse-number'):
                    newVerseText = newVerseText + el.text
            if (newVerseText != '' and chapterID != 0 and verseID != 0):
                sql = 'insert into verses values(%i, %s, %s, "%s")' % (bookID, chapterID, verseID, newVerseText)
                print sql   
                db.execute(sql)
                db.commit()
        j=j+1
        bookID = bookID + 10
    db.close()

# CUV-T Chinese version
def convertCUVT(dbpath, cuvtdb):
    db_to = sqlite3.connect(dbpath)
    cu_to = db_to.cursor()
    db_from = sqlite3.connect(cuvtdb)
    cu_from = db_from.cursor()
    
    # get all books 
    cu_from.execute('select * from BibleBooks')
    books = cu_from.fetchall()
    
    # put all books
    for book in books:
        sql = 'insert into books values("#111111", %i, "%s", "%s", %i)' % (book[0], book[2], book[1], book[3])
        print sql
        cu_to.execute(sql)
    db_to.commit()
    
    # get all verses
    cu_from.execute('select * from BibleVerses')
    verses = cu_from.fetchall()
    
    # put all verses
    for verse in verses:
        sql = 'insert into verses values(%i, %i, %i, "%s")' % (verse[1], verse[2], verse[3], verse[4])
        print sql
        cu_to.execute(sql)
    db_to.commit()
    pass

def convertLXXA(dbpath, cuvtTXT, bookLongNames, bookShortNames, bookChapterCounts):
    pass

def convertCCB(dbpath, ccbPath, bookLongNames, bookShortNames, bookChapterCounts):
    import bs4
    from bs4 import BeautifulSoup
    
    db_to = sqlite3.connect(dbpath)
    cu_to = db_to.cursor()
    
    
    i = 0
    for book in bookLongNames:
        bookFolder = ccbPath + book
        files = os.listdir(bookFolder)
        # put book
        sql = 'insert into books values("#111111", %i, "%s", "%s", %i)' % (i+1, book, bookShortNames[i], bookChapterCounts[i])
        print sql
        cu_to.execute(sql)
        
        # bookfile = open(bookFolder + '/' + book + '.txt', 'w')
        booktext = ''
        for f in files:
            if f == '.' or f == '..':
                continue
            fo = open(bookFolder + '/' + f)
            soup = BeautifulSoup(fo)
            
            bookShort = bookShortNames[i]
            verses = soup.select('span.text')
            
            chapternum = f.split('.')[0]
            versenum = '0'
            preversenum = '0'
            versetext = ''
            for verse in verses:
                if verse.parent.name == 'h3':
                    continue
                chapterverse = verse['class'][1].split('-')
                versenum = chapterverse[2]
                if preversenum != versenum:
                    if preversenum != '0':
                        booktext += "%s %s %s\n" % (chapternum, preversenum, versetext)
                        sql = 'insert into verses values(%i, %s, %s, "%s")' % (i+1, chapternum, preversenum, versetext)
                        print sql
                        cu_to.execute(sql)
                    preversenum = versenum
                    versetext = ''
                for child in verse.children:
                    if isinstance(child, bs4.element.Tag) == True:
                        if child.name == 'versenum':
                            pass
                        elif child['class'] == 'chapternum':
                            pass
                        elif child['class'] == 'versenum':
                            pass
                        elif child['class'] == 'footnote':
                            pass
                    else:
                        versetext = versetext + child
            if (versetext != ''):
                booktext += "%s %s %s\n" % (chapternum, preversenum, versetext)
                sql = 'insert into verses values(%i, %s, %s, "%s")' % (i+1, chapternum, preversenum, versetext)
                print sql
                cu_to.execute(sql)
        i=i+1
        # bookfile.write(booktext)
        
        db_to.commit()
    pass
    
# main entrance
if __name__ == '__main__':
    bookLongNamesEn = ['Genesis', 'Exodus', 'Leviticus', 'Numbers', 'Deuteronomy',
        'Joshua', 'Judges', 'Ruth', '1 Samuel', '2 Samuel', '1 Kings', '2 Kings', '1 Chronicles', '2 Chronicles', 'Ezra', 'Nehemiah', 'Esther',
        'Job', 'Psalms','proverbs','Ecclesiastes','Song of songs',
        'Isaish', 'Jeremiah','Lamentations', 'Ezekiel', 'Daniel',
        'Hosea', 'Joel', 'Amos', 'Obadiah', 'Jonah','Micah','Nahum', 'Habakkuk', 'Zepaniah', 'Haggai','Zechariah','Malachi',
        'Matthew','Mark','Luke','John',
        'Acts', 
        'Romans', '1 Corinthians', '2 Cornthians', 'Galatians', 'Ephesians','Philippians', 'Colossians', '1 Thessalonians', 
        '2 Thessalonians', '1 Timothy', '2 Timothy', 'Titus', 'Philemon','Hebrews', 'James','1 Peter', '2 Peter', '1 John', 
        '2 John', '3 John', 'Jude', 'Revelation']
    bookShortNamesEn = ['Gen', 'Exo', 'Lev', 'Num', 'Deu', 'Jos', 'Jug', 'Rut', '1 Sa', '2 Sa', '1 Ki', '2 Ki',
                        '1 Ch', '2 Ch', 'Ezr', 'Neh', 'Est', 'Job', 'Psm', 'Pro', 'Son', 'Isa', 'Jer', 'Lam', 'Eze',
                        'Dan', 'Hos', 'Joe', 'Amo', 'Oba', 'Jon', 'Mic', 'Nah', 'Hab', 'Zep', 'Hag', 'Zec', 'Mal',
                        'Mat', 'Mak', 'Luk', 'Jhn', 'Act', 'Rom', '1 Co', '2 Co', 'Gal', 'Eph', 'Phl', 'Col', '1 Ts',
                        '2 Ts', '1 Ti', '2 Ti', 'Tit', 'Phm', 'Heb', 'Jas', '1 Pe', '2 Pe', '1 Jn', '2 Jn', '3 Jn',
                        'Jud', 'Rev']
    bookChapterCounts = [50, 40, 27, 36, 34, 24, 21, 4, 31, 24, 22, 25, 29,
                         36, 10, 13, 10, 42, 150, 31, 12, 8, 66, 52, 5, 48,
                         12, 14, 3, 9, 1, 4, 7, 3, 3, 3, 2, 14, 4, 28, 16, 24, 21,
                         28, 16, 16, 13, 6, 6, 4, 4, 5, 3, 6, 4, 3, 1, 13, 5, 5, 3, 5,
                         1, 1, 1, 22]
    bookLongNamesZh = [u'创世记', u'出埃及记', u'利未记', u'民数记', u'申命记',
                        u'约书亚记', u'士师记', u'路得记', u'撒母耳记上', u'撒母耳记下', u'列王纪上', u'列王纪下', u'历代志上', u'历代志下',
                        u'以斯拉记', u'尼希米记', u'以斯帖记',
                        u'约伯记', u'诗篇', u'箴言', u'传道书', u'雅歌',
                        u'以赛亚书', u'耶利米书', u'耶利米哀歌', u'以西结书', u'但以理书', u'何西阿书', u'约珥书', u'阿摩司书', u'俄巴底亚书',
                        u'约拿书', u'弥迦书', u'那鸿书', u'哈巴谷书', u'西番雅书', u'哈该书', u'撒迦利亚书', u'玛拉基书',
                        u'马太福音', u'马可福音', u'路加福音', u'约翰福音', u'使徒行传', u'罗马书', u'哥林多前书', u'哥林多后书',
                        u'加拉太书', u'以弗所书', u'腓立比书', u'歌罗西书', u'帖撒罗尼迦前书', u'帖撒罗尼迦后书', u'提摩太前书', u'提摩太后书',
                        u'提多书', u'腓利门书', u'希伯来书', u'雅各书', u'彼得前书', u'彼得后书', u'约翰一书', u'约翰二书', u'约翰三书', u'犹大书', u'启示录']
    bookShortNamesZh = ['创','出','利','民','申',
                        '书','士','得','撒上','撒下','王上','王下','代上','代下','拉','尼','斯',
                        '伯','诗','箴','传','歌',
                        '赛','耶','哀','结','但','何','珥','摩','俄','拿','弥','鸿','哈','番','该','亚','玛',
                        '太','可','路','约','徒','罗','林前','林后','加','弗','腓','西','帖前','帖后',
                        '提前','提后','多','门','来','雅','彼前','彼后','约一','约二','约三','犹','启']
    # book IDs in database
    bookIDs = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                110, 120, 130, 140, 150, 160, 190, 220, 230, 240, 250, 260, 290, 300,
                310, 330, 340, 350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450,
                460, 470, 480, 490, 500, 510, 520, 530, 540, 550, 560, 570, 580, 590,
                600, 610, 620, 630, 640, 650, 660, 670, 680, 690, 700, 710, 720, 730]

    # convertCUVT('./bibles/WLUVT/WLUVT.BIB', './bibles/wluvt.wd')
    convertCCB('./bibles/CCB/CCB.BIB', 'C:/TDDownload/CCB/', bookLongNamesZh, bookShortNamesZh, bookChapterCounts)
