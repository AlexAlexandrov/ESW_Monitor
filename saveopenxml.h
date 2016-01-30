#ifndef SAVEOPENXML_H
#define SAVEOPENXML_H

#include "MonitorScreen.h"
#include <QtXml>
class SaveOpenXML
{
private:
    QObject* parent;
/*************************** Functions for save project **********************/
    QDomElement addBar(QDomDocument& domDoc, GrBar* fun);
    QDomElement addButton(QDomDocument &domDoc, GrButton *item);
    QDomElement addStaticText(QDomDocument &domDoc, GrText *item);
    QDomElement addEnterArea(QDomDocument &domDoc, GrEnterArea *item);
    QDomElement addImage(QDomDocument &domDoc, GrImage *item);
    QDomElement addShape(QDomDocument &domDoc, GrShape *item);
    QDomElement addAnimation( QDomDocument &domDoc, GrAnimation* item );
    QDomElement makeElement(      QDomDocument& domDoc,
                            const QString&      strName,                    // name of element
                            const QString&      strAttr = QString::null,    // Attribute
                            const QString&      strText = QString::null     // value
                           );
/************* Functions and variables for open project **********************/
    QList<TItemState> parseStates( QDomNode node, int statesCnt );
    GrText *parseStaticText( MonitorScreen *scr, QDomNode elem );
    GrBar  *parseBar( MonitorScreen* scr, QDomNode element );
    GrButton *parseButton(MonitorScreen *scr, QDomNode elem);
    GrImage *parseImage(MonitorScreen *scr, QDomNode elem);
    GrEnterArea *parseEnterArea(MonitorScreen *scr, QDomNode elem);
    GrShape *parseShape(MonitorScreen *scr, QDomNode elem);
    GrAnimation* parseAnimation(MonitorScreen *scr, QDomNode elem);
protected:
    SaveOpenXML( QObject* _parent = 0);
public:
    static SaveOpenXML* getXML( QObject* _parent = 0 );
    bool saveProject( QString path, QList<MonitorScreen *> scrList );
    bool openProject( QString path, QList<MonitorScreen *> *scrList );
};

#endif // SAVEOPENXML_H
