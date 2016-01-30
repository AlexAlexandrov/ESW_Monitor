#include "GrText.h"

IDGenerator* GrText::idgen = new IDGenerator( GrTextType );

GrText::GrText( QGraphicsScene *_scene ) : GrObject( 0, _scene ){
    textList.clear();
    setFlags( QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
    setAcceptHoverEvents( true );
    addTextColor( QColor( Qt::black ) );
    addText( "text" );
//    setCurrentIndex( penIndex, 0 );
    curIndexText = 0;
    curIndexPen = 0;

    setFont( QFont( "Arial", 16 ) );
    UniqueNumber = idgen->NewID();
    TItemState state;
    state.isVisible = true;
    state.name = "default";
    state.indexList.push_back( curIndexText );
    state.indexList.push_back( curIndexPen );
    stateList.push_back( state );
    curState = 0;

    textRect.setX( 0 );
    textRect.setY( 0 );

    textAligment = Qt::AlignLeft;

}

QString GrText::name(){
    return QString("Text #"+QString().number(SetLSB(UniqueNumber)));//+" unique number #"+QString().number(UniqueNumber));
}

GrText::~GrText(){
    deleteUniqueNumber();
}


const QStringList GrText::text(){
    return textList;
}

const QString GrText::text( int index ){
    if ( index > (-1) && index < textList.count()) return textList.at( index );
    else return QString("");
}

const QString GrText::currentText(){
    return textList.at(curIndexText);
}

quint16 GrText::currentIndex( TGrIndexes indexType ){
   quint16 res;
   switch( indexType ){
   case textIndex: res = curIndexText; break;
   case penIndex: res = curIndexPen; break;
   }
   return res;
}

void GrText::addText( QStringList _text ){
    textList = _text;
}

void GrText::addText( QString _str ){
    if ( !textList.contains( _str )){
        textList.push_back( _str );
//        curIndexText = textList.indexOf( _str );
    }
}

void GrText::setText( QString _str, int index ){
    if ( index >= 0 && index < textList.count() ){
        textList.replace( index, _str );
        setCurrentIndex(textIndex, index);
    }
}

void GrText::setText(QStringList list){
    textList = list;
}

void GrText::deleteText(quint16 index){
    if ( index < textList.count()){
        textList.removeAt( index );
        curIndexText = 0;
        while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    }
}

bool GrText::setCurrentIndex( TGrIndexes indexType , quint16 index){
    bool res = false;
    switch( indexType ){
    case textIndex:
        if ( index < textList.count()){
            curIndexText = index;
            res = true;
        }
    break;
    case penIndex:
        if ( index < penList.count()){
            curIndexPen = index;
            res = true;
        }
    break;
    }
    if ( res ){
        prepareGeometryChange();
        QFontMetrics fm( textFont );
        textRect.setWidth( fm.width( textList.at( curIndexText ) ) );
        textRect.setHeight( fm.height() );
        update();
    }
    return res;
}

bool GrText::setNextIndex( TGrIndexes indexType ){
    bool res = false;
    switch( indexType ){
        case textIndex: res = nextIndexIsValid( textList.count(), &curIndexText ); break;
        case penIndex: res = nextIndexIsValid( penList.count(), &curIndexPen ); break;
    }
    prepareGeometryChange();
    QFontMetrics fm( textFont );
    textRect.setWidth( fm.width( textList.at( curIndexText ) ) );
    textRect.setHeight( fm.height() );
    update();
    return res;
}

bool GrText::setPrevIndex(TGrIndexes indexType){
    bool res = false;

    switch( indexType ){
    case textIndex: res = prevIndexIsValid( &curIndexText ); break;
    case penIndex: res = prevIndexIsValid( &curIndexPen ); break;
    }
    prepareGeometryChange();
    QFontMetrics fm( textFont );
    textRect.setWidth( fm.width( textList.at( curIndexText ) ) );
    textRect.setHeight( fm.height() );
    update();
    qDebug()<<res<<curIndexPen<<curIndexText<<textList;
    return res;
}

void GrText::setFont(QFont f ){
    prepareGeometryChange();
    textFont = f;
    QFontMetrics fm( textFont );
    textRect.setWidth( fm.width( textList.at( curIndexText ) ) );
    textRect.setHeight( fm.height() );
}

QFont GrText::font(){
    return textFont;
}

void GrText::setAligment(int align){
    textAligment = align;
}

int GrText::align(){
    return textAligment;
}



void GrText::addTextColor(QColor col){
    penList.push_back( QPen(col) );
}

void GrText::setTextColor(QColor col, quint16 index){
    if ( index < penList.count()) penList[index].setColor( col );
}

void GrText::setTextColor(QList<QColor> list){
    QList<QPen> temp;
    foreach( QColor col, list){
        temp.push_back(QPen(col));
    }
    penList = temp;
}

void GrText::deleteTextColor(quint16 index){
    if ( index < penList.count()){
        curIndexText = 0;
        penList.removeAt( index );
        while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    }
}

QList<QPen> GrText::textColor(){
    return penList;
}

void GrText::setRect(QRectF rect){
    prepareGeometryChange();
    textRect = rect;
}

void GrText::setRect(qreal x, qreal y, qreal width, qreal height){
    prepareGeometryChange();
    textRect = QRectF(x,y,width, height);
}

void GrText::setRectWidth(qreal width){
    prepareGeometryChange();
    textRect.setWidth( width );
}

void GrText::setRectHeight(qreal height){
    prepareGeometryChange();
    textRect.setHeight( height );
}

QRectF GrText::getRect(){
    QRectF maxRect;
    QRectF tempRect;
    QFontMetrics fm( textFont );
    textRect.setWidth( fm.width( textList.at( curIndexText ) ) );
    textRect.setHeight( fm.height() );
    foreach( QString str, textList ){
        tempRect.setWidth( fm.width( str ) );
        tempRect.setHeight( fm.height() );
        if ( tempRect.width() > maxRect.width() && tempRect.height() >= maxRect.height() ) maxRect = tempRect;
    }
    return maxRect;
}

void GrText::mouseMoveEvent(QGraphicsSceneMouseEvent *pe){
    moveTo(pe->scenePos());
}

QRectF GrText::boundingRect() const {
    return textRect;
}

void GrText::paint(QPainter *ppainter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    ppainter->save();
    ppainter->setBrush( Qt::NoBrush );
    if ( isSelected() ){
        ppainter->setPen( Qt::darkBlue );
        ppainter->drawRect( textRect );
    }
    ppainter->setPen( penList.at( curIndexPen ) );
    ppainter->setFont( textFont );
    ppainter->drawText( textRect, textAligment,textList.at( curIndexText ) );
    ppainter->restore();
    update();
}

void GrText::moveTo(QPointF pos){
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

bool GrText::nextIndexIsValid(quint16 listcount, quint16 *curIndex){
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

void GrText::setNextState(){
    if ( curState < ( stateList.count() - 1 )) curState++;
    else return;
    curIndexText = stateList.at(curState).indexList.at( state_textIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_textColorIndex );
    setVisible( stateList.at(curState).isVisible );
    prepareGeometryChange();
    QFontMetrics fm( textFont );
    textRect.setWidth( fm.width( textList.at( curIndexText ) ) );
    textRect.setHeight( fm.height() );
    update();
}

void GrText::setPreviousState(){
    if ( curState > 0 ) curState--;
    else return;
    curIndexText = stateList.at(curState).indexList.at( state_textIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_textColorIndex );
    setVisible( stateList.at(curState).isVisible );
    prepareGeometryChange();
    QFontMetrics fm( textFont );
    textRect.setWidth( fm.width( textList.at( curIndexText ) ) );
    textRect.setHeight( fm.height() );
    update();
}

void GrText::setCurrentState(quint16 index){
    if ( index < stateList.count()) curState = index;
    else return;
    curIndexText = stateList.at(curState).indexList.at( state_textIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_textColorIndex );
    setVisible( stateList.at(curState).isVisible );
    prepareGeometryChange();
    QFontMetrics fm( textFont );
    textRect.setWidth( fm.width( textList.at( curIndexText ) ) );
    textRect.setHeight( fm.height() );
    update();
}

bool GrText::prevIndexIsValid(quint16 *curIndex){
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
