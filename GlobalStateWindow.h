#ifndef GLOBALSTATEWINDOW_H
#define GLOBALSTATEWINDOW_H

#include <QtGui>
#include "MonitorScreen.h"
#include "GrObject.h"

typedef struct{
    GrObject* object;
    quint8  stateNumber;
}TOBJSTATE;

typedef struct{
    QString GlobalStateName;
    MonitorScreen* screen;
    QList<TOBJSTATE> objectState;
}TGlobalState;

class GlobalStateWindow : public QDialog
{
    Q_OBJECT
private:
    QListWidget* lwGS, *lwObjects, *lwObjStates;
    QComboBox* cbScreens;
    QPushButton* pbAdd, *pbRemove, *pbRename;
    QDialog* win;
    QLineEdit* leName;
    QList<MonitorScreen*>* screenList;
    bool allowObjSlot;
    QList<TGlobalState> gsList;
    QVector<quint32> gsVector;
public:
    GlobalStateWindow(QList<MonitorScreen*> *scrList, QWidget* _parent = 0);
    void setScreenList( QList<MonitorScreen*>* scrList ){ screenList = scrList;}
private slots:
    // buttons slots
    void slotAddGS();
    void slotRemoveGS();
    void slotRenameGS();
    void slotOkPressed();
    void slotCancelPressed();
    // comboBox slot
    void slotScreenChanged(int index );
    // listWidgets slots
    void slotObjectChanged(int row );
};

#endif // GLOBALSTATEWINDOW_H
