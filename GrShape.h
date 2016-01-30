#ifndef GRSHAPE_H
#define GRSHAPE_H
#include <QtGui>
#include "GlobalDefinition.h"
#include "IdGenerator.h"
#include "GrObject.h"
typedef enum{
    shape_rect = 0,
    shape_triangle,
    shape_ellipse
}TShapeTypes;

class GrShape : public GrObject
{
private:
    enum { Type = GrShapeType };
    QString _name;
    QList<QBrush> brushList;
    QList<QPen> penList;
    quint16 curIndexBrush, curIndexPen;
    QPainterPath _path;
    qreal _width;
    qreal _height;
    qreal _x;
    qreal _y;
    TShapeTypes _shapeType;
    void calcNewPath( qreal x, qreal y, qreal w, qreal h );
    quint16 UniqueNumber;
    static IDGenerator* idgen;

    bool nextIndexIsValid(quint16 listcount, quint16 *curIndex);
    bool prevIndexIsValid(quint16 *curIndex);
//    QList<TItemState> stateList;
//    quint16 curState;
public:
    enum{
        state_backgroundIndex = 0,
        state_borderIndex
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
    virtual quint16 uniqueNumber() const { return UniqueNumber; }
    void deleteUniqueNumber() { idgen->DeleteID( UniqueNumber ); }

    GrShape( QGraphicsItem* _parent = 0, QGraphicsScene* _scene = 0);
    void setObjectShape( TShapeTypes t, qreal x, qreal y, qreal w, qreal h );
    void setObjectShape( TShapeTypes t);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape () const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual int type() const { return Type; }

    QPainterPath path(){ return _path;}
    qreal width(){ return _width;}
    qreal height(){ return _height;}

    QList<QBrush> brush(){ return brushList;}
    QList<QPen> pen(){ return penList;}
    TShapeTypes shapeType() { return _shapeType; }
    virtual QString name() { return _name;}

    void setCurrentIndex( TGrIndexes indexType, quint16 index );
    quint16 currentIndex( TGrIndexes indexType );
    bool setNextIndex( TGrIndexes indexType );
    bool setPrevIndex( TGrIndexes indexType );


    void setPath( QPainterPath p ) {prepareGeometryChange(); _path = p; update();}

    void setWidth( qreal w );
    void setHeight( qreal h );
    void setRect( QRectF rect );
    void setRect( qreal x, qreal y, qreal w, qreal h);
    void addBackgroundColor( QColor col );
    void addBorderColor(QColor col );
    void deleteBackgroundColor( quint16 index );
    void deleteBorderColor( quint16 index );
    void setBackgroundColor(QColor br, quint16 index ) { if (index < brushList.count()) brushList[index].setColor( br );}
    void setBorderColor( QColor p, quint16 index  ){ if (index < penList.count())  penList[index].setColor( p );}
    void setBackgroundColor( QList<QColor> list );
    void setBorderColor( QList<QColor> list );
    void setName( QString n ){ _name =  n;}

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *pe);
    void moveTo(QPointF pos);

    virtual void setNextState();
    virtual void setPreviousState();
    virtual void setCurrentState(quint16 index);

};



#endif // GRSHAPE_H
