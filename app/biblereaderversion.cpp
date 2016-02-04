#include "biblereaderversion.h"

BibleReaderVersion::BibleReaderVersion(int major, int middle, int minor):
    major(major), middle(middle), minor(minor)
{
}

int BibleReaderVersion::getMajor() const
{
    return major;
}

void BibleReaderVersion::setMajor(int value)
{
    major = value;
}

int BibleReaderVersion::getMiddle() const
{
    return middle;
}

void BibleReaderVersion::setMiddle(int value)
{
    middle = value;
}

int BibleReaderVersion::getMinor() const
{
    return minor;
}

void BibleReaderVersion::setMinor(int value)
{
    minor = value;
}

