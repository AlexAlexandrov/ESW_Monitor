#ifndef FONTCONVERTER_H
#define FONTCONVERTER_H

#include <QtGui>

#define FONTTOPINDEND 10
#define FONTBOTINDEND 2
#define GLYPHSTARTID  32

typedef struct{
    QVector<quint8> glyphBitMap;
    QString litera;
    int width;
    int height;
    int uniqueCode;
    int glyphArrayOffset;
}GlyphInfo;

typedef struct{
    QString block;
    int blockOffset;
}TStructureInfo;

class FontConverter
{
private:
    QList<GlyphInfo> glyphInfo; // glyphInfo sorted the same as litList
    QStringList textList;
    QStringList litList;
    QVector<quint8> glyphArray, onecharArray;
    QVector<quint8> glyphHeader;
    QList< QVector<quint8> > uniqueCodeText;
    QFont font;
    QImage generateImage( QString onechar );
    int glyphArraySize;
    int glyphHeight;
    int generateOneCharArray(QImage img, int* glyphWidth );
    void getLitsFromTextList();
    QVector<quint8> generateGlyphArray();
    QList< QVector<quint8> > fromTextToUniqueCode();
    QVector<TStructureInfo> structureInfo;
public:
    QVector< TStructureInfo > getInfo(){ return structureInfo; }
    FontConverter( QString _text , QFont _font );
    FontConverter( QStringList _textList, QFont _font );
    FontConverter( QFont _font );
    FontConverter();
    ~FontConverter();
    QFont getFont();
    void setFont( QFont _font );
    void addText( QStringList _textList);
    void addTextString( QString _text);
    QList<GlyphInfo> getGlyphsInfo();
    QVector<quint32> getTextArrays();
    QVector<quint32> getTextArrays(QString text);
    quint16 getGlyphNumber(QString symbol);
    QVector<quint32> getGlyphTable();
    void clear();

    QVector<quint8> fromTextToUniqueCode(QString str);
};



class GlyphTableCreator
{
private:
    typedef struct{
       quint32 offset;
       QFont font;
    }FontStr;
    QList<FontConverter*> fontConverterList;
    QList<FontStr> fonts;
    QVector<TStructureInfo> structureInfo;
public:
    GlyphTableCreator();
    ~GlyphTableCreator();
    QVector<TStructureInfo> getStructureInfo();
    void addText( QStringList text, QFont font );
    void addText( QString text, QFont font );
    QVector<quint32> getGlyphTable();
    quint32 fontOffset( QFont font );
    quint32 getTextArraysSize();
    QVector<quint32> getTextArrays(QString text, QFont font );
    quint16 getGlyphNumber( QString symbol, QFont font );
    void clear();
};

#endif // FONTCONVERTER_H
