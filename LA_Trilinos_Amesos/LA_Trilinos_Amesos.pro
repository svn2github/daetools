include(../dae.pri)
QT -= core \
    gui
TARGET = TrilinosAmesos
TEMPLATE = lib

####################################################################################
#                       Suppress some warnings
####################################################################################
#unix::QMAKE_CXXFLAGS += -ansi -pedantic
unix::QMAKE_CXXFLAGS += -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
unix::QMAKE_CFLAGS   += -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable

# ####################################################################################
# TRILINOS Amesos solvers
# ####################################################################################
linux-g++::TRILINOS_DIR = ../trilinos/build
linux-g++-64::TRILINOS_DIR = ../trilinos/build

TRILINOS_INCLUDE = $${TRILINOS_DIR}/include

linux-g++::TRILINOS_LIBS = -L$${TRILINOS_DIR}/lib \
    -lamesos \
    -lepetra
linux-g++-64::TRILINOS_LIBS = -L$${TRILINOS_DIR}/lib \
    -lblas -llapack -lsuperlu -lumfpack -lamd \
    -lamesos -lepetra -lepetraext -lgaleri \
    -lsimpi -lzoltan -lteuchos -ltriutils
  

INCLUDEPATH += $${BOOSTDIR} \
    $${PYTHON_INCLUDE_DIR} \
    $${PYTHON_SITE_PACKAGES_DIR} \
    $${SUNDIALS_INCLUDE} \
    $${TRILINOS_INCLUDE}
QMAKE_LIBDIR += $${PYTHON_LIB_DIR}
LIBS += $${BOOST_PYTHON_LIB} \
    $${BLAS_LIBS} \
    $${LAPACK_LIBS} \
    $${TRILINOS_LIBS}
SOURCES += stdafx.cpp \
    dllmain.cpp \
    dae_python.cpp \
    trilinos_amesos_la_solver.cpp
HEADERS += stdafx.h \
    trilinos_amesos_la_solver.h
win32-msvc2008::QMAKE_POST_LINK = move \
    /y \
    $${DAE_DEST_DIR}/TrilinosAmesos1.dll \
    $${DAE_DEST_DIR}/pyTrilinosAmesos.pyd
win32-g++::QMAKE_POST_LINK = move \
    /y \
    $${DAE_DEST_DIR}/TrilinosAmesos1.dll \
    $${DAE_DEST_DIR}/pyTrilinosAmesos.pyd
unix::QMAKE_POST_LINK = cp \
    -f \
    $${DAE_DEST_DIR}/lib$${TARGET}.so.$${VERSION} \
    $${DAE_DEST_DIR}/pyTrilinosAmesos.so
