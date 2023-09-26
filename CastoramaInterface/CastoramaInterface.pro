QT       += core gui sql\
            printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
LIBS += -L/home/pulinka/sdk_build/src/aws-cpp-sdk-core -laws-cpp-sdk-core
LIBS += -L/home/pulinka/sdk_build/generated/src/aws-cpp-sdk-dynamodb -laws-cpp-sdk-dynamodb
LIBS += -lsqlite3
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CDataBase.cpp \
    cinfowindow.cpp \
    cinsertwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    CDataBase.h \
    cinfowindow.h \
    cinsertwindow.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    cinfowindow.ui \
    cinsertwindow.ui \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
