#include "MonitorProgrammer.h"
#include "CanKingdomInterface.h"
#include "CanKingdomMonitorInterface.h"

MonitorProgrammer::MonitorProgrammer( QList<MonitorScreen*> _screenList, QWidget *_parent ): toolwindow( _parent ){
    screenList = _screenList;
    QVBoxLayout* mainlay = new QVBoxLayout( this );
    QPushButton* pbAskOrig = new QPushButton("2. &Ask original table address", this );
    QPushButton* pbAskBPNums = new QPushButton("1. Ask &device address", this );
    pbAskBPNums->setAttribute( Qt::WA_DeleteOnClose);
    pbAskOrig->setAttribute( Qt::WA_DeleteOnClose);
    pbCreateStructure = new QPushButton("3. &Create structure", this );
    pbCreateStructure->setAttribute(Qt::WA_DeleteOnClose);
    pbSendProgram = new QPushButton("4. &Send monitors program", this );
    pbSendProgram->setAttribute( Qt::WA_DeleteOnClose );

    lOrigin = new QLabel( this );
    pbSendProgram->setDisabled( true );
    pbCreateStructure->setDisabled( true );
    lOrigin->setAttribute(Qt::WA_DeleteOnClose);

    mainlay->addWidget( pbAskBPNums );
    mainlay->addWidget( pbAskOrig );
    mainlay->addWidget( lOrigin );
    mainlay->addWidget( pbCreateStructure );
    mainlay->addWidget( pbSendProgram );
#ifdef TESTING
    pbCreateStructure->setEnabled( true );
    pbSendProgram->setEnabled( true );
    mainlay->addSpacing( 10 );

    leScrNum = new QLineEdit( this );
    leScrNum->setText("0");
    leScrNum->setAttribute( Qt::WA_DeleteOnClose );
    QLabel* labelScrNum = new QLabel("Screen number:", this );
    QPushButton* pbSetScreen = new QPushButton("Set Screen", this );
    QHBoxLayout* layScreen = new QHBoxLayout();
    QLabel* lTestScreen = new QLabel("TEST. Set screen", this );
    layScreen->addWidget( labelScrNum );
    layScreen->addWidget( leScrNum );
    mainlay->addWidget( lTestScreen );
    mainlay->addItem( layScreen );
    mainlay->addWidget( pbSetScreen );
    mainlay->addSpacing( 10 );

    QPushButton* pbSetState = new QPushButton("Set object state");
    QHBoxLayout* layObject = new QHBoxLayout();
    QHBoxLayout* layState = new QHBoxLayout();
    QLabel* lObject = new QLabel( "Object num:", this );
    QLabel* lState = new QLabel( "State num:", this );
    leObjNum = new QLineEdit(this );
    leObjNum->setText( "0" );
    leStateNum = new QLineEdit(this );
    leStateNum->setText("1");
    pbSetState->setAttribute( Qt::WA_DeleteOnClose );
    leObjNum->setAttribute( Qt::WA_DeleteOnClose );
    leStateNum->setAttribute( Qt::WA_DeleteOnClose );
    layObject->addWidget( lObject );
    layObject->addWidget( leObjNum );
    layState->addWidget( lState );
    layState->addWidget( leStateNum );
    QLabel* lTestState = new QLabel("TEST. Set state.", this );
    labelFontOffset = new QLabel("Font address:",this);
    labelTextStringOffset = new QLabel("text string address:", this);
    mainlay->addWidget( lTestState );
    mainlay->addItem( layObject );
    mainlay->addItem( layState );
    mainlay->addWidget( pbSetState );
    mainlay->addWidget( labelFontOffset );
    mainlay->addWidget( labelTextStringOffset);

    connect( pbSetScreen, SIGNAL(clicked()), this, SLOT(slotSetScreen()));
    connect( pbSetState, SIGNAL(clicked()), this, SLOT(slotSetState()));
#endif
    connect( pbCreateStructure, SIGNAL(clicked()), this, SLOT(slotCreateStr()));
    connect( pbSendProgram, SIGNAL(clicked()), this, SLOT(slotOnSendProgram()));
    connect( pbAskBPNums, SIGNAL(clicked()), this, SLOT(slotAskBPNums()));
    connect( pbAskOrig, SIGNAL(clicked()), this, SLOT(slotAskOrigin()));
//    this->setLayout( mainlay );
    originAsked = false;
//    sendProgram();
    continueSend = false;
}

void MonitorProgrammer::CANmessages(TCanMessage Mes[], int count){
    for( int i = 0; i < count; i++){
        if ( Mes[i].DLC == kMayorOriginMesLength && Mes[i].Data[1] == kGetMonitorOrigin && originAsked ){
            switch( Mes[i].Data[kOriginCommand] ){
            case kOriginComFlash:
                tblpag = SetValMSBLSB( Mes[i].Data[kOriginPagLineMsb], Mes[i].Data[kOriginPagLineLsb] );
                tblOffset = SetValMSBLSB( Mes[i].Data[kOriginOffLineMsb], Mes[i].Data[kOriginOffLineLsb] );
                OriginAddress = (tblpag) | (tblOffset);
                lOrigin->setText("Origin address tblpag(16): " + QString().number(tblpag, 16) + " tblOff(16): " + QString().number(tblOffset, 16));
                QMessageBox::information(0, "Flash origin", lOrigin->text(), QMessageBox::Ok );
                currentOffset = tblOffset;
                break;
            case kOriginComRAM:
                ramOrigin = SetValMSBLSB( Mes[i].Data[kOriginRamAdrMsb], Mes[i].Data[kOriginRamAdrLsb] );
                ramSize = SetValMSBLSB( Mes[i].Data[kOriginRamSizeMsb], Mes[i].Data[kOriginRamSizeLsb] );
                QMessageBox::information(0, "Ram origin", "Ram address(16):"+QString().number( ramOrigin, 16 ) + "\nRam Size(16): "+QString().number(ramSize, 16), QMessageBox::Ok );
                lOrigin->setText( lOrigin->text()+"\nRam address:"+QString().number( ramOrigin ) );
                break;
            case kOriginComFlashSize:
                flashPageSize = SetValMSBLSB( Mes[i].Data[kOriginPagSizeMsb], Mes[i].Data[kOriginPagSizeLsb] );
                flashPageQuontity = SetValMSBLSB( Mes[i].Data[kOriginPagesCntMsb], Mes[i].Data[kOriginPagesCntLsb] );
                QMessageBox::information(0, "Flash page size/cnt", "Flash page size(16):"+QString().number( flashPageSize, 16 ) + "\nFlash page quontity( dec ): "+QString().number(flashPageQuontity, 10), QMessageBox::Ok );
                break;
            }
            pbSendProgram->setEnabled( true );
            pbCreateStructure->setEnabled( true );
        }
        else if ( Mes[i].DLC == kMayorMemCheckLength && Mes[i].Data[1] == kMayorMemCheckPage ){
            if ( Mes[i].Data[kMayorMemCheckCodeLine] == kMonitorFlashEmpty ) sendProgram();
            else eraseBeforeSend();
        }
         else if ( Mes[i].Data[1] == ckERRORPAGE && Mes[i].Data[5] == kPrgMemPage && continueSend ){
            eraseBeforeSend();
        }
        else if ( Mes[i].Data[1] == ckERRORPAGE && Mes[i].Data[5] == kPrgMemPage && eraseSended ){
            eraseSended = false;
            slotOnSendProgram();
        }
        else if ( Mes[i].Data[1] == kPhysicalNumPage ){
            physicalNumber = Mes[i].Data[kPhysNumLine];
        }
    }
}


void MonitorProgrammer::sendProgram(){
    quint16 lw;
//    quint32* structure;
//    quint16 size;

//    try{
//       structure = StructureCreator::getCreator()->calcControllerStructure( screenList, OriginAddress, ramOrigin, &size );
//       QMessageBox::critical(0, "Info","Structure creation finished for "+QString().number(screenList.count())+" screens"
//                                 +"\nStructure size:"+QString().number(size), QMessageBox::Ok, QMessageBox::NoButton);
//    }
//    catch(...){
//        QMessageBox::critical(0, "test", "Error while generate structure", QMessageBox::Ok, QMessageBox::NoButton);
//    }

#ifdef TESTING
    labelTextStringOffset->setText( "The first text string(line) address:"+QString().number(StructureCreator::getCreator()->textAddress()));
    labelFontOffset->setText( "The bitmap address:"+QString().number(StructureCreator::getCreator()->fontAddress()));
#endif
//    return;
    int j = 1;
    char buf[USB_BULK_SIZE];
    quint32 off = OriginAddress;
    try{
        QMessageBox::information(0, "Info","Start sending structure", QMessageBox::Ok);
        for( int i = 0; i < size; i++){
            j = 1;
            // Begin write flash word
            NEXT(j) = SetIDLow(0,0,6);
            NEXT(j) = SetIDHi(0);
            NEXT(j) = physicalNumber;     //d0
            NEXT(j) = kPrgMemPage;   //d1
            NEXT(j) = kPrgBeginWordWriteCommand; //d2
            NEXT(j) = SetHSB(off); //d3
            NEXT(j) = SetMSB(off); //d4
            NEXT(j) = SetLSB(off); //d5
            // Write flash word
            NEXT(j) = SetIDLow(0,0,6);
            NEXT(j) = SetIDHi(0);
            NEXT(j) = physicalNumber;     //d0
            NEXT(j) = kPrgMemPage;   //d1
            NEXT(j) = kPrgWordWriteCommand; //d2
            NEXT(j) = SetLSBfromHW(structure[i]); //d3
            lw = SetLW( structure[i] ); //get low word from structure
            NEXT(j) = SetMSB(lw); //d4
            NEXT(j) = SetLSB(lw); //d5
            buf[0] = j - 1;
            LibUSBConnector::GetLibUSBConnector()->SendToUSB(buf, j);
//            QThread::usleep( 100 );
        #ifdef TESTING
            if ( i % 50 == 0 ){
            QString temp = "With message appears every 50th message\nFlashword full"+QString().number(structure[i])+"\nHighWord = "+QString().number(SetHW(structure[i]))
                    +"\nLowWord"+QString().number(lw)+"\noffset ="+QString().number(off);
            QMessageBox::critical(0, "test", temp, QMessageBox::Ok, QMessageBox::NoButton);
            }
        #endif
            off += 2;
        }
        QMessageBox::information(0, "Info","Finish sending structure", QMessageBox::Ok);
    }
    catch(...){
        QMessageBox::critical(0, "test", "Error while sending structure by CAN", QMessageBox::Ok, QMessageBox::NoButton);
    }
}



void MonitorProgrammer::eraseBeforeSend(){
//    eraseSended = false;
//    QString temp = "Monitor contains old program.\nBefore uploading new program old will be deleted";
//    if ( QMessageBox::question(this, "Do you want to erase old program?", temp, QMessageBox::Ok, QMessageBox::Cancel) ){
//        // erase command and call check again
//        char buf[USB_BULK_SIZE];
//        int j = 1;
//        NEXT(j) = SetIDLow(0,0,6);
//        NEXT(j) = SetIDHi(0);
//        NEXT(j) = physicalNumber;       //d0
//        NEXT(j) = kPrgMemPage;          //d1
//        NEXT(j) = kPrgErasePageCommand; //d2
//        NEXT(j) = tblpag;               //d3
//        NEXT(j) = SetMSB(tblOffset);    //d4
//        NEXT(j) = SetLSB(tblOffset);    //d5
//        buf[0] = j - 1;
//        if ( LibUSBConnector::GetLibUSBConnector()->SendToUSB(buf, j)) eraseSended = true;
//    }
    static quint32 off = OriginAddress;
    static quint16 pgNum = 0;
    char buf[USB_BULK_SIZE];
    int j = 1;
    NEXT(j) = SetIDLow(0,0,6);
    NEXT(j) = SetIDHi(0);
    NEXT(j) = physicalNumber;       //d0
    NEXT(j) = kPrgMemPage;          //d1
    NEXT(j) = kPrgErasePageCommand; //d2
    NEXT(j) = SetHSB(off);               //d3
    NEXT(j) = SetMSB(off);    //d4
    NEXT(j) = SetLSB(off);    //d5
    buf[0] = j - 1;
    if ( LibUSBConnector::GetLibUSBConnector()->SendToUSB(buf, j)){
        pgNum++;
        if ( pgNum < flashPageQuontity ){
            off += flashPageSize;
            continueSend = true;
        }
        else{
            off = tblOffset;
            pgNum = 0;
            continueSend = false;
            eraseSended = true;
        }
    }

}

void MonitorProgrammer::slotAskOrigin(){
    originAsked = true;
    int i = 1;
    char buf[USB_BULK_SIZE];
    NEXT(i) = SetIDLow(0, 0, 2);                // King's ID = 0 Message data length - 5
    NEXT(i) = SetIDHi(0);                       // King's ID = 0
    NEXT(i) = physicalNumber;                                // line 0 Phys num
    NEXT(i) = 150;                              // line 1 Page num
    buf[0] = i - 1;
    LibUSBConnector::GetLibUSBConnector()->SendToUSB(buf,i);
}

void MonitorProgrammer::slotAskBPNums(){
    int i = 1;
    char buf[USB_BULK_SIZE];
    NEXT(i) = SetIDLow(0, 0, 5);                // King's ID = 0 Message data length - 5
    NEXT(i) = SetIDHi(0);                       // King's ID = 0
    NEXT(i) = 0;                                // line 0 Phys num
    NEXT(i) = kPhysicalNumPage;                 // line 1 Page num
    NEXT(i) = kPhysicalNumGet;                  // line 2 command(get/set) Physical Number and Base Number
    NEXT(i) = 0;                                // line 3 empty
    NEXT(i) = 0;                                // line 4 empty
    buf[0] = i - 1;
    LibUSBConnector::GetLibUSBConnector()->SendToUSB(buf,i);
}

void MonitorProgrammer::slotCreateStr(){
    size = 0;
    while( size == 0 ){
        structure = StructureCreator::getCreator()->calcControllerStructure( screenList, OriginAddress, ramOrigin, &size );
    }
}

void MonitorProgrammer::slotOnSendProgram(){
    //    sendProgram();
    originAsked = false;
    int i = 1;
    char buf[USB_BULK_SIZE];
    NEXT(i) = SetIDLow(0, 0, 3);                // King's ID = 0 Message data length - 3
    NEXT(i) = SetIDHi(0);                       // King's ID = 0
    NEXT(i) = physicalNumber;                   // line 0 Phys num
    NEXT(i) = kCheckFlashMem;                   // line 1 Page num
    NEXT(i) = kCheckFlashMemFlagOnly;           // line 2 command
    buf[0] = i - 1;
    LibUSBConnector::GetLibUSBConnector()->SendToUSB(buf,i);
}

#ifdef TESTING
void MonitorProgrammer::slotSetScreen(){
    quint16 scrNum = leScrNum->text().toInt();
    int i = 1;
    char buf[USB_BULK_SIZE];
    NEXT(i) = SetIDLow(0, 0, 3);                // King's ID = 0 Message data length - 3
    NEXT(i) = SetIDHi(0);                       // King's ID = 0
    NEXT(i) = physicalNumber;                   // line 0 Phys num
    NEXT(i) = kTstDrawScreenDefault;            // line 1 Page num
    NEXT(i) = scrNum;                           // line 2 screen number
    buf[0] = i - 1;
    LibUSBConnector::GetLibUSBConnector()->SendToUSB(buf,i);
}

void MonitorProgrammer::slotSetState(){
    quint16 stateNum = leStateNum->text().toInt();
    quint16 objNum = leObjNum->text().toInt();
    int i = 1;
    char buf[USB_BULK_SIZE];
    NEXT(i) = SetIDLow(0, 0, 4);                // King's ID = 0 Message data length - 4
    NEXT(i) = SetIDHi(0);                       // King's ID = 0
    NEXT(i) = physicalNumber;                   // line 0 Phys num
    NEXT(i) = kTstDrawObject;                   // line 1 Page num
    NEXT(i) = objNum;                           // line 2 Object number
    NEXT(i) = stateNum;                         // line 3 State number
    buf[0] = i - 1;
    LibUSBConnector::GetLibUSBConnector()->SendToUSB(buf,i);

}
#endif
