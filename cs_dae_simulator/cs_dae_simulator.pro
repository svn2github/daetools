#************************************************************************************
#                 DAE Tools Project: www.daetools.com
#                 Copyright (C) Dragan Nikolic
#************************************************************************************
# DAE Tools is free software; you can redistribute it and/or modify it under the
# terms of the GNU General Public License version 3 as published by the Free Software
# Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with the
# DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
#************************************************************************************
QT -= core gui
TARGET = cs_dae_simulator
TEMPLATE = app

CONFIG(debug, debug|release) {
    OBJECTS_DIR = debug
}

CONFIG(release, debug|release) {
    OBJECTS_DIR = release
}
DESTDIR = bin

# MPI Settings
QMAKE_CXX  = mpicxx
QMAKE_LINK = $$QMAKE_CXX
QMAKE_CC   = mpicc

#unix::QMAKE_CFLAGS   += $$system(mpicc --showme:compile)
#unix::QMAKE_LFLAGS   += $$system(mpicxx --showme:link)
#unix::QMAKE_CXXFLAGS += $$system(mpicxx --showme:compile)

unix::QMAKE_CXXFLAGS += -std=c++11 -fopenmp

unix::QMAKE_CFLAGS_RELEASE   -= -O2
unix::QMAKE_CFLAGS_RELEASE   -= -O2
unix::QMAKE_CFLAGS_RELEASE   += -O3
unix::QMAKE_CXXFLAGS_RELEASE += -O3

unix::QMAKE_CFLAGS           += -std=c99
unix::QMAKE_CFLAGS_WARN_ON    = -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-sign-compare
unix::QMAKE_CXXFLAGS_WARN_ON  = -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-sign-compare

# Valgrind profilig
#unix::QMAKE_CXXFLAGS += -g

# Path to daetools/trunk directory
BOOST_DIR       = ../boost
IDAS_DIR        = ../idas-mpi
OPEN_CS_DIR     = ../opencs

# Trilinos related libraries
UMFPACK_LIBPATH = ../umfpack/build/lib
TRILINOS_DIR    = ../trilinos/build
SUPERLU_PATH    = ../superlu/build
SUPERLU_LIBPATH = $${SUPERLU_PATH}/lib
BLAS_LAPACK_LIBDIR = ../lapack/lib
BLAS_LAPACK_LIBS = $${BLAS_LAPACK_LIBDIR}/liblapack.a $${BLAS_LAPACK_LIBDIR}/libblas.a -lgfortran -lm
SUPERLU_LIBS = -L$${SUPERLU_LIBPATH} -lsuperlu
UMFPACK_LIBS = $${UMFPACK_LIBPATH}/libumfpack.a \
               $${UMFPACK_LIBPATH}/libcholmod.a \
               $${UMFPACK_LIBPATH}/libamd.a \
               $${UMFPACK_LIBPATH}/libcamd.a \
               $${UMFPACK_LIBPATH}/libcolamd.a \
               $${UMFPACK_LIBPATH}/libccolamd.a \
               $${UMFPACK_LIBPATH}/libsuitesparseconfig.a
TRILINOS_LIBS = -L$${TRILINOS_DIR}/lib -L$${SUPERLU_PATH}/lib \
                           -laztecoo -lml -lifpack \
                           -lamesos -lepetraext -ltriutils -lepetra \
                           -lteuchosremainder -lteuchosnumerics -lteuchoscomm \
                           -lteuchosparameterlist -lteuchoscore \
                            $${UMFPACK_LIBS} \
                            $${SUPERLU_LIBS}

# OpenCL
unix::INTEL_OPENCL_DIR          = /opt/intel/opencl
unix::INTEL_OPENCL_INCLUDE      = $${INTEL_OPENCL_DIR}/include
unix::INTEL_OPENCL_LIBS         = -L$${INTEL_OPENCL_DIR} -lOpenCL
unix::DAE_EVALUATOR_OPENCL_LIB  = -lcdaeEvaluator_OpenCL

INCLUDEPATH  += $${BOOST_DIR} \
                $${IDAS_DIR}/build/include \
                ../trilinos/build/include \
                $${INTEL_OPENCL_INCLUDE} \
                $${OPEN_CS_DIR}

QMAKE_LIBDIR += $${BOOST_DIR}/stage/lib \
                $${IDAS_DIR}/build/lib

unix::LIBS += -lcdaeEvaluator_OpenCL \
              $${INTEL_OPENCL_LIBS} \

LIBS += -L../release -lcdaeTrilinos_LASolver $${TRILINOS_LIBS} $${UMFPACK_LIBS} $${BLAS_LAPACK_LIBS}

# IDAS + BOOST libraries
LIBS += -lsundials_idas -lsundials_nvecparallel -lboost_mpi -lboost_serialization -lboost_filesystem -lboost_system

LIBS += -lgomp -lstdc++fs

SOURCES += main.cpp \
           auxiliary.cpp \
           config.cpp \
           daesolver.cpp \
           daemodel.cpp \
           simulation.cpp \
           lasolver.cpp \
           preconditioner_jacobi.cpp \
           preconditioner_ifpack.cpp \
           preconditioner_ml.cpp \
           ../opencs/dae_model/cs_dae_model.cpp \
           ../opencs/openmp/cs_evaluator_openmp.cpp \
           ../opencs/sequential/cs_evaluator_sequential.cpp

HEADERS += cs_simulator.h \
           auxiliary.h \
           idas_la_functions.h \
           ../opencs/dae_model/cs_dae_model.h \
           ../opencs/sequential/cs_evaluator_openmp.h \
           ../opencs/sequential/cs_evaluator_sequential.h
