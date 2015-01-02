#ifndef VERSEPOS_H
#define VERSEPOS_H

class BibleVersePos
{
public:
    BibleVersePos();
    BibleVersePos(int b, int c, int v);

    int getBookNumber() const;
    void setBookNumber(int value);

    int getChapterNumber() const;
    void setChapterNumber(int value);

    int getVerseNumber() const;
    void setVerseNumber(int value);

private:
    int bookNumber;
    int chapterNumber;
    int verseNumber;
};

#endif // VERSEPOS_H
