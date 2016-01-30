#ifndef GRRESIZER_H
#define GRRESIZER_H

#include <QtGui>
#include "GlobalDefinition.h"

class GrResizer : public QGraphicsItem
{
public:
    GrResizer(QGraphicsItem* _parent ,QGraphicsScene *_scene);
    ~GrResizer();
    virtual int	type() const { return Type;}
    virtual void paint(QPainter *ppainter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *pe);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *pe);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *pe);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *pe);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *pe);
    virtual QRectF boundingRect() const;
    void abortResize();
private:
    enum { Type = GrResizerType };
    void calcGrips();
    void changeGripPos(QPointF pos);
    QRectF rect;
    QPainterPath path;
    QPainterPath backupPath;
    QRectF backupRect;
    QList<QRectF> gripList;
    QPen pen;
    bool blockHoverMove;
    int selectedGrip;
};

typedef enum{
    grip_topLeft = 0,
    grip_botLeft,
    grip_topRight,
    grip_botRight,
    grip_topMid,
    grip_botMid,
    grip_leftMid,
    grip_rightMid
}GripPos;
#define GRIPSIZE 6

#endif // GRRESIZER_H
