#ifndef BRUPDATERDATA_H
#define BRUPDATERDATA_H

#include <QString>

class BRUpdaterData
{
public:
    BRUpdaterData();
    ~BRUpdaterData();

    int getMajorVersion() const;
    void setMajorVersion(int value);

    int getMinorVersion() const;
    void setMinorVersion(int value);

    int getTailVersion() const;
    void setTailVersion(int value);

    QString getNewVersionDownloadUrl() const;
    void setNewVersionDownloadUrl(const QString &value);

private:
    int majorVersion;
    int minorVersion;
    int tailVersion;

    QString newVersionDownloadUrl;
};

#endif // BRUPDATERDATA_H
