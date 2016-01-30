#include "GrEnterArea.h"

IDGenerator* GrEnterArea::idgen = new IDGenerator( GrEnterAreaType );

GrEnterArea::GrEnterArea( QGraphicsScene* _scene ) : GrObject(0, _scene){
    setFlags( ItemIsSelectable | ItemIsMovable );
    eaFont = QFont("Arial", 16);
    UniqueNumber = idgen->NewID();

    curIndexBrush = 0;
    curIndexInvalidPen = 0;
    curIndexPen = 0;
    curIndexValidPen = 0;
    TItemState state;
    state.isVisible = true;
    state.name = "default";
    state.indexList.push_back( curIndexBrush );
    state.indexList.push_back( curIndexPen );
    state.indexList.push_back( curIndexValidPen );
    state.indexList.push_back( curIndexInvalidPen );

    stateList.push_back( state );
        curState = 0;
}

QString GrEnterArea::name(){
    return QString("Enter Area #"+QString().number(SetLSB(UniqueNumber)));//+" unique number #"+QString().number(UniqueNumber));
}


GrEnterArea::~GrEnterArea(){
    deleteUniqueNumber();
}

QRectF GrEnterArea::boundingRect() const {
    return rect;
}

void GrEnterArea::paint(QPainter *ppainter, const QStyleOptionGraphicsItem *, QWidget *){
    ppainter->save();
    ppainter->setBrush( brushList.at( curIndexBrush ) );
    ppainter->setPen( penList.at( curIndexPen ) );
    ppainter->drawRect( rect );
    if ( isTextValid ) ppainter->setPen( validTextPenList.at( curIndexValidPen ) );
    else ppainter->setPen( invalidTextPenList.at( curIndexInvalidPen ) );
    adjustFontSize( eaFont, textRect.width(), textRect.height() ,areaText);
    ppainter->setFont( eaFont );
    ppainter->drawText( textRect, Qt::AlignRight, areaText );
    ppainter->restore();
    update();
}

void GrEnterArea::hoverEnterEvent(QGraphicsSceneHoverEvent *pe){
    QApplication::setOverrideCursor( Qt::PointingHandCursor );
    QColor color = brushList.at(curIndexBrush).color();
    color.setRed( color.red()/2 );
    color.setGreen( color.green()/2 );
    color.setBlue( color.blue()/2 );
    brushList[curIndexBrush].setColor( color );
    QGraphicsItem::hoverEnterEvent( pe );
}

void GrEnterArea::hoverLeaveEvent(QGraphicsSceneHoverEvent *pe){
    QApplication::restoreOverrideCursor();
    QColor color = brushList.at(curIndexBrush).color();
    color.setRed( color.red()*2 );
    color.setGreen( color.green()*2 );
    color.setBlue( color.blue()*2 );
    brushList[curIndexBrush].setColor( color );
    QGraphicsItem::hoverLeaveEvent( pe );
}

void GrEnterArea::mousePressEvent(QGraphicsSceneMouseEvent *pe){
    QApplication::setOverrideCursor( Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent(pe);
}

void GrEnterArea::mouseMoveEvent(QGraphicsSceneMouseEvent *pe){
    moveTo(pe->scenePos());
}

void GrEnterArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *pe){
    QApplication::restoreOverrideCursor();
    QGraphicsItem::mouseReleaseEvent(pe);
}


void GrEnterArea::adjustFontSize( QFont &font, int width,  int height, const QString &text ){
    QFontMetrics fm( font );
    int fontSize = -1;
    do {
        ++fontSize;
        font.setPointSize( fontSize + 1 );
        fm = QFontMetrics( font );
    } while( fm.width( text ) <= width && fm.height() <= height );
    font.setPointSize( fontSize );
}

bool GrEnterArea::nextIndexIsValid(quint16 listcount, quint16 *curIndex){
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

bool GrEnterArea::prevIndexIsValid(quint16 *curIndex){
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

void GrEnterArea::setNextState(){
    if ( curState < ( stateList.count() - 1 ) ) curState++;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    curIndexValidPen = stateList.at(curState).indexList.at( state_validColorIndex );
    curIndexInvalidPen = stateList.at(curState).indexList.at( state_invalidColorIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrEnterArea::setPreviousState(){
    if ( curState > 0 ) curState--;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    curIndexValidPen = stateList.at(curState).indexList.at( state_validColorIndex );
    curIndexInvalidPen = stateList.at(curState).indexList.at( state_invalidColorIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrEnterArea::setCurrentState(quint16 index){
    if ( curState < stateList.count() ) curState = index;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    curIndexValidPen = stateList.at(curState).indexList.at( state_validColorIndex );
    curIndexInvalidPen = stateList.at(curState).indexList.at( state_invalidColorIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrEnterArea::setRect(QRectF _rect){
    prepareGeometryChange();
    rect = _rect;
    textRect.setRight( rect.right() - TEXTINDENT );
    textRect.setBottom( rect.bottom() - TEXTINDENT );
    textRect.setTopLeft( QPointF( rect.left() + TEXTINDENT, rect.top() + TEXTINDENT ));
}

void GrEnterArea::setRect(qreal x, qreal y, qreal width, qreal height){
    rect = QRect(x, y, width, height);
    textRect.setRight( rect.right() - TEXTINDENT );
    textRect.setBottom( rect.bottom() - TEXTINDENT );
    textRect.setTopLeft( QPointF( rect.left() + TEXTINDENT, rect.top() + TEXTINDENT ));
}

void GrEnterArea::setRectWidth(qreal width){
    rect.setWidth( width );
    textRect.setRight( rect.right() - TEXTINDENT );
    textRect.setBottom( rect.bottom() - TEXTINDENT );
    textRect.setTopLeft( QPointF( rect.left() + TEXTINDENT, rect.top() + TEXTINDENT ));
}

void GrEnterArea::setRectHeight(qreal height){
    rect.setHeight( height );
    textRect.setRight( rect.right() - TEXTINDENT );
    textRect.setBottom( rect.bottom() - TEXTINDENT );
    textRect.setTopLeft( QPointF( rect.left() + TEXTINDENT, rect.top() + TEXTINDENT ));
}

QRectF GrEnterArea::getRect(){
    return rect;
}

QString GrEnterArea::text(){
    return areaText;
}

bool GrEnterArea::setText(QString t){
    bool ok;
    int temp = t.toInt(&ok, 10);
    if ( ok ){
        areaText = t;
        textRect.setRight( rect.right() - TEXTINDENT );
        textRect.setBottom( rect.bottom() - TEXTINDENT );
        textRect.setTopLeft( QPointF( rect.left() + TEXTINDENT, rect.top() + TEXTINDENT ));
        if ( temp >= validator.min  && temp <= validator.max )  isTextValid = true;
        else isTextValid = false;
        return true;
    }
    else return false;
}

void GrEnterArea::setTextValidator(TEnterAreaValidator val){
    validator = val;
}

void GrEnterArea::setTextValidator(quint16 min, quint16 max){
    validator.min = min;
    validator.max = max;
    if ( areaText.toInt() >= validator.min  && areaText.toInt() <= validator.max )  isTextValid = true;
    else isTextValid = false;
}

TEnterAreaValidator GrEnterArea::getTextValidator(){
    return validator;
}

void GrEnterArea::addValidTextColor(QColor col){
    validTextPenList.push_back(QPen(col));
}

void GrEnterArea::addInvalidTextColor(QColor col){
    invalidTextPenList.push_back(QPen(col));
}

void GrEnterArea::setValidTextColor(QColor col, quint16 index){
    if ( index < validTextPenList.count() ) validTextPenList[index].setColor( col );
}

void GrEnterArea::setInvalidTextColor(QColor col, quint16 index){
    if ( index < invalidTextPenList.count() ) invalidTextPenList[index].setColor( col );
}

void GrEnterArea::deleteValidTextColor(quint16 index){
    if ( index < validTextPenList.count()){
        curIndexValidPen = 0;
        validTextPenList.removeAt( index );
        while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    }
}

void GrEnterArea::deleteInvalidTextColor(quint16 index){
    if ( index < invalidTextPenList.count()){
        curIndexInvalidPen = 0;
        invalidTextPenList.removeAt( index );
        while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    }
}

QList<QPen> GrEnterArea::validTextColor(){
    return validTextPenList;
}

QList<QPen> GrEnterArea::invalidTextColor(){
    return invalidTextPenList;
}

void GrEnterArea::addBackgroundColor(QColor col ){
    brushList.push_back( QBrush(col) );
}

void GrEnterArea::setBackgroundColor(QColor col, quint16 index){
    if ( index < brushList.count() ) brushList[index].setColor( col );
}

void GrEnterArea::deleteBackgroundColor(quint16 index){
    if ( index < brushList.count() ){
        curIndexBrush = 0;
        brushList.removeAt( index );
        while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    }
}

QList<QBrush> GrEnterArea::backgroundColor(){
    return brushList;
}

QFont GrEnterArea::font(){
    return eaFont;
}

void GrEnterArea::setFont(QFont _font){
    eaFont = _font;
}

void GrEnterArea::addBorderColor(QColor col){
    penList.push_back( QPen(col) );
}

void GrEnterArea::setBorderColor(QColor col, quint16 index){
    if ( index < penList.count() ) penList[index].setColor( col );
}

void GrEnterArea::deleteBorderColor(quint16 index){
    if ( index < penList.count() ){
        curIndexPen = 0;
        penList.removeAt( index );
        while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    }
}

QList<QPen> GrEnterArea::borderColor(){
    return  penList;
}


void GrEnterArea::setValidTextColor(QList<QColor> list){
    QList<QPen> temp;
    foreach( QColor col, list){
        temp.push_back(QPen(col));
    }
    validTextPenList = temp;
}

void GrEnterArea::setInvalidTextColor( QList<QColor> list){
    QList<QPen> temp;
    foreach( QColor col, list){
        temp.push_back(QPen(col));
    }
    invalidTextPenList = temp;
}

void GrEnterArea::setBackgroundColor(QList<QColor> list){
    QList<QBrush> temp;
    foreach( QColor col, list){
        temp.push_back(QBrush(col));
    }
    brushList = temp;
}

void GrEnterArea::setBorderColor(QList<QColor> list){
    QList<QPen> temp;
    foreach( QColor col, list){
        temp.push_back(QPen(col));
    }
    penList = temp;
}

quint16 GrEnterArea::currentIndex(TGrIndexes indexType){
    quint16 index;
    switch( indexType ){
    case brushIndex: index = curIndexBrush; break;
    case penIndex: index = curIndexPen; break;
    case validTextPenIndex: index = curIndexValidPen; break;
    case invalidTextPenIndex: index = curIndexInvalidPen; break;
    }
    return index;
}

void GrEnterArea::setCurrentIndex(TGrIndexes indexType, quint16 index){
    switch( indexType ){
    case brushIndex: curIndexBrush = index; break;
    case penIndex: curIndexPen = index; break;
    case validTextPenIndex: curIndexValidPen = index; break;
    case invalidTextPenIndex: curIndexInvalidPen = index; break;
    }
}

bool GrEnterArea::setNextIndex(TGrIndexes indexType){
    bool res;
    switch( indexType ){
    case brushIndex: res = nextIndexIsValid( brushList.count(), &curIndexBrush ); break;
    case penIndex: res = nextIndexIsValid( penList.count(), &curIndexPen); break;
    case validTextPenIndex: res = nextIndexIsValid( validTextPenList.count(), &curIndexValidPen ); break;
    case invalidTextPenIndex: res = nextIndexIsValid( invalidTextPenList.count(), &curIndexInvalidPen ); break;
    }
    return res;
}

bool GrEnterArea::setPrevIndex(TGrIndexes indexType){
    bool res;
    switch( indexType ){
    case brushIndex: res = prevIndexIsValid( &curIndexBrush ); break;
    case penIndex: res = prevIndexIsValid( &curIndexPen ); break;
    case validTextPenIndex: res = prevIndexIsValid( &curIndexValidPen ); break;
    case invalidTextPenIndex: res = prevIndexIsValid( &curIndexInvalidPen ); break;
    }
    return res;
}

void GrEnterArea::moveTo(QPointF pos){
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
