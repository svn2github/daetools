include(../dae.pri)
QT -= core gui
TARGET = pyDealII
TEMPLATE = lib
CONFIG += shared

DEALII_DIR               = ../deal.II/build
DEALII_INCLUDE           = $${DEALII_DIR}/include
DEALII_LIB_DIR           = $${DEALII_DIR}/lib
unix::DEALII_LIBS        = -ldeal_II-daetools -lz -lblas -lgfortran -lm
win32-g++-*::DEALII_LIBS = -ldeal_II-daetools -lblas -lgfortran -lm

INCLUDEPATH += $${BOOSTDIR} \
               $${PYTHON_INCLUDE_DIR} \
               $${PYTHON_SITE_PACKAGES_DIR} \
               #$${NUMPY_INCLUDE_DIR} \
               $${DEALII_INCLUDE}

QMAKE_LIBDIR += $${PYTHON_LIB_DIR} \
                $${BLAS_LAPACK_LIBDIR} \
                $${DEALII_LIB_DIR}

QMAKE_CXXFLAGS += -fpic -Wall -Wpointer-arith -Wwrite-strings -Wsynth -Wsign-compare -Wswitch -Wno-unused-local-typedefs \
                  -O2 -funroll-loops -funroll-all-loops -fstrict-aliasing -felide-constructors -Wno-unused \
                  -DBOOST_NO_HASH -DBOOST_NO_SLIST
                  
unix::QMAKE_CXXFLAGS += -std=c++11 

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CFLAGS_DEBUG   += -DDEBUG

QMAKE_LFLAGS   += -pedantic -fpic -Wall -Wpointer-arith -Wwrite-strings -Wsynth -Wsign-compare -Wswitch -Wno-unused-local-typedefs \
                  -O2 -funroll-loops -funroll-all-loops -fstrict-aliasing -felide-constructors -Wno-unused

unix::QMAKE_LFLAGS += -std=c++11

LIBS += $${DEALII_LIBS} \
        $${DAE_CORE_LIB} \
        $${DAE_UNITS_LIB} \
        $${BOOST_PYTHON_LIB} \
        $${BOOST_LIBS} \
        $${RT}

SOURCES += stdafx.cpp \
           dllmain.cpp \
           dae_python.cpp

HEADERS += stdafx.h \
           docstrings.h \
           python_wraps.h

#######################################################
#                Install files
#######################################################
QMAKE_POST_LINK = $${COPY_FILE} \
                  $${DAE_DEST_DIR}/$${SHARED_LIB_PREFIX}$${TARGET}$${SHARED_LIB_POSTFIX}.$${SHARED_LIB_APPEND} \
                  $${SOLVERS_DIR}/$${TARGET}.$${PYTHON_EXTENSION_MODULE_EXT}
                  
# win32{
# QMAKE_POST_LINK = move /y \
# 	$${DAE_DEST_DIR}/pyDealII1.dll \
#     $${SOLVERS_DIR}/pyDealII.pyd
# }
# 
# unix{
# QMAKE_POST_LINK = cp -f \
#         $${DAE_DEST_DIR}/lib$${TARGET}.$${SHARED_LIB_APPEND} \
#         $${SOLVERS_DIR}/$${TARGET}.so
# }
