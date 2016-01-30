#include "GrAnimation.h"
IDGenerator* GrAnimation::idgen = new IDGenerator( GrAnimationType );

GrAnimation::GrAnimation(QGraphicsScene *_scene) : GrObject(0, _scene){
    setAcceptHoverEvents( true );
    setFlags( ItemIsSelectable | ItemIsMovable );
    uNumber = idgen->NewID();
    curFrame = 0;
    QImage defaultImg = QImage("an0.bmp");
    Rect = QRect( 0, 0, defaultImg.width(), defaultImg.height() );

    addFrame( "an0.bmp" );
    addFrame( "an1.bmp" );
    addFrame( "an2.bmp" );
    addFrame( "an3.bmp" );
    addFrame( "an4.bmp" );
    addFrame( "an5.bmp" );
    addFrame( "an6.bmp" );
    addFrame( "an7.bmp" );

    freq = 1000;
    currentTime = 0;
    TItemState state;
    state.isVisible = true;
    state.name = "default";
    state.indexList.clear();
//    state.indexList.push_back( currentImage );
    stateList.push_back( state );
    curState = 0;
    active = true;
    AnimeType = anime_single;
}

GrAnimation::~GrAnimation(){
}

QString GrAnimation::name(){
    return QString("Animation #"+QString().number(SetLSB(uNumber)));
}

quint16 GrAnimation::uniqueNumber() const{
    return uNumber;
}

QRectF GrAnimation::boundingRect() const{
    return Rect;
}

void GrAnimation::setNextState(){
    if ( curState < ( stateList.count() - 1 )) curState++;
    else return;
//    curFrame = stateList.at( curState ).indexList.at( state_frameIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrAnimation::setCurrentState(quint16 index){
    if ( index < states().count() ) curState = index;
    else return;
//    curFrame = stateList.at( curState ).indexList.at( state_frameIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrAnimation::setPreviousState(){
    if ( curState > 0 ) curState--;
    else return;
//    curFrame = stateList.at( curState ).indexList.at( state_frameIndex );
    setVisible( stateList.at(curState).isVisible );
}


void GrAnimation::paint(QPainter *ppainter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    ppainter->save();
    ppainter->drawImage( Rect, frameList.at(curFrame));
    ppainter->restore();
}

void GrAnimation::mousePressEvent(QGraphicsSceneMouseEvent *pe){
    QApplication::setOverrideCursor( Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent(pe);
}

void GrAnimation::mouseReleaseEvent(QGraphicsSceneMouseEvent *pe){
    QApplication::restoreOverrideCursor();
    QGraphicsItem::mouseReleaseEvent(pe);
}

void GrAnimation::mouseMoveEvent(QGraphicsSceneMouseEvent *pe){
    moveTo( pe->scenePos() );
}

void GrAnimation::notifyTimer( quint16 interval ){
    if ( !active ) return;
    currentTime += interval;
    if ( currentTime >= freq ){
        if ( curFrame + 1 < frameList.count() ) curFrame++;
        else if ( AnimeType == anime_infinity ) curFrame = 0;
        else{ active = false; curFrame = 0; }
        update();
        currentTime = 0;
    }
}

void GrAnimation::setFrequency(int msec){
    freq = msec;
}

int GrAnimation::frequency(){
    return freq;
}

TAnimeType GrAnimation::repeatType(){
    return AnimeType;
}

void GrAnimation::setRepeatType(TAnimeType r_type){
    AnimeType = r_type;
    active = true;
}

QRectF GrAnimation::rect(){
    return Rect;
}

void GrAnimation::setRect(QRectF _r){
    prepareGeometryChange();
    Rect = _r;
}

void GrAnimation::setRect(QPointF p, QSize s){
    prepareGeometryChange();
    Rect = QRectF( p, s );
}

void GrAnimation::setWidth(qreal w){
    prepareGeometryChange();
    Rect.setWidth( w );
}

void GrAnimation::setHeight(qreal h){
    prepareGeometryChange();
    Rect.setHeight( h );
}


void GrAnimation::setFrame(QStringList path){
    framePath = path;
    frameList.clear();
    foreach( QString str, framePath ){
        frameList.push_back( QImage(str));
    }
}

void GrAnimation::setFrame(QString path, int index){
    if ( index > 0 && index < frameList.count() ){
        framePath.replace(index, path );
        frameList.replace(index, QImage(path));
    }
}

void GrAnimation::addFrame(QString path){
    framePath.push_back( path );
    frameList.push_back( QImage(path) );
}

void GrAnimation::setFrame(QList<QImage> list){
    frameList = list;
    prepareGeometryChange();
    Rect.setSize( frameList.first().size() );
}

void GrAnimation::setFrame(QImage img, int index){
    if ( index > 0 && index < frameList.count() )
        frameList.replace( index, img );
}

void GrAnimation::addFrame(QImage img){
    frameList.push_back( img );
}

int GrAnimation::moveFrameUp(int curIndex){
    if ( curIndex > 0 ){
        int temp = curIndex - 1;
        frameList.swap( curIndex, temp );
        framePath.swap( curIndex, temp );
        return temp;
    }
    return -1;
}

int GrAnimation::moveFrameDown(int curIndex){
    if ( curIndex < frameList.count() - 1 ){
        int temp = curIndex + 1;
        frameList.swap( curIndex, temp );
        framePath.swap( curIndex, temp );
        return temp;
    }
    return -1;
}

QList<QImage> GrAnimation::frame(){
    return frameList;
}

QImage GrAnimation::frame(int index){
    if ( index < frameList.count() && index >= 0 ){
        return frameList.at( index );
    }
}

QStringList GrAnimation::frameAdr(){
    return framePath;
}

QString GrAnimation::frameAdr(int index){
    if ( index < frameList.count() && index >= 0 ) return framePath.at( index );
}

void GrAnimation::deleteFrame(int index){
    if ( index < frameList.count() && index >= 0 ){
        framePath.removeAt( index );
        frameList.removeAt( index );
    }
}

void GrAnimation::deleteFrame(QImage img){
    int index = frameList.indexOf( img );
    frameList.removeAt( index );
    framePath.removeAt( index );
}

void GrAnimation::deleteFrame(QString path){
    int index = framePath.indexOf( path );
    frameList.removeAt( index );
    framePath.removeAt( index );
}

void GrAnimation::clearAll(){
    frameList.clear();
    framePath.clear();
}

void GrAnimation::moveTo(QPointF pos){
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

