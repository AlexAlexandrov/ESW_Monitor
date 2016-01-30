#include "GrResizer.h"
#include "GrBar.h"
#include "GrImage.h"
#include "GrButton.h"
#include "GrEnterArea.h"
#include "GrShape.h"
#include "GrAnimation.h"

GrResizer::GrResizer(QGraphicsItem* _parent, QGraphicsScene *_scene) : QGraphicsItem( _parent, _scene ){
    setAcceptHoverEvents( true );
    setFlags( ItemIsSelectable );
    rect = parentItem()->boundingRect();
    if ( parentItem()->type() == GrShapeType ){
        GrShape* shp = (GrShape*)parentItem();
        if ( shp->shapeType() == shape_triangle ){
            path = shp->path();
        }
    }
    backupRect = rect;
    backupPath = path;
    setZValue( parentItem()->zValue() + 1 );
    calcGrips();
    pen.setColor( Qt::darkGray );
    pen.setWidth( 0 );
    selectedGrip = -1;
    blockHoverMove = false;
}

GrResizer::~GrResizer(){
    QRectF temp;
    temp.setX( 0 );
    temp.setY( 0 );
    temp.setWidth( rect.width() );
    temp.setHeight( rect.height() );
    switch( parentItem()->type() ){
        case GrButtonType:{
            GrButton* but = (GrButton*)parentItem();
            but->setRect( temp );
        }
        break;
        case GrEnterAreaType:{
            GrEnterArea* ea = (GrEnterArea*)parentItem();
            ea->setRect( temp );
        }
        break;
        case GrBarType:{
            GrBar* bar = (GrBar*)parentItem();
            bar->setRect( temp );
        }
        break;
        case GrAnimationType:{
            GrAnimation* anime = (GrAnimation*)parentItem();
            anime->setWidth( rect.width() );
            anime->setHeight( rect.height() );
        }
        break;
        case GrImageType:{
            GrImage* img = (GrImage* )parentItem();
            img->setRect( temp );
        }
        break;
        case GrShapeType:{
            GrShape* shp = (GrShape*)parentItem();
            switch( shp->shapeType() ){
            case shape_rect: case shape_ellipse: shp->setRect( temp ); break;
            case shape_triangle: shp->setPath( path ); break;
            }
        }
        break;
    }
}

void GrResizer::paint(QPainter *ppainter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    ppainter->save();
    ppainter->setBrush( Qt::NoBrush );
    ppainter->setPen( pen );
    if ( parentItem()->type() == GrShapeType ){
        GrShape* shp = (GrShape*)parentItem();
        if ( shp->shapeType() == shape_triangle ) ppainter->drawPath( path );
        else ppainter->drawRect( rect );
    }else ppainter->drawRect( rect );
    for(int i = 0; i < gripList.count(); i++){
        if ( i == selectedGrip ){
            ppainter->setBrush( QBrush( Qt::darkRed, Qt::SolidPattern) );
            ppainter->drawRect( gripList.at(i));
            ppainter->setBrush( Qt::NoBrush );
        }
        else ppainter->drawRect( gripList.at(i));
    }
    ppainter->restore();
    update();
}

void GrResizer::hoverMoveEvent(QGraphicsSceneHoverEvent *pe){
    int i = 0;
    selectedGrip = -1;
    foreach(QRectF rec, gripList){
        if ( rec.contains( pe->pos())) selectedGrip = i;
        i++;
    }
        switch( selectedGrip ){
        case grip_topLeft: case grip_botRight:
            QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
            break;
        case grip_botLeft: case grip_topRight:
            QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
            break;
        case grip_topMid: case grip_botMid:
            QApplication::setOverrideCursor(Qt::SizeVerCursor);
            break;
        case grip_leftMid: case grip_rightMid:
            QApplication::setOverrideCursor(Qt::SizeHorCursor);
            break;
        default: QApplication::setOverrideCursor(Qt::PointingHandCursor); break;
        }
    QGraphicsItem::hoverMoveEvent( pe );
}

void GrResizer::hoverLeaveEvent(QGraphicsSceneHoverEvent *pe){
    QApplication::restoreOverrideCursor();
    selectedGrip = -1;
    QGraphicsItem::hoverLeaveEvent(pe);
}

void GrResizer::mousePressEvent(QGraphicsSceneMouseEvent *pe){
//    if ( selectedGrip != -1 ){
//        blockHoverMove = true;
//    }
    QGraphicsItem::mousePressEvent( pe );
}

void GrResizer::mouseMoveEvent(QGraphicsSceneMouseEvent *pe){
    if ( selectedGrip == -1 ) return;
    prepareGeometryChange();
    changeGripPos( pe->pos() );
    calcGrips();
    switch( parentItem()->type() ){
        case GrButtonType:{
            GrButton* but = (GrButton*)parentItem();
            but->setRect( rect );
        }
        break;
        case GrEnterAreaType:{
            GrEnterArea* ea = (GrEnterArea*)parentItem();
            ea->setRect( rect );
        }
        break;
        case GrBarType:{
            GrBar* bar = (GrBar*)parentItem();
            bar->setRect( rect );
        }
        break;
        case GrAnimationType:{
            GrAnimation* anime = (GrAnimation*)parentItem();
            anime->setRect( rect );
        }
        break;
        case GrImageType:{
            GrImage* img = (GrImage* )parentItem();
            img->setRect( rect );
        }
        break;
        case GrShapeType:{
            GrShape* shp = (GrShape*)parentItem();
            switch( shp->shapeType() ){
            case shape_ellipse:{
                shp->setRect( rect.x(), rect.y(), rect.width(), rect.height() );
            }break;
            case shape_rect: shp->setRect( rect ); break;
            case shape_triangle:{
                shp->setPath( path );
            }break;
            }
        }
        break;
    }

}

void GrResizer::mouseReleaseEvent(QGraphicsSceneMouseEvent *pe){
//    blockHoverMove = false;
    selectedGrip = -1;
    scene()->update();
    QGraphicsItem::mouseReleaseEvent( pe );

}

QRectF GrResizer::boundingRect() const {
    QPointF point( -GRIPSIZE / 2 + rect.topLeft().x(), -GRIPSIZE / 2 + rect.topLeft().y() );
    QSizeF size( rect.size().width() + GRIPSIZE, rect.size().height() + GRIPSIZE);
    return QRectF( point, size );
}

void GrResizer::calcGrips(){
    gripList.clear();
    if ( parentItem()->type() == GrShapeType ){
        GrShape* shp = (GrShape*)parentItem();
        if ( shp->shapeType() == shape_triangle ){
            for( int i = 0; i < path.elementCount() - 1; i++){
                QPointF pos = QPointF ( path.elementAt( i )) ;
                gripList.push_back( QRectF(pos.x() - GRIPSIZE / 2, pos.y() - GRIPSIZE / 2, GRIPSIZE, GRIPSIZE ));
            }
            return;
        }
    }
    // left corners
    gripList.push_back( QRectF(rect.topLeft().x() - GRIPSIZE / 2, rect.topLeft().y() - GRIPSIZE / 2, GRIPSIZE, GRIPSIZE ));
    gripList.push_back( QRectF(rect.bottomLeft().x() - GRIPSIZE / 2, rect.bottomLeft().y() - GRIPSIZE / 2, GRIPSIZE, GRIPSIZE ));
    // right corners
    gripList.push_back( QRectF(rect.topRight().x() - GRIPSIZE / 2, rect.topRight().y() - GRIPSIZE / 2, GRIPSIZE, GRIPSIZE ));
    gripList.push_back( QRectF(rect.bottomRight().x() - GRIPSIZE / 2, rect.bottomRight().y() - GRIPSIZE / 2, GRIPSIZE, GRIPSIZE ));
    // horizontal mid
    gripList.push_back( QRectF(rect.topLeft().x() - GRIPSIZE / 2 + rect.width()/2, rect.topLeft().y() - GRIPSIZE / 2, GRIPSIZE, GRIPSIZE ));
    gripList.push_back( QRectF(rect.bottomLeft().x() - GRIPSIZE / 2 + rect.width()/2, rect.bottomLeft().y() - GRIPSIZE / 2, GRIPSIZE, GRIPSIZE ));
    // vertical mid
//    if ( parentItem()->type() != GrBarType ){
        gripList.push_back( QRectF(rect.topLeft().x() - GRIPSIZE / 2, rect.topLeft().y() - GRIPSIZE / 2 + rect.height()/2, GRIPSIZE, GRIPSIZE ));
        gripList.push_back( QRectF(rect.topRight().x() - GRIPSIZE / 2, rect.topRight().y() - GRIPSIZE / 2 + rect.height()/2, GRIPSIZE, GRIPSIZE ));
//    }
}

void GrResizer::changeGripPos(QPointF position){
    if ( gripList.count() == 3 && selectedGrip != -1 ){
        path.setElementPositionAt( selectedGrip, position.x(), position.y() );
        if ( selectedGrip == 0 ){
            path.setElementPositionAt( 3, position.x(), position.y());
        }
        return;
    }
    switch( selectedGrip ){
        case grip_topMid: rect.setTop( position.y() ); break;
        case grip_botMid: rect.setBottom( position.y() ); break;
        case grip_leftMid: rect.setLeft( position.x() ); break;
        case grip_rightMid:rect.setRight( position.x() ); break;
        case grip_topLeft: rect.setTopLeft( position );  break;
        case grip_botLeft: rect.setBottomLeft( position ); break;
        case grip_topRight: rect.setTopRight( position ); break;
        case grip_botRight: rect.setBottomRight( position); break;
    }
}

void GrResizer::abortResize(){
switch( parentItem()->type() ){
    case GrButtonType:{
        GrButton* but = (GrButton*)parentItem();
        but->setRect( backupRect );
    }
    break;
    case GrEnterAreaType:{
        GrEnterArea* ea = (GrEnterArea*)parentItem();
        ea->setRect( backupRect );
    }
    break;
    case GrBarType:{
        GrBar* bar = (GrBar*)parentItem();
        bar->setRect( backupRect );
    }
    break;
    case GrAnimationType:{
        GrAnimation* anime = (GrAnimation*)parentItem();
        anime->setRect( backupRect );
    }
    break;
    case GrImageType:{
        GrImage* img = (GrImage* )parentItem();
        img->setRect( backupRect );
    }
    break;
    case GrShapeType:{
        GrShape* shp = (GrShape*)parentItem();
        switch( shp->shapeType() ){
        case shape_ellipse:{
            shp->setRect( backupRect.x(), backupRect.y(), backupRect.width(), backupRect.height() );
        }break;
        case shape_rect: shp->setRect( backupRect ); break;
        case shape_triangle:{
            shp->setPath( backupPath );
        }break;
        }
    }
    }
}
