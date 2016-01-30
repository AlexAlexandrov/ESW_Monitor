#include "GrImage.h"

IDGenerator* GrImage::idgen = new IDGenerator( GrImageType );

GrImage::GrImage(QGraphicsScene *_scene) : GrObject(0, _scene){
    setAcceptHoverEvents( true );
    setFlags( ItemIsSelectable | ItemIsMovable );
    UniqueNumber = idgen->NewID();
    currentImage = 0;
    imgList.clear();
    imagePathList.clear();
//    addImage("no-image.jpg");
    addImage("example_pic3.bmp");
    TItemState state;
    state.isVisible = true;
    state.name = "default";
    state.indexList.push_back( currentImage );
    stateList.push_back( state );
    curState = 0;
}

QString GrImage::name(){
    return QString("Image #"+QString().number(SetLSB(UniqueNumber)));//+" unique number #"+QString().number(UniqueNumber));
}

GrImage::~GrImage(){
    deleteUniqueNumber();
}

void GrImage::paint(QPainter *ppainter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    ppainter->save();
    if ( imgList.isEmpty() ){
        ppainter->setBrush( Qt::darkGray );
        ppainter->setPen( Qt::black );
        ppainter->drawRect( _rect );
        ppainter->setPen( Qt::white );
        ppainter->drawText( _rect, Qt::AlignCenter, "image" );
    }
    else{
        QPixmap pixmap = QPixmap().fromImage( imgList.at( currentImage ) );
        ppainter->drawPixmap( _rect.toRect(), pixmap );
    }
    ppainter->restore();
    update();
}

QRectF GrImage::boundingRect() const {
    return _rect;
}

void GrImage::addImage(QString imagePath){
    imagePathList.push_back( imagePath );
    QImage _img = QImage( imagePath );
    imgList.push_back( _img );

}

void GrImage::setImage(int index ,QString imagePath){
    if ( index >= 0 && index < imagePathList.count() ){
        QImage _img = QImage( imagePath );
        imgList.replace( index, _img );
        imagePathList.replace( index, imagePath );
    }
}

void GrImage::setImage(QStringList imgPaths){
    imagePathList = imgPaths;
    foreach( QString str, imgPaths){
        imgList.push_back( QImage(str) );
    }
}

void GrImage::deleteImage(quint16 index){
    imgList.removeAt( index );
    imagePathList.removeAt( index );
    currentImage = 0;
    while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    update();
}

void GrImage::deleteCurrentImage(){
    imgList.removeAt( currentImage );
    imagePathList.removeAt( currentImage );
    currentImage = 0;
    while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    update();
}

void GrImage::deleteImages(){
    imgList.clear();
    imagePathList.clear();
    while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    currentImage = 0;
    update();
}

bool GrImage::setCurrentIndex(quint16 index){
    if ( index < imgList.count() ){
        currentImage = index;
        update();
        return true;
    }
    else return false;
}

bool GrImage::setNextImage(){
    int index = currentImage + 1;
    if ( index < imgList.count() ){
        currentImage = index;
        index++;
        update();
        if ( index < imgList.count()) return true;
        else return false;
    }
    return false;
}

bool GrImage::setPrevImage(){
    int index = currentImage - 1;
    if ( index >= 0 ){
        currentImage = index;
        index--;
        update();
        if ( index >= 0 ) return true;
        else return false;
    }
    return false;
}

void GrImage::setRect(QPointF p, QSizeF s){
    prepareGeometryChange();
    _rect = QRectF( p ,s );
}

void GrImage::setRect(QRectF r){
    prepareGeometryChange();
    _rect = r;
}

void GrImage::setRect(qreal x, qreal y, qreal w, qreal h){
    prepareGeometryChange();
    _rect = QRectF( x, y, w, h );
}

void GrImage::setWidth(qreal w){
    prepareGeometryChange();
    _rect.setWidth( w );
}

void GrImage::setHeight(qreal h){
    prepareGeometryChange();
    _rect.setHeight( h );
}

QImage GrImage::image(quint16 index) const{
    QImage temp = imgList.at( index ).scaled( rect().size().toSize() );
    return temp;
}

QList<QImage> GrImage::images() const{
    QList<QImage> tempList;
    foreach( QImage img, imgList ){
        tempList.push_back( img.scaled( rect().size().toSize()));
    }
    return tempList;
}

quint16 GrImage::currentIndex() const{
    return currentImage;
}

QRectF GrImage::rect() const{
    return _rect;
}

QString GrImage::imagePath(quint16 index) const {
    return imagePathList.at( index );
}

QStringList GrImage::imagesPath() const{
    return imagePathList;
}

void GrImage::mouseMoveEvent(QGraphicsSceneMouseEvent *pe){
    moveTo(pe->scenePos());
}

void GrImage::moveTo(QPointF pos){
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


void GrImage::setNextState(){
    if ( curState < ( stateList.count() - 1 ) ) curState++;
    else return;
    currentImage = stateList.at(curState).indexList.at( state_imageIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrImage::setPreviousState(){
    if ( curState > 0 ) curState--;
    else return;
    currentImage = stateList.at(curState).indexList.at( state_imageIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrImage::setCurrentState(quint16 index){
    if ( curState < stateList.count() ) curState = index;
    else return;
    currentImage = stateList.at(curState).indexList.at( state_imageIndex );
    setVisible( stateList.at(curState).isVisible );
}
