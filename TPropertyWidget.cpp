#include "TPropertyWidget.h"
#include "TGetFileWindow.h"
TPropertyWidget::TPropertyWidget(QWidget *parent) : QTableWidget(parent){
    buttonPropertyList<<tr("name")<<tr("tooltip")<<tr("position X")<<tr("position Y")<<tr("position Z")<<tr("width")<<tr("height")
                      <<tr("font")<<tr("button text")<<tr("background color")<<tr("border color")
                      <<tr("text color");

    TextPropertyList<<tr("name")<<tr("tooltip")<<tr("position X")<<tr("position Y")<<tr("position Z")
                     <<tr("font")<<tr("text")<<tr("text color")<<tr("font size");//<<tr("font size");

    BarPropertyList<<tr("name")<<tr("tooltip")<<tr("position X")<<tr("position Y")<<tr("position Z")<<tr("width")<<tr("height")
                  <<tr("background color")<<tr("border color");

    EnterAreaPropertyList<<tr("name")<<tr("tooltip")<<tr("position X")<<tr("position Y")<<tr("position Z")<<tr("font")<<tr("text")
                         <<tr("width")<<tr("height")<<tr("background color")<<tr("border color")<<tr("valid text color")
                         <<tr("invalid text color")<<tr("min valid value")<<tr("max valid value");

    ImagePropertyList<<tr("name")<<tr("tooltip")<<tr("position X")<<tr("position Y")<<tr("position Z")<<tr("width")<<tr("height")<<
                       tr("image setup");
    ShapePropertyList<<tr("name")<<tr("tooltip")<<tr("position X")<<tr("position Y")<<tr("position Z")<<tr("width")<<tr("height")<<
                       tr("path")<<tr("background color")<<tr("border color");

    AnimationPropertyList<<tr("name")<<tr("tooltip")<<tr("position X")<<tr("position Y")<<tr("position Z")<<tr("width")<<tr("height")<<
                           tr("frequency")<<tr("repeat Type")<<tr("frames");

    setColumnCount( 2 );

    headers<<tr("Description")<<tr("Value");
    setColumnWidth( 0, 155 );
    setColumnWidth( 1, 125 );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalHeaderLabels( headers );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setVisible( true);
    setRowCount( 50 );
    connect(this, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(onItemDoubleClicked(QTableWidgetItem*)));
    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(onItemChanged(QTableWidgetItem*)));

    activeObject = 0;

}

void TPropertyWidget::setActiveObject(QGraphicsItem *actObject){
    activeObject = actObject;
    clear();
    setHorizontalHeaderLabels( headers );
    if ( activeObject == 0 ) return;
    switch( actObject->type() ){
        case GrButtonType: showButtonProperty(); break;
        case GrTextType:   showTextProperty(); break;
        case GrBarType:    showBarProperty(); break;
        case GrEnterAreaType: showEnterAreaProperty(); break;
        case GrImageType: showImageProperty(); break;
        case GrShapeType: showShapeProperty(); break;
        case GrAnimationType: showAnimationProperty(); break;
    }
}
#define RGB(r,g,b)  ((quint16)((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))
void TPropertyWidget::onItemDoubleClicked(QTableWidgetItem *item){
    if ( activeObject == 0 ) return;
    if ( item->column() != 1) return;
    switch( activeObject->type()){
        case GrButtonType:
            if (item->row() == but_backColorRow) configColorList( brushIndex );
            else if ( item->row() == but_textColorRow) configColorList( textPenIndex );
            else if ( item->row() == but_bordColorRow ) configColorList( penIndex );
            else if ( item->row() == but_buttonTextRow ) configTextList();
            else if ( item->row() == but_fontRow ){
                bool ok;
                GrButton* item = (GrButton*)activeObject;
                QFont font = QFontDialog::getFont(&ok, item->font(), this );
                if ( ok ) item->setFont( font );
            }
            break;
        case GrTextType:
            if ( item->row() == text_textColorRow ) configColorList( penIndex );
            else if ( item->row() == text_textRow ) configTextList();
            else if ( item->row() == text_fontRow ){
                bool ok;
                GrText* item = (GrText*)activeObject;
                QFont font = QFontDialog::getFont(&ok, item->font(), this );
                if ( ok ) item->setFont( font );
            }
            break;
        case GrBarType:
            if ( item->row() == bar_backColorRow ) configColorList( brushIndex );
            else if ( item->row() == bar_bordColorRow ) configColorList( penIndex );
            break;
        case GrEnterAreaType:
            if ( item->row() == ea_backColorRow ) configColorList( brushIndex );
            else if ( item->row() == ea_borderColorRow ) configColorList( penIndex );
            else if ( item->row() == ea_invalidColorRow ) configColorList( invalidTextPenIndex );
            else if ( item->row() == ea_validColorRow) configColorList( validTextPenIndex );
            else if ( item->row() == ea_fontRow ){
                bool ok;
                GrEnterArea* item = (GrEnterArea*)activeObject;
                QFont font = QFontDialog::getFont(&ok, item->font(), this );
                if ( ok ) item->setFont( font );
            }
            break;
        case GrImageType:
            if ( item->row() == img_pathRow ) configImageList();
            break;
        case GrShapeType:
            if ( item->row() == sh_backColorRow ) configColorList( brushIndex );
            else if ( item->row() == sh_borderColorRow ) configColorList( penIndex );
            else if ( item->row() == sh_pathRow ) configShapePath();
            break;
            case GrAnimationType:
            if ( item->row() == anime_framesRow ) configAnimation();
    }
}


void TPropertyWidget::onItemChanged(QTableWidgetItem *item){
    if ( !changingAllowed ) return;
    if ( activeObject == 0 ) return;
    switch( activeObject->type()){
        case GrButtonType:{
            GrButton* but = (GrButton*)activeObject;
            switch( item->row() ){
                case but_posXRow: but->setPos( item->text().toFloat() ,but->scenePos().y()); break;
                case but_posYRow: but->setPos( but->scenePos().x(), item->text().toFloat()); break;
                case but_posZRow: but->setZValue( item->text().toFloat()); emit ZValueChanged(); break;
                case but_widthRow: but->setWidth( item->text().toFloat()); break;
                case but_heightRow: but->setHeight( item->text().toFloat()); break;
                case but_tooltipRow: but->setToolTip( item->text() ); break;
            }
        break;
        }
        case GrTextType:{
            GrText* GrItem = (GrText*) activeObject;
            switch( item->row() ){
                case text_posXRow: GrItem->setPos( item->text().toFloat() ,GrItem->scenePos().y()); break;
                case text_posYRow: GrItem->setPos( GrItem->scenePos().x(), item->text().toFloat()); break;
                case text_posZRow: GrItem->setZValue( item->text().toFloat()); emit ZValueChanged(); break;
                case text_tooltipRow: GrItem->setToolTip( item->text() ); break;
                case text_textSizeRow: GrItem->setFont( QFont( "Arial", item->text().toInt() )); break;
            }
        break;
        }
        case GrBarType:{
            GrBar* GrItem = (GrBar*)activeObject;
            switch( item->row() ){
                case bar_posXRow: GrItem->setPos( item->text().toFloat(), GrItem->scenePos().y()); break;
                case bar_posYRow: GrItem->setPos( GrItem->scenePos().x(), item->text().toFloat()); break;
                case bar_posZRow: GrItem->setZValue( item->text().toFloat()); emit ZValueChanged(); break;
                case bar_tooltipRow: GrItem->setToolTip( item->text() ); break;
                case bar_widthRow: GrItem->setRect(GrItem->rect().x(), GrItem->rect().y(), item->text().toFloat(),GrItem->rect().height()); break;
                case bar_heightRow: GrItem->setRect(GrItem->rect().x(), GrItem->rect().y(), GrItem->rect().width(), item->text().toFloat()); break;
            }
        break;
        }
        case GrEnterAreaType:{
            GrEnterArea* GrItem = (GrEnterArea*)activeObject;
            switch( item->row() ){
                case ea_posXRow: GrItem->setPos( item->text().toFloat(), GrItem->pos().y() ); break;
                case ea_posYRow: GrItem->setPos( GrItem->pos().x(), item->text().toFloat() ); break;
                case ea_posZRow: GrItem->setZValue( item->text().toFloat()); emit ZValueChanged(); break;
                case ea_widthRow: GrItem->setRectWidth( item->text().toFloat() ); break;
                case ea_heightRow: GrItem->setRectHeight( item->text().toFloat() ); break;
                case ea_tooltipRow: GrItem->setToolTip( item->text()); break;
                case ea_textRow:  GrItem->setText( item->text() ); break;
                case ea_validatorMinRow: GrItem->setTextValidator( item->text().toInt(), GrItem->getTextValidator().max ); break;
                case ea_validatorMaxRow: GrItem->setTextValidator( GrItem->getTextValidator().min, item->text().toInt() ); break;
            }
        break;
        }
        case GrImageType:{
            GrImage* GrItem = (GrImage*)activeObject;
            switch( item->row() ){
                case img_posXRow: GrItem->setPos( item->text().toFloat(), GrItem->pos().y() ); break;
                case img_posYRow: GrItem->setPos( GrItem->pos().x(), item->text().toFloat() ); break;
                case img_posZRow: GrItem->setZValue( item->text().toFloat()); emit ZValueChanged(); break;
                case img_widthRow: GrItem->setWidth( item->text().toFloat() ); break;
                case img_heightRow: GrItem->setHeight( item->text().toFloat() ); break;
                case img_tooltipRow: GrItem->setToolTip( item->text()); break;
            }
            break;
        }
        case GrAnimationType:{
            GrAnimation* GrItem = (GrAnimation*)activeObject;
            switch( item->row() ){
                case anime_posXRow: GrItem->setPos( item->text().toFloat(), GrItem->pos().y() ); break;
                case anime_posYRow: GrItem->setPos( GrItem->pos().x(), item->text().toFloat() ); break;
                case anime_posZRow: GrItem->setZValue( item->text().toFloat()); emit ZValueChanged(); break;
                case anime_widthRow: GrItem->setWidth( item->text().toFloat() ); break;
                case anime_heightRow: GrItem->setHeight( item->text().toFloat() ); break;
                case anime_tooltipRow: GrItem->setToolTip( item->text()); break;
                case anime_frequencyRow: GrItem->setFrequency( item->text().toInt() ); break;
                case anime_repeatRow: GrItem->setRepeatType( (TAnimeType)item->text().toInt()); break;
            }
        }break;
        case GrShapeType:{
            GrShape* GrItem = (GrShape*)activeObject;
            switch( item->row() ){
                case sh_nameRow: GrItem->setName( item->text()); break;
                case sh_tooltipRow: GrItem->setToolTip( item->text()); break;
                case sh_posXRow: GrItem->setPos( item->text().toFloat(), GrItem->pos().y() ); break;
                case sh_posYRow: GrItem->setPos( GrItem->pos().x(), item->text().toFloat() ); break;
                case sh_posZRow: GrItem->setZValue( item->text().toFloat()); emit ZValueChanged(); break;
                case sh_widthRow: GrItem->setWidth( item->text().toFloat() ); break;
                case sh_heightRow: GrItem->setHeight( item->text().toFloat() ); break;
            }
        }

    }
    activeObject->scene()->update();
}

void TPropertyWidget::onConfigTextAddClicked(){
    addtextDialog = new QDialog( w );
    addtextDialog->setAttribute( Qt::WA_DeleteOnClose);
    addtextDialog->setWindowTitle(tr( "Add text to item") );

    QVBoxLayout* mainlay = new QVBoxLayout(addtextDialog);
    QPushButton* pbOk = new QPushButton( tr("&Ok"), addtextDialog );
    QPushButton* pbCancel = new QPushButton(tr( "&Cancel"), addtextDialog);
    pbOk->setAttribute(Qt::WA_DeleteOnClose);
    pbCancel->setAttribute(Qt::WA_DeleteOnClose);
    QHBoxLayout* pbLay = new QHBoxLayout();
    pbLay->addWidget( pbOk );
    pbLay->addSpacing( 5 );
    pbLay->addWidget( pbCancel );

    leAddText = new QLineEdit(addtextDialog);
    leAddText->setAttribute(Qt::WA_DeleteOnClose);

    mainlay->addWidget( leAddText );
    mainlay->addItem( pbLay );

    connect(pbOk, SIGNAL(clicked()), this, SLOT(onAddTextOkClicked()));
    connect(pbCancel, SIGNAL(clicked()), addtextDialog, SLOT(close()));

    addtextDialog->exec();
}

void TPropertyWidget::onConfigTextDeleteClicked(){
    if ( lwItemsCount < 2 ) return;
    quint16 index = lwItemList->currentRow();
    if ( activeObject->type() == GrTextType ){
        GrText* grit = (GrText*)activeObject;
        grit->deleteText( index );
    }else if ( activeObject->type() == GrButtonType){
        GrButton* grit = (GrButton*)activeObject;
        grit->deleteText( index );
    }
    QListWidgetItem* it = lwItemList->takeItem( lwItemList->currentRow());
    lwItemsCount--;
    if (lwItemsCount == 0 ) pbDelete->setDisabled( true );
    delete it;
}

void TPropertyWidget::onConfigTextDoubleClicked(QListWidgetItem* item){
    if ( activeObject->type() == GrTextType ){
        GrText* grIt = (GrText*)activeObject;
        selectedTextNum = grIt->text().indexOf(item->text());
    }
    else if ( activeObject->type() == GrButtonType) {
        GrButton* grIt = (GrButton*)activeObject;
        selectedTextNum = grIt->text().indexOf(item->text());
    }
    d = new QDialog( );
    d->setWindowTitle("Change text");
    d->setAttribute( Qt::WA_DeleteOnClose );
    QVBoxLayout* mainlay = new QVBoxLayout( d );
    leAddText = new QLineEdit( d );
    leAddText->setText( item->text() );
    mainlay->addWidget( leAddText );

    QPushButton* pbOk = new QPushButton("&Ok", d);
    QPushButton* pbCancel = new QPushButton("&Cancel", d);
    QHBoxLayout* layPB = new QHBoxLayout();
    layPB->addWidget( pbOk );
    layPB->addSpacing( 5 );
    layPB->addWidget( pbCancel );


    connect( pbCancel, SIGNAL(clicked()), d, SLOT(close()));
    connect( pbOk, SIGNAL(clicked()), this, SLOT(onChangeTextOkClicked()));
    mainlay->addItem( layPB );
    d->show();

}

void TPropertyWidget::onChangeTextOkClicked(){
    QStringList tempList;
    if ( activeObject->type() == GrTextType ){
        GrText* grIt = (GrText*)activeObject;
        grIt->setText( leAddText->text(), selectedTextNum );
        tempList = grIt->text();
    }
    else if ( activeObject->type() == GrButtonType){
        GrButton* grIt = (GrButton*)activeObject;
        grIt->setText( leAddText->text(), selectedTextNum );
        tempList = grIt->text();
    }
    d->close();
    delete leAddText;
    lwItemList->clear();
    lwItemsCount = 0;
    foreach(QString str, tempList){
        QListWidgetItem* it = new QListWidgetItem( str, lwItemList );
        lwItemsCount++;
    }
}

void TPropertyWidget::onAddTextOkClicked(){
    QListWidgetItem* it = new QListWidgetItem(leAddText->text(), lwItemList);
    lwItemsCount++;
    if ( activeObject->type() == GrTextType){
        GrText* grIt = (GrText*)activeObject;
        grIt->addText( leAddText->text());
    }
    else if ( activeObject->type() == GrButtonType){
        GrButton* grIt = (GrButton*)activeObject;
        grIt->addText( leAddText->text());
    }
    pbNext->setEnabled( true );
    pbDelete->setEnabled( true );
    delete leAddText;
    addtextDialog->close();
}

void TPropertyWidget::onConfigTextNextClicked(){
    if ( activeObject->type() == GrTextType ){
        GrText* grItem = (GrText*)activeObject;
        if ( !grItem->setNextIndex( textIndex ) ) pbNext->setDisabled( true );
        if ( grItem->currentIndex( textIndex) != 0 ) pbPrevious->setEnabled( true );
        lwItemList->setCurrentRow( grItem->currentIndex( textIndex ) );
    }
    else if ( activeObject->type() == GrButtonType ){
        GrButton* grItem = (GrButton*)activeObject;
        if ( !grItem->setNextIndex( textIndex ) ) pbNext->setDisabled( true );
        if ( grItem->currentIndex( textIndex) != 0 ) pbPrevious->setEnabled( true );
        lwItemList->setCurrentRow( grItem->currentIndex( textIndex ) );
    }
}

void TPropertyWidget::onConfigTextPrevClicked(){
    if ( activeObject->type() == GrTextType ){
        GrText* grItem = (GrText*)activeObject;
        if ( !grItem->setPrevIndex(textIndex) ) pbPrevious->setDisabled( true );
        if ( grItem->currentIndex(textIndex) != grItem->text().count() - 1 ) pbNext->setEnabled( true );
        lwItemList->setCurrentRow( grItem->currentIndex( textIndex) );    }
    else if ( activeObject->type() == GrButtonType ){
        GrButton* grItem = (GrButton*)activeObject;
        if ( !grItem->setPrevIndex(textIndex) ) pbPrevious->setDisabled( true );
        if ( grItem->currentIndex(textIndex) != grItem->text().count() - 1 ) pbNext->setEnabled( true );
        lwItemList->setCurrentRow( grItem->currentIndex( textIndex ) );
    }
}

void TPropertyWidget::onConfigShapeTypeChanged(int index){
    sh_mainlay->removeItem(  gridlay );
    GrShape* shape = (GrShape*)activeObject;
    foreach(QLineEdit* le, leList){
        le->setVisible( false );
    }
    leWidth->setVisible( false );
    leHeight->setVisible( false );
    labelHeight->setVisible( false );
    labelWidth->setVisible( false );

    if ( gridlay ) delete gridlay;
    gridlay = new QGridLayout();
    switch( index ){
        case 0:{
            shape->setObjectShape( shape_triangle );
            QPainterPath temppath = shape->path();
            foreach(QLineEdit* le, leList){
                le->setVisible( true );
            }

            gridlay->addWidget( leXY1, 0, 1, Qt::AlignCenter  );
            gridlay->addWidget( leXY2, 2, 2, Qt::AlignCenter  );
            gridlay->addWidget( leXY0,2, 0, Qt::AlignCenter  );
            QString pos = "( ";
                    pos.append(QString().number(  QPointF(temppath.elementAt(0)).x()));
                    pos.append("; ");
                    pos.append(QString().number(  QPointF(temppath.elementAt(0)).y()));
                    pos.append(" )");
            leXY0->setText( pos );
                    pos = "( ";
                    pos.append(QString().number(  QPointF(temppath.elementAt(1)).x()));
                    pos.append("; ");
                    pos.append(QString().number(  QPointF(temppath.elementAt(1)).y()));
                    pos.append(" )");
            leXY1->setText( pos );
            pos = "( ";
                    pos.append(QString().number(  QPointF(temppath.elementAt(2)).x()));
                    pos.append("; ");
                    pos.append(QString().number(  QPointF(temppath.elementAt(2)).y()));
                    pos.append(" )");
            leXY2->setText( pos );
            connect(leXY0, SIGNAL(textEdited(QString)), this, SLOT(onConfigShapePathPosChanged(QString)));
            connect(leXY1, SIGNAL(textEdited(QString)), this, SLOT(onConfigShapePathPosChanged(QString)));
            connect(leXY2, SIGNAL(textEdited(QString)), this, SLOT(onConfigShapePathPosChanged(QString)));
        }
        break;
        case 1:{
            shape->setObjectShape( shape_rect );
            labelHeight->setVisible( true );
            labelWidth->setVisible( true );
            leWidth->setVisible( true );
            leHeight->setVisible( true );
            gridlay->addWidget( labelWidth, 0, 0 );
            gridlay->addWidget( labelHeight, 1, 0 );
            gridlay->addWidget( leWidth, 0, 1 );
            gridlay->addWidget( leHeight, 1, 1 );
            leWidth->setText( QString::number( shape->width()) );
            leHeight->setText( QString::number( shape->height()) );
            connect(leWidth, SIGNAL(textEdited(QString)), this, SLOT(onConfigShapeWidthChanged(QString)));
            connect(leHeight, SIGNAL(textEdited(QString)), this, SLOT(onConfigShapeHeightChanged(QString)));
        }
        break;
        case 2:{
            shape->setObjectShape( shape_ellipse );
            labelHeight->setVisible( true );
            labelWidth->setVisible( true );
            leWidth->setVisible( true );
            leHeight->setVisible( true );
            gridlay->addWidget( labelWidth, 0, 0 );
            gridlay->addWidget( labelHeight, 1, 0 );
            gridlay->addWidget( leWidth, 0, 1 );
            gridlay->addWidget( leHeight, 1, 1 );
            leWidth->setText( QString::number( shape->width()) );
            leHeight->setText( QString::number( shape->height()) );
            connect(leWidth, SIGNAL(textEdited(QString)), this, SLOT(onConfigShapeWidthChanged(QString)));
            connect(leHeight, SIGNAL(textEdited(QString)), this, SLOT(onConfigShapeHeightChanged(QString)));
        }
            break;
    }
    sh_mainlay->insertLayout( 1, gridlay);
}

void TPropertyWidget::onConfigShapeWidthChanged(QString str){
    GrShape* GrItem = (GrShape*)activeObject;
    GrItem->setWidth( str.toFloat( ));
}

void TPropertyWidget::onConfigShapeHeightChanged(QString str){
    GrShape* GrItem = (GrShape*)activeObject;
    GrItem->setHeight( str.toFloat( ));
}

void TPropertyWidget::onConfigShapePathPosChanged(QString str){
    quint16 num = 0;
    quint16 i = 0;
    foreach(QLineEdit* le, leList){
        if (le == sender()){
            num = i;
        }
        i++;
    }
    QString text = str;
    text.remove("(");
    text.remove(")");
    text.remove(" ");
    QStringList strPos = text.split(";");
    qreal x = strPos.at(0).toFloat();
    qreal y = strPos.at(1).toFloat();
    GrShape* GrItem = (GrShape*)activeObject;
    QPainterPath path = GrItem->path();
    path.setElementPositionAt(num, x, y);
    if ( num == 0 ){ path.setElementPositionAt(4, x, y); }
    GrItem->setPath( path );
}

void TPropertyWidget::showButtonProperty(){
    int i = 0;
    changingAllowed = false;
    GrButton* but = (GrButton*)activeObject;
    foreach(QString str, buttonPropertyList){
        setItem( i, 0, new QTableWidgetItem(str));
        item( i, 0)->setFlags(item( i, 0)->flags() ^ Qt::ItemIsEditable);
        switch(i){
            case but_name: setItem(i, 1, new QTableWidgetItem( but->name())); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case but_posXRow: setItem(i, 1, new QTableWidgetItem( QString().number(but->scenePos().x()))); break;
            case but_posYRow: setItem(i, 1, new QTableWidgetItem( QString().number(but->scenePos().y()))); break;
            case but_posZRow: setItem(i, 1, new QTableWidgetItem( QString().number(but->zValue()))); break;
            case but_widthRow: setItem(i, 1, new QTableWidgetItem( QString().number(but->boundingRect().width()))); break;
            case but_heightRow: setItem(i, 1, new QTableWidgetItem( QString().number(but->boundingRect().height()))); break;
            case but_tooltipRow: setItem(i, 1, new QTableWidgetItem( but->toolTip())); break;
            case but_fontRow:    setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case but_buttonTextRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case but_backColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case but_bordColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case but_textColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
        }
        i++;
    }
    for( int j = 0; j < i; j++) showRow(j);
    changingAllowed = true;
}

void TPropertyWidget::showEnterAreaProperty(){
    int i = 0;
    changingAllowed = false;
    GrEnterArea* GrItem = (GrEnterArea*)activeObject;
    foreach(QString str, EnterAreaPropertyList){
        setItem( i, 0, new QTableWidgetItem(str));
        item( i, 0)->setFlags(item( i, 0)->flags() ^ Qt::ItemIsEditable);
        switch(i){
            case ea_name: setItem(i, 1, new QTableWidgetItem( GrItem->name())); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case ea_posXRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().x() ))); break;
            case ea_posYRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().y() ))); break;
            case ea_posZRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->zValue() ))); break;
            case ea_widthRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->boundingRect().width() ))); break;
            case ea_heightRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->boundingRect().height() ))); break;
            case ea_tooltipRow: setItem(i, 1, new QTableWidgetItem( GrItem->toolTip() )); break;
            case ea_fontRow:    setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case ea_textRow: setItem(i, 1, new QTableWidgetItem( GrItem->text() )); break;
            case ea_backColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case ea_borderColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case ea_validColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case ea_invalidColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case ea_validatorMinRow: setItem(i, 1, new QTableWidgetItem( QString().number( GrItem->getTextValidator().min ) )); break;
            case ea_validatorMaxRow: setItem(i, 1, new QTableWidgetItem( QString().number( GrItem->getTextValidator().max ) )); break;

        }
        i++;
    }
    for( int j = 0; j < i; j++) showRow(j);
    changingAllowed = true;
}

void TPropertyWidget::showBarProperty(){
    int i = 0;
    changingAllowed = false;
    GrBar* GrItem = (GrBar*)activeObject;
    foreach(QString str, BarPropertyList){
        setItem( i, 0, new QTableWidgetItem(str));
        item( i, 0)->setFlags(item( i, 0)->flags() ^ Qt::ItemIsEditable);
        switch(i){
            case bar_name: setItem(i, 1, new QTableWidgetItem( GrItem->name())); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case bar_posXRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().x()))); break;
            case bar_posYRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().y()))); break;
            case bar_posZRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->zValue()))); break;
            case bar_widthRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->boundingRect().width()))); break;
            case bar_heightRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->boundingRect().height()))); break;
            case bar_tooltipRow: setItem(i, 1, new QTableWidgetItem( GrItem->toolTip())); break;
            case bar_backColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case bar_bordColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
        }
        i++;
    }
    for( int j = 0; j < i; j++) showRow(j);
    changingAllowed = true;
}

void TPropertyWidget::showTextProperty(){
    int i = 0;
    changingAllowed = false;
    GrText* GrItem = (GrText*)activeObject;
    foreach(QString str, TextPropertyList){
        setItem( i, 0, new QTableWidgetItem(str));
        item( i, 0)->setFlags(item( i, 0)->flags() ^ Qt::ItemIsEditable);
        switch(i){
            case text_name: setItem(i, 1, new QTableWidgetItem( GrItem->name())); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case text_posXRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().x()))); break;
            case text_posYRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().y()))); break;
            case text_posZRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->zValue()))); break;
            case text_fontRow:    setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case text_textRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case text_tooltipRow: setItem(i, 1, new QTableWidgetItem( GrItem->toolTip())); break;
            case text_textColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case text_textSizeRow: setItem(i, 1, new QTableWidgetItem( QString().number( GrItem->font().pointSizeF() ))); break;
//            case text_currentTextRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->font().si))); break;
        }
        i++;
    }
    for( int j = 0; j < i; j++) showRow(j);
    changingAllowed = true;
}

void TPropertyWidget::showImageProperty(){
    int i = 0;
    changingAllowed = false;
    GrImage* GrItem = (GrImage*)activeObject;
    foreach(QString str, ImagePropertyList){
        setItem( i, 0, new QTableWidgetItem(str));
        item( i, 0)->setFlags(item( i, 0)->flags() ^ Qt::ItemIsEditable);
        switch(i){
            case img_name: setItem(i, 1, new QTableWidgetItem( GrItem->name())); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case img_posXRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().x()))); break;
            case img_posYRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().y()))); break;
            case img_posZRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->zValue()))); break;
            case img_tooltipRow: setItem(i, 1, new QTableWidgetItem( GrItem->toolTip()));  break;
            case img_pathRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open image" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case img_widthRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->rect().width()))); break;
            case img_heightRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->rect().height()))); break;
        }
        i++;
    }
    for( int j = 0; j < i; j++) showRow(j);
    changingAllowed = true;
}

void TPropertyWidget::showShapeProperty(){
    int i = 0;
    changingAllowed = false;
    GrShape* GrItem = (GrShape*)activeObject;
    foreach(QString str, ShapePropertyList){
        setItem( i, 0, new QTableWidgetItem(str));
        item( i, 0)->setFlags(item( i, 0)->flags() ^ Qt::ItemIsEditable);
        switch(i){
            case sh_nameRow: setItem(i, 1, new QTableWidgetItem( GrItem->name())); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case sh_posXRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().x()))); break;
            case sh_posYRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().y()))); break;
            case sh_posZRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->zValue()))); break;
            case sh_tooltipRow: setItem(i, 1, new QTableWidgetItem( GrItem->toolTip())); break;
            case sh_widthRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->width()))); break;
            case sh_heightRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->height()))); break;
            case sh_pathRow: setItem(i, 1, new QTableWidgetItem( "Double click to open settings")); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case sh_backColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case sh_borderColorRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open settings" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
        }
        i++;
    }

    for( int j = 0; j < i; j++) showRow(j);
    changingAllowed = true;
}

void TPropertyWidget::showAnimationProperty(){
    int i = 0;
    changingAllowed = false;
    GrAnimation* GrItem = (GrAnimation*)activeObject;
    foreach(QString str, AnimationPropertyList){
        setItem( i, 0, new QTableWidgetItem(str));
        item( i, 0)->setFlags(item( i, 0)->flags() ^ Qt::ItemIsEditable);
        switch(i){
            case anime_name: setItem(i, 1, new QTableWidgetItem( GrItem->name())); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case anime_posXRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().x()))); break;
            case anime_posYRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->scenePos().y()))); break;
            case anime_posZRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->zValue()))); break;
            case anime_tooltipRow: setItem(i, 1, new QTableWidgetItem( GrItem->toolTip()));  break;
            case anime_framesRow: setItem(i, 1, new QTableWidgetItem( "Double Click to open images" )); item( i, 1)->setFlags(item( i, 1)->flags() ^ Qt::ItemIsEditable); break;
            case anime_frequencyRow: setItem(i, 1, new QTableWidgetItem( QString().number( GrItem->frequency()))); break;
            case anime_repeatRow: setItem(i, 1, new QTableWidgetItem( QString().number( GrItem->repeatType()))); break;
            case anime_widthRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->rect().width()))); break;
            case anime_heightRow: setItem(i, 1, new QTableWidgetItem( QString().number(GrItem->rect().height()))); break;
        }
        i++;
    }
    for( int j = 0; j < i; j++) showRow(j);
    changingAllowed = true;
}

void TPropertyWidget::configTextList(){
    w = new QDialog( this );
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->setWindowTitle( tr("Set text item string list") );
    QVBoxLayout* mainlay = new QVBoxLayout( w );

    lwItemList = new QListWidget( w );
    lwItemList->setToolTip(tr( "List of item's texts" ));
    lwItemList->setAttribute( Qt::WA_DeleteOnClose );

    quint16 curtextIndex;
    QStringList tempList;
    if ( activeObject->type() == GrTextType){
        GrText* grItem = (GrText*)activeObject;
        curtextIndex = grItem->currentIndex( textIndex );
        tempList = grItem->text();
    }
    else if ( activeObject->type() == GrButtonType){
        GrButton* grItem = (GrButton*)activeObject;
        curtextIndex = grItem->currentIndex( textIndex );
        tempList = grItem->text();
    }
    lwItemsCount = 0;
    foreach(QString str, tempList){
        QListWidgetItem* it = new QListWidgetItem( str, lwItemList );
        lwItemsCount++;
    }

    lwItemList->setCurrentRow( curtextIndex );
    connect(lwItemList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(onConfigTextDoubleClicked(QListWidgetItem*)));

    QPushButton* pbAdd = new QPushButton(tr("&Add"), w );
    pbAdd->setAttribute( Qt::WA_DeleteOnClose );
    pbDelete = new QPushButton(tr("&Delete"), w);
    pbDelete->setAttribute( Qt::WA_DeleteOnClose );
    pbNext = new QPushButton( tr("&Next"), w );
    pbNext->setAttribute( Qt::WA_DeleteOnClose );
    pbPrevious = new QPushButton( tr("&Previous"), w );
    pbPrevious->setAttribute( Qt::WA_DeleteOnClose );
    if ( lwItemsCount == 0 ) pbDelete->setDisabled( true );
    if ( lwItemList->currentRow() == 0 ) pbPrevious->setDisabled( true );
    QHBoxLayout* buttonsLay =  new QHBoxLayout();
    buttonsLay->addWidget( pbAdd );
    buttonsLay->addSpacing( 5 );
    buttonsLay->addWidget( pbDelete );
    buttonsLay->addSpacing( 5 );
    buttonsLay->addWidget( pbNext );
    buttonsLay->addSpacing( 5 );
    buttonsLay->addWidget( pbPrevious );

    if ( lwItemsCount < 2 ){
        pbNext->setDisabled( true );
        pbPrevious->setDisabled( true );
    }


    mainlay->addWidget(lwItemList);
    mainlay->addSpacing(5);
    mainlay->addItem(buttonsLay);
//    mainlay->addSpacing(10);
//    mainlay->addItem(pblay);

    connect(pbAdd, SIGNAL(clicked()), this, SLOT(onConfigTextAddClicked()));
    connect(pbDelete, SIGNAL(clicked()), this, SLOT(onConfigTextDeleteClicked()));
    connect(pbNext, SIGNAL(clicked()), this, SLOT(onConfigTextNextClicked()));
    connect(pbPrevious, SIGNAL(clicked()),this, SLOT(onConfigTextPrevClicked()));
//    connect(pbOk, SIGNAL(clicked()), this, SLOT(onConfigTextOkClicked()));
//    connect(pbCancel, SIGNAL(clicked()), w, SLOT(close()));

    w->show();
}

void TPropertyWidget::configShapePath(){
    leList.clear();

    w = new QDialog( this );
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->setWindowTitle( tr("Set shape path") );
    sh_mainlay = new QVBoxLayout( w );

    leXY0 = new QLineEdit( w );
    leXY1 = new QLineEdit( w );
    leXY2 = new QLineEdit( w );
    leXY0->setAttribute( Qt::WA_DeleteOnClose );
    leXY1->setAttribute( Qt::WA_DeleteOnClose );
    leXY2->setAttribute( Qt::WA_DeleteOnClose );
    leWidth = new QLineEdit( w );
    leWidth->setAttribute( Qt::WA_DeleteOnClose );
    leHeight = new QLineEdit( w );
    leHeight->setAttribute( Qt::WA_DeleteOnClose );
    leList.push_back( leXY0 );
    leList.push_back( leXY1 );
    leList.push_back( leXY2 );
    labelWidth = new QLabel( "Rectangle width", w );
    labelWidth->setAttribute(Qt::WA_DeleteOnClose);
    labelHeight = new QLabel( "Rectangle height", w );
    labelHeight->setAttribute(Qt::WA_DeleteOnClose);
    labelHeight->setVisible( false );
    labelWidth->setVisible( false );
    foreach(QLineEdit* le, leList){
        le->setVisible( false );
    }
    leWidth->setVisible( false );
    leHeight->setVisible( false );


    cbType = new QComboBox( w );
    cbType->setAttribute( Qt::WA_DeleteOnClose );
    cbType->addItem("shape triangle");
    cbType->addItem("shape rectangle");
    cbType->addItem("shape ellipse");
    connect(cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(onConfigShapeTypeChanged(int)));

    gridlay = new QGridLayout( );
    GrShape* shape = (GrShape*)activeObject;
    TShapeTypes type = shape->shapeType();
    if ( type == shape_triangle ){
        cbType->setCurrentIndex( 0 );
    }else if ( type == shape_rect ){
        cbType->setCurrentIndex( 1 );
    }else
        cbType->setCurrentIndex( 2 );

    sh_mainlay->addWidget( cbType );
    sh_mainlay->addItem( gridlay );
//    sh_mainlay->addItem( pblay );
    w->show();
}

void TPropertyWidget::configImageList(){
    w = new QDialog( this );
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->setWindowTitle( "Configurate images" );
    QVBoxLayout* mainlay = new QVBoxLayout( w );
    QHBoxLayout* pblay = new QHBoxLayout( );

    QPushButton* pbAdd = new QPushButton( "&Add", w );
    pbAdd->setAttribute( Qt::WA_DeleteOnClose );

    pbDelete = new QPushButton( "&Delete", w );
    pbDelete->setAttribute( Qt::WA_DeleteOnClose );

    pbNext = new QPushButton( "Show &Next", w );
    pbNext->setAttribute( Qt::WA_DeleteOnClose );

    pbPrevious = new QPushButton( "Show &Previous", w );
    pbPrevious->setAttribute( Qt::WA_DeleteOnClose );

    lwItemList = new QListWidget( w );
    lwItemList->setToolTip(tr( "List of item's images" ));
    lwItemList->setAttribute( Qt::WA_DeleteOnClose );

    GrImage* grItem = (GrImage*)activeObject;
    lwItemsCount = 0;
    foreach(QString str, grItem->imagesPath()){
        QListWidgetItem* it = new QListWidgetItem( str, lwItemList );
        lwItemsCount++;
    }

    lwItemList->setCurrentRow( grItem->currentIndex() );
    if ( lwItemsCount < 2 ){
        pbNext->setDisabled( true );
        pbPrevious->setDisabled( true );
    }
    connect(lwItemList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(onConfigImageDoubleClicked(QListWidgetItem*)));
    connect( pbAdd, SIGNAL(clicked()), this, SLOT(onConfigImageAddClicked()));
    connect( pbDelete, SIGNAL(clicked()), this, SLOT(onConfigImageDelClicked()));
    connect( pbNext, SIGNAL(clicked()), this, SLOT(onConfigImageNextClicked()));
    connect( pbPrevious, SIGNAL(clicked()), this, SLOT(onConfigImagePrevClicked()));


    pblay->addWidget( pbAdd );
    pblay->addWidget( pbDelete );
    pblay->addWidget( pbNext );
    pblay->addWidget( pbPrevious );

    mainlay->addWidget( lwItemList );
    mainlay->addItem( pblay );

    w->show();
}

void TPropertyWidget::configColorList(TGrIndexes colorType){
    w = new QDialog( this );
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->setWindowTitle( "Configurate colors" );
    QVBoxLayout* mainlay = new QVBoxLayout( w );
    QHBoxLayout* pblay = new QHBoxLayout( );

    QPushButton* pbAdd = new QPushButton( "&Add", w );
    pbAdd->setAttribute( Qt::WA_DeleteOnClose );

    pbDelete = new QPushButton( "&Delete", w );
    pbDelete->setAttribute( Qt::WA_DeleteOnClose );

    pbNext = new QPushButton( "Show &Next", w );
    pbNext->setAttribute( Qt::WA_DeleteOnClose );

    pbPrevious = new QPushButton( "Show &Previous", w );
    pbPrevious->setAttribute( Qt::WA_DeleteOnClose );

    lwItemList = new QListWidget( w );
    lwItemList->setToolTip(tr( "List of item's colors" ));
    lwItemList->setAttribute( Qt::WA_DeleteOnClose );

    QStringList tempList;
    colorIndex = colorType;
    switch( activeObject->type()){
    case GrButtonType:{
        GrButton* grit = (GrButton*)activeObject;
        switch( colorType ){
        case brushIndex:
            foreach( QBrush br, grit->backgroundColor()){ tempList.push_back( br.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( brushIndex ) );
            break;
        case penIndex:
            foreach( QPen pen, grit->borderColor()){ tempList.push_back( pen.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( penIndex ) );
            break;
        case textPenIndex:
            foreach( QPen pen, grit->textColor()){ tempList.push_back( pen.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( textIndex ) );
            break;
        }
    }break;
    case GrTextType:{
        GrText* grit = (GrText*)activeObject;
        foreach( QPen pen, grit->textColor()){ tempList.push_back( pen.color().name() ); }
        lwItemList->setCurrentRow( grit->currentIndex( penIndex ) );
    }break;
    case GrBarType:{
        GrBar* grit = (GrBar*)activeObject;
        switch( colorType ){
        case brushIndex:
            foreach( QBrush br, grit->backgroundColor()){ tempList.push_back( br.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( brushIndex ) );
            break;
        case penIndex:
            foreach( QPen pen, grit->borderColor()){ tempList.push_back( pen.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( penIndex ) );
            break;
        }
    }break;
    case GrEnterAreaType:{
        GrEnterArea* grit = (GrEnterArea*)activeObject;
        switch( colorType ){
        case brushIndex:
            foreach( QBrush br, grit->backgroundColor()){ tempList.push_back( br.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( brushIndex ) );
            break;
        case penIndex:
            foreach( QPen pen, grit->borderColor()){ tempList.push_back( pen.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( penIndex ) );
            break;
        case validTextPenIndex:
            foreach( QPen pen, grit->validTextColor()){ tempList.push_back( pen.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( validTextPenIndex ) );
            break;
        case invalidTextPenIndex:
            foreach( QPen pen, grit->invalidTextColor()){ tempList.push_back( pen.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( invalidTextPenIndex ) );
            break;
        }
    }break;
    case GrShapeType:{
        GrShape* grit = (GrShape*)activeObject;
        switch( colorType ){
        case brushIndex:
            foreach( QBrush br, grit->brush()){ tempList.push_back( br.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( brushIndex ) );
            break;
        case penIndex:
            foreach( QPen pen, grit->pen()){ tempList.push_back( pen.color().name() ); }
            lwItemList->setCurrentRow( grit->currentIndex( penIndex ) );
            break;
        }
    }break;
    }
    lwItemsCount = 0;
    foreach(QString str, tempList ){
        QListWidgetItem* it = new QListWidgetItem( str, lwItemList );
        lwItemsCount++;
    }

    if ( lwItemsCount < 2 ){
        pbNext->setDisabled( true );
        pbPrevious->setDisabled( true );
    }

    pblay->addWidget( pbAdd );
    pblay->addWidget( pbDelete );
    pblay->addWidget( pbNext );
    pblay->addWidget( pbPrevious );

    mainlay->addWidget( lwItemList );
    mainlay->addItem( pblay );

    connect(lwItemList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(onConfigColorDoubleClicked(QListWidgetItem*)));
    connect( pbAdd, SIGNAL(clicked()), this, SLOT(onConfigColorAddClicked()));
    connect( pbDelete, SIGNAL(clicked()), this, SLOT(onConfigColorDelClicked()));
    connect( pbNext, SIGNAL(clicked()), this, SLOT(onConfigColorNextClicked()));
    connect( pbPrevious, SIGNAL(clicked()), this, SLOT(onConfigColorPrevClicked()));

    w->show();
}

void TPropertyWidget::configAnimation(){
    w = new QDialog( this );
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->setWindowTitle( "Configurate images" );
    QVBoxLayout* mainlay = new QVBoxLayout( w );
    QHBoxLayout* pblay = new QHBoxLayout( );

    QPushButton* pbAdd = new QPushButton( "&Add", w );
    pbAdd->setAttribute( Qt::WA_DeleteOnClose );

    pbDelete = new QPushButton( "&Delete", w );
    pbDelete->setAttribute( Qt::WA_DeleteOnClose );

    pbNext = new QPushButton( "Move &up", w );
    pbNext->setAttribute( Qt::WA_DeleteOnClose );

    pbPrevious = new QPushButton( "Move &down", w );
    pbPrevious->setAttribute( Qt::WA_DeleteOnClose );

    lwItemList = new QListWidget( w );
    lwItemList->setToolTip(tr( "List of item's images" ));
    lwItemList->setAttribute( Qt::WA_DeleteOnClose );

    GrAnimation* grItem = (GrAnimation*)activeObject;
    lwItemsCount = 0;
    foreach(QString str, grItem->frameAdr()){
        QListWidgetItem* it = new QListWidgetItem( str, lwItemList );
        lwItemsCount++;
    }

    pblay->addWidget( pbAdd );
    pblay->addWidget( pbDelete );
    pblay->addWidget( pbNext );
    pblay->addWidget( pbPrevious );

    mainlay->addWidget( lwItemList );
    mainlay->addItem( pblay );

    connect( pbAdd, SIGNAL(clicked()), this, SLOT(onConfigAnimeAddClicked()));
    connect( pbDelete, SIGNAL( clicked()), this, SLOT(onConfigAnimeDelClicked()));
    connect( pbNext, SIGNAL(clicked()), this, SLOT(onConfigAnimeUpClicked()));
    connect( pbPrevious, SIGNAL(clicked()), this, SLOT(onConfigAnimeDownClicked()));
    connect( lwItemList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onConfigAnimeDoubleClicked(QListWidgetItem*)));


    w->show();
}

void TPropertyWidget::onConfigImageAddClicked(){
    #ifdef Q_WS_WIN
        QString temp = QFileDialog::getOpenFileName(this, "Open picture", "../", "*");
    #else
        TGetFileWindow* window = new TGetFileWindow();
        QString temp = window->getOpenFileWindow("*", QDir::homePath(), "Open picture").first();
    #endif
    if ( !temp.isEmpty() ){
        GrImage* GrItem = (GrImage*)activeObject;
        GrItem->addImage( temp );
        QListWidgetItem* it = new QListWidgetItem( temp, lwItemList );
        lwItemsCount++;
        if ( lwItemsCount > 1 && GrItem->currentIndex() == 0 ) pbNext->setEnabled( true );
    }
}

void TPropertyWidget::onConfigImageDelClicked(){
    if ( lwItemsCount < 2 ) return;
    int row = lwItemList->currentRow();
    GrImage* GrItem = (GrImage*)activeObject;
    GrItem->deleteImage( row );
    lwItemsCount--;
    delete lwItemList->takeItem( row );
    if ( lwItemsCount <= 1 ){
        pbNext->setDisabled( true );
        pbPrevious->setDisabled( true );
    }
}

void TPropertyWidget::onConfigImageNextClicked(){
    GrImage* grItem = (GrImage*)activeObject;
    if ( !grItem->setNextImage() ) pbNext->setDisabled( true );
    if ( grItem->currentIndex() != 0 ) pbPrevious->setEnabled( true );
    lwItemList->setCurrentRow( grItem->currentIndex() );
}

void TPropertyWidget::onConfigImagePrevClicked(){
    GrImage* grItem = (GrImage*)activeObject;
    pbPrevious->setEnabled( grItem->setPrevImage() );
    if ( grItem->currentIndex() != grItem->images().count( ) - 1 ) pbNext->setEnabled( true );
    lwItemList->setCurrentRow( grItem->currentIndex() );
}

void TPropertyWidget::onConfigImageDoubleClicked( QListWidgetItem* item ){
    QString currentPath= item->text();
    int index = currentPath.lastIndexOf("/");
    if ( index == -1 ) currentPath = "../";
    else{
        int toDel = currentPath.count() - index;
        currentPath.remove( index, toDel );
    }
    #ifdef Q_WS_WIN
        QString temp = QFileDialog::getOpenFileName(this, "Open picture", currentPath, "*");
    #else
        TGetFileWindow* window = new TGetFileWindow();
        QString temp = window->getOpenFileWindow("*", currentPath, "Open picture").first();
    #endif
    GrImage* GrItem = (GrImage*)activeObject;
    index = lwItemList->currentRow();
    GrItem->setImage( index, temp );
    item->setText( temp );
}

void TPropertyWidget::onConfigImageItemClicked(QListWidgetItem *item){
}

void TPropertyWidget::onConfigColorAddClicked(){
    QColor col = QColorDialog::getColor();
    if ( !col.isValid()) return;
    lwItemsCount++;
    lwItemList->addItem(new QListWidgetItem(col.name()));
    pbNext->setEnabled( true );

    switch( activeObject->type() ){
    case GrButtonType:{
        GrButton* grit = (GrButton*)activeObject;
        switch ( colorIndex ) {
        case brushIndex: grit->addBackgroundColor( col ); break;
        case penIndex: grit->addBorderColor( col ); break;
        case textPenIndex: grit->addTextColor( col ); break;
        }
    }break;
    case GrBarType:{
        GrBar* grit = (GrBar*)activeObject;
        switch ( colorIndex ) {
        case brushIndex: grit->addBackgroundColor( col ); break;
        case penIndex: grit->addBorderColor( col ); break;
        }
    }break;
    case GrTextType:{
        GrText* grit = (GrText*)activeObject;
        switch ( colorIndex ) {
        case penIndex: grit->addTextColor( col ); break;
        }
    }break;
    case GrEnterAreaType:{
        GrEnterArea* grit = (GrEnterArea*)activeObject;
        switch ( colorIndex ) {
        case brushIndex: grit->addBackgroundColor( col ); break;
        case penIndex: grit->addBorderColor( col ); break;
        case validTextPenIndex: grit->addValidTextColor( col ); break;
        case invalidTextPenIndex: grit->addInvalidTextColor( col ); break;
        }
    }break;
    case GrShapeType:{
        GrShape* grit = (GrShape*)activeObject;
        switch ( colorIndex ) {
        case brushIndex: grit->addBackgroundColor( col ); break;
        case penIndex: grit->addBorderColor( col ); break;
        }
    }break;
    }
}

void TPropertyWidget::onConfigColorDelClicked(){
    if ( lwItemsCount < 2 ) return;
    quint16 index = lwItemList->currentRow();
    if ( index >= lwItemsCount ) return;
    lwItemsCount--;
    delete lwItemList->takeItem( index );

    switch( activeObject->type() ){
    case GrButtonType:{
        GrButton* grit = (GrButton*)activeObject;
        grit->setCurrentIndex( colorIndex, 0 );
        switch ( colorIndex ) {
        case brushIndex: grit->deleteBackgroundColor( index ); break;
        case penIndex: grit->deleteBorderColor( index ); break;
        case textPenIndex: grit->deleteTextColor( index ); break;
        }
    }break;
    case GrBarType:{
        GrBar* grit = (GrBar*)activeObject;
        grit->setCurrentIndex( colorIndex, 0 );
        switch ( colorIndex ) {
        case brushIndex: grit->deleteBackgroundColor( index ); break;
        case penIndex: grit->deleteBorderColor( index ); break;
        }
    }break;
    case GrTextType:{
        GrText* grit = (GrText*)activeObject;
        grit->setCurrentIndex( colorIndex, 0 );
        switch ( colorIndex ) {
        case penIndex: grit->deleteTextColor( index ); break;
        }
    }break;
    case GrEnterAreaType:{
        GrEnterArea* grit = (GrEnterArea*)activeObject;
        grit->setCurrentIndex( colorIndex, 0 );
        switch ( colorIndex ) {
        case brushIndex: grit->deleteBackgroundColor( index ); break;
        case penIndex: grit->deleteBorderColor( index ); break;
        case validTextPenIndex: grit->deleteValidTextColor( index ); break;
        case invalidTextPenIndex: grit->deleteInvalidTextColor( index ); break;
        }
    }break;
    case GrShapeType:{
        GrShape* grit = (GrShape*)activeObject;
        grit->setCurrentIndex( colorIndex, 0 );
        switch ( colorIndex ) {
        case brushIndex: grit->deleteBackgroundColor( index ); break;
        case penIndex: grit->deleteBorderColor( index ); break;
        }
    }break;
    }
}

void TPropertyWidget::onConfigColorNextClicked(){
    quint16 curIn;
    switch(activeObject->type()){
    case GrButtonType:{
        GrButton* grItem = (GrButton*)activeObject;
        pbNext->setEnabled(grItem->setNextIndex( colorIndex ));
        curIn = grItem->currentIndex( colorIndex );
    }break;
    case GrBarType:{
        GrBar* grItem = (GrBar*)activeObject;
        pbNext->setEnabled(grItem->setNextIndex( colorIndex ));
        curIn = grItem->currentIndex( colorIndex );
    } break;
    case GrTextType:{
        GrText* grItem = (GrText*)activeObject;
        pbNext->setEnabled(grItem->setNextIndex( colorIndex ));
        curIn = grItem->currentIndex( colorIndex );
    } break;
    case GrEnterAreaType:{
        GrEnterArea* grItem = (GrEnterArea*)activeObject;
        pbNext->setEnabled(grItem->setNextIndex( colorIndex ));
        curIn = grItem->currentIndex( colorIndex );
    } break;
    case GrShapeType:{
        GrShape* grItem = (GrShape*)activeObject;
        pbNext->setEnabled(grItem->setNextIndex( colorIndex ));
        curIn = grItem->currentIndex( colorIndex );
    } break;
    }
    if ( curIn != 0 ) pbPrevious->setEnabled( true );
    lwItemList->setCurrentRow( curIn );
}

void TPropertyWidget::onConfigColorPrevClicked(){
    quint16 curIn;
    switch(activeObject->type()){
    case GrButtonType:{
        GrButton* grItem = (GrButton*)activeObject;
        pbPrevious->setEnabled(grItem->setPrevIndex( colorIndex ));
        curIn = grItem->currentIndex( colorIndex );
    }break;
    case GrBarType:{
        GrBar* grItem = (GrBar*)activeObject;
        pbPrevious->setEnabled(grItem->setPrevIndex( colorIndex ));
        curIn = grItem->currentIndex( colorIndex );
    } break;
    case GrTextType:{
        GrText* grItem = (GrText*)activeObject;
        pbPrevious->setEnabled(grItem->setPrevIndex( colorIndex ));
        curIn = grItem->currentIndex( colorIndex );
    } break;
    case GrEnterAreaType:{
        GrEnterArea* grItem = (GrEnterArea*)activeObject;
        pbPrevious->setEnabled(grItem->setPrevIndex( colorIndex ));
        curIn = grItem->currentIndex( colorIndex );
    } break;
    case GrShapeType:{
        GrShape* grItem = (GrShape*)activeObject;
        pbPrevious->setEnabled(grItem->setPrevIndex( colorIndex ));
        curIn = grItem->currentIndex( colorIndex );
    } break;
    }
    if ( curIn != lwItemsCount - 1 ) pbNext->setEnabled( true );
    lwItemList->setCurrentRow( curIn );
}

void TPropertyWidget::onConfigColorDoubleClicked(QListWidgetItem *item){
    quint16 index = lwItemList->currentRow();
    QColor col = QColorDialog::getColor( QColor(item->text()));
    if ( !col.isValid()) return;
    item->setText( col.name() );
    switch( activeObject->type() ){
    case GrButtonType:{
        GrButton* grit = (GrButton*)activeObject;
        switch ( colorIndex ) {
        case brushIndex: grit->setBackgroundColor( col, index ); break;
        case penIndex: grit->setBorderColor( col, index ); break;
        case textPenIndex: grit->setTextColor( col, index ); break;
        }
    }break;
    case GrBarType:{
        GrBar* grit = (GrBar*)activeObject;
        switch ( colorIndex ) {
        case brushIndex: grit->setBackgroundColor( col, index ); break;
        case penIndex: grit->setBorderColor( col, index ); break;
        }
    }break;
    case GrTextType:{
        GrText* grit = (GrText*)activeObject;
        switch ( colorIndex ) {
        case penIndex: grit->setTextColor( col, index ); break;
        }
    }break;
    case GrEnterAreaType:{
        GrEnterArea* grit = (GrEnterArea*)activeObject;
        switch ( colorIndex ) {
        case brushIndex: grit->setBackgroundColor( col, index ); break;
        case penIndex: grit->setBorderColor( col, index ); break;
        case validTextPenIndex: grit->setValidTextColor( col, index ); break;
        case invalidTextPenIndex: grit->setInvalidTextColor( col, index ); break;
        }
    }break;
    case GrShapeType:{
        GrShape* grit = (GrShape*)activeObject;
        switch ( colorIndex ) {
        case brushIndex: grit->setBackgroundColor( col, index ); break;
        case penIndex: grit->setBorderColor( col, index ); break;
        }
    }break;
    }
}

void TPropertyWidget::onConfigColorItemClicked(QListWidgetItem *item){
}

void TPropertyWidget::onConfigAnimeAddClicked(){
#ifdef Q_WS_WIN
    QString temp = QFileDialog::getOpenFileName(this, "Open frames", "../", "*");
#else
    TGetFileWindow* window = new TGetFileWindow();
    QStringList temp = window->getOpenFileWindow("*", QDir::homePath(), "Open frames", TGetFileWindow::multySelection );
#endif
    if ( !temp.isEmpty() ){
        GrAnimation* GrItem = (GrAnimation*)activeObject;
        foreach( QString str, temp ){
            GrItem->addFrame( str );
            QListWidgetItem* it = new QListWidgetItem( str, lwItemList );
            lwItemsCount++;
        }
    }
//    if ( lwItemsCount > 1 && GrItem->currentIndex() == 0 ) pbNext->setEnabled( true );
}

void TPropertyWidget::onConfigAnimeDelClicked(){
    if ( lwItemsCount < 2 ) return;
    int row = lwItemList->currentRow();
    GrAnimation* GrItem = (GrAnimation*)activeObject;
    GrItem->deleteFrame( row );
    lwItemsCount--;
    delete lwItemList->takeItem( row );
//    if ( lwItemsCount <= 1 ){
//        pbNext->setDisabled( true );
//        pbPrevious->setDisabled( true );
//    }

}

void TPropertyWidget::onConfigAnimeUpClicked(){
    int row = lwItemList->currentRow();
    GrAnimation* GrItem = (GrAnimation*)activeObject;
    int newRow = GrItem->moveFrameUp( row );
    if ( newRow != -1 ){
        QListWidgetItem* item = lwItemList->takeItem( row );
        lwItemList->insertItem( newRow, item );
        lwItemList->setCurrentRow( newRow );
    }
}

void TPropertyWidget::onConfigAnimeDownClicked(){
    int row = lwItemList->currentRow();
    GrAnimation* GrItem = (GrAnimation*)activeObject;
    int newRow = GrItem->moveFrameDown( row );
    if ( newRow != -1 ){
        QListWidgetItem* item = lwItemList->takeItem( row );
        lwItemList->insertItem( newRow, item );
        lwItemList->setCurrentRow( newRow );
    }
}

void TPropertyWidget::onConfigAnimeDoubleClicked(QListWidgetItem* item ){
    QString currentPath= item->text();
    int index = currentPath.lastIndexOf("/");
    if ( index == -1 ) currentPath = "../";
    else{
        int toDel = currentPath.count() - index;
        currentPath.remove( index, toDel );
    }
    #ifdef Q_WS_WIN
        QString temp = QFileDialog::getOpenFileName(this, "Open picture", currentPath, "*");
    #else
        TGetFileWindow* window = new TGetFileWindow();
        QString temp = window->getOpenFileWindow("*", currentPath, "Open picture").first();
    #endif
    GrAnimation* GrItem = (GrAnimation*)activeObject;
    index = lwItemList->currentRow();
    GrItem->setFrame( temp, index );
    item->setText( temp );
}
