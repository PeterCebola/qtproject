#-------------------------------------------------
#
# Project created by QtCreator 2014-11-27T09:08:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCvTest1
TEMPLATE = app


SOURCES += main.cpp\
    dialog.cpp \
    mouse_qlabel.cpp \
    qpicturelabel.cpp

HEADERS  += \
    dialog.h \
    cvimagewidget.h \
    mouse_qlabel.h \
    qpicturelabel.h

FORMS    += \
    dialog.ui

LIBS += `pkg-config opencv --libs` -O1 -larmadillo

#INCLUDEPATH += "/home/pi/armadillo-5.200.1/b"
#LIBS += -O2 -DARMA_DONT_USE_WRAPPER -lblas -llapack -lgfortran
#QMAKE_LFLAGS += -larmadillo

#QMAKE_LFLAGS += -O1 -larmadillo
#-larmadillo -std=C++11
#-O2 -larmadillo -llapack -lblas
#-I usr/include/armadillo -L usr/lib/lapack -llapack  -L usr/lib/BLAS -lblas
#usr/include/armadillo_bits -DARMA_DONT_USE_WRAPPER -DARMA_USE_BLAS -DARMA_USE_LAPACK -lblas -llapack

