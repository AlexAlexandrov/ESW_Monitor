#include "saveopenxml.h"
static SaveOpenXML* _self  = 0;

SaveOpenXML::SaveOpenXML( QObject *_parent ){
    parent = _parent;
}

SaveOpenXML *SaveOpenXML::getXML(QObject *_parent){
    if ( _self ) return _self;
    _self = new SaveOpenXML( _parent );
    return _self;
}

/********************************************* Save **************************************/
bool SaveOpenXML::saveProject(QString path, QList<MonitorScreen*> scrList ){
    QDomDocument xmldoc("Esw_Monitor_Project");
    QDomElement domProject = xmldoc.createElement("Project");

    xmldoc.appendChild( domProject );
    domProject.appendChild( makeElement(xmldoc, "Screen_Size_width", "", QString().number( scrList.first()->sceneRect().size().width())) );
    domProject.appendChild( makeElement(xmldoc, "Screen_Size_height", "", QString().number( scrList.first()->sceneRect().size().height())) );
    QDomElement domScreens = xmldoc.createElement("Screens");
    QDomAttr attScrCnt = xmldoc.createAttribute("count");
    attScrCnt.setValue(QString().number(scrList.count()));
    domScreens.setAttributeNode( attScrCnt );

    domProject.appendChild( domScreens );
    foreach(MonitorScreen* scr, scrList ){
        QDomElement screen = xmldoc.createElement( "Screen" );
        domScreens.appendChild( screen );
        screen.appendChild( makeElement(xmldoc, "Screen_Color","", scr->backgroundColor().name()));
        QDomElement domObjects = xmldoc.createElement( "Objects" );
        QDomAttr attObjCnt = xmldoc.createAttribute("count");
        attObjCnt.setValue(QString().number(scr->items().count()));
        domObjects.setAttributeNode( attObjCnt );
        screen.appendChild( domObjects );
        foreach( GrObject* obj, scr->objects()){
            switch( obj->type() ){
            case GrBarType: domObjects.appendChild(addBar(xmldoc, (GrBar*)obj)); break;
            case GrButtonType: domObjects.appendChild(addButton(xmldoc, (GrButton*)obj )); break;
            case GrTextType: domObjects.appendChild(addStaticText(xmldoc, (GrText*)obj )); break;
            case GrImageType: domObjects.appendChild(addImage(xmldoc, (GrImage*)obj ));break;
            case GrShapeType: domObjects.appendChild(addShape(xmldoc, (GrShape*)obj ));break;
            case GrEnterAreaType: domObjects.appendChild(addEnterArea(xmldoc, (GrEnterArea*)obj ));break;
            case GrAnimationType: domObjects.appendChild(addAnimation(xmldoc, (GrAnimation*)obj ));break;
            }
        }
    }

    QFile file(path);
    if(file.open(QIODevice::WriteOnly)){
        QTextStream(&file) << xmldoc.toString();
        file.close();
        return true;
    }
    else return false;
}
QDomElement SaveOpenXML::addBar(QDomDocument &domDoc, GrBar *item){
    QDomElement domElement = makeElement( domDoc, "Object", QString().number( GrBarType ), "" );
    domElement.appendChild( makeElement( domDoc, "x0", "", QString().number( item->scenePos().x() )) );
    domElement.appendChild( makeElement( domDoc, "y0", "", QString().number( item->scenePos().y() )) );
    domElement.appendChild( makeElement( domDoc, "x1", "", QString().number( item->scenePos().x() + item->rect().width() )) );
    domElement.appendChild( makeElement( domDoc, "y1", "", QString().number( item->scenePos().y() + item->rect().height() )) );
    domElement.appendChild( makeElement( domDoc, "z", "", QString().number( item->zValue())) );

    for( int i = 0; i < item->backgroundColor().count(); i++){
        domElement.appendChild( makeElement( domDoc, "backgroundColor", QString().number(i), item->backgroundColor().at(i).color().name() ) );
    }
    for( int i = 0; i < item->borderColor().count(); i++){
        domElement.appendChild( makeElement( domDoc, "borderColor", QString().number(i), item->borderColor().at(i).color().name()));
    }
    QDomElement domStates = makeElement( domDoc, "Object_States", QString().number( item->states().count()), "");
    domElement.appendChild( domStates );
    foreach(TItemState state, item->states() ){
        QDomElement domOneState = makeElement( domDoc, "state", QString().number(state.indexList.count()), "");
        domStates.appendChild( domOneState );
        domOneState.appendChild( makeElement( domDoc, "name", "", state.name));
        domOneState.appendChild( makeElement( domDoc, "isVisible", "", QString().number(state.isVisible)));
        for(int i = 0; i < state.indexList.count(); i++){
            domOneState.appendChild( makeElement( domDoc, "IndexList", QString().number(i), QString().number(state.indexList.at(i))));
        }
    }
    return domElement;
}

QDomElement SaveOpenXML::addButton(QDomDocument &domDoc, GrButton *item){
    QDomElement domElement = makeElement( domDoc, "Object", QString().number( item->type() ), "" );
    domElement.appendChild( makeElement( domDoc, "x0", "", QString().number( item->scenePos().x() )) );
    domElement.appendChild( makeElement( domDoc, "y0", "", QString().number( item->scenePos().y() )) );
    domElement.appendChild( makeElement( domDoc, "x1", "", QString().number( item->scenePos().x() + item->boundingRect().width() )) );
    domElement.appendChild( makeElement( domDoc, "y1", "", QString().number( item->scenePos().y() + item->boundingRect().height() )) );
    domElement.appendChild( makeElement( domDoc, "z", "", QString().number( item->zValue())) );
    domElement.appendChild( makeElement( domDoc, "font_family", "", item->font().family()));
    domElement.appendChild( makeElement( domDoc, "font_pointSize", "", QString().number( item->font().pointSize() ) ));
    for( int i = 0; i < item->backgroundColor().count(); i++){
        domElement.appendChild( makeElement( domDoc, "backgroundColor", QString().number(i), item->backgroundColor().at(i).color().name() ) );
    }
    for( int i = 0; i < item->borderColor().count(); i++){
        domElement.appendChild( makeElement( domDoc, "borderColor", QString().number(i), item->borderColor().at(i).color().name()));
    }
    for( int i = 0; i < item->text().count(); i++){
        domElement.appendChild( makeElement( domDoc, "text", QString().number(i), item->text().at(i)));
    }
    for( int i = 0; i < item->textColor().count(); i++){
        domElement.appendChild( makeElement( domDoc, "textColor", QString().number(i), item->textColor().at(i).color().name()));
    }
    QDomElement domStates = makeElement( domDoc, "Object_States", QString().number( item->states().count()), "");
    domElement.appendChild( domStates );
    foreach(TItemState state, item->states() ){
        QDomElement domOneState = makeElement( domDoc, "state", QString().number(state.indexList.count()), "");
        domStates.appendChild( domOneState );
        domOneState.appendChild( makeElement( domDoc, "name", "", state.name));
        domOneState.appendChild( makeElement( domDoc, "isVisible", "", QString().number(state.isVisible)));
        for(int i = 0; i < state.indexList.count(); i++){
            domOneState.appendChild( makeElement( domDoc, "IndexList", QString().number(i), QString().number(state.indexList.at(i))));
        }
    }
    return domElement;
}

QDomElement SaveOpenXML::addStaticText(QDomDocument &domDoc, GrText *item){
    QDomElement domElement = makeElement( domDoc, "Object", QString().number( item->type() ), "" );
    domElement.appendChild( makeElement( domDoc, "x0", "", QString().number( item->scenePos().x() )) );
    domElement.appendChild( makeElement( domDoc, "y0", "", QString().number( item->scenePos().y() )) );
    domElement.appendChild( makeElement( domDoc, "x1", "", QString().number( item->scenePos().x() + item->getRect().width() )) );
    domElement.appendChild( makeElement( domDoc, "y1", "", QString().number( item->scenePos().y() + item->getRect().height() )) );
    domElement.appendChild( makeElement( domDoc, "z", "", QString().number( item->zValue())) );
    domElement.appendChild( makeElement( domDoc, "font_family", "", item->font().family()));
    domElement.appendChild( makeElement( domDoc, "font_pointSize", "", QString().number( item->font().pointSize() ) ));

    for( int i = 0; i < item->text().count(); i++){
        domElement.appendChild( makeElement( domDoc, "text", QString().number(i), item->text().at(i)));
    }
    for( int i = 0; i < item->textColor().count(); i++){
        domElement.appendChild( makeElement( domDoc, "textColor", QString().number(i), item->textColor().at(i).color().name()));
    }
    QDomElement domStates = makeElement( domDoc, "Object_States", QString().number( item->states().count()), "");
    domElement.appendChild( domStates );
    foreach(TItemState state, item->states() ){
        QDomElement domOneState = makeElement( domDoc, "state", QString().number(state.indexList.count()), "");
        domStates.appendChild( domOneState );
        domOneState.appendChild( makeElement( domDoc, "name", "", state.name));
        domOneState.appendChild( makeElement( domDoc, "isVisible", "", QString().number(state.isVisible)));
        for(int i = 0; i < state.indexList.count(); i++){
            domOneState.appendChild( makeElement( domDoc, "IndexList", QString().number(i), QString().number(state.indexList.at(i))));
        }
    }
    return domElement;
}

QDomElement SaveOpenXML::addEnterArea(QDomDocument &domDoc, GrEnterArea *item){
    QDomElement domElement = makeElement( domDoc, "Object", QString().number( item->type() ), "" );
    domElement.appendChild( makeElement( domDoc, "x0", "", QString().number( item->scenePos().x() )) );
    domElement.appendChild( makeElement( domDoc, "y0", "", QString().number( item->scenePos().y() )) );
    domElement.appendChild( makeElement( domDoc, "x1", "", QString().number( item->scenePos().x() + item->boundingRect().width() )) );
    domElement.appendChild( makeElement( domDoc, "y1", "", QString().number( item->scenePos().y() + item->boundingRect().height() )) );
    domElement.appendChild( makeElement( domDoc, "z", "", QString().number( item->zValue())) );
    domElement.appendChild( makeElement( domDoc, "font_family", "", item->font().family()));
    domElement.appendChild( makeElement( domDoc, "font_pointSize", "", QString().number( item->font().pointSize() ) ));
    domElement.appendChild( makeElement( domDoc, "min_value", "", QString().number( item->getTextValidator().min ) ));
    domElement.appendChild( makeElement( domDoc, "max_value", "", QString().number( item->getTextValidator().max ) ));

    for( int i = 0; i < item->backgroundColor().count(); i++){
        domElement.appendChild( makeElement( domDoc, "backgroundColor", QString().number(i), item->backgroundColor().at(i).color().name() ) );
    }
    for( int i = 0; i < item->borderColor().count(); i++){
        domElement.appendChild( makeElement( domDoc, "borderColor", QString().number(i), item->borderColor().at(i).color().name()));
    }
    domElement.appendChild( makeElement( domDoc, "text", "", item->text()));
    for( int i = 0; i < item->validTextColor().count(); i++){
        domElement.appendChild( makeElement( domDoc, "valid_textColor", QString().number(i), item->validTextColor().at(i).color().name()));
    }
    for( int i = 0; i < item->invalidTextColor().count(); i++){
        domElement.appendChild( makeElement( domDoc, "invalid_textColor", QString().number(i), item->invalidTextColor().at(i).color().name()));
    }

    QDomElement domStates = makeElement( domDoc, "Object_States", QString().number( item->states().count()), "");
    domElement.appendChild( domStates );
    foreach(TItemState state, item->states() ){
        QDomElement domOneState = makeElement( domDoc, "state", QString().number(state.indexList.count()), "");
        domStates.appendChild( domOneState );
        domOneState.appendChild( makeElement( domDoc, "name", "", state.name));
        domOneState.appendChild( makeElement( domDoc, "isVisible", "", QString().number(state.isVisible)));
        for(int i = 0; i < state.indexList.count(); i++){
            domOneState.appendChild( makeElement( domDoc, "IndexList", QString().number(i), QString().number(state.indexList.at(i))));
        }
    }
    return domElement;
}

QDomElement SaveOpenXML::addImage(QDomDocument &domDoc, GrImage *item){
    QDomElement domElement = makeElement( domDoc, "Object", QString().number( item->type() ), "" );
    domElement.appendChild( makeElement( domDoc, "x0", "", QString().number( item->scenePos().x() )) );
    domElement.appendChild( makeElement( domDoc, "y0", "", QString().number( item->scenePos().y() )) );
    domElement.appendChild( makeElement( domDoc, "x1", "", QString().number( item->scenePos().x() + item->rect().width() )) );
    domElement.appendChild( makeElement( domDoc, "y1", "", QString().number( item->scenePos().y() + item->rect().height() )) );
    domElement.appendChild( makeElement( domDoc, "z", "", QString().number( item->zValue())) );
    for( int i = 0; i < item->imagesPath().count(); i++){
        domElement.appendChild( makeElement( domDoc, "image_adress", QString().number(i), item->imagesPath().at(i) ) );
    }

    QDomElement domStates = makeElement( domDoc, "Object_States", QString().number( item->states().count()), "");
    domElement.appendChild( domStates );
    foreach(TItemState state, item->states() ){
        QDomElement domOneState = makeElement( domDoc, "state", QString().number(state.indexList.count()), "");
        domStates.appendChild( domOneState );
        domOneState.appendChild( makeElement( domDoc, "name", "", state.name));
        domOneState.appendChild( makeElement( domDoc, "isVisible", "", QString().number(state.isVisible)));
        for(int i = 0; i < state.indexList.count(); i++){
            domOneState.appendChild( makeElement( domDoc, "IndexList", QString().number(i), QString().number(state.indexList.at(i))));
        }
    }
    return domElement;
}

QDomElement SaveOpenXML::addShape(QDomDocument &domDoc, GrShape *item){
    QDomElement domElement = makeElement( domDoc, "Object", QString().number( item->type() ), "" );
    domElement.appendChild( makeElement( domDoc, "shape_type", "", QString().number( item->shapeType() )) );
    switch(item->shapeType()){
        case shape_rect: case shape_ellipse:
            domElement.appendChild( makeElement( domDoc, "x0", "", QString().number( item->scenePos().x() )) );
            domElement.appendChild( makeElement( domDoc, "y0", "", QString().number( item->scenePos().y() )) );
            domElement.appendChild( makeElement( domDoc, "width", "", QString().number( item->boundingRect().width() )) );
            domElement.appendChild( makeElement( domDoc, "height", "", QString().number( item->boundingRect().height() )) );
        break;
        case shape_triangle:
            domElement.appendChild( makeElement( domDoc, "posX", "", QString().number( item->scenePos().x() ) ));
            domElement.appendChild( makeElement( domDoc, "posY", "", QString().number( item->scenePos().y() ) ));
            domElement.appendChild( makeElement( domDoc, "x0", "", QString().number( QPointF(item->path().elementAt(0)).x() )) );
            domElement.appendChild( makeElement( domDoc, "y0", "", QString().number( QPointF(item->path().elementAt(0)).y() )) );
            domElement.appendChild( makeElement( domDoc, "x1", "", QString().number( QPointF(item->path().elementAt(1)).x() )) );
            domElement.appendChild( makeElement( domDoc, "y1", "", QString().number( QPointF(item->path().elementAt(1)).y() )) );
            domElement.appendChild( makeElement( domDoc, "x2", "", QString().number( QPointF(item->path().elementAt(2)).x() )) );
            domElement.appendChild( makeElement( domDoc, "y2", "", QString().number( QPointF(item->path().elementAt(2)).y() )) );
        break;
    }
    domElement.appendChild( makeElement( domDoc, "z", "", QString().number( item->zValue())) );
    for( int i = 0; i < item->brush().count(); i++){
        domElement.appendChild( makeElement( domDoc, "backgroundColor", QString().number(i), item->brush().at(i).color().name() ) );
    }
    for( int i = 0; i < item->pen().count(); i++){
        domElement.appendChild( makeElement( domDoc, "borderColor", QString().number(i), item->pen().at(i).color().name()));
    }
    QDomElement domStates = makeElement( domDoc, "Object_States", QString().number( item->states().count()), "");
    domElement.appendChild( domStates );
    foreach(TItemState state, item->states() ){
        QDomElement domOneState = makeElement( domDoc, "state", QString().number(state.indexList.count()), "");
        domStates.appendChild( domOneState );
        domOneState.appendChild( makeElement( domDoc, "name", "", state.name));
        domOneState.appendChild( makeElement( domDoc, "isVisible", "", QString().number(state.isVisible)));
        for(int i = 0; i < state.indexList.count(); i++){
            domOneState.appendChild( makeElement( domDoc, "IndexList", QString().number(i), QString().number(state.indexList.at(i))));
        }
    }
    return domElement;
}

QDomElement SaveOpenXML::addAnimation(QDomDocument &domDoc, GrAnimation *item){
    QDomElement domElement = makeElement( domDoc, "Object", QString().number( item->type() ), "" );
    domElement.appendChild( makeElement( domDoc, "x0", "", QString().number( item->scenePos().x() )) );
    domElement.appendChild( makeElement( domDoc, "y0", "", QString().number( item->scenePos().y() )) );
    domElement.appendChild( makeElement( domDoc, "x1", "", QString().number( item->scenePos().x() + item->rect().width() )) );
    domElement.appendChild( makeElement( domDoc, "y1", "", QString().number( item->scenePos().y() + item->rect().height() )) );
    domElement.appendChild( makeElement( domDoc, "z", "", QString().number( item->zValue())) );
    domElement.appendChild( makeElement( domDoc, "frequency", "", QString().number( item->frequency())) );
    domElement.appendChild( makeElement( domDoc, "repeat_type", "", QString().number( item->repeatType())) );

    for( int i = 0; i < item->frameAdr().count(); i++){
        domElement.appendChild( makeElement( domDoc, "frames_adress", QString().number(i), item->frameAdr( i ) ) );
    }

    QDomElement domStates = makeElement( domDoc, "Object_States", QString().number( item->states().count()), "");
    domElement.appendChild( domStates );
    foreach(TItemState state, item->states() ){
        QDomElement domOneState = makeElement( domDoc, "state", QString().number(state.indexList.count()), "");
        domStates.appendChild( domOneState );
        domOneState.appendChild( makeElement( domDoc, "name", "", state.name));
        domOneState.appendChild( makeElement( domDoc, "isVisible", "", QString().number(state.isVisible)));
        for(int i = 0; i < state.indexList.count(); i++){
            domOneState.appendChild( makeElement( domDoc, "IndexList", QString().number(i), QString().number(state.indexList.at(i))));
        }
    }
    return domElement;
}

QDomElement SaveOpenXML::makeElement(QDomDocument &domDoc, const QString &strName, const QString &strAttr, const QString &strText){
    QDomElement domElement = domDoc.createElement(strName);

    if (!strAttr.isEmpty()) {
        QDomAttr domAttr = domDoc.createAttribute("number");
        domAttr.setValue(strAttr);
        domElement.setAttributeNode(domAttr);
    }

    if (!strText.isEmpty()) {
        QDomText domText = domDoc.createTextNode(strText);
        domElement.appendChild(domText);
    }
    return domElement;
}

/********************************************* Open **************************************/
bool SaveOpenXML::openProject(QString path, QList<MonitorScreen *> *scrList){
    QDomDocument doc;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)){
        doc.setContent(&file);
        QDomElement root = doc.documentElement();
        if (root.tagName() != "Project") {
          qWarning("The file is not an items file");
          return false;
        }
        QSizeF screenSize;
        QDomNode node = root.firstChild();
        screenSize.setWidth( node.toElement().text().toFloat() );
        node = node.nextSibling();
        screenSize.setHeight( node.toElement().text().toFloat() );
        node = node.nextSibling();
        int screensCnt = node.toElement().attribute("count","").toInt();
        QDomNode ScreensNode = node.firstChild();
        for(int i = 0; i < screensCnt; i++){
            QDomNode screenNode = ScreensNode.firstChild();
            MonitorScreen* scr = new MonitorScreen( parent, QRectF(QPointF(0,0), screenSize));
            scrList->push_back( scr );
            scr->setBackgroundColor( screenNode.toElement().text() );
            screenNode = screenNode.nextSibling();
            int objectCnt = screenNode.toElement().attribute("count").toInt();
            QDomNode objectsNode = screenNode.firstChild();
            for( int j = 0; j < objectCnt; j++ ){
                quint16 type = objectsNode.toElement().attribute("number").toInt();
                switch( type ){
                case GrTextType: parseStaticText( scr, objectsNode.firstChild()); break;
                case GrBarType: parseBar( scr, objectsNode.firstChild()); break;
                case GrButtonType: parseButton( scr, objectsNode.firstChild()); break;
                case GrEnterAreaType: parseEnterArea( scr, objectsNode.firstChild()); break;
                case GrImageType: parseImage( scr, objectsNode.firstChild()); break;
                case GrShapeType: parseShape( scr, objectsNode.firstChild()); break;
                case GrAnimationType: parseAnimation( scr, objectsNode.firstChild()); break;
                }
                objectsNode = objectsNode.nextSibling();
            }
            ScreensNode = ScreensNode.nextSibling();
            qDebug()<<screenNode.toElement().tagName();
        }
    }

}

QList<TItemState> SaveOpenXML::parseStates(QDomNode node, int statesCnt){
    QList<TItemState> tempList;
    for(int i = 0; i < statesCnt; i++){
        TItemState tempState;
        int indexCnt = node.toElement().attribute("number").toInt();
        QDomNode stateChild = node.firstChild();
        tempState.name = stateChild.toElement().text();
        stateChild = stateChild.nextSibling();
        tempState.isVisible = stateChild.toElement().text().toInt();
        stateChild = stateChild.nextSibling();
        for(int j = 0; j < indexCnt; j++ ){
            tempState.indexList.push_back( stateChild.toElement().text().toInt() );
            stateChild = stateChild.nextSibling();
        }
        tempList.push_back( tempState );
        node = node.nextSibling();
    }

    foreach(TItemState st, tempList){
        qDebug()<<st.name<<st.isVisible<<st.indexList;
    }

    return tempList;
}

GrText* SaveOpenXML::parseStaticText(MonitorScreen* scr, QDomNode elem){
    QPointF p0, p1;
    p0.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p0.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    int z = elem.toElement().text().toInt();
    elem = elem.nextSibling();
    QString font_family = elem.toElement().text();
    elem = elem.nextSibling();
    QFont font(font_family, elem.toElement().text().toInt());
    elem = elem.nextSibling();
    QStringList textList;
    while( !elem.isNull() && elem.toElement().tagName() == "text" ){
        textList.push_back( elem.toElement().text() );
        elem = elem.nextSibling();
    }
    QList<QColor> colorList;
    while( !elem.isNull() && elem.toElement().tagName() == "textColor" ){
        colorList.push_back( QColor(elem.toElement().text()));
        elem = elem.nextSibling();
    }
    QList<TItemState> states = parseStates( elem.firstChild(), elem.toElement().attribute("number").toInt());
    GrText* textObj = scr->slotAddTextItem();
    textObj->setPos( p0 );
    QPointF zeroPoint(0,0);
    textObj->setRect( QRectF(zeroPoint, p1));
    textObj->setFont( font );
    textObj->addText( textList );
    textObj->setTextColor( colorList );
    textObj->setZValue( z );
    textObj->setStates( states );
    return textObj;
}

GrBar *SaveOpenXML::parseBar(MonitorScreen* scr, QDomNode elem){
    QPointF p0, p1;
    QPointF zeroPoint(0,0);
    // coords
    p0.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p0.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    int z = elem.toElement().text().toInt();
    elem = elem.nextSibling();
    //colors
    QList<QColor> backgroundColorList;
    while( !elem.isNull() && elem.toElement().tagName() == "backgroundColor" ){
        backgroundColorList.push_back( QColor(elem.toElement().text()) );
        elem = elem.nextSibling();
    }
    QList<QColor> borderColorList;
    while( !elem.isNull() && elem.toElement().tagName() == "borderColor" ){
        borderColorList.push_back( QColor(elem.toElement().text()));
        elem = elem.nextSibling();
    }
    //states
    QList<TItemState> states = parseStates( elem.firstChild(), elem.toElement().attribute("number").toInt());
    //creation
    GrBar* barObj = scr->slotAddBarItem();
    barObj->setPos( p0 );
    barObj->setZValue( z );
    barObj->setRect( QRectF(zeroPoint, p1 - p0 ));
    barObj->setBackgroundColor( backgroundColorList );
    barObj->setBorderColor( borderColorList );
    barObj->setStates( states );
    return barObj;
}

// coords
// font
// bgColor
// borderColor
// textList
// textColor
// States
GrButton* SaveOpenXML::parseButton(MonitorScreen* scr, QDomNode elem){
    QPointF p0, p1;
    QPointF zeroPoint(0,0);
    // parse coords
    p0.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p0.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    int z = elem.toElement().text().toInt();
    elem = elem.nextSibling();
    // parse font
    QString font_family = elem.toElement().text();
    elem = elem.nextSibling();
    QFont font(font_family, elem.toElement().text().toInt());
    elem = elem.nextSibling();
    // parse bgColor
    QList<QColor> backgroundColorList;
    while( !elem.isNull() && elem.toElement().tagName() == "backgroundColor" ){
        backgroundColorList.push_back( QColor(elem.toElement().text()) );
        elem = elem.nextSibling();
    }
    // parse border color
    QList<QColor> borderColorList;
    while( !elem.isNull() && elem.toElement().tagName() == "borderColor" ){
        borderColorList.push_back( QColor(elem.toElement().text()));
        elem = elem.nextSibling();
    }
    // parse text List
    QStringList textList;
    while( !elem.isNull() && elem.toElement().tagName() == "text" ){
        textList.push_back( elem.toElement().text() );
        elem = elem.nextSibling();
    }
    // parse text color
    QList<QColor> colorList;
    while( !elem.isNull() && elem.toElement().tagName() == "textColor" ){
        colorList.push_back( QColor(elem.toElement().text()));
        elem = elem.nextSibling();
    }
    // parse state
    QList<TItemState> states = parseStates( elem.firstChild(), elem.toElement().attribute("number").toInt());
    // create object
    GrButton* obj = scr->slotAddButtonItem();
    obj->setPos( p0 );
    obj->setZValue( z );
    obj->setRect( QRectF(zeroPoint, p1 - p0 ));
    obj->setFont( font );
    obj->setBackgroundColor( backgroundColorList );
    obj->setBorderColor( borderColorList );
    obj->setText(textList);
    obj->setTextColor( colorList );
    obj->setStates( states );
    return obj;
}

// coords
// image address
// states
GrImage *SaveOpenXML::parseImage(MonitorScreen *scr, QDomNode elem){
    QPointF p0, p1;
    QPointF zeroPoint(0,0);
    // parse coords
    p0.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p0.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    int z = elem.toElement().text().toInt();
    elem = elem.nextSibling();
    // parse image addresses
    QStringList adrList;
    while( !elem.isNull() && elem.toElement().tagName() == "image_adress" ){
        adrList.push_back( elem.toElement().text() );
        elem = elem.nextSibling();
    }
    // parse states
    QList<TItemState> states = parseStates( elem.firstChild(), elem.toElement().attribute("number").toInt());
    // create object
    GrImage* obj = scr->slotAddImageItem();
    obj->setPos( p0 );
    obj->setZValue( z );
    obj->setRect( QRectF(zeroPoint, p1 - p0 ));
    obj->setImage( adrList );
    obj->setStates( states );
    return obj;
}

// coords
// font
// min Value
// max value
// bgColor
// borderColor
// text ( one string )
// validText Color
// invalidText Color
// States
GrEnterArea *SaveOpenXML::parseEnterArea(MonitorScreen *scr, QDomNode elem){
    QPointF p0, p1;
    QPointF zeroPoint(0,0);
    // parse coords
    p0.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p0.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    int z = elem.toElement().text().toInt();
    elem = elem.nextSibling();
    // parse font
    QString font_family = elem.toElement().text();
    elem = elem.nextSibling();
    QFont font(font_family, elem.toElement().text().toInt());
    elem = elem.nextSibling();
    int minVal = elem.toElement().text().toInt();
    elem = elem.nextSibling();
    int maxVal = elem.toElement().text().toInt();
    elem = elem.nextSibling();
    // parse bgColor
    QList<QColor> backgroundColorList;
    while( !elem.isNull() && elem.toElement().tagName() == "backgroundColor" ){
        backgroundColorList.push_back( QColor(elem.toElement().text()) );
        elem = elem.nextSibling();
    }
    // parse border color
    QList<QColor> borderColorList;
    while( !elem.isNull() && elem.toElement().tagName() == "borderColor" ){
        borderColorList.push_back( QColor(elem.toElement().text()));
        elem = elem.nextSibling();
    }
    // parse text
    QString text = elem.toElement().text();
    elem = elem.nextSibling();
    // parse valid text color
    QList<QColor> validColorList;
    while( !elem.isNull() && elem.toElement().tagName() == "valid_textColor" ){
        validColorList.push_back( QColor(elem.toElement().text()));
        elem = elem.nextSibling();
    }
    // parse invalid text color
    QList<QColor> invalidColorList;
    while( !elem.isNull() && elem.toElement().tagName() == "invalid_textColor" ){
        invalidColorList.push_back( QColor(elem.toElement().text()));
        elem = elem.nextSibling();
    }
    // parse state
    QList<TItemState> states = parseStates( elem.firstChild(), elem.toElement().attribute("number").toInt());
    // create object
    GrEnterArea* obj = scr->slotAddEnterAreaItem();
    obj->setPos( p0 );
    obj->setZValue( z );
    obj->setRect( QRectF(zeroPoint, p1 - p0 ));
    obj->setFont( font );
    obj->setBackgroundColor( backgroundColorList );
    obj->setBorderColor( borderColorList );
    obj->setValidTextColor( validColorList );
    obj->setInvalidTextColor( invalidColorList );
    obj->setText( text );
    obj->setTextValidator( minVal, maxVal );
    obj->setStates( states );
    return obj;

}

// shape_type
// coords
// bgColor
// borderColor
// shape
GrShape *SaveOpenXML::parseShape(MonitorScreen *scr, QDomNode elem){
    QPointF p0, p1, p2, pos;
    QPointF zeroPoint(0,0);
    QSizeF  size;
    TShapeTypes shape_type = (TShapeTypes)elem.toElement().text().toInt();
    elem = elem.nextSibling();
    // parse coords
    switch( shape_type ){
        case shape_rect: case shape_ellipse:
            pos.setX( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
            pos.setY( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
            size.setWidth( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
            size.setHeight( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
        break;
        case shape_triangle:
            pos.setX( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
            pos.setY( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
            p0.setX( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
            p0.setY( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
            p1.setX( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
            p1.setY( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
            p2.setX( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
            p2.setY( elem.toElement().text().toInt() );
            elem = elem.nextSibling();
        break;
    }
    int z = elem.toElement().text().toInt();
    elem = elem.nextSibling();
    // parse bgColor
    QList<QColor> backgroundColorList;
    while( !elem.isNull() && elem.toElement().tagName() == "backgroundColor" ){
        backgroundColorList.push_back( QColor(elem.toElement().text()) );
        elem = elem.nextSibling();
    }
    // parse border color
    QList<QColor> borderColorList;
    while( !elem.isNull() && elem.toElement().tagName() == "borderColor" ){
        borderColorList.push_back( QColor(elem.toElement().text()));
        elem = elem.nextSibling();
    }
    // parse state
    QList<TItemState> states = parseStates( elem.firstChild(), elem.toElement().attribute("number").toInt());
    // create object
    GrShape* obj = scr->slotAddShapeItem();
    obj->setPos( pos );
    obj->setZValue( z );
    obj->setObjectShape( shape_type );
    if ( shape_type == shape_triangle ){
        QPainterPath path;
        path.moveTo( p0 );
        path.lineTo( p1 );
        path.lineTo( p2 );
        path.lineTo( p0 );
        obj->setPath( path );
    }
    else obj->setRect( QRectF( zeroPoint, size ));
    obj->setBackgroundColor( backgroundColorList );
    obj->setBorderColor( borderColorList );
    obj->setStates( states );
    return obj;
}

// coords
// frequency
// repeat type
// frames
// states
GrAnimation *SaveOpenXML::parseAnimation(MonitorScreen *scr, QDomNode elem){
    QPointF p0, p1;
    QPointF zeroPoint(0,0);
    // parse coords
    p0.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p0.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setX( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    p1.setY( elem.toElement().text().toInt() );
    elem = elem.nextSibling();
    int z = elem.toElement().text().toInt();
    elem = elem.nextSibling();
    // parse frequency
    quint16 frequency = elem.toElement().text().toInt();
    elem = elem.nextSibling();
    // parse type
    TAnimeType repeatType = (TAnimeType)elem.toElement().text().toInt();
    elem = elem.nextSibling();
    // parse image addresses
    QStringList adrList;
    while( !elem.isNull() && elem.toElement().tagName() == "frames_adress" ){
        adrList.push_back( elem.toElement().text() );
        elem = elem.nextSibling();
    }
    // parse states
    QList<TItemState> states = parseStates( elem.firstChild(), elem.toElement().attribute("number").toInt());
    // create object
    GrAnimation* obj = scr->slotAddAnimationItem();
    obj->setPos( p0 );
    obj->setZValue( z );
    obj->setRect( QRectF(zeroPoint, p1 - p0 ));
    obj->setFrame( adrList );
    obj->setFrequency( frequency );
    obj->setRepeatType( repeatType );
    obj->setStates( states );
    return obj;
}
