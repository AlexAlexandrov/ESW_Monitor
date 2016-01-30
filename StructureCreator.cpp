#include "StructureCreator.h"
//#define SHOWSTR 1
StructureCreator* _self = 0;

StructureCreator::StructureCreator(){
    structure = 0;
    textOffset = 0;
}

StructureCreator::~StructureCreator(){
    _self = 0;
    if ( structure ) delete structure;
}

quint32 StructureCreator::textAddress(){
    return originAddress + strSize;
}

quint32 StructureCreator::fontAddress(){
    return originAddress + strSize + textArraySize;
}

StructureCreator *StructureCreator::getCreator(){
    if ( _self ) return _self;
    _self = new StructureCreator();
    return _self;
}

void StructureCreator::resizeStructure(QList<MonitorScreen *> screenList){
    fontConverter = new GlyphTableCreator();
    imageConverter = new ImageConverter();
    scrList = screenList;
    quint32 size = 0;
    size += scrList.count() + 1; // add screen cnt and screen adresses
    foreach ( MonitorScreen* scr, scrList ) {
        size += scr->items().count() + 1 + 1; // add screen color, objects cnt, objects adresses
        foreach( QGraphicsItem* item, scr->items()){
            if ( item->type() > USERTYPE ){
                GrObject* obj = (GrObject*)item;
                TItemState defaultState = obj->states().first();
                size += obj->states().count() + 1 + 1;  // add objType, states Cnt, states adresses
                switch( obj->type() ){
                    case GrBarType: size += bar_prop_end; break; // add default property values
                    case GrButtonType:{
                        size += button_prop_end;
                        GrButton* button = (GrButton*)obj;
                        fontConverter->addText( button->text(), button->font());
                    }break;
                    case GrTextType:{
                        size += stTxt_end;
                        GrText* text = (GrText*)obj;
                        fontConverter->addText( text->text(), text->font() );
                    }break;
                    case GrEnterAreaType:{
                        size += db_end;
                    }break;
                    case GrShapeType: size += shape_end; break;
                    case GrImageType:{
                        GrImage* img = (GrImage*)obj;
                        imageConverter->addImage( img->images());
                        size += img_end;
                    }break;

                }
                for( int j = 1; j < obj->states().count(); j++ ){
                    TItemState st = obj->states().at( j );
                    size++;                             // add changedProperty Cnt in state
                    for( int i = 0; i < st.indexList.count(); i++ )
                        if ( st.indexList.at(i) != defaultState.indexList.at(i) ) size += 2; // add changedProperty
                }
            }
        }
    }
    textArraySize = fontConverter->getTextArraysSize();
    foreach ( MonitorScreen* scr, scrList ) {
        foreach( QGraphicsItem* item, scr->items()){
            if ( item->type() == GrEnterAreaType ){
                GrEnterArea* ea = (GrEnterArea*)item;
                fontConverter->addText("0123456789", ea->font());
            }
        }
    }
    glyphTable = fontConverter->getGlyphTable();
    imagesTable = imageConverter->getImageTables();
    if ( structure != 0 ) free(structure);
    int totalSize = size + glyphTable.count() + imagesTable.count() + textArraySize;
    structure = (quint32 *)malloc( totalSize * sizeof(quint32));
    if ( structure == NULL ){
        QMessageBox::critical(0, "Allocate memory error", " Unable to allocate memory for creating structure", QMessageBox::Ok, QMessageBox::NoButton );
    }
    strSize = size; // size of main structure
    textOffset = size;  // offset of text arrays
    fontOffset = textOffset + textArraySize;    // offset of FontTables
    imagesOffset = size + glyphTable.count() + textArraySize;   // offset of ImageTables
    // recalc offset in infoVector of fonts tables
    fontStrInfo = fontConverter->getStructureInfo();
    for(int i = 0; i < fontStrInfo.count(); i++){
        fontStrInfo[i].blockOffset += fontOffset;
    }
}


#define firstScreenHdr( pBegin ) ( pBegin + getScreenCnt( pBegin ) + 1)
#define firstObjectHdr( pScr, objCnt )( pScr + 2 + objCnt )
#define AddToStrInfo( str, off ) tempInfo.block = str; tempInfo.blockOffset = off; mainStrInfo.push_back( tempInfo )
void StructureCreator::generateInnerStructure(){
    TStructureInfo tempInfo;
    int screenNum = 0;
    quint32* pw = &structure[0];
    structure[0] = scrList.count();
    quint32* pScreen = pw + 1;     // pointer on screen address in main header
    quint32* pObject;              // pointer on object address in screen header
    quint32* pState;               // pointer on state  address in object header
    pw = firstScreenHdr( &structure[0] );
    AddToStrInfo( "Begin of the Structure", 0 );
    foreach( MonitorScreen* scr, scrList ){ // screens structure
        currentScreen = scr;
        *pScreen = (quintptr)pw; // write pointer to current screen
        pScreen++;   // pointer to next screen
        QColor col = scr->backgroundColor();
        AddToStrInfo( "Screen Header ", (quintptr)(pw - &structure[0]));
        *pw = RGB565CONVERT(col.red(), col.green(), col.blue()); // write screen color to curPointer
        pw++;                                                       // go next
        *pw = scrList.at( screenNum )->items().count();                 // write objects count to curPointer
        pw++;                                                   //go next
        pObject = pw;                                           // pointer to pointer to first object
        pw += scrList.at( screenNum )->items().count();             // pw now points to first object address
        foreach( QGraphicsItem* item, scr->items() ){   // object structure
            if ( item->type() > USERTYPE ){
                GrObject* grObj = (GrObject*)item;
                *pObject = (quintptr)pw;                // pObject contains first objects address
                AddToStrInfo( "Object Header of " + grObj->name(), (quintptr)(pw - &structure[0]));
                pObject++;                                                  // pObject show next object
                writeToPtrMOT( grObj, pw );                         // write object type to curPointer
                pw++;   //go next
                *pw = grObj->states().count();  // write state count to currentPointer
                pw++;   //go next
                pState = pw;
                // states structure
                pw += grObj->states().count();  // current pointer now show first state( default )
                *pState = (quintptr)pw;                   // pState - pointer to pointer to first state
                writeDefaultProperties( grObj, pw );// write default properties structure
                pw += getObjectPropCount( grObj );  // add to current pointer property count. now pw shows Next state
                AddToStrInfo( "End of default properties", (quintptr)(pw - &structure[0]));
                // not default ( state 1, state 2, etc. )
                if ( grObj->states().count() > 1 ){
                    TItemState defState = grObj->states().first();
                    for( int i = 1; i < grObj->states().count(); i++ ){
                        pState++;                       // inc pState. Now it will point to State #i
                        *pState = (quintptr)pw;                   // write to pState address of state #i structure
                        *pw = 0;                        // clear counter
                        *pw++;                          // now pw point to the first property
                        TItemState state = grObj->states().at( i );
                        for( int j = 0; j < state.indexList.count(); j++)
                            if ( defState.indexList.at(j) != state.indexList.at(j) ){
                                quint32 *temp;
                                temp = (quint32*)*pState;
                                *(temp) += 1;
                                writeStateProperties( grObj, pw, j, state );
                                pw+=2;
                            }
                    }
                }
            }
        }
    }
    int j = fontOffset;
    if ( glyphTable.count()) AddToStrInfo( "Text Strings in innerCode", j);
    foreach(quint32 w, glyphTable){
        structure[j] = w;
        j++;
    }
    j = imagesOffset;
    if ( imagesTable.count()) AddToStrInfo( "Images tables", j );
    foreach(quint32 w, imagesTable ){
        structure[j] = w;
        j++;
    }
    mainStrInfo<<fontStrInfo;
#ifdef SHOWSTR
    qDebug()<<"mainStr"<<(quintptr)&structure[0]<<sizeof(quint32);
    int i = 0;
    for( i = 0; i < strSize; i++){
        qDebug()<<(quint32)structure[i]<<" --- address == "<<(quintptr)&structure[i];
    }
#endif
}


#define MinHeapAddr 65536
quint32* StructureCreator::calcControllerStructure(QList<MonitorScreen*> screenList, quint32 origin, quint32 ramOr, quint16 *size ){
//    origin = 0x800;
    mainStrInfo.clear();
    fontStrInfo.clear();
    glyphTable.clear();
    textArraySize = 0;
    originAddress = origin;
    ramOriginAddress = ramOr;
    resizeStructure( screenList );
//    QMessageBox::critical(0, "Info","Structure cleared", QMessageBox::Ok, QMessageBox::NoButton);
    generateInnerStructure();
    *size = strSize + glyphTable.count() + textArraySize + imagesTable.count();
    if ( *size == 0 ) return structure;
    quint32* strBegin = &structure[0];
    for(int i = 0; i < strSize; i++){
        // TODO setup to choose a type of processor. For now we will use div 2.
        if ( structure[i] > MinHeapAddr ) structure[i] = ( structure[i] - (quintptr)strBegin )/2 + originAddress;
    }
#ifndef SHOWSTR
    QMessageBox::critical(0, "Info","Start writing structure to file", QMessageBox::Ok, QMessageBox::NoButton);
    QFile file("1_Last_Memory_damp.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.close();
    file.open(QIODevice::WriteOnly);
    QTextStream stream( &file );
    stream<<"Address - Value ( HSB, MSB, LSB )\n";
    for( int i = 0; i < *size; i++){
        QString tempStr;
        bool hasInfo = false;
        int index;
        for(int j = 0; j < mainStrInfo.count(); j++ ){
            if ( mainStrInfo.at(j).blockOffset == i ) { hasInfo = true; index = j; }
        }
        if ( hasInfo ) tempStr += mainStrInfo.at(index).block + "\n";
        tempStr += QString::number(( (quintptr)&structure[i] - (quintptr)&structure[0] )/2 + originAddress, 16);
        tempStr += " - ";
        tempStr += QString().number( SetLSBfromHW(structure[i]), 16 ) + " ";
        tempStr += QString().number( SetMSB(structure[i]), 16 ) + " ";
        tempStr += QString().number( SetLSB(structure[i]), 16 ) + "\n";
        stream<<tempStr;
    }
    stream<<"End";
    file.close();
    //################### Structure file for simulator ######################
    QFile file2("1_Structure_vector.txt");
    file2.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file2.close();
    file2.open(QIODevice::WriteOnly);
    QTextStream stream2( &file2 );
    for( int i = 0; i < *size; i++ ) stream2<<QString().number( structure[i] ) + "\n";
    file2.close();
    //######################### End files creation ###########################
    QMessageBox::critical(0, "Info","End writing to file.\nStructure size = " + QString().number(*size),
                          QMessageBox::Ok, QMessageBox::NoButton);
    delete fontConverter;
    delete imageConverter;
#else
    qDebug()<<"After calc out Structure. Origin = "<<originAddress;
    for( int i = 0; i < strSize; i++){
        qDebug()<<(quint32)structure[i]<<" --- address == "<<(quintptr)&structure[i];
    }
#endif
    return &structure[0];
}

void StructureCreator::writeToPtrMOT( GrObject* grObj, quint32 *pw){
    qDebug()<<"mot "<<grObj->uniqueNumber()<<SetMSB(grObj->uniqueNumber())<<SetLSB(grObj->uniqueNumber());
    *pw = grObj->uniqueNumber();
//    switch( grObj->type()){
//        case GrBarType: *pw = motBar; break;
//        case GrButtonType: *pw = motButton; break;
//        case GrTextType: *pw = motStaticText; break;
//        case GrImageType: *pw = motImage; break;
//        case GrEnterAreaType: *pw = motDigEnterArea; break;
//    }
}

void StructureCreator::writeDefaultProperties( GrObject* obj, quint32* pw ){
    TStructureInfo tempInfo;
    AddToStrInfo( "Default properties of object", (quintptr)(pw - &structure[0]));
    switch( obj->type() ){
        case GrBarType:{
            GrBar* item = ( GrBar* )obj;
            TItemState state = item->states().first();
            QColor col = item->backgroundColor().at( state.indexList.at(GrBar::state_backgroundIndex)).color();
            *(pw + bar_Color ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            col = currentScreen->colorUnderItem( obj );
            *(pw + bar_bgColor ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            QPointF topLeft = item->rect().topLeft() + item->scenePos();
            QPointF botRight = item->rect().bottomRight() + item->scenePos();
            *(pw + bar_y0 ) = topLeft.y();
            *(pw + bar_y1 ) = botRight.y();
            *(pw + bar_x0 ) = topLeft.x();
            *(pw + bar_x1 ) = botRight.x();

        }break;
        case GrButtonType:{
            GrButton* item = (GrButton*)obj;
            TItemState state = item->states().first();
            QColor col = item->backgroundColor().at( state.indexList.at(GrButton::state_backgroundIndex)).color();
            *(pw + button_bgColor ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            *(pw + button_x0 ) = item->scenePos().x();
            *(pw + button_y0 ) = item->scenePos().y();
            *(pw + button_x1 ) = item->scenePos().x() + item->boundingRect().width();
            *(pw + button_y1 ) = item->scenePos().y() + item->boundingRect().height();
            *(pw + button_text ) = (quintptr)(&structure[0] + textOffset);
            quint32 *pText;
            pText = &structure[0] + textOffset;
            QVector<quint32> textArray = fontConverter->getTextArrays( item->text().at(state.indexList.at(GrButton::state_textIndex)), item->font());
            foreach( quint32 reg, textArray){
                *(pText++) = reg;
            }
            textOffset += textArray.count();
        }break;
        case GrTextType:{
            GrText* item = (GrText*)obj;
            TItemState state = item->states().first();
            QColor col = item->textColor().at( state.indexList.at(GrText::state_textColorIndex)).color();
            *(pw + stTxt_Color0 ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            col = currentScreen->colorUnderItem( obj );
            *(pw + stTxt_BkColor ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            switch( item->align() ){
            case Qt::AlignLeft: *(pw + stTxt_Align ) = txtAlignLeft; break;
            case Qt::AlignHCenter: *(pw + stTxt_Align ) = txtAlignCentre; break;
            case Qt::AlignRight: *(pw + stTxt_Align ) = txtAlignRight; break;
            default:
                *(pw + stTxt_Align ) = txtAlignLeft;
                break;
            }

            *(pw + stTxt_X0) = item->scenePos().x();
            *(pw + stTxt_Y0) = item->scenePos().y();
            *(pw + stTxt_X1) = item->scenePos().x() + item->getRect().width();
            *(pw + stTxt_Y1) = item->scenePos().y()+ item->getRect().height();
            *(pw + stTxt_state ) = txtModeNormal;
            *(pw + stTxt_Array ) = (quintptr)(&structure[0] + textOffset);
            *(pw + stTxt_Font) = (quintptr)(&structure[0] + fontOffset + fontConverter->fontOffset( item->font() ));

            quint32 *pText;
            pText = &structure[0] + textOffset;
            QVector<quint32> textArray = fontConverter->getTextArrays( item->text().at(state.indexList.at(GrText::state_textIndex)), item->font() );
            foreach( quint32 reg, textArray){ *(pText++) = reg; }
            textOffset += textArray.count();
        }break;
        case GrShapeType:{
            GrShape* item = (GrShape*)obj;
            TItemState state = item->states().first();
            QColor col = item->brush().at( state.indexList.at( GrShape::state_backgroundIndex )).color();
            *(pw + sh_bgColor ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            switch( item->shapeType() ){
            case shape_ellipse:
                *(pw + sh_type) = sh_type_ellipse;
//                *(pw + sh_radius )
                *(pw + sh_x0 ) = item->x();
                *(pw + sh_y0 ) = item->y();
                *(pw + sh_x1) = item->x() + item->width();
                *(pw + sh_y1) = item->y() + item->height();
                break;
            case shape_rect:
                *(pw + sh_type) = sh_type_rect;
                *(pw + sh_x0 ) = item->x();
                *(pw + sh_y0 ) = item->y();
                *(pw + sh_x1) = item->x() + item->width();
                *(pw + sh_y1) = item->y() + item->height();
                break;
            case shape_triangle:
                *(pw + sh_type) = sh_type_triangle;
                *(pw + sh_x0) = QPointF( item->path().elementAt(0)).x();
                *(pw + sh_y0) = QPointF( item->path().elementAt(0)).y();
                *(pw + sh_x1) = QPointF( item->path().elementAt(1)).x();
                *(pw + sh_y1) = QPointF( item->path().elementAt(1)).y();
                *(pw + sh_x2) = QPointF( item->path().elementAt(2)).x();
                *(pw + sh_y2) = QPointF( item->path().elementAt(2)).y();
                break;
            }
        }break;
        case GrEnterAreaType:{
            GrEnterArea* item = (GrEnterArea*)obj;
            TItemState state = item->states().first();
            QColor col = item->backgroundColor().at( state.indexList.at(GrEnterArea::state_backgroundIndex)).color();
            *( pw + db_flColor ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            *( pw + db_x0pos ) = item->scenePos().x();
            *( pw + db_y0pos ) = item->scenePos().y();
            *( pw + db_x1pos ) = item->scenePos().x() + item->boundingRect().width();
            *( pw + db_y1pos ) = item->scenePos().y() + item->boundingRect().height();
            *( pw + db_initVal ) = item->text().toInt();
            *( pw + db_state ) = state.isVisible;
            col = currentScreen->colorUnderItem( obj );
            *(pw + db_bgColor ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            col = item->borderColor().at( state.indexList.at(GrEnterArea::state_borderIndex)).color();
            *( pw + db_frColor ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            col = item->validTextColor().at( state.indexList.at(GrEnterArea::state_validColorIndex)).color();
            *( pw + db_dgColr ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            col = item->invalidTextColor().at( state.indexList.at(GrEnterArea::state_invalidColorIndex)).color();
            *( pw + db_invColor ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            *( pw + db_frWidth ) = item->borderColor().at( state.indexList.at(GrEnterArea::state_borderIndex)).width() + 1;
            *( pw + db_Font ) = (quintptr)(&structure[0] + fontOffset + fontConverter->fontOffset( item->font()) );
            *( pw + db_minVal ) = item->getTextValidator().min;
            *( pw + db_maxVal ) = item->getTextValidator().max;
            *( pw + db_dgCnt ) = 5;
            *( pw + db_Zero ) = fontConverter->getGlyphNumber("0", item->font());
            *( pw + db_adrRam ) = ramOriginAddress;
        }break;
        case GrImageType:{
            GrImage* item = (GrImage*)obj;
            TItemState state = item->states().first();
            QColor col = currentScreen->colorUnderItem( obj );
            *( pw + img_bgColor ) = RGB565CONVERT(col.red(), col.green(), col.blue());
            *( pw + img_x0 ) = item->scenePos().x();
            *( pw + img_y0 ) = item->scenePos().y();
            *( pw + img_scale ) = 1;
            *( pw + img_state ) = state.isVisible;
            QImage temp = item->images().at( state.indexList.at(GrImage::state_imageIndex));
            *( pw + img_image ) = (quintptr)(&structure[0] + imagesOffset + imageConverter->getImageOffset(temp));
        }break;
    }
}

void StructureCreator::writeStateProperties(GrObject *obj, quint32 *pw, quint16 index, TItemState state ){
    switch ( obj->type() ) {
        case GrBarType:
            if ( GrBar::state_backgroundIndex == index ){
                GrBar* item = ( GrBar* )obj;
                QColor col = item->backgroundColor().at( state.indexList.at(index) ).color();
                *pw = bar_Color;
                *(pw + 1) = RGB565CONVERT( col.red(), col.green(), col.blue() );
            }
            break;
        case GrButtonType:{
            GrButton* item = (GrButton*)obj;
            switch( index ){
                case GrButton::state_backgroundIndex:{
                    QColor col = item->backgroundColor().at( state.indexList.at(index) ).color();
                    *pw = button_bgColor;
                    *( pw + 1 ) = RGB565CONVERT( col.red(), col.green(), col.blue() );
                }break;
                case GrButton::state_textIndex:{
                    *pw = button_text;
                    *(pw+1) = (quintptr)(&structure[0] + textOffset);
                    quint32 *pText;
                    pText = &structure[0] + textOffset;
                    QVector<quint32> textArray;
                    textArray = fontConverter->getTextArrays( item->text().at(state.indexList.at(GrButton::state_textIndex)), item->font() );
                    foreach( quint32 reg, textArray){
                        *(pText++) = reg;
                    }
                    textOffset += textArray.count();
                }
                case GrButton::state_textPenIndex:{

                }break;
            }
        }break;
        case GrShapeType:
            if ( GrShape::state_backgroundIndex == index ){
                GrShape* item = ( GrShape* )obj;
                QColor col = item->brush().at( state.indexList.at( index )).color();
                *pw = sh_bgColor;
                *(pw + 1) = RGB565CONVERT( col.red(), col.green(), col.blue() );
            }
        break;
        case GrTextType:{
            GrText* item = (GrText*)obj;
            switch( index ){
                case GrText::state_textColorIndex:{
                    QColor col = item->textColor().at( state.indexList.at(index) ).color();
                    *pw = stTxt_Color0;
                    *( pw + 1 ) = RGB565CONVERT( col.red(), col.green(), col.blue() );
                }break;
                case GrText::state_textIndex:{
                    *( pw ) = stTxt_Array;
                    *( pw + 1 ) = (quintptr)(&structure[0] + textOffset);
                    quint32 *pText;
                    pText = &structure[0] + textOffset;
                    QVector<quint32> textArray = fontConverter->getTextArrays( item->text().at(state.indexList.at(index)), item->font() );
                    foreach( quint32 reg, textArray){ *(pText++) = reg; }
                    textOffset += textArray.count();
                }break;
            }
        }break;
        case GrEnterAreaType:{
            GrEnterArea* item = (GrEnterArea*)obj;
            switch( index ){
                case GrEnterArea::state_backgroundIndex:{
                    QColor col = item->backgroundColor().at( state.indexList.at(index) ).color();
                    *pw = db_flColor;
                    *( pw + 1 ) = RGB565CONVERT( col.red(), col.green(), col.blue() );
                }break;
                case GrEnterArea::state_borderIndex:{
                    QColor col = item->borderColor().at( state.indexList.at(index) ).color();
                    *pw = db_frColor;
                    *( pw + 1 ) = RGB565CONVERT( col.red(), col.green(), col.blue() );
                }break;
                case GrEnterArea::state_validColorIndex:{
                    QColor col = item->validTextColor().at( state.indexList.at(index)).color();
                    *pw = db_dgColr;
                    *( pw + 1 ) = RGB565CONVERT( col.red(), col.green(), col.blue() );
                }break;
                case GrEnterArea::state_invalidColorIndex:{
                    QColor col = item->validTextColor().at( state.indexList.at(index)).color();
                    *pw = db_invColor;
                    *( pw + 1 ) = RGB565CONVERT( col.red(), col.green(), col.blue() );
                }break;
            }
        }break;
        case GrImageType:{
            GrImage* item = (GrImage*)obj;
            switch( index ){
                case GrImage::state_imageIndex:{
                    QImage temp = item->images().at( state.indexList.at(index));
                    *pw = img_image;
                    *( pw + 1 ) = (quintptr)(&structure[0] + imagesOffset + imageConverter->getImageOffset(temp));
                }break;
            }
        }break;
        default:
            break;
    }
}

quint16 StructureCreator::getObjectPropCount( GrObject* obj ){
    quint16 res;
    switch( obj->type()){
        case GrBarType: res = bar_prop_end; break;
        case GrButtonType: res = button_prop_end; break;
        case GrTextType: res = stTxt_end; break;
        case GrImageType: res = img_end; break;
        case GrEnterAreaType: res = db_end; break;
    }
    return res;
}

void StructureCreator::setOrigin( quint32 origin){
    originAddress = origin;
}
