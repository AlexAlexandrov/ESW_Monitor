#ifndef MONITORSCREEN_H
#define MONITORSCREEN_H

#include <QtGui>
#include "GrButton.h"
#include "GrBar.h"
#include "GrEnterArea.h"
#include "GrText.h"
#include "GrResizer.h"
#include "GrImage.h"
#include "GrShape.h"
#include "GrAnimation.h"
#include <QTimer>



class MonitorScreen : public QGraphicsScene
{
    Q_OBJECT
private:
    QTimer* timer;
    QString _name;
    QObject* Program;
    bool nextClickAddItem;
    QGraphicsItem* creationItem;
    GrResizer* resizer;
    QRectF screenRect;
    QGraphicsRectItem* screenArea;
    QVector<GrObject*> *itemsList;
    //variables for config state
    QDialog* w;
    QDialog* pdRenameState;
    QLineEdit* leStateName;
    QList<QListWidget*> lwList;
    QComboBox* cbState;
    GrObject* confItem;
    QList<QLabel*> labelList;
    QCheckBox* chbInvis;
    typedef enum{
        dirUp = 0,
        dirDown,
        dirLeft,
        dirRight
    }TMoveDirection;
    void moveItemTo(TMoveDirection dir , int speed);

public:
    void configItemsState(GrObject *item );

    MonitorScreen( QObject* Parent  = 0, QRectF sceneRectangle = QRectF(0,0,320,240));
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *ke);
    virtual QList<QGraphicsItem*> items() const;
    QVector<GrObject *> objects() const;
    void    addObject( GrObject *obj );
    GrButton*       slotAddButtonItem();
    GrBar*          slotAddBarItem();
    GrEnterArea*    slotAddEnterAreaItem();
    GrText*         slotAddTextItem();
    GrImage*        slotAddImageItem();
    GrShape*        slotAddShapeItem();
    GrAnimation*    slotAddAnimationItem();
    QColor          backgroundColor();
    void            setBackgroundColor( QColor col );
    QString         name();
    QColor colorUnderItem( GrObject* item );
private slots:
    void lwCurrrentIndexChanged( int curItem );
    void cbCurrentIndexChanged( int curIndex );
    void setItemInvisible( bool );
    void addStateClicked();
    void deleteStateClicked();
    void renameStateClicked();
    void renameOkClicked();
    void deleteDialogWindow();
    void onTimerShot();
};
//================ Event triggered when user click on Function's in ==================

class SelectedItemChanged  : public QEvent{
public:
    SelectedItemChanged(GrObject* from) : QEvent((Type)EventSelectionChanged){
        Sender = from;
    }
    GrObject* Sender;
};


class SelectedDeleted : public QEvent{
public:
    SelectedDeleted(int DeletedItemIndex) : QEvent((Type)EventDeleteSelection){
        deletedItemIndex = DeletedItemIndex;
    };
    int deletedItemIndex;
};



#endif // MONITORSCREEN_H
