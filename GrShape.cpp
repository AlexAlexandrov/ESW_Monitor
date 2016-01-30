#include "GrShape.h"
IDGenerator* GrShape::idgen = new IDGenerator( GrShapeType );

GrShape::GrShape(QGraphicsItem *_parent, QGraphicsScene *_scene) : GrObject( _parent, _scene ){
    penList.push_back( QPen(Qt::black) );
    brushList.push_back( QBrush( Qt::yellow) );
    setFlags( ItemIsMovable | ItemIsSelectable );
    setAcceptsHoverEvents( true );
    UniqueNumber = idgen->NewID();
    _name = "Shape #"+QString().number(SetLSB(UniqueNumber));//+" unique number #"+QString().number(UniqueNumber);

    curIndexBrush = 0;
    curIndexPen = 0;
    TItemState state;
    state.isVisible = true;
    state.name = "default";
    state.indexList.push_back( curIndexBrush );
    state.indexList.push_back( curIndexPen );

    stateList.push_back( state );
        curState = 0;
}

void GrShape::calcNewPath(qreal x, qreal y, qreal w, qreal h){
    QPainterPath tempPath;
    switch( _shapeType ){
    case shape_ellipse:
            tempPath.addEllipse( x, y, w, h );
        break;
    case shape_rect:
            tempPath.addRect( x, y, w, h);
        break;
    case shape_triangle:
            tempPath.moveTo( x, y );
            tempPath.lineTo( x + w/2, y + h );
            tempPath.lineTo( x + w, y );
            tempPath.closeSubpath();
        break;
    }
    _width = w;
    _height = h;
    _x = x;
    _y = y;
    _path = tempPath;
}

bool GrShape::nextIndexIsValid(quint16 listcount, quint16 *curIndex){
    int index = *curIndex + 1;
    if ( index < listcount ){
        *curIndex = index;
        index++;
        update();
        if ( index <  listcount ) return true;
        else return false;
    }
    return false;
}

bool GrShape::prevIndexIsValid(quint16 *curIndex){
    int index = *curIndex - 1;
    if ( index >= 0 ){
        *curIndex = index;
        index--;
        update();
        if ( index >= 0 ) return true;
        else return false;
    }
    return false;
}

void GrShape::setNextState(){
    if ( curState < ( stateList.count() - 1 )) curState++;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrShape::setPreviousState(){
    if ( curState > 0 ) curState--;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at(state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrShape::setCurrentState(quint16 index){
    if ( index < stateList.count()) curState = index;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    setVisible( stateList.at(curState).isVisible );
}


void GrShape::setObjectShape(TShapeTypes t, qreal x, qreal y, qreal w, qreal h){
    prepareGeometryChange();
    _shapeType = t;
    calcNewPath( x, y, w, h );
    update();
}

void GrShape::setObjectShape(TShapeTypes t){
    prepareGeometryChange();
    _shapeType = t;
    calcNewPath( _x, _y, _width, _height );
    update();
}


QRectF GrShape::boundingRect() const{
    return _path.boundingRect();
}

QPainterPath GrShape::shape() const{
    return _path;
}

void GrShape::paint(QPainter *ppainter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    ppainter->save();
    ppainter->setPen( penList.at( curIndexPen ) );
    ppainter->setBrush( brushList.at( curIndexBrush ) );
    ppainter->drawPath( _path );
    ppainter->restore();
    update();
}

void GrShape::setCurrentIndex(TGrIndexes indexType, quint16 index){
    switch( indexType ){
        case brushIndex: curIndexBrush = index; break;
        case penIndex: curIndexPen = index; break;
    }
}

quint16 GrShape::currentIndex(TGrIndexes indexType){
    quint16 index;
    switch(indexType){
    case brushIndex: index = curIndexBrush; break;
    case penIndex: index = curIndexPen; break;
    }
    return index;
}

bool GrShape::setNextIndex(TGrIndexes indexType){
    bool res = false;
    switch( indexType ){
        case brushIndex: res = nextIndexIsValid( brushList.count(), &curIndexBrush ); break;
        case penIndex: res = nextIndexIsValid( penList.count(), &curIndexPen ); break;
    }
    return res;
}

bool GrShape::setPrevIndex(TGrIndexes indexType){
    bool res = false;
    switch( indexType ){
        case brushIndex: res = prevIndexIsValid( &curIndexBrush ); break;
        case penIndex: res = prevIndexIsValid( &curIndexPen ); break;
    }
    return res;
}

void GrShape::setWidth( qreal w ){
    prepareGeometryChange();
    calcNewPath(_x, _y, w, _height);
    update();
}

void GrShape::setHeight(qreal h){
    prepareGeometryChange();
    calcNewPath(_x, _y, _width, h);
    update();
}

void GrShape::setRect(QRectF rect){
    prepareGeometryChange();
    calcNewPath( rect.x(), rect.y(), rect.width(), rect.height() );
    update();
}

void GrShape::setRect(qreal x, qreal y, qreal w, qreal h){
    prepareGeometryChange();
    calcNewPath( x, y, w, h);
    update();
}

void GrShape::addBackgroundColor(QColor col){
    brushList.push_back( QBrush( col ));
}

void GrShape::addBorderColor(QColor col){
    penList.push_back( QPen( col ));
}

void GrShape::setBackgroundColor(QList<QColor> list){
    QList<QBrush> temp;
    foreach( QColor col, list){
        temp.push_back(QBrush(col));
    }
    brushList = temp;
}

void GrShape::setBorderColor(QList<QColor> list){
    QList<QPen> temp;
    foreach( QColor col, list){
        temp.push_back(QPen(col));
    }
    penList = temp;
}

void GrShape::deleteBackgroundColor(quint16 index){
  if ( index < brushList.count() ){
        curIndexBrush = 0;
        brushList.removeAt( index );
        while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
  }
}

void GrShape::deleteBorderColor(quint16 index){
    if ( index < penList.count() ){
      curIndexPen = 0;
      penList.removeAt( index );
      while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    }
}

void GrShape::mouseMoveEvent(QGraphicsSceneMouseEvent *pe){
    moveTo(pe->scenePos());
}

void GrShape::moveTo(QPointF pos){
    QRectF sceneRect = scene()->sceneRect();
    QRectF tempRect;
    tempRect.setX( pos.x() );
    tempRect.setY( pos.y() );
    tempRect.setWidth( boundingRect().width());
    tempRect.setHeight( boundingRect().height() );
    if ( sceneRect.contains( tempRect ) && sceneRect.right() >= tempRect.right() && sceneRect.bottom() >= tempRect.bottom() ){
        setPos( pos );
    }
}


