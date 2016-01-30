#ifndef GRIMAGE_H
#define GRIMAGE_H

#include <QtGui>
#include "GlobalDefinition.h"
#include "IdGenerator.h"
#include "GrObject.h"
class GrImage : public GrObject
{

private:
    static IDGenerator* idgen;
    quint16 UniqueNumber;
    enum { Type = GrImageType };
//    QImage _img;
    quint16 currentImage;
    QList<QImage> imgList;
    QRectF _rect;
    QStringList imagePathList;
//    QList<TItemState> stateList;
//    quint16 curState;
public:
    enum{
        state_imageIndex = 0
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
    GrImage(QGraphicsScene* _scene);
    ~GrImage();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual int	type() const { return Type;}
    virtual QRectF boundingRect() const;

    void addImage( QString imagePath );
    void setImage( int index, QString imagePath );
    void setImage( QStringList imgPaths);

    void deleteImage( quint16 index );
    void deleteCurrentImage();
    void deleteImages();

    bool setCurrentIndex( quint16 index );
    bool setNextImage();
    bool setPrevImage();

    void setRect(QPointF p, QSizeF s );
    void setRect( QRectF r );
    void setRect( qreal x, qreal y, qreal w, qreal h);
    void setWidth( qreal w );
    void setHeight( qreal h );

    QImage image(quint16 index) const;
    QList<QImage> images() const;

    quint16 currentIndex() const;

    QRectF rect() const;

    QString imagePath( quint16 index ) const;
    QStringList imagesPath() const;

    virtual QString name();

    virtual quint16 uniqueNumber() const { return UniqueNumber; }
    void deleteUniqueNumber() { idgen->DeleteID( UniqueNumber ); }
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *pe);
    void moveTo(QPointF pos);

    virtual void setNextState();
    virtual void setPreviousState();
    virtual void setCurrentState(quint16 index);

};

#endif // GRIMAGE_H
