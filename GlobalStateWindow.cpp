#include "GlobalStateWindow.h"

GlobalStateWindow::GlobalStateWindow(QList<MonitorScreen*> *scrList, QWidget* _parent ) : QDialog(_parent){
    screenList = scrList;
    setWindowTitle( "Monitor Global States setup");
    QHBoxLayout* mainlay = new QHBoxLayout( this );
    QVBoxLayout* lay_GS = new QVBoxLayout();        // global states list and button's lay lay
    QVBoxLayout* lay_obj = new QVBoxLayout();       // objects list and screens list lay
    QHBoxLayout* lay_pb = new QHBoxLayout();        // button lay

    pbAdd = new QPushButton( "&Add ", this );
    pbRemove = new QPushButton( "&Remove", this );
    pbRename = new QPushButton( "Re&name", this );

    pbRemove->setDisabled( true );
    pbRename->setDisabled( true );


    lwGS = new QListWidget( this );
    lwObjects = new QListWidget( this );
    lwObjStates = new QListWidget( this );
    lwGS->setSelectionMode( QAbstractItemView::SingleSelection );
    lwObjects->setSelectionMode( QAbstractItemView::SingleSelection );
    lwObjStates->setSelectionMode( QAbstractItemView::SingleSelection );

    cbScreens = new QComboBox( this );
    cbScreens->setDisabled( true );
    cbScreens->addItem("-----");
    if ( !screenList->isEmpty() ){
        allowObjSlot = false;
        foreach( MonitorScreen* scr, *screenList ){
            cbScreens->addItem( scr->name() );
        }
    }

    lay_pb->setSpacing( 10 );
    lay_pb->addWidget( pbAdd );
    lay_pb->addWidget( pbRemove );
    lay_pb->addWidget( pbRename );

    lay_GS->setSpacing( 5 );
    lay_GS->addItem( lay_pb );
    lay_GS->addWidget( lwGS );

    lay_obj->setSpacing( 5 );
    lay_obj->addWidget( cbScreens );
    lay_obj->addWidget( lwObjects );

    mainlay->setSpacing( 10 );
    mainlay->addItem( lay_GS );
    mainlay->addItem( lay_obj );
    mainlay->addWidget( lwObjStates );

    pbAdd->setAttribute( Qt::WA_DeleteOnClose );
    pbRemove->setAttribute( Qt::WA_DeleteOnClose );
    pbRename->setAttribute( Qt::WA_DeleteOnClose );
    lwGS->setAttribute( Qt::WA_DeleteOnClose );
    lwObjects->setAttribute( Qt::WA_DeleteOnClose );
    lwObjStates->setAttribute( Qt::WA_DeleteOnClose );
    cbScreens->setAttribute( Qt::WA_DeleteOnClose );

    connect(pbAdd, SIGNAL(clicked()), this, SLOT(slotAddGS()));
    connect(pbRename, SIGNAL(clicked()), this, SLOT(slotRenameGS()));
    connect(pbRemove, SIGNAL(clicked()), this, SLOT(slotRemoveGS()));
    connect(cbScreens, SIGNAL(currentIndexChanged(int)), this, SLOT(slotScreenChanged(int)));
    connect(lwObjects, SIGNAL(currentRowChanged(int)), this, SLOT(slotObjectChanged(int)));
    allowObjSlot = true;
}

void GlobalStateWindow::slotAddGS(){
    static int i = 0;
    lwGS->addItem(new QListWidgetItem("Global state #"+QString().number(i)));
    i++;
    pbRemove->setEnabled( true );
    pbRename->setEnabled( true );
    cbScreens->setEnabled( true );
}

void GlobalStateWindow::slotRemoveGS(){
    lwObjects->clear();
    lwObjStates->clear();
    QListWidgetItem* item = lwGS->takeItem( lwGS->currentRow() );
    delete item;
    if ( lwGS->count() == 0 ){
        pbRemove->setDisabled( true );
        pbRename->setDisabled( true );
        cbScreens->setDisabled( true );
    }
}

void GlobalStateWindow::slotRenameGS(){
    win = new QDialog( this );
    win->setWindowTitle("Set global state name");
    win->setAttribute( Qt::WA_DeleteOnClose );
    QVBoxLayout* mainlay = new QVBoxLayout( win );
    QHBoxLayout* pbLay = new QHBoxLayout();

    leName = new QLineEdit( win );
    QPushButton* pbOk = new QPushButton("&Ok",win );
    QPushButton* pbCancel = new QPushButton("&Cancel", win );
    pbLay->addWidget( pbOk );
    pbLay->addSpacing( 5 );
    pbLay->addWidget( pbCancel );
    QLabel* label = new QLabel("Set new name of the global state:",this);

    mainlay->addWidget( label );
    mainlay->addWidget( leName );
    mainlay->addItem( pbLay );

    pbOk->setAttribute( Qt::WA_DeleteOnClose );
    pbCancel->setAttribute( Qt::WA_DeleteOnClose );
    leName->setAttribute( Qt::WA_DeleteOnClose );
    label->setAttribute( Qt::WA_DeleteOnClose );

    connect( pbOk, SIGNAL(clicked()), this, SLOT(slotOkPressed()));
    connect( pbCancel, SIGNAL(clicked()), this, SLOT(slotCancelPressed()));
    win->show();
}

void GlobalStateWindow::slotOkPressed(){
    lwGS->selectedItems().first()->setText( leName->text() );
    delete leName;
    win->close();
}

void GlobalStateWindow::slotCancelPressed(){
    delete leName;
    win->close();
}

void GlobalStateWindow::slotScreenChanged( int index ){
    allowObjSlot = false;
    lwObjects->clear();
    lwObjStates->clear();
    lwObjects->clearSelection();
    lwObjStates->clearSelection();
    if ( index != 0 ){
        index--;
        QList<QGraphicsItem*> objList = screenList->at(index)->items();
        foreach(QGraphicsItem* it, objList){
            GrObject* obj = (GrObject*)it;
            if ( obj->states().count() > 1 ) lwObjects->addItem( new QListWidgetItem( obj->name() ));
        }
    }
    allowObjSlot = true;
}

void GlobalStateWindow::slotObjectChanged( int row ){
    if ( !allowObjSlot ) return;
    MonitorScreen* scr = screenList->at( cbScreens->currentIndex() - 1 );
    GrObject* obj = (GrObject*)scr->items().at( row );
    lwObjStates->clear();
    foreach( TItemState state, obj->states() ){
        lwObjStates->addItem( new QListWidgetItem( state.name ) );
    }
}
