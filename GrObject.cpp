#include "GrObject.h"

GrObject::GrObject(QGraphicsItem* _parent, QGraphicsScene* _scene ): QGraphicsItem( _parent, _scene){
    static int i = 1;
    setZValue( i );
    i++;
}

void GrObject::replaceCurState(TItemState state){
    if ( curState ) stateList.replace( curState, state );
    setCurrentState( curState );
}

bool GrObject::operator <(GrObject *other){
    return zValue() < other->zValue();
}

bool GrObject::operator >(GrObject *other){
    return zValue() > other->zValue();
}

