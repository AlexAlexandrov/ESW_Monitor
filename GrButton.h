#ifndef GRBUTTON_H
#define GRBUTTON_H
#include <QtGui>
#include "IdGenerator.h"
#include "GlobalDefinition.h"
#include "GrObject.h"


class GrButton : public GrObject
{
private:
    QRectF buttonRect;
    QStringList buttonText;
    QRectF textRect;
    enum { Type = GrButtonType };
    QList<QBrush> brushList;
    QList<QPen> penList;
    QList<QPen> textPenList;
    quint16 curIndexBrush, curIndexPen, curIndexTextPen, curIndexText;
    quint16 UniqueNumber;
    static IDGenerator* idgen;

    bool nextIndexIsValid(quint16 listcount, quint16 *curIndex );
    bool prevIndexIsValid(quint16 *index );
    QFont _font;

public:
    enum{
        state_backgroundIndex = 0,
        state_borderIndex,
        state_textIndex,
        state_textPenIndex
    };


    virtual quint16 uniqueNumber() const { return UniqueNumber; }
    void deleteUniqueNumber() { idgen->DeleteID( UniqueNumber ); }
    GrButton(QGraphicsScene* parent);
    ~GrButton();
    void setRect( QRectF rect );
    void setWidth( qreal w);
    void setHeight( qreal h);

    void setTextColor( QColor col, quint16 index );
    void setBackgroundColor( QColor col, quint16 index   );
    void setBorderColor( QColor col, quint16 index  );
    void setTextColor( QList<QColor> list );
    void setBackgroundColor( QList<QColor> list );
    void setBorderColor( QList<QColor> list );

    void addBorderColor( QColor col );
    void addBackgroundColor( QColor col );
    void addTextColor( QColor col );

    void deleteBorderColor( int index );
    void deleteBackgroundColor( int index );
    void deleteTextColor( int index );

    bool setCurrentIndex( TGrIndexes indexType, quint16 index );
    bool setNextIndex( TGrIndexes indexType );
    bool setPrevIndex( TGrIndexes indexType );

    void setFont( QFont f);
    QFont font();

    QList<QPen> textColor();
    QList<QBrush> backgroundColor();
    QList<QPen> borderColor();
    QStringList text();
    quint16 currentIndex( TGrIndexes indexType );

    void addText( QString text);
    void setText( QString text, quint16 index );
    void setText( QStringList list );
    void deleteText( quint16 index );

    virtual QRectF  boundingRect() const;
    virtual void    paint(QPainter* ppainter, const QStyleOptionGraphicsItem*, QWidget*);
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *pe);
    virtual void    hoverEnterEvent(QGraphicsSceneHoverEvent* pe);
    virtual void    hoverLeaveEvent(QGraphicsSceneHoverEvent *pe);
    virtual int		type() const { return Type;}

    void moveTo( QPointF pos);

    virtual QString name();
    virtual void setNextState();
    virtual void setPreviousState();
    virtual void setCurrentState(quint16 index);
};

#endif // GRBUTTON_H
