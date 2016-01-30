#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <QList>
#include "GlobalDefinition.h"

class IDGenerator
{
private:
    QList<void*> *IDList;
    quint8 type;
public:
    IDGenerator( quint8 deviceType );
    ~IDGenerator();
    quint16 NewID();
    void DeleteID( quint16 id );
    int IDCount();
    void ClearIDList();
    void SortAllId();
};

#endif // IDGENERATOR_H
