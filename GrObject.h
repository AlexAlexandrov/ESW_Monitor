#ifndef GROBJECT_H
#define GROBJECT_H

#include <QtGui>
#include "GlobalDefinition.h"
class GrObject : public QGraphicsItem
{
public:
    GrObject(QGraphicsItem *_parent = 0, QGraphicsScene *_scene = 0);
    TItemState currentState(){ return stateList[curState]; }
    QList<TItemState> states(){ return stateList; }
    void setStates( QList<TItemState> list ) { stateList = list; }
    void addState( TItemState state ) { stateList.push_back( state ); }
    void renameState( QString name ){ stateList[curState].name = name; }
    void replaceCurState( TItemState state );
    void deleteState( quint16 index ) { stateList.removeAt( index ); }
    virtual void setNextState() {};
    virtual void setPreviousState() {};
    virtual void setCurrentState( quint16 index) {};
    virtual QString name() {};
    virtual quint16 uniqueNumber() const {};
    quint16 currentStateIndex(){ return curState;}
    bool operator <(GrObject* other);
    bool operator >(GrObject* other);
protected:
    QList<TItemState> stateList;
    quint16 curState;
private:
    QString Name;
};

#endif // GROBJECT_H
