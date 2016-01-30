#ifndef GLOBALDEFINITION_H
#define GLOBALDEFINITION_H

#include "MonitorDriver.h"
// screen items types
#include <QtGui>
// screen events types
#define EventSelectionChanged   1777
#define EventDeleteSelection 1778

// text def
#define TEXTHEIGHT 14
#define TEXTWIDTH  7
#define TEXTINDENT 4
// color
#define RGB565CONVERT(red, green, blue) (quint32)(((((red) & 0xF8) >> 3) << 11) | ((((green) & 0xFC) >> 2) << 5) | (((blue) & 0xF8) >> 3))
// monitor states
#define INVISIBLEINDEX 65000
typedef enum{
    brushIndex = 0,
    penIndex,
    textPenIndex,
    textIndex,
    validTextPenIndex,
    invalidTextPenIndex
}TGrIndexes;
typedef struct{
    QString name;
    bool isVisible;
    QList<quint16> indexList;
    quint32 globalStateIndex;
}TItemState;

enum{
    defaultState = 0
};

#define USERTYPE 220

typedef enum{
    GrResizerType = USERTYPE,
    GrBarType,
    GrButtonType,
    GrEnterAreaType,
    GrTextType,
    GrImageType,
    GrShapeType,
    GrAnimationType
}MonitorGrObjects;

#define TIMERINTERVAL 100

#define SetLSB(w)                       ( (w)&0xff )
#define SetMSB(w)                       ( SetLSB((w)>>8) )
#define SetHSB(w)                       ( w >> 16 )
#define SetValMSBLSB(msb,lsb)           ( ((msb)<<8) | (lsb) )
#define SetValHSBMSBLSB( h, m, l)       ( (h)<<16 | (m)<<8 | l )
#define SetHW(dw)                       ( ((dw)>>16) & 0xffff )
#define SetLSBfromHW(dw)                ( ( SetHW(dw) )&0xff )
#define SetLW(dw)                       ( ((dw)&0xffff) )
#endif // ITEMSTYPES_H
