#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include <QtGui>
#include "GlobalDefinition.h"

#define SetByteFrom4Bits( ms, ls ) ( ms << 4 | ls )

class ImageConverter
{
private:
    typedef struct{
      QImage   bitmap;
      quint32   offset;
      bool      useRLE;
    }ImageStruct;
    QList<ImageStruct> imageList;
    QVector<quint32> generateOneImageTable(ImageStruct imgStr );
    QVector<quint32> createPalette(QImage img);
    inline QVector<quint32> from8to24( QVector<quint8> tempVec );
    inline QVector<quint32> fromNbitsTo24bits( QVector<quint8> tempVec, int Nbits ); // N <= 8
    inline QVector<quint8>  fromColorToIndex(QImage image, QVector<quint32> palette );
public:
    ImageConverter();
    void    addImage(QImage img, bool rle = false );
    QVector<quint32> getImageTables();
    quint32 getImageOffset( QImage img );
    void addImage(QList<QImage> imgList);
};

#endif // IMAGECONVERTER_H
