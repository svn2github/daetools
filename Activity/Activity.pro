include(../dae.pri)

QT -= core gui
TARGET = cdaeActivity
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += $${BOOSTDIR} \
	           $${MPI_INCLUDE}

LIBS += $${DAE_CORE_LIB} \
        $${MPI_LIBS}

HEADERS += stdafx.h \
    simulation.h \
    activity_class_factory.h \
    base_activities.h 
SOURCES += stdafx.cpp \
    simulation.cpp \
    dllmain.cpp \ 
    class_factory.cpp \
    base_activities.cpp \
    optimization.cpp
