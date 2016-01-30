#include "IdGenerator.h"

//================== Generate New ID and Add to end of the List or to an empty slot in the List ===========
IDGenerator::IDGenerator( quint8 deviceType ){
    IDList = new QList<void*>;
    type = deviceType;
}

IDGenerator::~IDGenerator(){
    delete IDList;
}

quint16 IDGenerator::NewID(){
    int i = 0;
    while(1){
        if (IDList->count() < (i+1)) break;
        if (IDList->at(i) == (void*)-1){
            IDList->replace(i,(void*)i);
            return i;
        }
        i++;
    }
    IDList->push_back((void*)i);
    return SetValMSBLSB(type - USERTYPE, i);
}
//================== Write value of non-existent ID ==========================
void IDGenerator::DeleteID(quint16 id){
    if (id < IDList->count()){
        IDList->replace( SetLSB(id),(void*)-1);
    }
}
//================== Return current IDs count ====================
int IDGenerator::IDCount(){
    return IDList->count();
}

void IDGenerator::ClearIDList(){
    IDList->clear();
}

void IDGenerator::SortAllId(){
    int cnt = IDList->count();
    for(int i = 0; i < cnt; i++){
        if ( IDList->at(i) == (void*)-1 ){
            IDList->move( i, cnt - 1 );
        }
    }
}
