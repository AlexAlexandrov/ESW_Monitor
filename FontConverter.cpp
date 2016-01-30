#include "FontConverter.h"
#include "GlobalDefinition.h"

QImage FontConverter::generateImage(QString onechar ){

    QFontMetrics fm( font );
    QSize mapSize = fm.size( Qt::TextSingleLine, onechar );
//    static int counter = 0;

//    QString filename = "sample"+QString().number(counter)+".bmp";
//    QString format = "bmp";
//    counter++;

    QImage img( mapSize.width(), mapSize.height(), QImage::Format_MonoLSB );
    QPainter painter(&img);
    painter.setBrush( QBrush(Qt::white, Qt::SolidPattern ));
    painter.setPen( Qt::white );
    painter.drawRect( 0,0, img.size().width(), img.size().height()  );

    painter.setFont( font );
    painter.setPen( Qt::black );
    painter.drawText( 0, 0, mapSize.width(), mapSize.height(), Qt::AlignHCenter, onechar );
    painter.save();
//    static int num = 0;
//    img.save( QString().number(num)+".bmp" );
//    num++;
    painter.restore();
    if ( glyphHeight == 0 ) glyphHeight = img.size().height();
    return img;
}

int FontConverter::generateOneCharArray(QImage img, int *glyphWidth){
    int cnt = ( img.size().width()) / 8 + 1;
    if ( img.size().width() % 8 == 0 ) cnt = ( img.size().width()) / 8;
    onecharArray.resize( img.size().height() * cnt );
    onecharArray.fill(0, img.size().height() * cnt );
    quint8 byte = 0, offset = 0;
    quint16 off = 0;
    for( int i = 0; i < img.size().height(); i++){
        offset = 0;
        for( int j = 0; j < img.size().width(); j++){
            if ( j != 0 && (j) % 8 == 0 ){
                onecharArray[ i*cnt + offset ] = byte;
                offset++;
                byte = 0;
            }
            if ( QColor(img.pixel(j, i)) == QColor( Qt::black ) ){
                byte |= 1 << ( j - 8*offset);
            }
            if ( (j + 1) == img.size().width() && cnt != 1 ){
                onecharArray[ i*cnt + offset ] = byte;
                byte = 0;
            }
        }
        if ( cnt == 1 ){
            onecharArray[ i ] = byte;
            byte = 0;
        }
    }
    *glyphWidth = img.size().width();
    return onecharArray.count();
}

void FontConverter::getLitsFromTextList(){
    foreach(QString str, textList){
        for(int i = 0; i < str.length(); i++)
            if ( !litList.contains( str.at(i)) ) litList.push_back( str.at(i));
    }
    litList.sort();
}

FontConverter::FontConverter(QString _text, QFont _font ){
    textList.push_back(_text);
    font = _font;
    glyphHeight = 0;
    glyphArraySize = 0;
}

FontConverter::FontConverter(QStringList _textList, QFont _font){
    textList = _textList;
    font = _font;
    glyphHeight = 0;
    glyphArraySize = 0;
}

FontConverter::FontConverter(QFont _font){
    font = _font;
    glyphHeight = 0;
    glyphArraySize = 0;
}

FontConverter::FontConverter(){
    glyphHeight = 0;
    glyphArraySize = 0;
}

FontConverter::~FontConverter(){

}

QFont FontConverter::getFont(){
    return font;
}

void FontConverter::setFont(QFont _font){
    font = _font;
}

void FontConverter::addText(QStringList _textList){
    textList.append( _textList );
}

void FontConverter::addTextString(QString _text){
    if ( !textList.contains( _text ) ) textList<<_text;
}

QVector<quint8> FontConverter::generateGlyphArray(){
    getLitsFromTextList();
    int sizeArr;
    int glyphWidth;
    GlyphInfo giTemp;
//    generateImage( litList.first() );

    glyphArray.clear();
    glyphArray.resize( 0 );

    foreach( QString lit, litList){
        sizeArr = generateOneCharArray( generateImage( lit ), &glyphWidth );
        giTemp.glyphArrayOffset = glyphArray.count();
        glyphArray += onecharArray ;
        giTemp.glyphBitMap = onecharArray;
        giTemp.height = sizeArr;
        giTemp.width = glyphWidth;
        giTemp.litera = lit;
        giTemp.uniqueCode = litList.indexOf( lit ) + GLYPHSTARTID;
        glyphInfo.push_back( giTemp );
    }
    return glyphArray;
}

QList<GlyphInfo> FontConverter::getGlyphsInfo(){
    return glyphInfo;
}

QVector<quint32> FontConverter::getTextArrays(){
    QList< QVector<quint8> > innerText = fromTextToUniqueCode();
    QVector<quint32> res;
    foreach( QVector<quint8> inV, innerText ){
        inV.push_back( 0x00 );
        int i = 0;
        while( i < inV.count()){
            if ( i + 1 == inV.count() ) {
                res.push_back( inV.at(i) );
                i++;
            }
            else if ( i + 2 == inV.count() ){
                res.push_back( SetValMSBLSB(inV.at(i), inV.at( i + 1 )));
                i += 2;
            }
            else{
                quint8 HSB = inV.at( i + 2 );
                quint8 MSB = inV.at( i + 1 );
                quint8 LSB = inV.at( i );
                i += 3;
                res.push_back( SetValHSBMSBLSB(HSB, MSB, LSB));
            }
        }
    }
    return res;
}

QVector<quint32> FontConverter::getTextArrays(QString text){
    QVector<quint8> inV = fromTextToUniqueCode( text );
    QVector<quint32> res;
    inV.push_back( 0x00 );
    int i = 0;
    while( i < inV.count()){
        if ( i + 1 == inV.count() ) {
            res.push_back( SetValHSBMSBLSB(0xFF, 0xFF,inV.at(i)) );
            i++;
        }
        else if ( i + 2 == inV.count() ){
            res.push_back( SetValHSBMSBLSB(0xFF, inV.at(i + 1), inV.at( i )));
            i += 2;
        }
        else{
            quint8 HSB = inV.at( i + 2 );
            quint8 MSB = inV.at( i + 1 );
            quint8 LSB = inV.at( i );
            i += 3;
            res.push_back( SetValHSBMSBLSB(HSB, MSB, LSB));
        }
    }
    return res;
}

quint16 FontConverter::getGlyphNumber(QString symbol){
    return litList.indexOf( symbol ) + GLYPHSTARTID;
}


QList< QVector<quint8> > FontConverter::fromTextToUniqueCode(){
    foreach(QString str, textList){
        QVector<quint8> tempVec;
        for(int i = 0; i < str.length(); i++)
            tempVec.push_back( GLYPHSTARTID + litList.indexOf( str.at( i ) ) );
        uniqueCodeText.push_back( tempVec );
    }
    return uniqueCodeText;
}

QVector<quint8> FontConverter::fromTextToUniqueCode( QString str ){
    QVector<quint8> tempVec;
    for(int i = 0; i < str.length(); i++)
        tempVec.push_back( GLYPHSTARTID + litList.indexOf( str.at( i ) ) );
    return tempVec;
}

#define AddToStrInfo( str, off ) tempInfo.block = str; tempInfo.blockOffset = off; structureInfo.push_back( tempInfo );
QVector<quint32> FontConverter::getGlyphTable(){
    generateGlyphArray();
    QVector<quint32> glyphTable;

    TStructureInfo tempInfo;
    glyphTable.clear();
    if (glyphInfo.isEmpty()) return glyphTable;
    // Table header block
    /*  offset
     *   0x00   Reserved     Orient     Bits per Pixel Reserved    Extended  Glyph Font ID
     *   0x02                              First Character ID
     *   0x04                              Last Character ID
     *   0x06                                    Height
     */

    AddToStrInfo( "Font header", 0 );
    glyphTable.push_back( 0 );
    glyphTable.push_back( GLYPHSTARTID );
    glyphTable.push_back( GLYPHSTARTID + litList.count() - 1 );
    glyphTable.push_back( glyphHeight );
    // Glyph Entry block
    //    Size of the Glyph Entry (Four bytes for Normal Glyph or 12 bytes for Extended Glyph)
    /*    1st word( width and offset )
     *      1 byte - width ( 0 - 7 )
     *      2 byte - LSB Offset ( 8 - 15 )
     *    2nd word ( MSB offset, HSB offset )
     *      1 byte - MSB offset ( 0 - 7 )
     *      2 byte - HSB offset ( 8 - 15 )
     */
    quint16 GlyphEntrySize = litList.count() * 2; // They calculate offset in bytes neither words
    quint8  HeaderSize = 4;
    quint32 glyphOffset = HeaderSize + GlyphEntrySize;
    AddToStrInfo( "Glyph entry table ", HeaderSize );
    foreach( GlyphInfo info, glyphInfo ){
        AddToStrInfo( "Bitmap of litera " + info.litera, glyphOffset );
        if ( glyphOffset*2 <= 0xFF ){
            glyphTable.push_back( SetValMSBLSB( glyphOffset*2 ,info.width) );
            glyphTable.push_back( 0 );
        }
        else{
            glyphTable.push_back( SetValMSBLSB( SetLSB(glyphOffset*2), info.width));
            glyphTable.push_back( SetValMSBLSB( SetHSB(glyphOffset*2), SetMSB(glyphOffset*2)));
        }
        glyphOffset += info.height/3;
        if ( info.height % 3 ) glyphOffset++;
    }
    // Characters bitmap block
    /*
     * We has offset in bytes to each character bitmap
     * So we could write bytes from glyphArray to HSB(16-24) MSB(8-16) and LSB( 0 - 8 ).
     */
    foreach( GlyphInfo info, glyphInfo){
        int i = 0;
        while( i < info.glyphBitMap.count()){
            if ( i + 1 == info.glyphBitMap.count() ) {
                glyphTable.push_back( SetValHSBMSBLSB(0xFF, 0xFF, info.glyphBitMap.at(i)) );
                i++;
            }
            else if ( i + 2 == info.glyphBitMap.count() ){
                glyphTable.push_back( SetValHSBMSBLSB(0xFF, info.glyphBitMap.at(i+1), info.glyphBitMap.at( i )));
                i += 2;
            }
            else{
                quint8 HSB = info.glyphBitMap.at( i + 2 );
                quint8 MSB = info.glyphBitMap.at( i + 1 );
                quint8 LSB = info.glyphBitMap.at( i );
                i += 3;
                glyphTable.push_back( SetValHSBMSBLSB(HSB, MSB, LSB));
            }
        }
    }
    return glyphTable;
}

void FontConverter::clear(){
    glyphInfo.clear();
    textList.clear();
    litList.clear();
    glyphArray.clear();
    onecharArray.clear();
    uniqueCodeText.clear();
}


GlyphTableCreator::GlyphTableCreator(){
}

GlyphTableCreator::~GlyphTableCreator(){
}

QVector<TStructureInfo> GlyphTableCreator::getStructureInfo(){
    return structureInfo;
}

void GlyphTableCreator::addText(QStringList text, QFont font){
    bool finded = false;
    foreach( FontConverter* fc, fontConverterList ){
        if ( fc->getFont() == font ){
            finded = true;
            fc->addText( text );
        }
    }
    if ( !finded ){
        FontConverter* conv = new FontConverter();
        conv->setFont( font );
        conv->addText( text );
        fontConverterList.push_back( conv );
    }
}

void GlyphTableCreator::addText(QString text, QFont font){
    bool finded = false;
    foreach( FontConverter* fc, fontConverterList ){
        if ( fc->getFont() == font ){
            finded = true;
            fc->addTextString( text );
        }
    }
    if ( !finded ){
        FontConverter* conv = new FontConverter();
        conv->setFont( font );
        conv->addTextString( text );
        fontConverterList.push_back( conv );
    }
}

QVector<quint32> GlyphTableCreator::getGlyphTable(){
    QVector<quint32> glyphTable;
    quint32 offset = 0;
    foreach( FontConverter* fc, fontConverterList ){
        QVector<quint32> tempVec = fc->getGlyphTable();
        QVector<TStructureInfo> tempInfo = fc->getInfo();
        for(int i = 0; i < tempInfo.count(); i++){
            tempInfo[i].blockOffset += offset;
        }
        structureInfo<<tempInfo;
        glyphTable += tempVec ;
        FontStr temp;
        temp.font = fc->getFont();
        temp.offset = offset;
        fonts.push_back( temp );
        offset += tempVec.count();
    }
    return glyphTable;
}

quint32 GlyphTableCreator::fontOffset(QFont font){
    foreach(FontStr str, fonts){
        if ( str.font == font ) return str.offset;
    }
}

QVector<quint32> GlyphTableCreator::getTextArrays(QString text, QFont font){
    foreach( FontConverter* fc, fontConverterList ){
        if ( fc->getFont() == font ){
            return fc->getTextArrays( text );
        }
    }
}

quint16 GlyphTableCreator::getGlyphNumber(QString symbol, QFont font){
    foreach( FontConverter* fc, fontConverterList ){
        if ( fc->getFont() == font ){
            return fc->getGlyphNumber( symbol );
        }
    }
}

void GlyphTableCreator::clear(){
    foreach( FontConverter* fc, fontConverterList ){
        delete fc;
    }
    fonts.clear();
}


quint32 GlyphTableCreator::getTextArraysSize(){
    quint32 cnt = 0;
    foreach( FontConverter* fc, fontConverterList ){
        cnt += fc->getTextArrays().count();
    }
    return cnt;
}
