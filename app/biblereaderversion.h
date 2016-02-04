#ifndef BIBLEREADERVERSION_H
#define BIBLEREADERVERSION_H


class BibleReaderVersion
{
public:
    BibleReaderVersion(int major, int middle, int minor);

    int getMajor() const;
    void setMajor(int value);

    int getMiddle() const;
    void setMiddle(int value);

    int getMinor() const;
    void setMinor(int value);


private:
    int major;
    int middle;
    int minor;
};

#endif // BIBLEREADERVERSION_H
