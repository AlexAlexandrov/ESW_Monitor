
#include "mainwindow.h"

#define PROGRAMVERSION 0.19

void MainWindow::AddAction(int id, const QString name, const QString text, const QString shortCut,
                           const QString toolTip, const QString statTip){
  // canreceiver
  _pqaction[id] = new QAction(name,this);
  _pqaction[id]->setText(text);
  _pqaction[id]->setShortcut(QKeySequence(shortCut));
  _pqaction[id]->setToolTip(toolTip);
  _pqaction[id]->setStatusTip(statTip);
  _pqaction[id]->setWhatsThis(statTip);
  //_pqaction[canreceiverId]->setIcon(QPixmap(img4_xpm));
  connect(_pqaction[id], SIGNAL(triggered()), SLOT(slotToolWindowShow()));
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    pSettings = new QSettings("ESW","PC USB-CAN libUSB",this);
    for (int i = 0; i<toolwinEndId; i++) {
      _ptoolWindow[i] = 0;
      _pqaction[i] = 0;
    }

    AddAction(canreceiverId,   "show can message receiver","&Receive CAN message","CTRL+1","CAN Receiver","CAN message Receiver form");
    AddAction(cansenderId,   "Send can message","&Send CAN message","CTRL+4","CAN Sender","CAN message sender form");

    AddAction(TLogCanBusId, "show can messages", "Show CAN bus Messages Log", "CTRL+2", "Log of CAN bus", "Log of CAN Bus");//
    AddAction(monitorPrgId, "Send monitor program", "Send monitor &program to device", "CTRL+3", "Monitor programmator", "Programmator");
    AddAction(setDeviceNumberId, "Change base and phys nums", "&Change Device Numbers", "CTRL+5", "Change device numbers", "Device Number Changer");
    AddAction(canKingdomSetupOpenId,	"CK setup and program",		 "Can&Kingdom setup OpenDB",	"CTRL+SHIFT+K", "cankingdomdb","cankingdom  form");//ALT+C
    AddAction(canKingdomSetupCreateId,	"CK setup and program",		 "&CanKingdom setup CreateDB",	"CTRL+SHIFT+C", "cankingdomdb","cankingdom  form");//ALT+C

    QMenu* pmnuTools = new QMenu("&Programming");
    pmnuTools->addAction(_pqaction[TLogCanBusId]);
    pmnuTools->addAction(_pqaction[canreceiverId]);
    pmnuTools->addSeparator();
    pmnuTools->addAction(_pqaction[setDeviceNumberId]);
    pmnuTools->addAction(_pqaction[cansenderId]);
    pmnuTools->addSeparator();
    pmnuTools->addAction(_pqaction[monitorPrgId] );
    pmnuTools->addSeparator();
    pmnuTools->addAction(_pqaction[canKingdomSetupCreateId]);
    pmnuTools->addAction(_pqaction[canKingdomSetupOpenId]);

    QAction* ActCreateNewScreen = new QAction(tr("Create New Screen"), this);
    ActCreateNewScreen->setShortcut(QKeySequence("CTRL+T"));
    QAction* ActCloseActiveScreen = new QAction(tr("Close Active Screen"), this);
    ActCloseActiveScreen->setShortcut(QKeySequence("CTRL+Q"));
    connect( ActCreateNewScreen, SIGNAL(triggered()), this, SLOT(addNewScreen()));
    connect( ActCloseActiveScreen, SIGNAL(triggered()), this, SLOT(closeActiveScreen()));


    pScreenMenu = new QMenu(tr("&Screen"));
    pScreenMenu->addAction( ActCreateNewScreen );
    pScreenMenu->addAction( ActCloseActiveScreen );

    pScreenMenu->setDisabled( true );

    actCreateProject = new QAction(tr("Create New Project"), this);
    actCreateProject->setShortcut(QKeySequence("CTRL+N"));
    QAction* actSaveProject = new QAction(tr("Save Project"), this);
    actSaveProject->setShortcut(QKeySequence("CTRL+S"));
    QAction* actSaveAsProject = new QAction(tr("Save Project As"), this);
    actSaveAsProject->setShortcut(QKeySequence("CTRL+SHIFT+S"));
    QAction* actOpenProject = new QAction(tr("Open Project"), this);
    actOpenProject->setShortcut(QKeySequence("CTRL+O"));
    actCloseProject = new QAction(tr("Close Project"), this);
    actCloseProject->setShortcut(QKeySequence("CTRL+L"));

    QMenu* projectMenu = new QMenu(tr("&Project"));
    projectMenu->addAction( actCreateProject );
    projectMenu->addAction( actOpenProject );
    projectMenu->addAction( actSaveProject );
    projectMenu->addAction( actSaveAsProject );
    projectMenu->addSeparator();
    projectMenu->addAction( actCloseProject );

    connect(actCreateProject, SIGNAL(triggered()),
            this, SLOT(createNewProject()));
    connect(actOpenProject, SIGNAL(triggered()),
            this, SLOT(openProject()));
    connect(actSaveAsProject, SIGNAL(triggered()),
            this, SLOT(saveProjectAs()));
    connect(actSaveProject, SIGNAL(triggered()),
            this, SLOT(saveProject()));
    connect(actCloseProject, SIGNAL(triggered()),
            this, SLOT(slotCloseProject()));


    actCloseProject->setDisabled( true );

    QMenu* helpMenu = new QMenu( tr("&Help"));
    QAction* actHelp = new QAction(tr("Help"), this );
    QAction* actAbout = new QAction( tr("About"), this );

    helpMenu->addAction( actHelp );
    helpMenu->addAction( actAbout );

    connect(actHelp, SIGNAL(triggered()), this, SLOT(showHelp()));

    menuBar()->addMenu( projectMenu );
    menuBar()->addMenu( pScreenMenu );
    menuBar()->addMenu( pmnuTools );
    menuBar()->addMenu( helpMenu );

    setWindowTitle("ESW Monitor Creator " + QString().number( PROGRAMVERSION ) );
    setAutoFillBackground(true);


    QToolBar* itemToolBar = new QToolBar(tr("items"), this);
    itemToolBar->setContextMenuPolicy( Qt::PreventContextMenu );
    tbAddButton = new QToolButton(this);
    tbAddButton->setText( tr("Add Button") );
    tbAddButton->setDisabled( true );
    tbAddText = new QToolButton(this);
    tbAddText->setText( tr("Add Text") );
    tbAddText->setDisabled( true );
    tbAddEnterArea = new QToolButton(this);
    tbAddEnterArea->setText( tr("Add EnterArea") );
    tbAddEnterArea->setDisabled( true );
    tbAddBar = new QToolButton(this);
    tbAddBar->setText( tr("Add Bar") );
    tbAddBar->setDisabled( true );
    tbAddImage = new QToolButton( this );
    tbAddImage->setText( tr("Add Image"));
    tbAddImage->setDisabled( true );
    tbAddShape = new QToolButton( this );
    tbAddShape->setText( tr("Add Shape"));
    tbAddShape->setDisabled( true );
    tbAnimation = new QToolButton( this );
    tbAnimation->setText( tr("Add Animation"));
    tbAnimation->setDisabled( true );
#ifdef TESTBUTTON
    QToolButton* testTB = new QToolButton( this);
    connect( testTB, SIGNAL(clicked()), this, SLOT(slotTest()));
    testTB->setText( " test ");
    itemToolBar->addWidget( testTB );
#endif
    connect(tbAddButton, SIGNAL(clicked()),
            this, SLOT(addButton()));
    connect(tbAddText, SIGNAL(clicked()),
            this, SLOT(addText()));
    connect(tbAddEnterArea, SIGNAL(clicked()),
            this, SLOT(addEnterArea()));
    connect(tbAddBar, SIGNAL(clicked()),
            this, SLOT(addBar()));
    connect(tbAddImage, SIGNAL(clicked()),
            this, SLOT(addImage()));
    connect(tbAddShape, SIGNAL(clicked()),
            this, SLOT(addShape()));
    connect(tbAnimation, SIGNAL(clicked()),
            this, SLOT(addAnimation()));

    itemToolBar->addWidget( tbAddButton );
    itemToolBar->addWidget( tbAddBar );
    itemToolBar->addWidget( tbAddText );
    itemToolBar->addWidget( tbAddEnterArea );
    itemToolBar->addWidget( tbAddImage);
    itemToolBar->addWidget( tbAddShape);
    itemToolBar->addWidget( tbAnimation );
    itemToolBar->setOrientation( Qt::Vertical );

    addToolBar( Qt::LeftToolBarArea, itemToolBar );

    pScreenArea = new QMdiArea();
    pMainArea = new QMdiArea();

    QMdiSubWindow* propertySubWindow = new QMdiSubWindow( pMainArea );
    tbWidget = new TPropertyWidget(this);
    propertySubWindow->setWidget( tbWidget );
    propertySubWindow->setFixedWidth( 290 );
    propertySubWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    QMdiSubWindow* screenTabsWindow = new QMdiSubWindow( pMainArea );
    screenTabsWindow->setWidget( pScreenArea );
    screenTabsWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    screenTabsWindow->setFixedWidth( 740 );
    twScreenItems = new QTreeWidget( this );
    twScreenItems->setColumnCount( 1 );
    twScreenItems->header()->hide();
    twScreenItems->setAttribute( Qt::WA_DeleteOnClose );

    QHBoxLayout* itemStatePBlay = new QHBoxLayout();
    QVBoxLayout* itemListLay = new QVBoxLayout();

    pbNextItemState = new QPushButton( "Next State", this );
    pbNextItemState->setShortcut( QKeySequence("ctrl+>") );
    pbPrevItemState = new QPushButton( "Previous State", this );
    pbPrevItemState->setShortcut( QKeySequence("ctrl+<") );
    pbNextItemState->setVisible( false );
    pbPrevItemState->setVisible( false );
    itemStatePBlay->addWidget( pbPrevItemState );
    itemStatePBlay->addSpacing( 5 );
    itemStatePBlay->addWidget( pbNextItemState );
    itemListLay->addItem( itemStatePBlay );
    itemListLay->addWidget( twScreenItems );
    QWidget* widget = new QWidget(this);
    widget->setLayout( itemListLay );
    connect( twScreenItems, SIGNAL(itemSelectionChanged()), this, SLOT(slotTreeItemSelected()));
    connect( twScreenItems, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(slotTreeItemDoubleClicked(QTreeWidgetItem*,int)));
    connect( pbNextItemState, SIGNAL(clicked()), this, SLOT(nextItemState()));
    connect( pbPrevItemState, SIGNAL(clicked()), this, SLOT(prevItemState()));

    QMdiSubWindow* itemListSubWindow = new QMdiSubWindow( pMainArea );
    itemListSubWindow->setWidget( widget );
    itemListSubWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint );
    itemListSubWindow->setFixedWidth( 290 );

    pMainArea->addSubWindow( propertySubWindow );
    pMainArea->addSubWindow( itemListSubWindow );
    pMainArea->addSubWindow( screenTabsWindow );
    pMainArea->tileSubWindows();
    setCentralWidget( pMainArea ); // установка рабочей области

    setMaximumWidth( 1130 );
    setMinimumHeight( 800 );
    pScreenArea->setViewMode( QMdiArea::TabbedView);
    //pMdiArea->setTabsClosable( true );
    connect(pScreenArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(setActiveSubWindow(QMdiSubWindow*)));
    connect(tbWidget, SIGNAL(ZValueChanged()), this, SLOT(slotFillTree()));
    activeSubWindow = 0;

    //####################################   Status bar ####################################
        statConnected = new QLabel(this);
        statusBar()->addWidget(statConnected);
        statMessage = new QLabel(this);
        statusBar()->addWidget(statMessage);
        statMessage->setText("no Error!");
        statREC = new QLabel(this);
        statREC->setText("REC: 000");
        statusBar()->addWidget(statREC);
        statTEC = new QLabel(this);
        statTEC->setText("TEC: 000");
        statusBar()->addWidget(statTEC);
        statusBar()->setAutoFillBackground(true);
            //    USB
        USBconnector = LibUSBConnector::GetLibUSBConnector(this);
        USBconnector->USBDeviceChannge();
        #ifdef Q_WS_X11
            connect(USBconnector,SIGNAL(usb_netlink_change()),SLOT(USB_Dev_Notify()));
        #endif
        USB_Dev_Changed();
        pmesList = new QList<int>;
        pcomList = new QList<int>;
        pregList = new QList<int>;
        SubscribeToMes();
        startTimer(READINTERVAL);
        #if defined(Q_WS_WIN)
            ActivateKeyboardLayout( (HKL)HKL_NEXT, 0 );
        #endif
//            LoadKeyboardLayout(QString("00000409"), KLF_ACTIVATE);	// english
        projectPath.clear();

        readSettings();
        toolwindow::setSettings( pSettings );
}

//######################## Settings ########################
void MainWindow::writeSettings(void){
  pSettings->beginGroup("/Settings");
  pSettings->setValue("/width", width());
  pSettings->setValue("/height", height());
  pSettings->endGroup();

}

void MainWindow::readSettings(void){
  pSettings->beginGroup("/Settings");
  int nWidth = pSettings->value("/width", width()).toInt();
  int nHeight = pSettings->value("/height", height()).toInt();
  resize(nWidth, nHeight);
  pSettings->endGroup();
}

//######################## Custom Event ######################
void MainWindow::customEvent(QEvent *e){
    if ( e->type() == (QEvent::Type)EventSelectionChanged){
        SelectedItemChanged* ev = (SelectedItemChanged*)e;

        tbWidget->setActiveObject( ev->Sender);
        int index = pScreenArea->subWindowList().indexOf( activeSubWindow );
        int itemIndex = ScreenList.at(index)->items().indexOf( ev->Sender );
        QTreeWidgetItem* topItem = twScreenItems->topLevelItem( index );
        if ( topItem ){
            topItem->setExpanded( true );
            QTreeWidgetItem* childItem = topItem->child( itemIndex );
            if ( childItem ){
                twScreenItems->setCurrentItem( childItem, 0);
                childItem->setSelected( true );
            }
        }
        configStateButtons( ev->Sender );
    }
    else if ( e->type() == (QEvent::Type)EventDeleteSelection){
        tbWidget->setActiveObject();
        int index = pScreenArea->subWindowList().indexOf( activeSubWindow );
        QTreeWidgetItem* topItem = twScreenItems->topLevelItem( index );
        SelectedDeleted* ev = (SelectedDeleted*)e;
        topItem->removeChild( topItem->child( ev->deletedItemIndex ) );
        currentItem = 0;
    }
}


MainWindow::~MainWindow(){
    
}

void MainWindow::createNewProject(){
    w = new QDialog( this );
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->setWindowTitle(tr( "Choose monitor" ));

    QVBoxLayout* mainlay = new QVBoxLayout( w );
    cbChooseMonitor = new QComboBox( w );
    cbChooseMonitor->addItem(tr("Small Monitor 320x240"));
    cbChooseMonitor->addItem(tr("Touch Monitor 640x480"));
    cbChooseMonitor->setAttribute( Qt::WA_DeleteOnClose );

    QPushButton* pbOk = new QPushButton( "&Ok", w );
    QPushButton* pbCancel = new QPushButton( "&Cancel", w );
    pbOk->setAttribute(Qt::WA_DeleteOnClose);
    pbCancel->setAttribute(Qt::WA_DeleteOnClose);
    QHBoxLayout* pblay = new QHBoxLayout();
    pblay->addWidget( pbOk );
    pblay->addSpacing( 5 );
    pblay->addWidget( pbCancel );

    QLabel* label = new QLabel("Choose monitor:", w);
    label->setAttribute( Qt::WA_DeleteOnClose );

    mainlay->addWidget(label);
    mainlay->addWidget(cbChooseMonitor);
    mainlay->addSpacing(5);
    mainlay->addItem(pblay);

    w->setLayout( mainlay );

    w->show();
    connect(pbOk, SIGNAL(clicked()), this, SLOT(slotCreateNewProjectOkClicked()));
    connect(pbCancel, SIGNAL(clicked()), w, SLOT(close()));
}

void MainWindow::slotCreateNewProjectOkClicked(){
    if ( cbChooseMonitor->currentIndex() == 0 ){
        screenSize.setHeight( SMALLHEIGHT );
        screenSize.setWidth( SMALLWIDTH );
    }
    else{
        screenSize.setHeight( TOUCHHEIGHT );
        screenSize.setWidth( TOUCHWIDTH );
    }
    w->close();
    delete cbChooseMonitor;
    projectPath.clear();
    pScreenMenu->setEnabled( true );
    actCloseProject->setEnabled( true );
    actCreateProject->setDisabled( true );
    addNewScreen();
}

void MainWindow::openProject(){
    if ( !ScreenList.isEmpty() ){
        slotCloseProject();
    }
    else{
        QString path;// = "213.xml";
    #ifdef Q_WS_WIN
        path = QFileDialog::getOpenFileName(this, "Open project", QDir::homePath(), "*.xml");
    #else
        TGetFileWindow* window = new TGetFileWindow();
        path = window->getOpenFileWindow("*.xml", QDir::homePath(), "Open project").first();
    #endif
        if (path.isEmpty()) return;
        if (!SaveOpenXML::getXML(this)->openProject( path, &ScreenList)) return;
        screenSize = ScreenList.first()->sceneRect().size().toSize();
        pScreenMenu->setEnabled( true );
        actCloseProject->setEnabled( true );
        actCreateProject->setDisabled( true );
        foreach(MonitorScreen* scr, ScreenList){
            addOpenedScreen( scr );
        }
        foreach(MonitorScreen* scr, ScreenList){
            int index = ScreenList.indexOf( scr );
            foreach( GrObject* grit, scr->objects() ){
                addToTreeWidget( index, grit);
            }
        }
        projectPath = path;
    }
}

void MainWindow::addOpenedScreen(MonitorScreen *Screen){
    QMdiSubWindow* sub = new QMdiSubWindow( pScreenArea );
    QGraphicsView* pView = new QGraphicsView(Screen);
    pView->setRenderHint(QPainter::SmoothPixmapTransform, true);  //Antialiasing
    pView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pView->setAlignment(Qt::AlignCenter);
    pView->setAttribute(Qt::WA_DeleteOnClose );

    sub->setWidget(pView);
    sub->setAttribute(Qt::WA_DeleteOnClose);
    sub->setWindowFlags(Qt::FramelessWindowHint);
    sub->setWindowTitle(Screen->name());
    pScreenArea->addSubWindow( sub );
    pView->showNormal();
    sub->setToolTip( QString().number(ScreenList.count()) );
    pViewList.push_back( pView );
    pScreenArea->setActiveSubWindow( sub );
    QStringList strList;
    strList<<sub->windowTitle();
    QTreeWidgetItem* it = new QTreeWidgetItem( twScreenItems, strList );
    twScreenItems->addTopLevelItem( it );

    tbAddButton->setEnabled( true );
    tbAddBar->setEnabled( true );
    tbAddText->setEnabled( true );
    tbAddEnterArea->setEnabled( true );
    tbAddImage->setEnabled( true );
    tbAddShape->setEnabled( true );
    tbAnimation->setEnabled( true );
}

void MainWindow::saveProject(){
//    qDebug()<<projectPath.isEmpty();
    if ( projectPath.isEmpty() ) saveProjectAs();
    else if (!SaveOpenXML::getXML(this)->saveProject( projectPath, ScreenList)){
        QMessageBox::critical(this, "Error in save operation", " Can't save file", QMessageBox::Ok, QMessageBox::NoButton);
    }

}

void MainWindow::saveProjectAs(){
    QString path;// = "213.xml";
#ifdef Q_WS_WIN
    path = QFileDialog::getSaveFileName(this, "Save project as", QDir::homePath(), "*.xml");
#else
    TGetFileWindow* window = new TGetFileWindow();
    path = window->getSaveFileWindow("*.xml", QDir::homePath(), "Save project as");
#endif
    if (path.isEmpty()) return;
    if ( !path.endsWith(".xml")) path.append(".xml");
    if (!SaveOpenXML::getXML(this)->saveProject( path, ScreenList)){
        QMessageBox::critical(this, "Error in save operation", " Can't save file", QMessageBox::Ok, QMessageBox::NoButton);
    }
    else projectPath = path;
}

void MainWindow::slotCloseProject(){
    tbWidget->clear();
    foreach( QMdiSubWindow* sub, pScreenArea->subWindowList()){
        closeScreen( sub );
    }
    pScreenMenu->setDisabled( true );
    actCloseProject->setDisabled( true );
    actCreateProject->setDisabled( false );
    projectPath.clear();
}

void MainWindow::addNewScreen( ) {
    QMdiSubWindow* sub = new QMdiSubWindow( pScreenArea );
    MonitorScreen* Screen = new MonitorScreen(this , QRectF(0, 0, screenSize.width(), screenSize.height()));
    QGraphicsView* pView = new QGraphicsView(Screen);
    pView->setRenderHint(QPainter::SmoothPixmapTransform, true);  //Antialiasing
    pView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    pView->setAlignment(Qt::AlignCenter);
    pView->setAttribute(Qt::WA_DeleteOnClose );

    sub->setWidget(pView);
    sub->setAttribute(Qt::WA_DeleteOnClose);
    sub->setWindowFlags(Qt::FramelessWindowHint);
    sub->setWindowTitle(Screen->name());
    pScreenArea->addSubWindow( sub );
    pView->showNormal();
    sub->setToolTip( QString().number(ScreenList.count()) );
    pViewList.push_back( pView );
    ScreenList.push_back( Screen );
    pScreenArea->setActiveSubWindow( sub );
    QStringList strList;
    strList<<sub->windowTitle();
    QTreeWidgetItem* it = new QTreeWidgetItem( twScreenItems, strList );
    twScreenItems->addTopLevelItem( it );

    tbAddButton->setEnabled( true );
    tbAddBar->setEnabled( true );
    tbAddText->setEnabled( true );
    tbAddEnterArea->setEnabled( true );
    tbAddImage->setEnabled( true );
    tbAddShape->setEnabled( true );
    tbAnimation->setEnabled( true );
}


//------------------------- set/close subwindows ------------------------------
void MainWindow::setActiveSubWindow(QMdiSubWindow *window){
    activeSubWindow = window;
}

void MainWindow::closeActiveScreen(){
    if ( activeSubWindow != 0 ) closeScreen( activeSubWindow );
}

void MainWindow::closeScreen( QMdiSubWindow* sub ){
    int index = pScreenArea->subWindowList().indexOf( sub );
    sub->close();
    delete pViewList[ index ];
    pViewList.removeAt( index );
    QList<QGraphicsItem*> itlist = ScreenList.at( index )->items();
    foreach( QGraphicsItem* item, itlist){
        delete item;
    }
    delete ScreenList[ index ];
    ScreenList.removeAt( index );

    twScreenItems->clearSelection();
    delete  twScreenItems->takeTopLevelItem( index );

    if ( pScreenArea->subWindowList().count() ) activeSubWindow = pScreenArea->subWindowList().last();
    else{
        tbAddShape->setDisabled( true );
        tbAddImage->setDisabled( true );
        tbAddBar->setDisabled( true );
        tbAddButton->setDisabled( true );
        tbAddText->setDisabled( true );
        tbAddEnterArea->setDisabled( true );
        tbAnimation->setDisabled( true );
        activeSubWindow = 0;
    }
}

//-------------------- items SLOTS --------------------------------------------
void MainWindow::addToTreeWidget(quint16 index, GrObject *grit){
    QTreeWidgetItem* trit = twScreenItems->topLevelItem( index );
    QStringList strList;
    quint16 objIndex = ScreenList.at( index )->objects().indexOf( grit );
    strList<<grit->name()+" scrNum #" + QString().number(objIndex);
    trit->addChild( new QTreeWidgetItem(strList) );
}

void MainWindow::addButton(){
    quint16 index = pScreenArea->subWindowList().indexOf( activeSubWindow );
    GrButton* grit = ScreenList.at( index )->slotAddButtonItem();
    addToTreeWidget(index, grit);
}

void MainWindow::addText(){
    quint16 index = pScreenArea->subWindowList().indexOf( activeSubWindow );
    GrText* grit = ScreenList.at( index )->slotAddTextItem();
    addToTreeWidget(index, grit);
}

void MainWindow::addBar(){
    quint16 index = pScreenArea->subWindowList().indexOf( activeSubWindow );
    GrBar* grit = ScreenList.at( index )->slotAddBarItem();
    addToTreeWidget(index, grit);}

void MainWindow::addEnterArea(){
    quint16 index = pScreenArea->subWindowList().indexOf( activeSubWindow );
    GrEnterArea* grit = ScreenList.at( index )->slotAddEnterAreaItem();
    addToTreeWidget(index, grit);
}

void MainWindow::addImage(){
    quint16 index = pScreenArea->subWindowList().indexOf( activeSubWindow );
    GrImage* grit = ScreenList.at( index )->slotAddImageItem();
    addToTreeWidget(index, grit);
}

void MainWindow::addShape(){
    quint16 index = pScreenArea->subWindowList().indexOf( activeSubWindow );
    GrShape* grit = ScreenList.at( index )->slotAddShapeItem();
    addToTreeWidget(index, grit);
}

void MainWindow::addAnimation(){
    quint16 index = pScreenArea->subWindowList().indexOf( activeSubWindow );
    GrAnimation* grit = ScreenList.at( index )->slotAddAnimationItem();
    addToTreeWidget(index, grit);
}

//------------------------- States slots --------------------------------------
void MainWindow::configStateButtons(GrObject *item){
    quint16 stateCnt = 0; //[listNum] = curIndex;
    currentItem = item;
    stateCnt = currentItem->states().count();
    if ( stateCnt > 1 ){
        pbNextItemState->setVisible( true );
        pbPrevItemState->setVisible( true );
    }
    else{
        pbNextItemState->setVisible( false );
        pbPrevItemState->setVisible( false );
    }
}

void MainWindow::nextItemState(){
    if ( currentItem == 0 || !pbNextItemState->isVisible() ) return;
    currentItem->setNextState();
}

void MainWindow::prevItemState(){
    if ( currentItem == 0 || !pbPrevItemState->isVisible() ) return;
    currentItem->setPreviousState();
}

void MainWindow::showHelp(){
    w = new QDialog( this);
    w->setAttribute(Qt::WA_DeleteOnClose );
    w->setWindowTitle( tr("Help") );
    QTextEdit* teHelp = new QTextEdit( w );
    QVBoxLayout* mainlay = new QVBoxLayout( w );

    QFile help("help.txt");
    if ( help.open( QIODevice::ReadOnly ) ){
        while(!help.atEnd()){
            QString temp = help.readLine();
            teHelp->setText( teHelp->toPlainText() + temp );
        }
        help.close();
    }
    else{
        teHelp->setText("Error in opening help file");
    }
    teHelp->setReadOnly( true );
    mainlay->addWidget( teHelp );

    w->show();
}

void MainWindow::slotTreeItemSelected(){
    if ( twScreenItems->selectedItems().isEmpty() ) return;
    QTreeWidgetItem* twit = twScreenItems->selectedItems().first();
    QTreeWidgetItem* TwParent = twit->parent();
    if ( !TwParent ) return;
    int childIndex = TwParent->indexOfChild( twit );
    int index = twScreenItems->indexOfTopLevelItem( TwParent );
    pScreenArea->setActiveSubWindow( pScreenArea->subWindowList().at( index ));
    QGraphicsItem* item = ScreenList.at( index )->items().at( childIndex);
    item->setSelected( true );
    tbWidget->setActiveObject( item );
}

void MainWindow::slotTreeItemDoubleClicked(QTreeWidgetItem *twit, int curIndex){
    QTreeWidgetItem* TwParent = twit->parent();
    if ( !TwParent ) return;
    int childIndex = TwParent->indexOfChild( twit );
    int index = twScreenItems->indexOfTopLevelItem( TwParent );
    pScreenArea->setActiveSubWindow( pScreenArea->subWindowList().at( index ));
    GrObject* item =(GrObject*) ScreenList.at( index )->items().at( childIndex);
    item->setSelected( true );
    ScreenList.at( index )->configItemsState( item );
}

void MainWindow::slotFillTree(){
    twScreenItems->clear();
    foreach( MonitorScreen* scr, ScreenList){
        QStringList strList;
        strList<<scr->name();
        QTreeWidgetItem* trit = new QTreeWidgetItem( twScreenItems, strList );
        twScreenItems->addTopLevelItem( trit );
        foreach(QGraphicsItem* it, scr->items()){
            GrObject* obj = (GrObject*)it;
            QStringList str;
            str<<obj->name();
            trit->addChild( new QTreeWidgetItem(str) );
        }
    }
}

//############################# Tool windows (plugins)####################################
void MainWindow::slotToolWindowShow(){
    for(int i = 0; i<toolwinEndId; i++)
            if( sender()== _pqaction[i]){
                getToolWindow(i)->show();
                getToolWindow(i)->deviceChanged(USBconnected);
                return;
            }
}

toolwindow *MainWindow::getToolWindow(int wId){
    if(!_ptoolWindow[wId]){
        switch(wId){
            case canKingdomSetupCreateId: _ptoolWindow[wId] = new CanKingdomTuner(this, true); break; //CanKingdomTuner
            case canKingdomSetupOpenId: _ptoolWindow[wId] = new CanKingdomTuner(this, false); break; //CanKingdomTuner
            case canreceiverId: _ptoolWindow[wId] = new CANreceiver(this); break;
            case cansenderId: _ptoolWindow[wId] = new CANsender(this); break;
            case TLogCanBusId: _ptoolWindow[wId] = new TLogCanBus( this ); break;
            case monitorPrgId: _ptoolWindow[wId] = new MonitorProgrammer( ScreenList, this ); break;
            case setDeviceNumberId: _ptoolWindow[wId] = new SetDeviceNumberWindow( this ); break;
        }
        _ptoolWindow[wId]->setModal(false);
        connect(_ptoolWindow[wId], SIGNAL(finished(int)), SLOT(slotToolWindowClose()));
    }
    return _ptoolWindow[wId];
}

void MainWindow::SubscribeToMes()
{
    pmesList->clear();
    pcomList->clear();
    pregList->clear();
    (*pmesList) << canreceiverId << TLogCanBusId << monitorPrgId << setDeviceNumberId// << TestButtonsWindowId;
                << canKingdomSetupCreateId << canKingdomSetupOpenId;
    (*pcomList) << canerrorId;
}

void MainWindow::slotToolWindowClose(){
    setWindowTitle("ESW Monitor Editor");
    for(int i = 0; i < toolwinEndId; i++)
            if( sender()== _ptoolWindow[i]){
                _ptoolWindow[i]->hide();
                _ptoolWindow[i] = 0;
                break;
            }
        USBconnector->clearAutoSending();
        for(int i =0; i<toolwinEndId; i++) if(_ptoolWindow[i]) _ptoolWindow[i]->startAutoSending();
}

#ifdef TESTBUTTON

void MainWindow::slotTest(){
    quint16 size;
    StructureCreator::getCreator()->calcControllerStructure( ScreenList, 0x800, 0x00, &size );
//    GlobalStateWindow* gsW = new GlobalStateWindow( &ScreenList, this );
//    gsW->show();
    SaveOpenXML::getXML(this)->saveProject( "213.xml", ScreenList);

//    qDebug()<<SaveOpenXML::getXML()->openProject( "213.xml", &ScreenList );
}
#endif
//############################# Libusb special function ####################################
void MainWindow::USB_Dev_Changed()
{
    if( USBconnector->USBConnectedStatus()) {
            statConnected->setText("Connected!");
            //statDrvRes->setEnabled(true);
            USBconnected = true;
    }
    else {
            statConnected->setText("Don Connected!");
            //statDrvRes->setEnabled(false);
            USBconnected = false;
    }
    for(int i=0; i<toolwinEndId; i++){
            if(_ptoolWindow[i]) _ptoolWindow[i]->deviceChanged(USBconnected);
    }
}

#if defined(Q_WS_WIN)
bool MainWindow::winEvent ( MSG * msg, long int * result ) {
    int msgType = msg->message;
    if( msgType ==  WM_DEVICECHANGE ){
            USBconnector->USBDeviceChannge();
            USB_Dev_Changed();
    }
    return false;
}
#endif

#ifdef Q_WS_X11
void MainWindow::USB_Dev_Notify()
{
    USB_Dev_Changed();
}
#endif

void MainWindow::timerEvent(QTimerEvent*){
    int count;
    TCanMessage Mes[MES_ARRAY_SIZE];
    TCanUSBCommand Com[COM_ARRAY_SIZE];
    TCanWriteReg Reg[REG_ARRAY_SIZE];
    QList<int>::const_iterator it;

    // for main window need to cach Command for error and Register for TEC and REC
    canmessageblock mesBlock;
    while(1){
        USBconnector->setBlock(&mesBlock);
        switch(mesBlock.getType()){
        case cmtEmpty:
            return;
            break;
        case cmtMessage:
            count = mesBlock.getMessages(Mes);
            for(int i = 0; i < count; i++){
                    //if( Mes[i].ID == idAlarm ) ShowDrvAlarm(Mes[i].Data[0]);
            }
            it = pmesList->constBegin();
            for (; it != pmesList->constEnd(); ++it)
                if(_ptoolWindow[*it]) {
                     _ptoolWindow[*it]->CANmessages(Mes,count);
                }
            break;
        case cmtCommand:
            count = mesBlock.getCommands(Com);
            getToolWindow(canerrorId)->show();
            it = pcomList->constBegin();
            for (; it != pcomList->constEnd(); ++it)if(_ptoolWindow[*it]) _ptoolWindow[*it]->CANcommands(Com,count);
            break;
        case cmtRegister:
            count = mesBlock.getRegisters(Reg);
            it = pregList->constBegin();
            for (; it != pregList->constEnd(); ++it)if(_ptoolWindow[*it]) _ptoolWindow[*it]->MCPregs(Reg,count);
            for (int i = 0; i < count; i++){
                if (Reg[i].address == REC)   statREC->setText("REC: " + QString::number(Reg[i].data));
                if (Reg[i].address == TEC)   statTEC->setText("TEC: " + QString::number(Reg[i].data));
            }
            break;
        }
        mesBlock.clear();
        repaint();
    }
}

