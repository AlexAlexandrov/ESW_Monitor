#ifndef MONITORPROGRAMMER_H
#define MONITORPROGRAMMER_H
#include "toolwindow.h"
#include "MonitorScreen.h"
#include "StructureCreator.h"
#include <QThread>
#define TESTING 1

#define NEXT(index) buf[index++]

class MonitorProgrammer : public toolwindow
{
    Q_OBJECT
public:
    MonitorProgrammer( QList<MonitorScreen*> _screenList, QWidget *_parent = 0);
    virtual void CANmessages(TCanMessage Mes[MES_ARRAY_SIZE], int count);
private:
    bool eraseSended, sendClicked, continueSend;
    quint16 flashPageSize;
    quint16 flashPageQuontity;
//    QTimer timer;
    QPushButton* pbSendProgram;
    QPushButton* pbCreateStructure;
    QLabel* lOrigin;
    QList<MonitorScreen*> screenList;
    quint32 OriginAddress, ramOrigin;
    quint16 ramSize;
    quint32 OriginSize;
    quint8 tblpag;
    quint16 tblOffset;
    quint16 currentOffset;
    quint32* structure;
    quint16 size;
    bool originAsked;
    quint16 physicalNumber;
    void sendProgram();
    void eraseBeforeSend();
#ifdef TESTING
    QLineEdit* leObjNum;
    QLineEdit* leStateNum;
    QLineEdit* leScrNum;
    QLabel* labelFontOffset;
    QLabel* labelTextStringOffset;

#endif
private slots:
    void slotAskOrigin();
    void slotAskBPNums();
    void slotCreateStr();
    void slotOnSendProgram();
#ifdef TESTING
    void slotSetScreen();
    void slotSetState();
#endif
};

#endif // MONITORPROGRAMMER_H
