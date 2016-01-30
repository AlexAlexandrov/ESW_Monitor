#include "MonitorScreen.h"
#include "FontConverter.h"

MonitorScreen::MonitorScreen(QObject* Parent , QRectF sceneRectangle) : QGraphicsScene( Parent ){
    setSceneRect( sceneRectangle );
    Program = Parent;
    screenRect = sceneRectangle;
    setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));
    QGraphicsRectItem* pRect = addRect( sceneRect(), QPen(Qt::black));
    pRect->setZValue( -50 );
    pRect->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    screenArea = pRect;
    nextClickAddItem = false;
    creationItem = 0;
    resizer = 0;
    w = 0;
    itemsList = new QVector<GrObject*>;
    static int i = 0;
    _name = "Screen #"+QString().number( i );
    i++;

//    slotAddAnimationItem();
    timer = new QTimer( this );
    timer->setInterval( TIMERINTERVAL );
    connect( timer, SIGNAL(timeout()), this, SLOT(onTimerShot()));
    timer->start();
}

void MonitorScreen::mousePressEvent(QGraphicsSceneMouseEvent *event){
    clearSelection();
    if ( itemAt(event->scenePos())){
        itemAt(event->scenePos())->setSelected( true );
    }
    QGraphicsScene::mousePressEvent(event);
}

void MonitorScreen::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    clearSelection();
    if ( itemAt(event->scenePos())){
        if ( event->button() == Qt::RightButton && itemAt( event->scenePos())->type() > USERTYPE )
            configItemsState( (GrObject*)itemAt(event->scenePos()) );
        else if ( event->button() == Qt::RightButton ){
            QColor col = QColorDialog::getColor();
            if ( col.isValid()) screenArea->setBrush( QBrush(col) );
            QGraphicsScene::mouseReleaseEvent( event );
            return;
        }
    }
    if ( Program != 0 && itemAt(event->scenePos())){
        if ( itemAt(event->scenePos())->type() > USERTYPE ){
            SelectedItemChanged* selEvent = new SelectedItemChanged((GrObject*)itemAt(event->scenePos()));
            QApplication::sendEvent( Program ,(QEvent*)selEvent );
            delete selEvent;
            clearSelection();
            itemAt(event->scenePos())->setSelected(true);
        }
    }
    if ( nextClickAddItem ){
        nextClickAddItem = false;
        creationItem->setSelected( true );
        SelectedItemChanged* selEvent = new SelectedItemChanged( (GrObject*)creationItem );
        QApplication::sendEvent( Program ,(QEvent*)selEvent );
        delete selEvent;
    }
//    QGraphicsScene::mouseReleaseEvent( event );
}

void MonitorScreen::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    if ( itemAt(event->scenePos())){
        if ( itemAt(event->scenePos())->type() == GrResizerType ){
            clearSelection();
            delete resizer;
            resizer = 0;
            update();

        }
    }
    else return;

}

void MonitorScreen::keyReleaseEvent(QKeyEvent *ke){
    if (ke->key() == Qt::Key_Delete && !selectedItems().isEmpty()){
        if (selectedItems().first()->type() == GrResizerType ) return;
        int index = items().indexOf( selectedItems().first());
        itemsList->remove( index );
        SelectedDeleted* selEvent = new SelectedDeleted( index );
        QApplication::sendEvent( Program ,(QEvent*)selEvent );
        delete selEvent;
        delete selectedItems().first();
        clearSelection();
    }
    if ( resizer != 0 && ke->key() == Qt::Key_Escape){
        resizer->abortResize();
        delete resizer;
        resizer = 0;
    }
    if ( ke->modifiers() == Qt::CTRL && ke->key() == Qt::Key_R && resizer == 0 && selectedItems().count() ){
        int _type = selectedItems().first()->type();
        if ( _type > USERTYPE && _type != GrTextType )
             resizer = new GrResizer( selectedItems().first(), this );
    }
    int speed;
    if ( ke->modifiers() == Qt::SHIFT ) speed = 1;
    else if ( ke->modifiers() == Qt::CTRL) speed = 10;
    else speed = 5;
    switch( ke->key() ){
    case Qt::Key_Up: moveItemTo( dirUp, speed ); break;
    case Qt::Key_Down: moveItemTo( dirDown, speed ); break;
    case Qt::Key_Left: moveItemTo( dirLeft, speed ); break;
    case Qt::Key_Right: moveItemTo( dirRight, speed ); break;
    }
    update();
}

bool lessThenZ(GrObject *a, GrObject *b){
    return a->zValue() < b->zValue();
}

QList<QGraphicsItem *> MonitorScreen::items() const{
    qSort( itemsList->begin(), itemsList->end(), lessThenZ);
    QList<QGraphicsItem*> list;
    foreach( GrObject* obj, *itemsList){
        list.push_back( (QGraphicsItem*)obj );
    }
    return list;
}

QVector<GrObject *> MonitorScreen::objects() const{
    qSort( itemsList->begin(), itemsList->end(), lessThenZ);
    return *itemsList;
}

void MonitorScreen::addObject(GrObject *obj){
    itemsList->push_back( obj );
    addItem( obj );
}

GrButton *MonitorScreen::slotAddButtonItem(){
    nextClickAddItem = true;
    GrButton* it;
    it = new GrButton(this);
    it->addText( "Button" );
    it->setHeight( 30 );
    creationItem = it;
    creationItem->setSelected( true );
    itemsList->push_back(it);
    return it;
}

GrBar *MonitorScreen::slotAddBarItem(){
    nextClickAddItem = true;
    GrBar* it;
    it = new GrBar( this );
    it->setRect( 0, 0, 40, 40 );
    creationItem = it;
    creationItem->setSelected( true );
    itemsList->push_back(it);
    return it;
}

GrEnterArea *MonitorScreen::slotAddEnterAreaItem(){
    nextClickAddItem = true;
    GrEnterArea* it;
    it = new GrEnterArea( this );
    it->setRect( 0, 0, 100, 30 );
    it->setTextValidator( 100, 150 );
    it->setText( "111" );
    it->addValidTextColor( Qt::darkGreen );
    it->addInvalidTextColor( Qt::darkRed );
    it->addBackgroundColor( Qt::lightGray );
    it->addBorderColor( Qt::black );
    creationItem = it;
    creationItem->setSelected( true );
    itemsList->push_back(it);
    return it;
}

GrText* MonitorScreen::slotAddTextItem(){
    nextClickAddItem = true;
    GrText* it = new GrText( this );
    creationItem = it;
    creationItem->setSelected( true );
    itemsList->push_back(it);
    return it;
}
GrImage *MonitorScreen::slotAddImageItem(){
    nextClickAddItem = true;
    GrImage* it;
    it = new GrImage( this );
    it->setPos( 10, 10 );
    it->setRect( 0, 0, 16, 16 );
    creationItem = it;
    creationItem->setSelected( true );
    itemsList->push_back(it);
    return it;
}

GrShape *MonitorScreen::slotAddShapeItem(){
    nextClickAddItem = true;
    GrShape* it = new GrShape(0, this);
    it->setObjectShape( shape_ellipse, 0,0, 100, 50 );
    creationItem = it;
    creationItem->setSelected( true );
    itemsList->push_back(it);
    return it;
}

GrAnimation *MonitorScreen::slotAddAnimationItem(){
    nextClickAddItem = true;
    GrAnimation* an = new GrAnimation( this );
    an->setPos( 100, 100 );
    creationItem = an;
    creationItem->setSelected( true );
    itemsList->push_back( an );
    return an;
}

QColor MonitorScreen::backgroundColor(){
    return screenArea->brush().color();
}

void MonitorScreen::setBackgroundColor(QColor col){
    screenArea->setBrush( QBrush(col));
}

QString MonitorScreen::name(){
    return _name;
}

QColor MonitorScreen::colorUnderItem( GrObject *item ){
    GrObject *finded = 0;
    foreach( GrObject* obj, *itemsList ){
        if ( obj->type() == GrShapeType && obj != item ){
            GrShape* shp = (GrShape*)obj;
            if ( shp->path().contains( item->boundingRect() )) finded = obj;
        }
        else if ( obj->boundingRect().contains(item->boundingRect()) && obj != item ) finded = obj;
    }
    if ( finded == 0 ) return screenArea->brush().color();
    QColor col;
    switch( finded->type() ){
        case GrBarType:{
            GrBar* item = (GrBar*)finded;
            col = item->backgroundColor().at(item->states().first().indexList.at(GrBar::state_backgroundIndex)).color();
        }break;
        case GrButtonType:{
            GrButton* item = (GrButton*)finded;
            col = item->backgroundColor().at(item->states().first().indexList.at(GrButton::state_backgroundIndex)).color();
        }break;
        case GrShapeType:{
            GrShape* item = (GrShape*)finded;
            col = item->brush().at(item->states().first().indexList.at(GrBar::state_backgroundIndex)).color();
        }break;
    }
    return col;
}

void MonitorScreen::lwCurrrentIndexChanged( int curIndex ){
    if ( curIndex < 0 ) return;
    QListWidget* listWidget = (QListWidget*)sender();
    quint16 listNum = lwList.indexOf( listWidget );
    labelList.at( listNum )->setText( listWidget->currentItem()->text() );
    TItemState state = confItem->currentState(); //[listNum] = curIndex;
    state.indexList.replace( listNum, curIndex);
    confItem->replaceCurState( state );
}

void MonitorScreen::cbCurrentIndexChanged( int curIndex ){
    TItemState state;
    confItem->setCurrentState( curIndex );
    state = confItem->currentState();
    chbInvis->setChecked( state.isVisible );
    for(int i = 0; i < state.indexList.count(); i++){
        lwList[i]->setCurrentRow( state.indexList.at(i) );
    }
    if ( chbInvis->isChecked() ){
        foreach( QLabel* lab, labelList){
            lab->clear();
        }
    }
}

void MonitorScreen::setItemInvisible( bool status ){
    quint16 curIndex = cbState->currentIndex();
    TItemState state = confItem->currentState(); //[listNum] = curIndex;
    state.isVisible = status;
    confItem->replaceCurState( state );
    confItem->setCurrentState( curIndex );
    confItem->setVisible( status );
}

void MonitorScreen::addStateClicked(){
    quint16 curIndex = cbState->currentIndex();
    TItemState state = confItem->currentState();
    state.name = " state #"+ QString().number( cbState->count());
    confItem->addState( state );
    cbState->addItem( state.name );
    cbState->setCurrentIndex( cbState->count() - 1);
}

void MonitorScreen::deleteStateClicked(){
    quint16 curStateIndex = confItem->currentStateIndex();
    if ( !curStateIndex ) return;
    confItem->setCurrentState( 0 );
    confItem->deleteState( curStateIndex );
    cbState->removeItem( curStateIndex );
}

void MonitorScreen::renameStateClicked(){
    pdRenameState = new QDialog( w );
    QVBoxLayout* mainlay = new QVBoxLayout( pdRenameState );
    QHBoxLayout* pblay = new QHBoxLayout();
    QPushButton* pbOk = new QPushButton("&Rename", pdRenameState );
    QPushButton* pbCancel = new QPushButton("&Cancel", pdRenameState );
    connect( pbOk, SIGNAL(clicked()), this, SLOT(renameOkClicked()));
    connect( pbCancel, SIGNAL(clicked()), pdRenameState, SLOT(close()));
    pbOk->setAttribute( Qt::WA_DeleteOnClose );
    pbCancel->setAttribute( Qt::WA_DeleteOnClose );
    pdRenameState->setAttribute( Qt::WA_DeleteOnClose );

    pdRenameState->setWindowTitle( "Rename State");

    leStateName = new QLineEdit( pdRenameState );

    pblay->addWidget( pbOk );
    pblay->addSpacing( 5 );
    pblay->addWidget( pbCancel );

    mainlay->addWidget( leStateName );
    mainlay->addItem( pblay );

    pdRenameState->exec();
}

void MonitorScreen::renameOkClicked(){
    if ( !leStateName->text().isEmpty() ){
        quint16 curStateIndex = confItem->currentStateIndex();
        if ( !curStateIndex ) return;
        confItem->renameState( leStateName->text() );
        cbState->setItemText( curStateIndex, leStateName->text());
        pdRenameState->close();
    }
    else{
        QMessageBox::critical( 0, "Name error", "Name should contains at least 1 character", QMessageBox::Ok, QMessageBox::NoButton );
    }
}

void MonitorScreen::deleteDialogWindow(){
    w = 0;
}

void MonitorScreen::onTimerShot(){
    foreach( GrObject* obj, *itemsList){
        if ( obj->type() == GrAnimationType){
            GrAnimation* grAn = (GrAnimation*)obj;
            grAn->notifyTimer( timer->interval() );
        }
    }
}

void MonitorScreen::configItemsState( GrObject *item ){
    if ( w ) return;
    lwList.clear();
    labelList.clear();
    w = new QDialog( );
    w->setAttribute( Qt::WA_DeleteOnClose );
    confItem = item;
    QHBoxLayout* listLay = new QHBoxLayout( );
    QHBoxLayout* setupLay = new QHBoxLayout( );
    QVBoxLayout* mainlay = new QVBoxLayout( w );

    cbState = new QComboBox( w );
    chbInvis = new QCheckBox( w );
    chbInvis->setText("Visible");
    connect(chbInvis, SIGNAL(toggled(bool)), this, SLOT(setItemInvisible(bool)));
    setupLay->addWidget( cbState );
    setupLay->addWidget( chbInvis );
    QHBoxLayout* pbLay = new QHBoxLayout();
    QPushButton* pbAddState = new QPushButton("&Add State", w);
    QPushButton* pbRenameState = new QPushButton("&Rename State", w);
    QPushButton* pbDeleteState = new QPushButton("&Delete State", w);
    pbAddState->setAttribute( Qt::WA_DeleteOnClose );
    pbRenameState->setAttribute( Qt::WA_DeleteOnClose );
    pbDeleteState->setAttribute( Qt::WA_DeleteOnClose );
    pbLay->addWidget( pbAddState );
    connect( pbAddState, SIGNAL(clicked()), this, SLOT(addStateClicked()));
    connect( pbRenameState, SIGNAL(clicked()), this, SLOT(renameStateClicked()));
    connect( pbDeleteState, SIGNAL(clicked()), this, SLOT(deleteStateClicked()));
    pbLay->addSpacing( 5 );
    pbLay->addWidget( pbRenameState );
    pbLay->addSpacing( 5 );
    pbLay->addWidget( pbDeleteState );

    switch( confItem->type()){
        case GrBarType:{
            GrBar* temp = (GrBar*)confItem;
            w->setWindowTitle("Configurate states for "+temp->name() );
            quint16 listsCnt = temp->states().at( defaultState ).indexList.count();
            for( int i = 0; i < listsCnt; i++){
                QListWidget* lw = new QListWidget( w );
                lwList.push_back( lw );
                listLay->addWidget( lw );
                listLay->addSpacing( 5 );
                connect( lw, SIGNAL(currentRowChanged(int)), this, SLOT(lwCurrrentIndexChanged(int)));
                QLabel* lab = new QLabel(w);
                labelList.push_back( lab );
                setupLay->addWidget( lab );
                setupLay->addSpacing( 5 );
            }
            lwList.at( GrBar::state_backgroundIndex )->setToolTip(tr("background colors"));
            foreach( QBrush br, temp->backgroundColor()){
                lwList.at( GrBar::state_backgroundIndex )->addItem( new QListWidgetItem( br.color().name() ));
            }
            lwList.at( GrBar::state_borderIndex )->setToolTip(tr("border colors"));
            foreach( QPen pen, temp->borderColor()){
                lwList.at( GrBar::state_borderIndex )->addItem( new QListWidgetItem( pen.color().name()) );
            }
            foreach( TItemState state, temp->states()){ cbState->addItem( state.name ); }
            cbState->setCurrentIndex( 0 );
        }break;
        case GrButtonType:{
            GrButton* temp = (GrButton*)confItem;
            w->setWindowTitle("Configurate states for "+temp->name() );
            quint16 listsCnt = temp->states().at( defaultState ).indexList.count();
            for( int i = 0; i < listsCnt; i++){
                QListWidget* lw = new QListWidget( w );
                lwList.push_back( lw );
                listLay->addWidget( lw );
                listLay->addSpacing( 5 );
                connect( lw, SIGNAL(currentRowChanged(int)), this, SLOT(lwCurrrentIndexChanged(int)));
                QLabel* lab = new QLabel(w);
                labelList.push_back( lab );
                setupLay->addWidget( lab );
                setupLay->addSpacing( 5 );
            }
            lwList.at( GrButton::state_backgroundIndex )->setToolTip(tr("background colors"));
            foreach( QBrush br, temp->backgroundColor()){
                lwList.at( GrButton::state_backgroundIndex )->addItem( new QListWidgetItem( br.color().name() ));
            }
            lwList.at( GrButton::state_borderIndex )->setToolTip(tr("border colors"));
            foreach( QPen pen, temp->borderColor()){
                lwList.at( GrButton::state_borderIndex )->addItem( new QListWidgetItem( pen.color().name()) );
            }
            lwList.at( GrButton::state_textIndex )->setToolTip(tr("text"));
            foreach( QString str, temp->text()){
                lwList.at( GrButton::state_textIndex )->addItem( new QListWidgetItem( str ));
            }
            lwList.at( GrButton::state_textPenIndex )->setToolTip(tr("text colors"));
            foreach( QPen pen, temp->textColor()){
                lwList.at( GrButton::state_textPenIndex )->addItem( new QListWidgetItem( pen.color().name()) );
            }
            foreach( TItemState state, temp->states()){ cbState->addItem( state.name ); }
            cbState->setCurrentIndex( 0 );
        }break;
        case GrShapeType:{
            GrShape* temp = (GrShape*)confItem;
            w->setWindowTitle("Configurate states for "+temp->name() );
            quint16 listsCnt = temp->states().at( defaultState ).indexList.count();
            for( int i = 0; i < listsCnt; i++){
                QListWidget* lw = new QListWidget( w );
                lwList.push_back( lw );
                listLay->addWidget( lw );
                listLay->addSpacing( 5 );
                connect( lw, SIGNAL(currentRowChanged(int)), this, SLOT(lwCurrrentIndexChanged(int)));
                QLabel* lab = new QLabel(w);
                labelList.push_back( lab );
                setupLay->addWidget( lab );
                setupLay->addSpacing( 5 );
            }
            lwList.at( GrShape::state_backgroundIndex )->setToolTip(tr("background colors"));
            foreach( QBrush br, temp->brush()){
                lwList.at( GrShape::state_backgroundIndex )->addItem( new QListWidgetItem( br.color().name() ));
            }
            lwList.at( GrShape::state_borderIndex )->setToolTip(tr("border colors"));
            foreach( QPen pen, temp->pen()){
                lwList.at( GrShape::state_borderIndex )->addItem( new QListWidgetItem( pen.color().name()) );
            }
            foreach( TItemState state, temp->states()){ cbState->addItem( state.name ); }
            cbState->setCurrentIndex( 0 );
        }break;
        case GrTextType:{
            GrText* temp = (GrText*)confItem;
            w->setWindowTitle("Configurate states for "+temp->name() );
            quint16 listsCnt = temp->states().at( defaultState ).indexList.count();
            for( int i = 0; i < listsCnt; i++){
                QListWidget* lw = new QListWidget( w );
                lwList.push_back( lw );
                listLay->addWidget( lw );
                listLay->addSpacing( 5 );
                connect( lw, SIGNAL(currentRowChanged(int)), this, SLOT(lwCurrrentIndexChanged(int)));
                QLabel* lab = new QLabel(w);
                labelList.push_back( lab );
                setupLay->addWidget( lab );
                setupLay->addSpacing( 5 );
            }
            lwList.at( GrText::state_textIndex )->setToolTip(tr("text"));
            foreach( QString str, temp->text()){
                lwList.at( GrText::state_textIndex )->addItem( new QListWidgetItem( str ));
            }
            lwList.at( GrText::state_textColorIndex )->setToolTip(tr("text colors"));
            foreach( QPen pen, temp->textColor()){
                lwList.at( GrText::state_textColorIndex )->addItem( new QListWidgetItem( pen.color().name()) );
            }
            foreach( TItemState state, temp->states()){ cbState->addItem( state.name ); }
            cbState->setCurrentIndex( 0 );
        }break;
        case GrEnterAreaType:{
            GrEnterArea* temp = (GrEnterArea*)confItem;
            w->setWindowTitle("Configurate states for "+temp->name() );
            quint16 listsCnt = temp->states().at( defaultState ).indexList.count();
            for( int i = 0; i < listsCnt; i++){
                QListWidget* lw = new QListWidget( w );
                lwList.push_back( lw );
                listLay->addWidget( lw );
                listLay->addSpacing( 5 );
                connect( lw, SIGNAL(currentRowChanged(int)), this, SLOT(lwCurrrentIndexChanged(int)));
                QLabel* lab = new QLabel(w);
                labelList.push_back( lab );
                lab->setText( QString().number(i));
                setupLay->addWidget( lab );
                setupLay->addSpacing( 5 );
            }
            lwList.at( GrEnterArea::state_backgroundIndex )->setToolTip(tr("background colors"));
            foreach( QBrush br, temp->backgroundColor()){
                lwList.at( GrEnterArea::state_backgroundIndex )->addItem( new QListWidgetItem( br.color().name() ));
            }
            lwList.at( GrEnterArea::state_borderIndex )->setToolTip(tr("border colors"));
            foreach( QPen pen, temp->borderColor()){
                lwList.at( GrEnterArea::state_borderIndex )->addItem( new QListWidgetItem( pen.color().name()) );
            }
            lwList.at( GrEnterArea::state_validColorIndex )->setToolTip(tr("valid text colors"));
            foreach( QPen pen, temp->validTextColor()){
                lwList.at( GrEnterArea::state_validColorIndex )->addItem( new QListWidgetItem( pen.color().name()) );
            }
            lwList.at( GrEnterArea::state_invalidColorIndex )->setToolTip(tr("invalid text colors"));
            foreach( QPen pen, temp->invalidTextColor()){
                lwList.at( GrEnterArea::state_invalidColorIndex )->addItem( new QListWidgetItem( pen.color().name()) );
            }
            foreach( TItemState state, temp->states()){ cbState->addItem( state.name ); }
            cbState->setCurrentIndex( 0 );
        }break;
        case GrImageType:{
            GrImage* temp = (GrImage*)confItem;
            w->setWindowTitle("Configurate states for "+temp->name() );
            quint16 listsCnt = temp->states().at( defaultState ).indexList.count();
            for( int i = 0; i < listsCnt; i++){
                QListWidget* lw = new QListWidget( w );
                lwList.push_back( lw );
                listLay->addWidget( lw );
                listLay->addSpacing( 5 );
                connect( lw, SIGNAL(currentRowChanged(int)), this, SLOT(lwCurrrentIndexChanged(int)));
                QLabel* lab = new QLabel(w);
                labelList.push_back( lab );
                setupLay->addWidget( lab );
                setupLay->addSpacing( 5 );
            }
            lwList.at( GrImage::state_imageIndex )->setToolTip(tr("image list"));
            foreach( QString str, temp->imagesPath()){
                lwList.at( GrImage::state_imageIndex )->addItem( new QListWidgetItem( str ));
            }
            foreach( TItemState state, temp->states()){ cbState->addItem( state.name ); }
            cbState->setCurrentIndex( 0 );
        }break;
    }
    connect(cbState, SIGNAL(currentIndexChanged(int)), this, SLOT(cbCurrentIndexChanged(int)));
    mainlay->addItem( pbLay );
    mainlay->addSpacing( 10 );
    mainlay->addItem( setupLay );
    mainlay->addSpacing( 10 );
    mainlay->addItem( listLay);

//    QPushButton* pbCloseWindow = new QPushButton( "Close window", w);
//    pbCloseWindow->setAttribute( Qt::WA_DeleteOnClose);
//    QHBoxLayout* pbCloseLay = new QHBoxLayout();
//    pbCloseLay->addSpacing( 150 * lwList.count() );
//    pbCloseLay->addWidget( pbCloseWindow );
//    mainlay->addItem( pbCloseLay );
//    connect( pbCloseWindow, SIGNAL(clicked()), w, SLOT(close()));
//    w->setWindowFlags( Qt::FramelessWindowHint );
    foreach( QListWidget* lw, lwList){
        lw->setAttribute( Qt::WA_DeleteOnClose );
    }
    foreach( QLabel* lab, labelList){
        lab->setAttribute( Qt::WA_DeleteOnClose );
    }


    connect( w, SIGNAL(destroyed()), this, SLOT(deleteDialogWindow()));
    w->show();
}


void MonitorScreen::moveItemTo( MonitorScreen::TMoveDirection dir, int speed ){
    if ( selectedItems().isEmpty()) return;
    QGraphicsItem *item = selectedItems().first();
    QPointF pos = item->scenePos();
    switch( dir ){
        case dirUp: item->setPos( pos.x(), pos.y() - speed ); break;
        case dirDown: item->setPos( pos.x(), pos.y() + speed ); break;
        case dirRight: item->setPos( pos.x() + speed , pos.y() ); break;
        case dirLeft: item->setPos( pos.x() - speed, pos.y()); break;
    }
}

