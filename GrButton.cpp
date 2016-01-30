#include "GrButton.h"

IDGenerator* GrButton::idgen = new IDGenerator( GrButtonType );

bool GrButton::nextIndexIsValid(quint16 listcount, quint16 *curIndex){
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

bool GrButton::prevIndexIsValid(quint16 *curIndex){
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

void GrButton::setNextState(){
    if ( curState < ( stateList.count() - 1 )) curState++;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    curIndexText = stateList.at(curState).indexList.at( state_textIndex );
    curIndexTextPen = stateList.at(curState).indexList.at( state_textPenIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrButton::setPreviousState(){
    if ( curState > 0 ) curState--;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    curIndexText = stateList.at(curState).indexList.at( state_textIndex );
    curIndexTextPen = stateList.at(curState).indexList.at( state_textPenIndex );
    setVisible( stateList.at(curState).isVisible );
}

void GrButton::setCurrentState(quint16 index){
    if ( index < stateList.count()) curState = index;
    else return;
    curIndexBrush = stateList.at(curState).indexList.at( state_backgroundIndex );
    curIndexPen = stateList.at(curState).indexList.at( state_borderIndex );
    curIndexText = stateList.at(curState).indexList.at( state_textIndex );
    curIndexTextPen = stateList.at(curState).indexList.at( state_textPenIndex );
    setVisible( stateList.at(curState).isVisible );
}

GrButton::GrButton(QGraphicsScene *scene) : GrObject(0, scene){
    buttonRect = QRectF( QPointF(0,0), QSize(50, 50) );
    brushList.push_back( QBrush(Qt::blue, Qt::SolidPattern));
    penList.push_back( QPen( Qt::black ));
    textPenList.push_back( QPen( Qt::black ));
    setAcceptHoverEvents( true );
    setFlags( ItemIsMovable | ItemIsSelectable );
    UniqueNumber = idgen->NewID();

    curIndexBrush = 0;
    curIndexPen = 0;
    curIndexText = 0;
    curIndexTextPen = 0;
    TItemState state;
    state.isVisible = true;
    state.name = "default";
    state.indexList.push_back( curIndexBrush );
    state.indexList.push_back( curIndexPen );
    state.indexList.push_back( curIndexText );
    state.indexList.push_back( curIndexTextPen );

    stateList.push_back( state );
    curState = 0;
}

QString GrButton::name(){
    return QString("Button #"+QString().number(SetLSB(UniqueNumber)));//+" unique number #"+QString().number(UniqueNumber));
}


GrButton::~GrButton(){
    deleteUniqueNumber();
}

void GrButton::setRect(QRectF rect){
    prepareGeometryChange();
    buttonRect = rect;
    qDebug()<<rect;
    addText( buttonText.at( curIndexText) );
}

void GrButton::setWidth(qreal w){
    prepareGeometryChange();
    buttonRect.setWidth( w );
    addText( buttonText.at( curIndexText) );
}

void GrButton::setHeight(qreal h){
    prepareGeometryChange();
    buttonRect.setHeight( h );
    textRect.setTopLeft(QPointF( TEXTINDENT , TEXTINDENT));
    addText( buttonText.at( curIndexText) );
}

void GrButton::setTextColor(QColor col, quint16 index){
    textPenList[index].setColor( col );
}

void GrButton::setBackgroundColor(QColor col, quint16 index ){
    brushList[index].setColor( col );
}

void GrButton::setBorderColor(QColor col, quint16 index ){
    penList[index].setColor( col );
}

void GrButton::setTextColor(QList<QColor> list){
    QList<QPen> temp;
    foreach( QColor col, list){
        temp.push_back(QPen(col));
    }
    textPenList = temp;
}

void GrButton::setBackgroundColor(QList<QColor> list){
    QList<QBrush> temp;
    foreach( QColor col, list){
        temp.push_back(QBrush(col));
    }
    brushList = temp;
}

void GrButton::setBorderColor(QList<QColor> list){
    QList<QPen> temp;
    foreach( QColor col, list){
        temp.push_back(QPen(col));
    }
    penList = temp;
}



void GrButton::addBorderColor(QColor col){
    penList.push_back( QPen( col ));
//    brushList.push_back( brushList.at( currentIndex ) );
//    textPenList.push_back( textPenList.at( currentIndex ));
}

void GrButton::addBackgroundColor(QColor col){
    brushList.push_back( QBrush( col ));
//    penList.push_back( penList.at( currentIndex ));
//    textPenList.push_back( textPenList.at( currentIndex ));
}

void GrButton::addTextColor(QColor col){
    textPenList.push_back( QPen( col )) ;
//    brushList.push_back( brushList.at( currentIndex ) );
//    penList.push_back( penList.at( currentIndex ));
}

void GrButton::deleteBorderColor(int index){
    if ( index >=0 && index < penList.count() && penList.count() > 1 ){
        curIndexPen = 0;
        penList.removeAt( index );
        while( stateList.count() != 1 ){
            stateList.removeLast();
        }
    }
}


void GrButton::deleteBackgroundColor(int index){
    if ( index >=0 && index < brushList.count() ){
        curIndexBrush = 0;
        brushList.removeAt( index );
        while( stateList.count() != 1 ){
            stateList.removeLast();
        }
    }
}

void GrButton::deleteTextColor(int index){
    if ( index >=0 && index < textPenList.count() ) {
        curIndexTextPen = 0;
        textPenList.removeAt( index );
        while( stateList.count() != 1 ){ stateList.removeLast(); } curState = 0;
    }
}

bool GrButton::setCurrentIndex(TGrIndexes indexType, quint16 index){
    switch( indexType ){
    case brushIndex: if ( brushList.count() <= index  ) return false; curIndexBrush = index; break;
    case penIndex: if ( penList.count() <= index  ) return false; curIndexPen = index; break;
    case textPenIndex: if ( textPenList.count() <= index  ) return false; curIndexTextPen = index; break;
    case textIndex: if ( buttonText.count() <= index  ) return false; curIndexText = index; break;
    }
}

bool GrButton::setNextIndex(TGrIndexes indexType ){
    bool res;
    switch( indexType ){
    case brushIndex: res = nextIndexIsValid( brushList.count(), &curIndexBrush); break;
    case penIndex: res = nextIndexIsValid( penList.count(), &curIndexPen); break;
    case textPenIndex: res = nextIndexIsValid( textPenList.count(), &curIndexTextPen); break;
    case textIndex: res = nextIndexIsValid( buttonText.count(), &curIndexText); break;
    }
    return res;
}

bool GrButton::setPrevIndex(TGrIndexes indexType){
    bool res;
    switch( indexType ){
    case brushIndex: res = prevIndexIsValid( &curIndexBrush); break;
    case penIndex: res = prevIndexIsValid( &curIndexPen); break;
    case textPenIndex: res = prevIndexIsValid( &curIndexTextPen); break;
    case textIndex: res = prevIndexIsValid( &curIndexText); break;
    }
    return res;
}

void GrButton::setFont(QFont f){
    _font = f;
}

QFont GrButton::font(){
    return _font;
}


QList<QPen> GrButton::textColor(){
    return textPenList;
}

QList<QBrush> GrButton::backgroundColor(){
    return brushList;
}

QList<QPen> GrButton::borderColor(){
    return penList;
}

QStringList GrButton::text(){
    return buttonText;
}

quint16 GrButton::currentIndex(TGrIndexes indexType){
    quint16 res;
    switch( indexType ){
    case brushIndex: res = curIndexBrush; break;
    case penIndex: res = curIndexPen; break;
    case textPenIndex: res = curIndexTextPen; break;
    case textIndex: res = curIndexText; break;
    }
    return res;
}

void GrButton::addText(QString text){
    if ( !buttonText.contains( text ) ){
        buttonText.push_back( text );
    }
    textRect.setRight( buttonRect.right() - TEXTINDENT );
    textRect.setBottom( buttonRect.bottom() - TEXTINDENT );
    textRect.setTopLeft( QPointF( buttonRect.left() + TEXTINDENT, buttonRect.top() + TEXTINDENT ));
}

void GrButton::setText(QString text, quint16 index){
    if ( index < buttonText.count() ) buttonText.replace( index, text );
}

void GrButton::setText(QStringList list){
    buttonText = list;
}

void GrButton::deleteText(quint16 index){
    if ( index < buttonText.count() ) {
        curIndexText = 0;
        buttonText.removeAt( index );
        while( stateList.count() != 1 ){
            stateList.removeLast();
        }
    }
}

QRectF GrButton::boundingRect() const{
    return buttonRect;
}

void GrButton::paint(QPainter *ppainter, const QStyleOptionGraphicsItem *, QWidget *){
    ppainter->save();
    ppainter->setPen( penList.at( curIndexPen ) );
    ppainter->setBrush( brushList.at( curIndexBrush ) );
    ppainter->drawRect( buttonRect );
    ppainter->setPen( textPenList.at( curIndexTextPen ) );
    ppainter->setFont( _font );
    ppainter->drawText( textRect, buttonText.at( curIndexText ) );
    ppainter->restore();
    update();

}

void GrButton::mouseMoveEvent(QGraphicsSceneMouseEvent *pe){
    moveTo( pe->scenePos() );
}

void GrButton::hoverEnterEvent(QGraphicsSceneHoverEvent *pe){
    QApplication::setOverrideCursor( Qt::PointingHandCursor );   
    QColor color = brushList.at(curIndexBrush).color();
    color.setRed( color.red()/2 );
    color.setGreen( color.green()/2 );
    color.setBlue( color.blue()/2 );
    brushList[curIndexBrush].setColor( color );
    QGraphicsItem::hoverEnterEvent( pe );
}

void GrButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *pe){
    QApplication::restoreOverrideCursor();
    QColor color = brushList.at(curIndexBrush).color();
    color.setRed( color.red()*2 );
    color.setGreen( color.green()*2 );
    color.setBlue( color.blue()*2 );
    brushList[curIndexBrush].setColor( color );
    QGraphicsItem::hoverLeaveEvent( pe );
}

void GrButton::moveTo(QPointF pos){
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


