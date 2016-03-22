#ifndef BIBLEVERSEXREF_H
#define BIBLEVERSEXREF_H

#include <QString>

class BibleVerseXref
{
public:
    BibleVerseXref();
    //friend bool operator == (BibleVerseXref &x1, BibleVerseXref &x2);

    int getFbi() const;
    void setFbi(int value);

    int getFci() const;
    void setFci(int value);

    int getFvi() const;
    void setFvi(int value);

    int getFspan() const;
    void setFspan(int value);

    int getTbi() const;
    void setTbi(int value);

    int getTci() const;
    void setTci(int value);

    int getTvi() const;
    void setTvi(int value);

    int getTspan() const;
    void setTspan(int value);

    QString getTbname() const;
    void setTbname(const QString &value);

private:
    int fbi;
    int fci;
    int fvi;
    int fspan;

    QString tbname;
    int tbi;
    int tci;
    int tvi;
    int tspan;
};

#endif // BIBLEVERSEXREF_H
