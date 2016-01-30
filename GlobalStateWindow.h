#ifndef GLOBALSTATEWINDOW_H
#define GLOBALSTATEWINDOW_H

#include <QtGui>
#include "MonitorScreen.h"

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
    QVector<quint32> gsVector;
public:
    GlobalStateWindow(QList<MonitorScreen*> *scrList, QWidget* _parent = 0);
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
