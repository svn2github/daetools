#************************************************************************************
#                 DAE Tools Project: www.daetools.com
#                 Copyright (C) Dragan Nikolic, 2010
#************************************************************************************
# DAE Tools is free software; you can redistribute it and/or modify it under the 
# terms of the GNU General Public License version 3 as published by the Free Software
# Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with the
# DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
#************************************************************************************

DAE_TOOLS_MAJOR = 1
DAE_TOOLS_MINOR = 1
DAE_TOOLS_BUILD = 2

# Set CONFIG += use_system_python to use the default system's python
# 1. On GNU/LINUX:
#    Set CONFIG += use_custom_python and for instance PYTHON_MAJOR=2 and PYTHON_MINOR=7 
#    to use Python located in /usr/lib/python2.7
# 2. On Windows:
#    Set CONFIG += use_custom_python and for instance PYTHON_MAJOR=2 and PYTHON_MINOR=7 
#    to use Python located in c:\Python27
CONFIG += use_system_python
PYTHON_MAJOR = 2
PYTHON_MINOR = 6

# 1. On GNU/LINUX:
#    a) Set CONFIG += use_system_boost to use the system's default version
#    b) Set CONFIG += use_custom_boost and for instance BOOST_MAJOR = 1, BOOST_MINOR = 42 
#       and BOOST_BUILD = 0 to use the boost build in ../boost_1_42_0
# 2. On Windows: 
#    BOOST_MAJOR, BOOST_MINOR and BOOST_BUILD must always be set!!
#    and Boost build must be located in ../boost_1_42_0 (for instance)
CONFIG += use_system_boost
BOOST_MAJOR = 1
BOOST_MINOR = 42
BOOST_BUILD = 0

# DAE Tools version (major, minor, build)
VERSION = $${DAE_TOOLS_MAJOR}.$${DAE_TOOLS_MINOR}.$${DAE_TOOLS_BUILD}

QMAKE_CXXFLAGS += -DDAE_MAJOR=$${DAE_TOOLS_MAJOR}
QMAKE_CXXFLAGS += -DDAE_MINOR=$${DAE_TOOLS_MINOR}
QMAKE_CXXFLAGS += -DDAE_BUILD=$${DAE_TOOLS_BUILD}

CONFIG(debug, debug|release):message(debug){
	DAE_DEST_DIR = ../debug
    OBJECTS_DIR = debug
}

CONFIG(release, debug|release):message(release){
	DAE_DEST_DIR = ../release
    OBJECTS_DIR = release
}

DESTDIR = $${DAE_DEST_DIR}


# Cross compiling:
# configure --prefix=/usr/i586-mingw32msvc --host=i586-mingw32msvc --build=x86_64-linux

####################################################################################
# Remove all symbol table and relocation information from the executable.
# Necessary to pass lintian test in debian  
####################################################################################
#CONFIG(release, debug|release){
#    unix:QMAKE_LFLAGS += -s
#}

####################################################################################
#                       Suppress some warnings
####################################################################################
#unix::QMAKE_CXXFLAGS += -ansi -pedantic
unix::QMAKE_CXXFLAGS += -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
unix::QMAKE_CFLAGS   += -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable

####################################################################################
# Creating .vcproj under windows:
# cd trunk
# qmake -tp vc -r dae.pro
####################################################################################

####################################################################################
#                       single/double precision control
####################################################################################
# SUNDIALS_DOUBLE/SINGLE_PRECISION must be also defined in sundials_config.h
####################################################################################
#QMAKE_CXXFLAGS += -DDAE_SINGLE_PRECISION

####################################################################################
#				                     ARM port
####################################################################################
# Undefined reference to `__sync_fetch_and_add_4' issue):
####################################################################################
# QMAKE_CXXFLAGS += -DBOOST_SP_USE_PTHREADS


#####################################################################################
#                                   PYTHON
#####################################################################################
# Numpy and Scipy must be installed
# Debian Squeeze: sometimes there are problems with _numpyconfig.h
# Add: /usr/include/python$${PYTHON_MAJOR}.$${PYTHON_MINOR}/numpy 
# to:  PYTHON_INCLUDE_DIR 
#####################################################################################
use_system_python {
PYTHON_MAJOR = $$system(python -c \"import sys; print sys.version_info[0]\")
PYTHON_MINOR = $$system(python -c \"import sys; print sys.version_info[1]\")
message(use_system_python: $${PYTHON_MAJOR}.$${PYTHON_MINOR})
}

use_custom_python { 
message(use_custom_python: $${PYTHON_MAJOR}.$${PYTHON_MINOR})
}

win32-msvc2008::PYTHONDIR                = C:\Python$${PYTHON_MAJOR}$${PYTHON_MINOR}
win32-msvc2008::PYTHON_INCLUDE_DIR       = $${PYTHONDIR}\include
win32-msvc2008::PYTHON_SITE_PACKAGES_DIR = $${PYTHONDIR}\Lib\site-packages
win32-msvc2008::PYTHON_LIB_DIR           = $${PYTHONDIR}\libs

linux-g++::PYTHONDIR           = /usr/lib/python$${PYTHON_MAJOR}.$${PYTHON_MINOR}
linux-g++-64::PYTHONDIR        = /usr/lib64/python$${PYTHON_MAJOR}.$${PYTHON_MINOR}

unix::PYTHON_INCLUDE_DIR       = /usr/include/python$${PYTHON_MAJOR}.$${PYTHON_MINOR} \
							     /usr/include/python$${PYTHON_MAJOR}.$${PYTHON_MINOR}/numpy \
                                 /usr/share/pyshared
unix::PYTHON_SITE_PACKAGES_DIR = $${PYTHONDIR}/dist-packages \
                                 $${PYTHONDIR}/site-packages
unix::PYTHON_LIB_DIR           =


#####################################################################################
#                                    BOOST
#####################################################################################
# Boost version installed must be 1.35+ (asio, system, python, thread, regex)
# 1. Install bjam and Boost.Build
#    a) On windows:
#       - Go to the directory tools\build\v2\.
#       - Run bootstrap.bat
#       - Run bjam install --prefix=PREFIX where PREFIX is the directory where you 
#         want Boost.Build to be installed
#       - Add PREFIX\bin to your PATH environment variable.
# 2) Build boost libraries (toolset=msvc or gcc; both static|shared)
#      bjam --build-dir=./build  
#      --with-date_time --with-python --with-system --with-regex --with-serialization --with-thread 
#      variant=release link=static,shared threading=multi runtime-link=shared
#    Python version could be set:
#    a) --with-python-version=2.7
#    b) in user-config.jam located in $BOOST_BUILD or $HOME directory
#       Add line: using python : 2.7 ;
#####################################################################################
win32-msvc2008::BOOSTDIR         = ../boost_$${BOOST_MAJOR}_$${BOOST_MINOR}_$${BOOST_BUILD}
win32-msvc2008::BOOSTLIBPATH     = $${BOOSTDIR}/stage/lib
win32-msvc2008::BOOST_PYTHON_LIB =
win32-msvc2008::BOOST_LIBS       =

use_system_boost {
unix::BOOSTDIR         = /usr/include/boost
unix::BOOSTLIBPATH     = 
unix::BOOST_PYTHON_LIB = -lboost_python
unix::BOOST_LIBS       = -lboost_system \
                         -lboost_thread
}

use_custom_boost { 
unix::BOOSTDIR         = ../boost_$${BOOST_MAJOR}_$${BOOST_MINOR}_$${BOOST_BUILD}
unix::BOOSTLIBPATH     = $${BOOSTDIR}/stage/lib
unix::BOOST_PYTHON_LIB = -lboost_python
unix::BOOST_LIBS       = -lboost_system \
                         -lboost_thread
}

#####################################################################################
#                                 SUNDIALS IDAS
#####################################################################################
# ./configure --prefix=${HOME}/Data/daetools/trunk/idas/build --disable-mpi
#             --enable-examples --enable-static=yes --enable-shared=no --with-pic
#             CFLAGS=-O3
#####################################################################################
SUNDIALS = ../idas/build
SUNDIALS_INCLUDE = $${SUNDIALS}/include
SUNDIALS_LIBDIR = $${SUNDIALS}/lib

win32-msvc2008::SUNDIALS_LIBS = sundials_idas.lib \
                                sundials_nvecserial.lib
unix::SUNDIALS_LIBS = -lsundials_idas \
                      -lsundials_nvecserial


#####################################################################################
#                                  MUMPS
#####################################################################################
MUMPS_DIR  = ../mumps
win32-msvc2008::G95_LIBDIR = c:\MinGW\lib\gcc-lib\i686-pc-mingw32\4.1.2

MUMPS_LIBDIR   = $${MUMPS_DIR}/lib \
                 $${MUMPS_DIR}/libseq \
                 $${MUMPS_DIR}/blas \
                 $${G95_LIBDIR}

win32-msvc2008::MUMPS_LIBS = blas.lib \
                             libmpiseq.lib \
                             libdmumps.lib \
                             libmumps_common.lib \
                             libpord.lib \
                             libf95.a \
                             libgcc.a
unix::MUMPS_LIBS =


#####################################################################################
#                                  IPOPT
#####################################################################################
IPOPT_DIR = ../bonmin/build

IPOPT_INCLUDE = $${IPOPT_DIR}/include/coin
IPOPT_LIBDIR  = $${IPOPT_DIR}/lib

win32-msvc2008::IPOPT_LIBS = libCoinBlas.lib libCoinLapack.lib libf2c.lib libIpopt.lib 
unix::IPOPT_LIBS           = -ldl -lblas -llapack -lipopt


#####################################################################################
#                                  BONMIN
#####################################################################################
BONMIN_DIR = ../bonmin/build

BONMIN_INCLUDE = $${BONMIN_DIR}/include/coin
BONMIN_LIBDIR  = $${BONMIN_DIR}/lib

win32-msvc2008::BONMIN_LIBS = libCoinBlas.lib libCoinLapack.lib libf2c.lib \
                              libBonmin.lib \
                              libIpopt.lib \
                              libCbc.lib \
                              libCgl.lib \
                              libClp.lib \
                              libCoinUtils.lib \
                              libOsiCbc.lib \
                              libOsiClp.lib \
                              libOsi.lib
unix::BONMIN_LIBS  =    -ldl -lblas -llapack \
						-lbonmin \
						-lCbc \
						-lCbcSolver \
						-lCgl \
						-lClp \
						-lCoinUtils \
						-lipopt \
						-lOsiCbc \
						-lOsiClp \
						-lOsi


#####################################################################################
#                                 NLOPT
#####################################################################################
NLOPT_DIR = ../nlopt/build
NLOPT_INCLUDE = $${NLOPT_DIR}/include
NLOPT_LIBDIR  = $${NLOPT_DIR}/lib

win32-msvc2008::NLOPT_LIBS = nlopt.lib
unix::NLOPT_LIBS           = -lnlopt -lm


######################################################################################
#                                   SuperLU
######################################################################################
win32-msvc2008::SUPERLU_PATH = ..\superlu
linux-g++::SUPERLU_PATH      = ../superlu
linux-g++-64::SUPERLU_PATH   = ../superlu

SUPERLU_LIBPATH = $${SUPERLU_PATH}/lib

SUPERLU_INCLUDE = $${SUPERLU_PATH}/SRC

win32-msvc2008::SUPERLU_LIBS = -L$${SUPERLU_LIBPATH} superlu.lib \
													 ../clapack/LIB/Win32/BLAS_nowrap.lib \
													 ../clapack/LIB/Win32/libf2c.lib
linux-g++::SUPERLU_LIBS      = -L$${SUPERLU_LIBPATH} -lcdaesuperlu -lrt
linux-g++-64::SUPERLU_LIBS   = -L$${SUPERLU_LIBPATH} -lcdaesuperlu -lrt


######################################################################################
#                                SuperLU_MT
######################################################################################
win32-msvc2008::SUPERLU_MT_PATH = ..\superlu_mt
linux-g++::SUPERLU_MT_PATH      = ../superlu_mt
linux-g++-64::SUPERLU_MT_PATH   = ../superlu_mt

SUPERLU_MT_LIBPATH = $${SUPERLU_MT_PATH}/lib

SUPERLU_MT_INCLUDE = $${SUPERLU_MT_PATH}/SRC

win32-msvc2008::SUPERLU_MT_LIBS = -L$${SUPERLU_MT_LIBPATH} superlu_mt.lib
linux-g++::SUPERLU_MT_LIBS      = -L$${SUPERLU_MT_LIBPATH} -lcdaesuperlu_mt -lrt
linux-g++-64::SUPERLU_MT_LIBS   = -L$${SUPERLU_MT_LIBPATH} -lcdaesuperlu_mt -lrt


######################################################################################
#                                SuperLU_CUDA
######################################################################################
win32-msvc2008::CUDA_PATH = 
linux-g++::CUDA_PATH      = /usr/local/cuda
linux-g++-64::CUDA_PATH   = /usr/local/cuda

win32-msvc2008::SUPERLU_CUDA_PATH = ..\superlu_mt-GPU
linux-g++::SUPERLU_CUDA_PATH      = ../superlu_mt-GPU
linux-g++-64::SUPERLU_CUDA_PATH   = ../superlu_mt-GPU

SUPERLU_CUDA_LIBPATH = $${SUPERLU_CUDA_PATH}/lib

SUPERLU_CUDA_INCLUDE = $${SUPERLU_CUDA_PATH} \
	                   $${CUDA_PATH}/include

win32-msvc2008::CUDA_LIBS = -L$${CUDA_PATH}/lib cuda.lib cudart.lib
linux-g++::CUDA_LIBS      = -L$${CUDA_PATH}/lib -lcuda -lcudart
linux-g++-64::CUDA_LIBS   = -L$${CUDA_PATH}/lib64 -lcuda -lcudart


#####################################################################################
#                                  DAE Tools
#####################################################################################
win32-msvc2008::DAE_CORE_LIB                = cdaeCore.lib
win32-msvc2008::DAE_DATAREPORTING_LIB       = cdaeDataReporting.lib
win32-msvc2008::DAE_ACTIVITY_LIB            = cdaeActivity.lib
win32-msvc2008::DAE_IDAS_SOLVER_LIB         = cdaeIDAS_DAESolver.lib
win32-msvc2008::DAE_SUPERLU_SOLVER_LIB      = cdaeSuperLU_LASolver.lib
win32-msvc2008::DAE_SUPERLU_MT_SOLVER_LIB   = cdaeSuperLU_MT_LASolver.lib
win32-msvc2008::DAE_SUPERLU_CUDA_SOLVER_LIB = cdaeSuperLU_CUDA_LASolver.lib
win32-msvc2008::DAE_BONMIN_SOLVER_LIB       = cdaeBONMIN_MINLPSolver.lib
win32-msvc2008::DAE_IPOPT_SOLVER_LIB        = cdaeIPOPT_NLPSolver.lib
win32-msvc2008::DAE_NLOPT_SOLVER_LIB        = cdaeNLOPT_NLPSolver.lib

unix::DAE_CORE_LIB                = -lcdaeCore
unix::DAE_DATAREPORTING_LIB       = -lcdaeDataReporting
unix::DAE_ACTIVITY_LIB            = -lcdaeActivity
unix::DAE_IDAS_SOLVER_LIB         = -lcdaeIDAS_DAESolver
unix::DAE_SUPERLU_SOLVER_LIB      = -lcdaeSuperLU_LASolver
unix::DAE_SUPERLU_MT_SOLVER_LIB   = -lcdaeSuperLU_MT_LASolver
unix::DAE_SUPERLU_CUDA_SOLVER_LIB = -lcdaeSuperLU_CUDA_LASolver
unix::DAE_BONMIN_SOLVER_LIB       = -lcdaeBONMIN_MINLPSolver
unix::DAE_IPOPT_SOLVER_LIB        = -lcdaeIPOPT_NLPSolver
unix::DAE_NLOPT_SOLVER_LIB        = -lcdaeNLOPT_NLPSolver

QMAKE_LIBDIR += $${DAE_DEST_DIR} $${BOOSTLIBPATH}

HEADERS += \
    ../config.h \
    ../dae_develop.h \
    ../dae.h
