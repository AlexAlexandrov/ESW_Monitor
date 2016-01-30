#ifndef GRBAR_H
#define GRBAR_H

#include <QtGui>

#include "IdGenerator.h"
#include "GlobalDefinition.h"
#include "GrObject.h"

class GrBar : public GrObject
{
private:
//    static QList<QBrush> brushList;
//    static QList<QPen> penList;
    QList<QBrush> brushList;
    QList<QPen> penList;
    quint16 curIndexBrush, curIndexPen;
    enum { Type = GrBarType };
    quint16 UniqueNumber;
    static IDGenerator* idgen;
    bool nextIndexIsValid(quint16 listcount, quint16 *curIndex);
    bool prevIndexIsValid(quint16 *curIndex);
//    QList<TItemState> stateList;
//    quint16 curState;
    QRectF barRect;
    void moveTo(QPointF);
public:
    enum{
        state_backgroundIndex = 0,
        state_borderIndex
    };
//    TItemState currentState(){ return stateList[curState]; }
//    QList<TItemState> states(){ return stateList; }
//    void addState( TItemState state ) { stateList.push_back( state ); }
//    void renameState( QString name ){ stateList[curState].name = name; }
//    void replaceCurState( TItemState state ){ if ( curState) stateList.replace( curState, state );}
//    void deleteState( quint16 index ) { stateList.removeAt( index ); }
//    quint16 currentStateIndex(){ return curState;}
    virtual void setNextState();
    virtual void setPreviousState();
    virtual void setCurrentState( quint16 index);
    virtual quint16 uniqueNumber() const { return UniqueNumber; }
    void deleteUniqueNumber() { idgen->DeleteID( UniqueNumber ); }
    virtual QString name();
    GrBar(QGraphicsScene* _scene);
    ~GrBar();
//    virtual QRectF  boundingRect() const;
    virtual void    paint(QPainter* ppainter, const QStyleOptionGraphicsItem*, QWidget*);
    virtual QRectF  boundingRect() const;
    virtual void    hoverEnterEvent(QGraphicsSceneHoverEvent* pe);
    virtual void    hoverLeaveEvent(QGraphicsSceneHoverEvent *pe);
    virtual int		type() const { return Type;}
    virtual void    mousePressEvent(QGraphicsSceneMouseEvent *pe);
    virtual void    mouseReleaseEvent(QGraphicsSceneMouseEvent *pe);
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *pe);
    void    setRect(const QRectF &_rect);
    void    setRect(qreal x, qreal y, qreal w, qreal h);
    QRectF  rect(){ return barRect; }

    void addBackgroundColor( QColor col );
    void addBorderColor( QColor col );
    void setBackgroundColor( QColor col, quint16 index );
    void setBorderColor( QColor col, quint16 index );
    void setBackgroundColor( QList<QColor> list );
    void setBorderColor( QList<QColor> list );

    void deleteBackgroundColor( quint16 index );
    void deleteBorderColor( quint16 index );
    QList<QBrush> backgroundColor();
    QList<QPen> borderColor();

    bool setCurrentIndex( TGrIndexes indexType, quint16 index );
    bool setNextIndex( TGrIndexes indexType );
    bool setPrevIndex( TGrIndexes indexType );

    quint16 currentIndex( TGrIndexes indexType );
};

#endif // GRBAR_H
