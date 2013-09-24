#************************************************************************************
#                 DAE Tools Project: www.daetools.com
#                 Copyright (C) Dragan Nikolic, 2013
#************************************************************************************
# DAE Tools is free software; you can redistribute it and/or modify it under the 
# terms of the GNU General Public License version 3 as published by the Free Software
# Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with the
# DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
#************************************************************************************

include(../dae.pri)
QT -= core gui
TARGET = pyIntelPardiso
TEMPLATE = lib

INCLUDEPATH += $${BOOSTDIR} \
               $${PYTHON_INCLUDE_DIR} \
               $${PYTHON_SITE_PACKAGES_DIR} \
               $${SUNDIALS_INCLUDE} \
               $${INTEL_MKL_INCLUDE}

QMAKE_LIBDIR += $${PYTHON_LIB_DIR}

LIBS += $${DAE_INTEL_PARDISO_SOLVER_LIB} \
        $${BOOST_PYTHON_LIB} \
        $${INTEL_MKL_LIBS}

SOURCES += stdafx.cpp \
           dllmain.cpp \
           dae_python.cpp \
           python_wraps.cpp

HEADERS += stdafx.h \
           python_wraps.h \
           docstrings.h

#######################################################
#                Install files
#######################################################
win32{
QMAKE_POST_LINK = move /y \
    $${DAE_DEST_DIR}/pyIntelPardiso11.dll \
    $${SOLVERS_DIR}/pyIntelPardiso.pyd
}

unix{
QMAKE_POST_LINK = cp -f \
    $${DAE_DEST_DIR}/lib$${TARGET}.$${SHARED_LIB_APPEND} \
    $${SOLVERS_DIR}/$${TARGET}.$${SHARED_LIB_EXT}
}
