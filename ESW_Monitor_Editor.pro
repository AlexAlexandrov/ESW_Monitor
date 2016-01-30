#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T11:12:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ESW_Monitor_Editor
TEMPLATE = app
QT += sql
QT += xml

SOURCES += main.cpp\
        mainwindow.cpp \
    MonitorScreen.cpp \
    TPropertyWidget.cpp \
    GrButton.cpp \
    GrBar.cpp \
    GrEnterArea.cpp \
    FontConverter.cpp \
    GrText.cpp \
    ScreenItem.cpp \
    GrResizer.cpp \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/TGetFileWindow.cpp \
    GrImage.cpp \
    IdGenerator.cpp \
    GrShape.cpp \
    GrObject.cpp \
    StructureCreator.cpp \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/TLogCanBus.cpp \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/libusbconnector.cpp \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/canreceiver.cpp \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/toolwindow.cpp \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/canerror.cpp \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/canmessage.cpp \
    ../../CAN_protocol/CANroutine.c \
    MonitorProgrammer.cpp \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/SetDeviceNumberWindow.cpp \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/CanKingdomTuner.cpp \
    GlobalStateWindow.cpp \
    ImageConverter.cpp \
    saveopenxml.cpp \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/cansender.cpp \
    GrAnimation.cpp

HEADERS  += mainwindow.h \
    MonitorScreen.h \
    TPropertyWidget.h \
    GrButton.h \
    GrBar.h \
    GrEnterArea.h \
    FontConverter.h \
    GrText.h \
    ScreenItem.h \
    GrResizer.h \
    GlobalDefinition.h \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/TGetFileWindow.h \
    GrImage.h \
    IdGenerator.h \
    GrShape.h \
    GrObject.h \
    StructureCreator.h \
    ../../MonitorCanSource/MonitorDriver.h \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/TLogCanBus.h \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/libusbconnector.h \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/canreceiver.h \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/toolwindow.h \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/canerror.h \
    ../../CAN_protocol/Regs2515.h \
    ../../CAN_protocol/CAN_Message.h \
    ../../CAN_protocol/CAN_driver_message.h \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/canmessage.h \
    ../../LibUSB_source/lusb0_usb.h \
    MonitorProgrammer.h \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/SetDeviceNumberWindow.h \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/CanKingdomTuner.h \
    GlobalStateWindow.h \
    ImageConverter.h \
    saveopenxml.h \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/cansender.h \
    GrAnimation.h

#unix:!macx:!symbian|win32: LIBS += -L$$PWD/../../LibUSB_source/ -llibusb \
win32 {
        LIBS += -L$$PWD/../../LibUSB_source/ -llibusb \
        -lqaxserver \
        -lqaxcontainer
}
 unix {
        LIBS	+= -lusb
 }

INCLUDEPATH += $$PWD/../../LibUSB_source \
        $$PWD/../../CAN_protocol \
        $$PWD/../../MonitorCanSource/ \
        $$PWD/../../QT_USB_CAN_LibUSB/QT_USB_CAN
DEPENDPATH += $$PWD/../../LibUSB_source \
        $$PWD/../../MonitorCanSource/ \
        $$PWD/../../CAN protocol \
        $$PWD/../../QT_USB_CAN_LibUSB/QT_USB_CAN

win32: PRE_TARGETDEPS += $$PWD/../../LibUSB_source/libusb.lib
#else:unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../LibUSB_source/liblibusb.a

FORMS += \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/CANreceiverForm.ui \
    ../../QT_USB_CAN_LibUSB/QT_USB_CAN/CANsenderForm.ui
