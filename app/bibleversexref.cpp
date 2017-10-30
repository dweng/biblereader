#include "bibleversexref.h"

BibleVerseXref::BibleVerseXref()
{
    fbi = 0;
    fci = 0;
    fvi = 0;
    fspan = 0;

    tbi = 0;
    tci = 0;
    tvi = 0;
    tspan = 0;
    tbname = "";
}
/*
bool BibleVerseXref::operator ==(BibleVerseXref &x1, BibleVerseXref &x2)
{
    if (x1.fbi == x2.fbi &&
            x1.fci == x2.fci &&
            x1.fvi == x2.fvi &&
            x1.fspan == x2.fspan &&
            x1.tbi == x2.tbi &&
            x1.tci == x2.tci &&
            x1.tvi == x2.tvi &&
            x1.tspan == x2.tspan) {
        return true;
    } else {
        return false;
    }
}
*/

int BibleVerseXref::getFbi() const
{
    return fbi;
}

void BibleVerseXref::setFbi(int value)
{
    fbi = value;
}

int BibleVerseXref::getFci() const
{
    return fci;
}

void BibleVerseXref::setFci(int value)
{
    fci = value;
}

int BibleVerseXref::getFvi() const
{
    return fvi;
}

void BibleVerseXref::setFvi(int value)
{
    fvi = value;
}

int BibleVerseXref::getFspan() const
{
    return fspan;
}

void BibleVerseXref::setFspan(int value)
{
    fspan = value;
}

int BibleVerseXref::getTbi() const
{
    return tbi;
}

void BibleVerseXref::setTbi(int value)
{
    tbi = value;
}

int BibleVerseXref::getTci() const
{
    return tci;
}

void BibleVerseXref::setTci(int value)
{
    tci = value;
}

int BibleVerseXref::getTvi() const
{
    return tvi;
}

void BibleVerseXref::setTvi(int value)
{
    tvi = value;
}

int BibleVerseXref::getTspan() const
{
    return tspan;
}

void BibleVerseXref::setTspan(int value)
{
    tspan = value;
}

QString BibleVerseXref::getTbname() const
{
    return tbname;
}

void BibleVerseXref::setTbname(const QString &value)
{
    tbname = value;
}

