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

#include "biblereaderapp.h"

int main(int argc, char *argv[]) {
    int ret = -1;
    do {
        BibleReaderApp a(argc, argv);
        ret = a.exec();
    } while(ret == 773);

    return ret;
}
