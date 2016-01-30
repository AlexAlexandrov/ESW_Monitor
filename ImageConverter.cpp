#include "ImageConverter.h"

QVector<quint32> ImageConverter::generateOneImageTable( ImageStruct imgStr ){
    QVector<quint32> imageTable;
    int colorDepth = 16;
    QImage image = imgStr.bitmap.convertToFormat( QImage::Format_RGB16 );
//    image.save("afterConvertToFormat.bmp");
    QVector<quint32> palette = createPalette( image );
    if ( palette.count() < 256 )    colorDepth = 8;
    if ( palette.count() < 17 )     colorDepth = 4;
    if ( palette.count() < 5 )      colorDepth = 2;
    if ( palette.count() < 3 )      colorDepth = 1;



//    colorDepth = 16;
/****************************** Header ****************************************
 *  0 RLE ( 0 not used / 1 used )
 *  1 ColorDepth
 *  2 Image height
 *  3 Image width
 *  4 Pallete size( depends on colors List and on colorDepth )
 ****************************** End of Header ********************************/
    imageTable.push_back( imgStr.useRLE );
    imageTable.push_back( colorDepth );
    imageTable.push_back( image.height() );
    imageTable.push_back( image.width() );
    if ( colorDepth != 16 ){
        imageTable.push_back( palette.count() );
        imageTable += palette;
    }
    else imageTable.push_back( 0 );
    if ( !imgStr.useRLE ){
        if ( colorDepth == 16 ){
            QVector<quint8> tempVec;
            for( int i = 0; i < image.height(); i++)
                for( int j = 0; j < image.width(); j++){
                    quint16 word = RGB565CONVERT(QColor(image.pixel(j,i)).red(),QColor(image.pixel(j,i)).green(), QColor(image.pixel(j,i)).blue());
                    tempVec.push_back( SetLSB(word) );
                    tempVec.push_back( SetMSB(word) );
                }
            imageTable += from8to24( tempVec );
        }
        else imageTable += fromNbitsTo24bits( fromColorToIndex( image, palette ), colorDepth );
    }
    else{
        //TODO RLE converter
    }
    return imageTable;
}

QVector<quint32> ImageConverter::createPalette( QImage img ){
    QVector<quint32> palette;
    QVector<QRgb> rgbVec = img.colorTable();
    for(int i = 0; i < img.width(); i++){
        for(int j = 0; j < img.height(); j++ ){
            QColor col(img.pixel(i,j));
            if ( !palette.contains( RGB565CONVERT(col.red(),col.green(),col.blue() )) )
                palette.push_back( RGB565CONVERT(col.red(),col.green(),col.blue()) );
        }
    }
    return palette;
}

QVector<quint32> ImageConverter::from8to24(QVector<quint8> tempVec){
    QVector<quint32> res;
    int i = 0;
    while( i < tempVec.count()){
        if ( i + 1 == tempVec.count() ) {
            res.push_back( tempVec.at(i) );
            i++;
        }
        else if ( i + 2 == tempVec.count() ){
            res.push_back( SetValMSBLSB(tempVec.at(i), tempVec.at( i + 1 )));
            i += 2;
        }
        else{
            quint8 HSB = tempVec.at( i + 2 );
            quint8 MSB = tempVec.at( i + 1 );
            quint8 LSB = tempVec.at( i );
            i += 3;
            res.push_back( SetValHSBMSBLSB(HSB, MSB, LSB));
        }
    }
    return res;
}

QVector<quint32> ImageConverter::fromNbitsTo24bits( QVector<quint8> tempVec, int Nbits ){
    QVector<quint32> res;
    int cnt = 24 / Nbits;                  // count of blocks with Nbits size in 24bits program word
    int mod = tempVec.count()%cnt;      // count of blocks in not full word( last word)
    int div = tempVec.count() / cnt;                  // count of full words
    int offset = 0;
    int data = 0;
    // start circle with sequence for full words
    for(int i = 0; i < div; i++){
        for( int j = 0; j < cnt; j++) data |= tempVec.at( j + offset )<<( Nbits * j );
        res.push_back( data );
        offset += cnt;
        data = 0;
    }
    // start circle for sequence for not full word
    for( int j = 0; j < mod; j++) data |= tempVec.at( j + offset )<<( Nbits * j );
    res.push_back( data );
    return res;
}

QVector<quint8> ImageConverter::fromColorToIndex(QImage image, QVector<quint32> palette){
    QVector<quint8> tempVec;
    for( int i = 0; i < image.height(); i++)
        for( int j = 0; j < image.width(); j++){
            quint16 pColor = RGB565CONVERT(QColor(image.pixel(j,i)).red(),QColor(image.pixel(j,i)).green(), QColor(image.pixel(j,i)).blue());
            tempVec.push_back( palette.indexOf( pColor ));
        }

    return tempVec;
}

ImageConverter::ImageConverter(){
}

void ImageConverter::addImage(QImage img, bool rle ){
    bool has = false;
    foreach( ImageStruct str, imageList ){
        if ( str.bitmap == img ) has = true;
    }
    if ( has ) return;
    ImageStruct temp;
    temp.bitmap = img;
    temp.useRLE = rle;
    imageList.push_back( temp );
}

void ImageConverter::addImage( QList<QImage> imgList ){
    if ( imgList.isEmpty() ) return;
    foreach( QImage img, imgList){
        addImage( img );
    }
}

QVector<quint32> ImageConverter::getImageTables(){
    QVector<quint32> res;
    res.clear();
    quint16 offset = 0;
    for(int i = 0; i < imageList.count(); i++){
        QVector<quint32> tempVec = generateOneImageTable( imageList.at(i) );
        imageList[i].offset = offset;
        offset += tempVec.count();
        res += tempVec;
    }
    return res;
}

quint32 ImageConverter::getImageOffset(QImage img){
    foreach( ImageStruct imgStr, imageList ){
        if ( img == imgStr.bitmap ) return imgStr.offset;
    }
}
