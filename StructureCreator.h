#ifndef STRUCTURECREATOR_H
#define STRUCTURECREATOR_H

#include <QtGui>
#include "MonitorScreen.h"
#include "MonitorDriver.h"
#include "FontConverter.h"
#include "ImageConverter.h"


#define TEXTPROPCNT stTxt_end
#define EDBOXPROPCNT edbox_end
#define SHAPEPROPCNT shape_end
#define IMGPROPCNT img_end
//128k
class StructureCreator
{
private:
    QVector< TStructureInfo > mainStrInfo;
    QVector< TStructureInfo > fontStrInfo;
    MonitorScreen* currentScreen;
    quint32* structure;
    quint32 originAddress, ramOriginAddress;
    quint16 strSize, textOffset, fontOffset, textArraySize, imagesOffset;
    QList<MonitorScreen*> scrList;    
    QList< QVector<quint8> > ourText;
    QVector<quint32> glyphTable, imagesTable;
    GlyphTableCreator* fontConverter;
    ImageConverter* imageConverter;
    void writeToPtrMOT(GrObject *grObj, quint32* pointer );
    void writeDefaultProperties(GrObject *obj, quint32 *pw );
    void writeStateProperties(GrObject *obj, quint32 *pw , quint16 index, TItemState state);
    quint16 getObjectPropCount(GrObject *obj);
    void resizeStructure( QList<MonitorScreen*> screenList );
    void generateInnerStructure();
protected:
    StructureCreator();
     ~StructureCreator();
public:
    quint32 textAddress();
    quint32 fontAddress();
    static StructureCreator* getCreator();
    quint32 *calcControllerStructure(QList<MonitorScreen*> screenList, quint32 origin, quint32 ramOrigin, quint16 *size );
    void setOrigin(quint32 origin);
};



#endif // STRUCTURECREATOR_H
