QT       += core gui

QT += multimedia

QT += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$PWD/Page \
               $$PWD/Page2 \
               $$PWD/Page3 \
               $$PWD/Page4\
               $$PWD/Page5\
               $$PWD/Page6\


SOURCES += \
    Page/page1.cpp \
    Page2/page2.cpp \
    Page3/page3.cpp \
    Page4/page4.cpp \
    Page4/page4_1.cpp \
    Page5/page5.cpp \
    Page5/page5_1.cpp \
    Page6/page6.cpp \
    Page6/page6_1.cpp \
    songwidget.cpp \
    databasemanager.cpp \
    main.cpp \
    toolclass.cpp \
    widget.cpp

HEADERS += \
    Page/page1.h \
    Page2/page2.h \
    Page3/page3.h \
    Page4/page4.h \
    Page4/page4_1.h \
    Page5/page5.h \
    Page5/page5_1.h \
    Page6/page6.h \
    Page6/page6_1.h \
    songwidget.h \
    databasemanager.h \
    toolclass.h \
    widget.h

FORMS += \
    Page/page1.ui \
    Page2/page2.ui \
    Page3/page3.ui \
    Page4/page4.ui \
    Page4/page4_1.ui \
    Page5/page5.ui \
    Page5/page5_1.ui \
    Page6/page6.ui \
    Page6/page6_1.ui \
    songwidget.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    matter.qrc
