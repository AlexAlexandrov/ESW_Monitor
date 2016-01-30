#ifndef GRENTERAREA_H
#define GRENTERAREA_H

#include <QtGui>

#include "IdGenerator.h"
#include "GlobalDefinition.h"
#include "GrObject.h"
typedef struct{
    quint16 min;
    quint16 max;
}TEnterAreaValidator;

class GrEnterArea : public GrObject
{
private:
    QList<QBrush> brushList;
    QList<QPen>   penList;
    QRectF rect, textRect;
    QString areaText;
    bool   isTextValid;
    QList<QPen>   validTextPenList;
    QList<QPen>   invalidTextPenList;
    TEnterAreaValidator validator;
    enum { Type = GrEnterAreaType };
    QFont eaFont;

    void adjustFontSize(QFont &font, int width, int height, const QString &text );

    quint16 UniqueNumber;
    quint16 curIndexBrush, curIndexPen, curIndexValidPen, curIndexInvalidPen;
    static IDGenerator* idgen;

    bool nextIndexIsValid(quint16 listcount, quint16 *curIndex);
    bool prevIndexIsValid(quint16 *curIndex);
//    QList<TItemState> stateList;
//    quint16 curState;
public:    enum{
        state_backgroundIndex = 0,
        state_borderIndex,
        state_validColorIndex,
        state_invalidColorIndex
    };

//    TItemState currentState(){ return stateList[curState]; }
//    QList<TItemState> states(){ return stateList; }
//        void addState( TItemState state ) { stateList.push_back( state ); }
//    void renameState( QString name ){ stateList[curState].name = name; }
//    void replaceCurState( TItemState state ){if ( curState)  stateList.replace( curState, state );}
//    void deleteState( quint16 index ) { stateList.removeAt( index ); }
//    void setNextState();
//    void setPreviousState();
//    void setCurrentState( quint16 index);
//    quint16 currentStateIndex(){ return curState;}

    virtual void setNextState();
    virtual void setPreviousState();
    virtual void setCurrentState( quint16 index);
    virtual quint16 uniqueNumber() const { return UniqueNumber; }
    void deleteUniqueNumber() { idgen->DeleteID( UniqueNumber ); }
    void setRect( QRectF rect );
    void setRect( qreal x, qreal y, qreal width, qreal height);

    void setRectWidth( qreal width);
    void setRectHeight( qreal height);
    QRectF getRect();
    QString text( );
    bool setText( QString t );
    void setTextValidator( TEnterAreaValidator val );
    void setTextValidator( quint16 min, quint16 max );

    TEnterAreaValidator getTextValidator();

    void addValidTextColor( QColor col );
    void addInvalidTextColor( QColor col );
    void setValidTextColor( QColor col, quint16 index );
    void setInvalidTextColor( QColor col, quint16 index );
    void deleteValidTextColor( quint16 index );
    void deleteInvalidTextColor( quint16 index );

    QList<QPen> validTextColor();
    QList<QPen> invalidTextColor();

    void addBackgroundColor(QColor col);
    void setBackgroundColor( QColor col, quint16 index  );
    void deleteBackgroundColor( quint16 index );
    QList<QBrush> backgroundColor();

    QFont font();
    void setFont(QFont _font);

    void addBorderColor( QColor col );
    void setBorderColor( QColor col, quint16 index  );
    void deleteBorderColor( quint16 index );
    QList<QPen> borderColor( );

    void setBorderColor(QList<QColor> list);
    void setBackgroundColor(QList<QColor> list);
    void setInvalidTextColor(QList<QColor> list);
    void setValidTextColor(QList<QColor> list);

    quint16 currentIndex( TGrIndexes indexType );
    void setCurrentIndex( TGrIndexes indexType, quint16 index );
    bool setNextIndex( TGrIndexes indexType );
    bool setPrevIndex( TGrIndexes indexType );


    GrEnterArea(QGraphicsScene* _scene);
    ~GrEnterArea();
    virtual QRectF  boundingRect() const;
    virtual void    paint(QPainter* ppainter, const QStyleOptionGraphicsItem*, QWidget*);
    virtual void    hoverEnterEvent(QGraphicsSceneHoverEvent* pe);
    virtual void    hoverLeaveEvent(QGraphicsSceneHoverEvent *pe);
    virtual int		type() const { return Type;}
    virtual void    mousePressEvent(QGraphicsSceneMouseEvent *pe);
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *pe);
    virtual void    mouseReleaseEvent(QGraphicsSceneMouseEvent *pe);
    virtual QString name();


    void moveTo(QPointF pos);
};

#endif // GRENTERAREA_H
