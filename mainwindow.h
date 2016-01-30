#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MonitorScreen.h"
#include "TPropertyWidget.h"
#include "FontConverter.h"
#include "MonitorProgrammer.h"
#include "Regs2515.h"
#include "canerror.h"
#include "libusbconnector.h"
#include "SetDeviceNumberWindow.h"
#include "CanKingdomTuner.h"
#include "TGetFileWindow.h"
#if defined(Q_WS_WIN)
#include <windows.h>
#include <Dbt.h>
#include <SetupAPI.h>
#endif

#include "toolwindow.h"
#include "canreceiver.h"
#include "TLogCanBus.h"
#include "GlobalStateWindow.h"
#include "cansender.h"
#include "saveopenxml.h"

#define TOUCHWIDTH 640
#define TOUCHHEIGHT 480

#define SMALLWIDTH 320
#define SMALLHEIGHT 240

#define canreceiverId 0
#define TLogCanBusId  1
#define canerrorId 2
#define monitorPrgId 3
#define setDeviceNumberId 4
#define canKingdomSetupCreateId 5
#define canKingdomSetupOpenId 6
#define cansenderId 7
#define toolwinEndId 8

#define READINTERVAL 200


#define TESTBUTTON 1
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    virtual void customEvent(QEvent *e);
    ~MainWindow();
protected:
#if defined(Q_WS_WIN)
        virtual bool winEvent ( MSG * msg, long * result );
#endif
        virtual void timerEvent(QTimerEvent*);
private:
    QSettings*  pSettings;
    QString projectPath;
    void writeSettings(void);
    void readSettings(void);
    bool USBconnected;
    LibUSBConnector* USBconnector;
    toolwindow* _ptoolWindow[toolwinEndId];
    QAction* _pqaction[toolwinEndId];
    void AddAction( int id, const QString name, const QString text, const QString shortCut, const QString toolTip, const QString statTip );
    toolwindow* getToolWindow(int);
    QList<int>* pmesList;
    QList<int>* pcomList;
    QList<int>* pregList;
    void SubscribeToMes(void);
    void USB_Dev_Changed(void);
    QLabel* statConnected;
    QLabel* statMessage;
    QLabel* statTEC;
    QLabel* statREC;
    QLabel* statSeparator;
    QLabel* statDrvError;
    //QPushButton* statDrvRes;
    void ShowDrvAlarm(unsigned char alarmFlags);
    void addToTreeWidget( quint16 index, GrObject* grit );
private slots:
    //---------- Create project slots ---------------
    void createNewProject();
    void slotCreateNewProjectOkClicked();
    //---------- Open Project slots -----------------
    void openProject();
    void addOpenedScreen( MonitorScreen* Screen );
    //---------- Save Project slot  -----------------
    void saveProject();
    void saveProjectAs();
    //---------- Close project slot -----------------
    void slotCloseProject();
    //----------    Screen slots    -----------------
    void addNewScreen();
    void setActiveSubWindow(QMdiSubWindow* window);
    void closeActiveScreen();
    //----------- Items slots -----------------------
    void addButton();
    void addText();
    void addBar();
    void addEnterArea();
    void addImage();
    void addShape();
    void addAnimation();
    void nextItemState();
    void prevItemState();
    //----------- help and about slots --------------
    void showHelp();
    //----------- tree widget slots -----------------
    void slotTreeItemSelected();
    void slotTreeItemDoubleClicked(QTreeWidgetItem*,int);
    void slotFillTree();
    // toolwindows
    void slotToolWindowShow();
    void slotToolWindowClose();
//    void slotDrvResetClicked();

#ifdef TESTBUTTON
    void slotTest();
#endif
#ifdef Q_WS_X11
    void USB_Dev_Notify();
#endif
private:
    QAction *actCreateProject, *actCloseProject;
    QTreeWidget* twScreenItems;
    QMenu* pScreenMenu;
    QToolButton *tbAddButton, *tbAddText, *tbAddEnterArea, *tbAddBar, *tbAddImage, *tbAddShape, *tbAnimation;
    QPushButton *pbNextItemState, *pbPrevItemState;
    TPropertyWidget* tbWidget;
    QMdiArea* pScreenArea, *pMainArea;
    QMdiSubWindow* activeSubWindow;
    QList<MonitorScreen*> ScreenList;
    QList<QGraphicsView*> pViewList;

    QDialog* w;
    QComboBox* cbChooseMonitor;
    QSize screenSize;

    GrObject* currentItem;
    void closeScreen( QMdiSubWindow* sub );

    void configStateButtons(GrObject *item );

};

#endif // MAINWINDOW_H
