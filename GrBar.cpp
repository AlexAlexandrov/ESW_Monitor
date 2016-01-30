#include "GrBar.h"

//QList<QBrush> GrBar::brushList;
//QList<QPen> GrBar::penList;

IDGenerator* GrBar::idgen = new IDGenerator( GrBarType );

QString GrBar::name(){
    return QString("Bar #"+QString().number(SetLSB(UniqueNumber)));//+" on screen #"+QString().number(UniqueNumber));
}

GrBar::GrBar(QGraphicsScene *_scene) : GrObject( 0, _scene ){

    addBackgroundColor( Qt::gray );
    addBorderColor( Qt::black );
    setFlags( ItemIsSelectable | ItemIsMovable );
    setAcceptHoverEvents( true );

    UniqueNumber = idgen->NewID();

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

GrBar::~GrBar(){
    deleteUniqueNumber();
}

void GrBar::paint(QPainter *ppainter, const QStyleOptionGraphicsItem *, QWidget *){
    ppainter->save();
    ppainter->setPen( penList.at(curIndexPen) );
    ppainter->setBrush( brushList.at(curIndexBrush) );
    ppainter->drawRect( boundingRect() );
    ppainter->restore();
    update();
}

QRectF GrBar::boundingRect() const{
    return barRect;
}

void GrBar::hoverEnterEvent(QGraphicsSceneHoverEvent *pe){
    QApplication::setOverrideCursor( Qt::PointingHandCursor );
    QColor color = brushList.at(curIndexBrush).color();
    color.setRed( color.red()/2 );
    color.setGreen( color.green()/2 );
    color.setBlue( color.blue()/2 );
    brushList[curIndexBrush].setColor( color );
    QGraphicsItem::hoverEnterEvent( pe );
}

void GrBar::hoverLeaveEvent(QGraphicsSceneHoverEvent *pe){
    QApplication::restoreOverrideCursor();
    QColor color = brushList.at(curIndexBrush).color();
    color.setRed( color.red()*2 );
    color.setGreen( color.green()*2 );
    color.setBlue( color.blue()*2 );
    brushList[curIndexBrush].setColor( color );
    QGraphicsItem::hoverLeaveEvent( pe );
}

void GrBar::mousePressEvent(QGraphicsSceneMouseEvent *pe){
    QApplication::setOverrideCursor( Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent(pe);
}

void GrBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *pe){
    QApplication::restoreOverrideCursor();
    QGraphicsItem::mouseReleaseEvent(pe);
}

void GrBar::mouseMoveEvent(QGraphicsSceneMouseEvent *pe){
    moveTo( pe->scenePos() );
}

void GrBar::setRect(const QRectF &_rect){
    prepareGeometryChange();
    barRect = _rect;
}

void GrBar::setRect(qreal x, qreal y, qreal w, qreal h){
    prepareGeometryChange();
    barRect = QRectF(x,y,w,h);
}

void GrBar::addBackgroundColor( QColor col ){
    brushList.push_back( QBrush( col ) );
}

void GrBar::addBorderColor( QColor col ){
    penList.push_back( QPen( col ) );
}

void GrBar::setBackgroundColor(QColor col, quint16 index){
    if ( index < brushList.count()) brushList[index].setColor( col );
}

void GrBar::setBorderColor(QColor col, quint16 index){
    if ( index < penList.count()) penList[index].setColor( col );
}

void GrBar::setBackgroundColor(QList<QColor> list){
    QList<QBrush> temp;
    foreach( QColor col, list){
        temp.push_back(QBrush(col));
    }
    brushList = temp;
}

void GrBar::setBorderColor(QList<QColor> list){
    QList<QPen> temp;
    foreach( QColor col, list){
        temp.push_back(QPen(col));
    }
    penList = temp;
}

void GrBar::deleteBackgroundColor(quint16 index){
    if ( index < brushList.count()){
        curIndexBrush = 0;
        brushList.removeAt( index );
        while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    }
}

void GrBar::deleteBorderColor(quint16 index){
    if ( index < penList.count()){
        curIndexPen = 0;
        penList.removeAt( index );
        while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    }

}

QList<QBrush> GrBar::backgroundColor(){
    return brushList;
}

QList<QPen> GrBar::borderColor(){
    return penList;
}

bool GrBar::setCurrentIndex(TGrIndexes indexType, quint16 index){
    bool res = false;
    switch( indexType ){
    case brushIndex: if ( brushList.count() <= index  ) return false; curIndexBrush = index; res = true; break;
    case penIndex: if ( penList.count() <= index  ) return false; curIndexPen = index; res = true; break;
    }
    return res;
}

bool GrBar::setNextIndex(TGrIndexes indexType){
    bool res;
    switch( indexType ){
    case brushIndex: res = nextIndexIsValid( brushList.count(), &curIndexBrush ); break;
    case penIndex: res = nextIndexIsValid( penList.count(), &curIndexPen); break;
    }
    return res;
}

bool GrBar::setPrevIndex(TGrIndexes indexType){
    bool res;
    switch( indexType ){
    case brushIndex: res = prevIndexIsValid( &curIndexBrush ); break;
    case penIndex: res = prevIndexIsValid( &curIndexPen); break;
    }
    return res;
}

quint16 GrBar::currentIndex(TGrIndexes indexType){
    quint16 res;
    switch( indexType ){
    case brushIndex: res = curIndexBrush; break;
    case penIndex: res = curIndexPen; break;
    }
    return res;
}

bool GrBar::nextIndexIsValid(quint16 listcount, quint16 *curIndex){
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

bool GrBar::prevIndexIsValid(quint16 *curIndex){
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

void GrBar::moveTo(QPointF pos){
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

void GrBar::setNextState(){
    if ( curState < ( stateList.count() - 1 )) curState++;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrBar::setPreviousState(){
    if ( curState > 0 ) curState--;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrBar::setCurrentState(quint16 index){
    if ( index < stateList.count()) curState = index;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    setVisible( stateList.at(curState).isVisible );
}

