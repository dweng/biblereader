#ifndef BIBLEREADERHTMLDELEGATE_H
#define BIBLEREADERHTMLDELEGATE_H

#include <QStyledItemDelegate>
#include <QTextDocument>

class BibleReaderHTMLDelegate : public QStyledItemDelegate
{
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};


#endif // BIBLEREADERHTMLDELEGATE_H
