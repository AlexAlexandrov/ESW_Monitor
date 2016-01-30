#ifndef GRTEXT_H
#define GRTEXT_H

#include <QtGui>

#include "IdGenerator.h"
#include "GlobalDefinition.h"

#include "GrObject.h"
class GrText : public GrObject
{
private:
    QStringList textList;
    QList<QPen> penList;
    QList<QBrush> brushList;
    enum { Type = GrTextType };
    quint16 curIndexText, curIndexPen;
    quint16 UniqueNumber;
    static IDGenerator* idgen;
    bool prevIndexIsValid(quint16 *curIndex);
    bool nextIndexIsValid(quint16 listcount, quint16 *curIndex);
    QRectF textRect;
    QFont textFont;
    int textAligment;
//    QList<TItemState> stateList;
//    quint16 curState;
public:
    enum{
        state_textIndex = 0,
        state_textColorIndex
    };
//    TItemState currentState(){ return stateList[curState]; }
//    QList<TItemState> states(){ return stateList; }
//    void addState( TItemState state ) { stateList.push_back( state ); }
//    void renameState( QString name ){ stateList[curState].name = name; }
//    void replaceCurState( TItemState state ){if ( curState)  stateList.replace( curState, state );}
//    void deleteState( quint16 index ) { stateList.removeAt( index ); }
    virtual void setNextState();
    virtual void setPreviousState();
    virtual void setCurrentState( quint16 index);
    quint16 currentStateIndex(){ return curState;}

    virtual quint16 uniqueNumber() const { return UniqueNumber; }
    void deleteUniqueNumber() { idgen->DeleteID( UniqueNumber ); }
    const QStringList text();
    const QString text( int index );
    const QString currentText();
    quint16 currentIndex(TGrIndexes indexType);

    void addText( QStringList _text );
    void addText( QString _str );
    void setText(QString _str, int index);
    void setText( QStringList list );
    void deleteText( quint16 index );
    bool setCurrentIndex( TGrIndexes indexType , quint16 index );
    bool setNextIndex( TGrIndexes indexType );
    bool setPrevIndex( TGrIndexes indexType );
    void setFont( QFont f );
    QFont font();

    void setAligment( int align );
    int align();
//    void addBackgroundColor( QColor col );
//    void setBackgroundColor( QColor col, quint16 index );
//    void deleteBackgroundColor( quint16 index );

    void addTextColor( QColor col );
    void setTextColor( QColor col, quint16 index );
    void setTextColor(QList<QColor> list);
    void deleteTextColor( quint16 index );
    QList<QPen> textColor();

    void setRect( QRectF rect );
    void setRect( qreal x, qreal y, qreal width, qreal height);

    void setRectWidth( qreal width);
    void setRectHeight( qreal height);
    QRectF getRect();

    GrText(QGraphicsScene* _scene = 0);
    ~GrText();
    virtual int		type() const { return Type;}
    virtual QString name();
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *pe);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void moveTo(QPointF pos);
};

#endif // GRTEXT_H
