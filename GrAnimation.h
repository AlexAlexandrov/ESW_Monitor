#ifndef GRANIMATION_H
#define GRANIMATION_H
#include "GrObject.h"
#include "IdGenerator.h"


typedef enum{
    anime_infinity = 0,
    anime_single
}TAnimeType;

class GrAnimation : public GrObject
{
private:
    static IDGenerator* idgen;
    quint16 uNumber;
    QRectF Rect;
    QList<QImage> frameList;
    QStringList   framePath;
    quint16 curFrame;
    quint32 freq;
    TAnimeType AnimeType;
    bool active;
    quint32 currentTime;
    enum { Type = GrAnimationType };
public:
    enum{
        state_typeIndex = 0
    };

    GrAnimation(QGraphicsScene *_scene);
    ~GrAnimation();
    virtual QString name();
    virtual quint16 uniqueNumber() const;
    virtual QRectF boundingRect() const;
    virtual void setNextState();
    virtual void setPreviousState();
    virtual void setCurrentState( quint16 index);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual int type() const { return Type; }
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *pe);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *pe);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *pe);

    void notifyTimer( quint16 interval );

    void setFrequency( int msec );
    int frequency();

    TAnimeType repeatType();
    void setRepeatType( TAnimeType r_type);

    QRectF rect();
    void setRect( QRectF _r );
    void setRect( QPointF p, QSize s);

    void setWidth( qreal w );
    void setHeight( qreal h );

    void setFrame( QStringList path);
    void setFrame( QString path, int index );
    void addFrame( QString path );

    void setFrame( QList<QImage> list );
    void setFrame( QImage img, int index );
    void addFrame( QImage img );
    int moveFrameUp( int curIndex );
    int moveFrameDown( int curIndex );

    QList<QImage> frame();
    QImage frame( int index );
    QStringList frameAdr();
    QString frameAdr( int index );

    void deleteFrame( int index );
    void deleteFrame( QImage img );
    void deleteFrame( QString path );
    void clearAll();

    void moveTo(QPointF pos);
};
#endif
