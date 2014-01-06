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
DAE_TOOLS_MINOR = 3
DAE_TOOLS_BUILD = 0

# DAE Tools version (major, minor, build)
VERSION = $${DAE_TOOLS_MAJOR}.$${DAE_TOOLS_MINOR}.$${DAE_TOOLS_BUILD}

QMAKE_CXXFLAGS += -DDAE_MAJOR=$${DAE_TOOLS_MAJOR}
QMAKE_CXXFLAGS += -DDAE_MINOR=$${DAE_TOOLS_MINOR}
QMAKE_CXXFLAGS += -DDAE_BUILD=$${DAE_TOOLS_BUILD}

# daetools always use the current system python version and custom compiled boost libs
# located in ../boost with the libraries in ../boost/stage/lib

# Build universal binaries for MAC OS-X
# There is a problem with ppc64 under OSX 10.6 so it is excluded
# Otherwise ppc64 should be added as well
macx-g++::CONFIG += x86 x86_64

#win32-msvc2008::SHARED_LIB_EXT  = dll
#win32-g++-*::SHARED_LIB_EXT     = dll
#linux-g++::SHARED_LIB_EXT       = so
#macx-g++::SHARED_LIB_EXT        = dylib

win32-msvc2008::SHARED_LIB_PREFIX   =
win32-g++-*::SHARED_LIB_PREFIX      = 
linux-g++::SHARED_LIB_PREFIX        = lib
macx-g++::SHARED_LIB_PREFIX         = lib

win32-msvc2008::SHARED_LIB_POSTFIX  = $${DAE_TOOLS_MAJOR}
win32-g++-*::SHARED_LIB_POSTFIX     = $${DAE_TOOLS_MAJOR}
linux-g++::SHARED_LIB_POSTFIX       = 
macx-g++::SHARED_LIB_POSTFIX        = 

win32-msvc2008::SHARED_LIB_APPEND   = dll
win32-g++-*::SHARED_LIB_APPEND      = dll
linux-g++::SHARED_LIB_APPEND        = so.$${VERSION}
macx-g++::SHARED_LIB_APPEND         = $${VERSION}.dylib

win32-msvc2008::PYTHON_EXTENSION_MODULE_EXT = pyd
win32-g++-*::PYTHON_EXTENSION_MODULE_EXT    = pyd
linux-g++::PYTHON_EXTENSION_MODULE_EXT      = so
macx-g++::PYTHON_EXTENSION_MODULE_EXT       = so

win32-msvc2008::COPY_FILE = copy /y
win32-g++-*::COPY_FILE    = cp -fa
unix::COPY_FILE           = cp -fa

# Set CONFIG += enable_mpi to use MPI libraries

CONFIG(debug, debug|release) {
    DAE_DEST_DIR = ../debug
    OBJECTS_DIR = debug
}

CONFIG(release, debug|release) {
	DAE_DEST_DIR = ../release
    OBJECTS_DIR = release
}

DESTDIR = $${DAE_DEST_DIR}

#####################################################################################
#                           System + Machine + Python info
#####################################################################################
crossCompile{
    # Not used
    PYTHON = "???" 
    
    PYTHON_MAJOR = 2
    PYTHON_MINOR = 7
    PYTHON_ABI   = 

    # Numpy version
    #NUMPY_VERSION = 

    # System := {'Linux', 'Windows', 'Darwin'}
    DAE_SYSTEM   = Windows

    # Machine := {'i386', ..., 'i686', 'AMD64'}
    DAE_MACHINE = win32
    
    RT =
    
    PYTHON_INCLUDE_DIR  = ../Python27/include
    PYTHON_LIB_DIR      = ../Python27/libs
    PYTHON_LIB          = -lpython$${PYTHON_MAJOR}$${PYTHON_MINOR}$${PYTHON_ABI} $${RT}
}

!crossCompile{
    # If compiling from the shell compile_linux.sh script specify the python binary
    shellCompile {
        PYTHON = $$customPython
    }
    
    # If compiling from the GUI use system's default python version
    !shellCompile {
        PYTHON = python
    }
    
    PYTHON_MAJOR = $$system($${PYTHON} -c \"import sys; print(sys.version_info[0])\")
    PYTHON_MINOR = $$system($${PYTHON} -c \"import sys; print(sys.version_info[1])\")
    PYTHON_ABI   = $$system($${PYTHON} -c \"import sysconfig; flags = sysconfig.get_config_vars(); abi = flags[\'ABIFLAGS\'] if (\'ABIFLAGS\' in flags) else \'\'; print(abi)\")

    # Numpy version
    #NUMPY_VERSION = $$system($${PYTHON} -c \"import numpy; print(\'\'.join(numpy.__version__.split(\'.\')[0:2]))\")

    # System := {'Linux', 'Windows', 'Darwin'}
    DAE_SYSTEM   = $$system($${PYTHON} -c \"import platform; print(platform.system())\")

    # Machine := {'i386', ..., 'i686', 'AMD64'}
    MACHINE_COMMAND = "import platform; p={'Linux':platform.machine(), 'Darwin':'universal', 'Windows':'win32'}; machine = p[platform.system()]; print(machine)"
    DAE_MACHINE = $$system($${PYTHON} -c \"$${MACHINE_COMMAND}\")
    
    win32-msvc2008::RT =
    linux-g++::RT      = -lrt
    macx-g++::RT       =
    
    win32::PYTHON_INCLUDE_DIR  = $$system($${PYTHON} -c \"import distutils.sysconfig; print(distutils.sysconfig.get_python_inc())\")
    unix::PYTHON_INCLUDE_DIR   = $$system($${PYTHON} -c \"import distutils.sysconfig; print(distutils.sysconfig.get_python_inc())\")

    win32::PYTHON_LIB_DIR      = $$system($${PYTHON} -c \"import sys; print(sys.prefix)\")/libs
    unix::PYTHON_LIB_DIR       = $$system($${PYTHON} -c \"import sys; print(sys.prefix)\")/lib
    
    win32::PYTHON_LIB          = python$${PYTHON_MAJOR}$${PYTHON_MINOR}.lib
    unix::PYTHON_LIB           = -lpython$${PYTHON_MAJOR}.$${PYTHON_MINOR}$${PYTHON_ABI} $${RT}
    
    !shellCompile {
        message(Using python [$${PYTHON}] v$${PYTHON_MAJOR}.$${PYTHON_MINOR}$${PYTHON_ABI})
    }

    #PYTHON_SITE_PACKAGES_DIR  = $$system($${PYTHON} -c \"import distutils.sysconfig; print(distutils.sysconfig.get_python_lib())\")
    #win32-msvc2008::NUMPY_INCLUDE_DIR     = $${PYTHON_SITE_PACKAGES_DIR}/numpy/core/include/numpy \
    #                               $${PYTHON_INCLUDE_DIR}/numpy/core/include/numpy
    #linux-g++::NUMPY_INCLUDE_DIR = $${PYTHON_SITE_PACKAGES_DIR}/numpy/core/include/numpy \
    #                               $${PYTHON_INCLUDE_DIR}/numpy/core/include/numpy \
    #                               /usr/include/python$${PYTHON_MAJOR}.$${PYTHON_MINOR}/numpy \
    #                               /usr/share/pyshared/numpy/core/include/numpy
    #macx-g++::NUMPY_INCLUDE_DIR  = $${PYTHON_SITE_PACKAGES_DIR}/numpy/core/include/numpy \
    #                               $${PYTHON_INCLUDE_DIR}/numpy/core/include/numpy

}

####################################################################################
# Remove all symbol table and relocation information from the executable.
# Necessary to pass lintian test in debian  
####################################################################################
#CONFIG(release, debug|release){
#    unix:QMAKE_LFLAGS += -s
#}

####################################################################################
#                       Compiler flags
####################################################################################
CONFIG += rtti

win32-msvc2008::QMAKE_CXXFLAGS += -D_CRT_SECURE_NO_WARNINGS -DNOMINMAX
win32-msvc2008::QMAKE_CXXFLAGS += -DBOOST_ALL_NO_LIB=1
win32-msvc2008::QMAKE_CXXFLAGS += /bigobj

win32-g++-*::QMAKE_LFLAGS += -mwindows

QMAKE_CXXFLAGS_DEBUG += -fno-default-inline

QMAKE_CXXFLAGS_DEBUG += -DDAE_DEBUG
QMAKE_CFLAGS_DEBUG   += -DDAE_DEBUG

#unix::QMAKE_CXXFLAGS += -ansi -pedantic

# Unresolved _gethostname problem in MinGW
#win32-g++-*::QMAKE_LFLAGS += -Wl,--enable-stdcall-fixup 

# If compiling from the compile_linux.sh shell script supress all warnings
shellCompile {
    unix::QMAKE_CXXFLAGS_WARN_ON = -w
    unix::QMAKE_CFLAGS_WARN_ON   = -w

    win32-g++-*::QMAKE_CXXFLAGS_WARN_ON = -w
    win32-g++-*::QMAKE_CFLAGS_WARN_ON   = -w
}
# If compiling from the GUI enable warnings
!shellCompile:message(Compiling from the GUI) {
win32-g++-*::QMAKE_CXXFLAGS_WARN_ON = -Wall -Wextra \
                                      -Wno-sign-compare \
                                      -Wno-unused-parameter \
                                      -Wno-unused-variable \
                                      -Wno-unused-but-set-variable
    
linux-g++::QMAKE_CXXFLAGS_WARN_ON = -Wall -Wextra \
                                    -Wno-sign-compare \
                                    -Wno-unused-parameter \
                                    -Wno-unused-variable \
                                    -Wno-unused-but-set-variable

macx-g++::QMAKE_CXXFLAGS_WARN_ON = -Wall -Wextra \
                                   -Wno-sign-compare \
                                   -Wno-unused-parameter \
                                   -Wno-unused-variable
}

unix::QMAKE_CFLAGS_RELEASE   -= -O2
unix::QMAKE_CXXFLAGS_RELEASE -= -O2

unix::QMAKE_CFLAGS_RELEASE   += -O3
unix::QMAKE_CXXFLAGS_RELEASE += -O3

# On some low-RAM machines certain boost.python modules cannot compile
# The workaround is to set the following flags:
#unix::QMAKE_CXXFLAGS += --param ggc-min-expand=30 --param ggc-min-heapsize=8192

# Use SSE for x86 32 bit machines (not used by default)
# When building for Mac-OS we build for all architectures and SSE flags should go away
#QMAKE_CXXFLAGS_RELEASE += -mfpmath=sse -msse -msse2 -msse3

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
#                                  FORTRAN
#####################################################################################
# gfortran does not exist in MacOS XCode and must be installed separately
#####################################################################################
win32-msvc2008::GFORTRAN    =
win32-g++-*::GFORTRAN       = -lgfortran
linux-g++::GFORTRAN         = -lgfortran
macx-g++::GFORTRAN          = -lgfortran


#####################################################################################
#                                  pthreads
#####################################################################################
win32-msvc2008::PTHREADS_LIB = -lpthread
unix::PTHREADS_LIB           = -lpthread
win32-g++-*::PTHREADS_LIB    = -lwinpthread


#####################################################################################
#                                    BOOST
#####################################################################################
# Boost version installed must be 1.42+ (asio, system, python, thread, regex)
# Starting with the version 1.2.1 daetools use manually compiled boost libraries.
# The compilation is done in the shell script compile_libraries_linux.sh
# For gcc-mingw32 first build bjam and then use:
# bjam --build-dir=./build --debug-building --layout=system --buildid=daetools-py27
#       --with-date_time --with-system --with-filesystem --with-regex --with-serialization
#       --with-thread --with-python variant=release link=shared threading=multi
#       runtime-link=shared toolset=gcc
#####################################################################################
win32-msvc2008::BOOSTDIR                  = ../boost$${PYTHON_MAJOR}.$${PYTHON_MINOR}
win32-msvc2008::BOOSTLIBPATH              = $${BOOSTDIR}/stage/lib
win32-msvc2008::BOOST_PYTHON_LIB_NAME     = boost_python-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}
win32-msvc2008::BOOST_SYSTEM_LIB_NAME     = boost_system-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}
win32-msvc2008::BOOST_THREAD_LIB_NAME     = boost_thread-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}
win32-msvc2008::BOOST_FILESYSTEM_LIB_NAME = boost_filesystem-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}
win32-msvc2008::BOOST_PYTHON_LIB          = $${BOOST_PYTHON_LIB_NAME}.lib $${PYTHON_LIB}
win32-msvc2008::BOOST_LIBS                = $${BOOST_SYSTEM_LIB_NAME}.lib \
                                            $${BOOST_THREAD_LIB_NAME}.lib \
                                            $${BOOST_FILESYSTEM_LIB_NAME}.lib

win32-g++-*::BOOSTDIR                   = ../boost$${PYTHON_MAJOR}.$${PYTHON_MINOR}
win32-g++-*::BOOSTLIBPATH               = $${BOOSTDIR}/stage/lib
win32-g++-*::BOOST_PYTHON_LIB_NAME      = boost_python-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}.dll
win32-g++-*::BOOST_SYSTEM_LIB_NAME      = boost_system-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}.dll
win32-g++-*::BOOST_THREAD_LIB_NAME      = boost_thread-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}.dll
win32-g++-*::BOOST_FILESYSTEM_LIB_NAME  = boost_filesystem-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}.dll
win32-g++-*::BOOST_PYTHON_LIB           = -L$${BOOSTLIBPATH} -l$${BOOST_PYTHON_LIB_NAME} \
                                          -L$${PYTHON_LIB_DIR} $${PYTHON_LIB}
win32-g++-*::BOOST_LIBS                 = -L$${BOOSTLIBPATH} -l$${BOOST_SYSTEM_LIB_NAME} \
                                                             -l$${BOOST_THREAD_LIB_NAME} \
                                                             -l$${BOOST_FILESYSTEM_LIB_NAME} \
                                                             -lws2_32 -lmswsock \
                                                             $${PTHREADS_LIB} \
                                                             $${RT}

unix::BOOSTDIR                   = ../boost$${PYTHON_MAJOR}.$${PYTHON_MINOR}
unix::BOOSTLIBPATH               = $${BOOSTDIR}/stage/lib
unix::BOOST_PYTHON_LIB_NAME      = boost_python-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}
unix::BOOST_SYSTEM_LIB_NAME      = boost_system-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}
unix::BOOST_THREAD_LIB_NAME      = boost_thread-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}
unix::BOOST_FILESYSTEM_LIB_NAME  = boost_filesystem-daetools-py$${PYTHON_MAJOR}$${PYTHON_MINOR}
unix::BOOST_PYTHON_LIB           = -L$${BOOSTLIBPATH} -l$${BOOST_PYTHON_LIB_NAME} \
                                   -L$${PYTHON_LIB_DIR} $${PYTHON_LIB}
unix::BOOST_LIBS                 = -L$${BOOSTLIBPATH} -l$${BOOST_SYSTEM_LIB_NAME} \
                                                      -l$${BOOST_THREAD_LIB_NAME} \
                                                      -l$${BOOST_FILESYSTEM_LIB_NAME} \
                                                       $${RT}


#####################################################################################
#                                 BLAS/LAPACK
#####################################################################################
win32-msvc2008::BLAS_LAPACK_LIBDIR = ../clapack/LIB/Win32
win32-g++-*::BLAS_LAPACK_LIBDIR    = ../lapack/lib
linux-g++::BLAS_LAPACK_LIBDIR      = ../lapack/lib
macx-g++::BLAS_LAPACK_LIBDIR       = ../lapack/lib

# Define DAE_USE_OPEN_BLAS if using OpenBLAS
win32-msvc2008::QMAKE_CXXFLAGS  +=
win32-g++-*::QMAKE_CXXFLAGS     +=
linux-g++::QMAKE_CXXFLAGS       += #-DDAE_USE_OPEN_BLAS
macx-g++::QMAKE_CXXFLAGS        +=

win32-msvc2008::BLAS_LAPACK_LIBS =  $${BLAS_LAPACK_LIBDIR}/BLAS_nowrap.lib \
                                    $${BLAS_LAPACK_LIBDIR}/clapack_nowrap.lib \
                                    $${BLAS_LAPACK_LIBDIR}/libf2c.lib
                                    
win32-g++-*::BLAS_LAPACK_LIBS    =  $${BLAS_LAPACK_LIBDIR}/liblapack.a $${BLAS_LAPACK_LIBDIR}/libblas.a -lgfortran -lm

# 1. OpenBLAS dynamically linked:
#linux-g++::BLAS_LAPACK_LIBS = -L$${BLAS_LAPACK_LIBDIR} -lopenblas_daetools -lm
# 2. daetools compiled reference BLAS and Lapack statically linked:
linux-g++::BLAS_LAPACK_LIBS = $${BLAS_LAPACK_LIBDIR}/liblapack.a $${BLAS_LAPACK_LIBDIR}/libblas.a -lgfortran -lm
macx-g++::BLAS_LAPACK_LIBS  = $${BLAS_LAPACK_LIBDIR}/liblapack.a $${BLAS_LAPACK_LIBDIR}/libblas.a -lgfortran -lm


#####################################################################################
#                                 SUNDIALS IDAS
#####################################################################################
# ./configure --prefix=${HOME}/Data/daetools/trunk/idas/build --disable-mpi
#             --enable-examples --enable-static=yes --enable-shared=no --with-pic
#             --enable-lapack CFLAGS=-O3
#####################################################################################
SUNDIALS         = ../idas/build
SUNDIALS_INCLUDE = $${SUNDIALS}/include
SUNDIALS_LIBDIR  = $${SUNDIALS}/lib

win32-msvc2008::SUNDIALS_LIBS = $${SUNDIALS_LIBDIR}/sundials_idas.lib \
                                $${SUNDIALS_LIBDIR}/sundials_nvecserial.lib
win32-g++-*::SUNDIALS_LIBS = $${SUNDIALS_LIBDIR}/libsundials_idas.a \
                             $${SUNDIALS_LIBDIR}/libsundials_nvecserial.a
unix::SUNDIALS_LIBS  = $${SUNDIALS_LIBDIR}/libsundials_idas.a \
                       $${SUNDIALS_LIBDIR}/libsundials_nvecserial.a


#####################################################################################
#                                  MUMPS
#####################################################################################
MUMPS_DIR  = ../mumps

win32-msvc2008::G95_LIBDIR = c:/g95/lib/gcc-lib/i686-pc-mingw32/4.1.2

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
win32-g++-*::MUMPS_LIBS = -lcoinmumps $${PTHREADS_LIB} $${RT} -lmsvcrt -lkernel32 -luser32 -lshell32 -luuid -lole32 -ladvapi32 -lws2_32
unix::MUMPS_LIBS        = -lcoinmumps $${PTHREADS_LIB} $${RT}


#####################################################################################
#                                  IPOPT
#####################################################################################
IPOPT_DIR = ../bonmin/build

IPOPT_INCLUDE = $${IPOPT_DIR}/include/coin
IPOPT_LIBDIR  = $${IPOPT_DIR}/lib

win32-msvc2008::IPOPT_LIBS = libCoinBlas.lib libCoinLapack.lib libf2c.lib libIpopt.lib
win32-g++-*::IPOPT_LIBS    = -lipopt -lcoinlapack -lcoinblas $${GFORTRAN}
unix::IPOPT_LIBS           = -lipopt -ldl


#####################################################################################
#                                  BONMIN
#####################################################################################
BONMIN_DIR = ../bonmin/build

BONMIN_INCLUDE = $${BONMIN_DIR}/include/coin
BONMIN_LIBDIR  = $${BONMIN_DIR}/lib

win32-msvc2008::BONMIN_LIBS = libCoinBlas.lib libCoinLapack.lib libf2c.lib \
                              libBonmin.lib libIpopt.lib libCbc.lib \
                              libCgl.lib libClp.lib libCoinUtils.lib \
                              libOsiCbc.lib libOsiClp.lib libOsi.lib
win32-g++-*::BONMIN_LIBS = -lbonmin -lCbc -lCbcSolver -lCgl \
                           -lClp -lCoinUtils -lipopt -lOsiCbc \
                           -lOsiClp -lOsi \
                           -lcoinlapack -lcoinblas
linux-g++::BONMIN_LIBS = -lbonmin -lCbc -lCbcSolver -lCgl \
                         -lClp -lCoinUtils -lipopt -lOsiCbc \
                         -lOsiClp -lOsi \
                         -ldl -lz -lbz2
macx-g++::BONMIN_LIBS  = -lbonmin -lCbc -lCbcSolver -lCgl \
                         -lClp -lCoinUtils -lipopt -lOsiCbc \
                         -lOsiClp -lOsi \
                         -ldl -lz -lbz2


#####################################################################################
#                                 NLOPT
#####################################################################################
NLOPT_DIR     = ../nlopt/build
NLOPT_INCLUDE = $${NLOPT_DIR}/include
NLOPT_LIBDIR  = $${NLOPT_DIR}/lib

win32-msvc2008::NLOPT_LIBS = nlopt.lib
win32-g++-*::NLOPT_LIBS    = -lnlopt -lm
unix::NLOPT_LIBS           = -lnlopt -lm


######################################################################################
#                                   SuperLU
######################################################################################
SUPERLU_PATH    = ../superlu
SUPERLU_LIBPATH = $${SUPERLU_PATH}/lib
SUPERLU_INCLUDE = $${SUPERLU_PATH}/SRC

win32-msvc2008::SUPERLU_LIBS = -L$${SUPERLU_LIBPATH} superlu.lib $${BLAS_LAPACK_LIBS}
win32-g++-*::SUPERLU_LIBS    = -L$${SUPERLU_LIBPATH} -lsuperlu_4.1 $${RT} $${PTHREADS_LIB}
unix::SUPERLU_LIBS           = -L$${SUPERLU_LIBPATH} -lsuperlu_4.1 $${RT} $${PTHREADS_LIB}


######################################################################################
#                                SuperLU_MT
######################################################################################
SUPERLU_MT_PATH    = ../superlu_mt
SUPERLU_MT_LIBPATH = $${SUPERLU_MT_PATH}/lib
SUPERLU_MT_INCLUDE = $${SUPERLU_MT_PATH}/SRC

win32-msvc2008::SUPERLU_MT_LIBS =
win32-g++-*::SUPERLU_MT_LIBS    = -L$${SUPERLU_MT_LIBPATH} -lsuperlu_mt_2.0 $${RT} $${PTHREADS_LIB}
unix::SUPERLU_MT_LIBS           = -L$${SUPERLU_MT_LIBPATH} -lsuperlu_mt_2.0 $${RT} $${PTHREADS_LIB}


######################################################################################
#                                SuperLU_CUDA
######################################################################################
win32-msvc2008::CUDA_PATH =
win32-g++-*::CUDA_PATH    =
linux-g++::CUDA_PATH      = /usr/local/cuda

SUPERLU_CUDA_PATH    = ../superlu_mt-GPU
SUPERLU_CUDA_LIBPATH = $${SUPERLU_CUDA_PATH}/lib
SUPERLU_CUDA_INCLUDE = $${SUPERLU_CUDA_PATH} \
                       $${CUDA_PATH}/include

win32-msvc2008::CUDA_LIBS =
win32-g++-*::CUDA_LIBS    =
linux-g++::CUDA_LIBS      = -L$${CUDA_PATH}/lib -lcuda -lcudart


######################################################################################
#                           Umfpack + AMD + CHOLMOD
######################################################################################
UMFPACK_LIBPATH = ../umfpack/build/lib

win32-msvc2008::UMFPACK_LIBS     =
linux-g++::UMFPACK_LIBS = $${UMFPACK_LIBPATH}/libumfpack.a \
                          $${UMFPACK_LIBPATH}/libcholmod.a \
                          $${UMFPACK_LIBPATH}/libamd.a \
                          $${UMFPACK_LIBPATH}/libcamd.a \
                          $${UMFPACK_LIBPATH}/libcolamd.a \
                          $${UMFPACK_LIBPATH}/libccolamd.a \
                          $${UMFPACK_LIBPATH}/libsuitesparseconfig.a
win32-g++-*::UMFPACK_LIBS = $${UMFPACK_LIBPATH}/libumfpack.a \
                            $${UMFPACK_LIBPATH}/libcholmod.a \
                            $${UMFPACK_LIBPATH}/libamd.a \
                            $${UMFPACK_LIBPATH}/libcamd.a \
                            $${UMFPACK_LIBPATH}/libcolamd.a \
                            $${UMFPACK_LIBPATH}/libccolamd.a \
                            $${UMFPACK_LIBPATH}/libsuitesparseconfig.a
#linux-g++::UMFPACK_LIBS = -lumfpack -lamd
macx-g++::UMFPACK_LIBS  = -lumfpack -lamd

#####################################################################################
#                                  TRILINOS 
#####################################################################################
TRILINOS_DIR  = ../trilinos/build

win32-msvc2008::TRILINOS_INCLUDE = $${TRILINOS_DIR}/include \
                                   $${TRILINOS_DIR}/../commonTools/WinInterface/include
win32-g++-*::TRILINOS_INCLUDE    = $${TRILINOS_DIR}/include \
unix::TRILINOS_INCLUDE           = $${TRILINOS_DIR}/include

win32-msvc2008::TRILINOS_LIBS = -L$${TRILINOS_DIR}/lib -L$${SUPERLU_PATH}/lib \
                                $${SUPERLU_LIBS} \
                                aztecoo.lib ml.lib ifpack.lib amesos.lib epetra.lib epetraext.lib teuchos.lib

win32-g++-*::TRILINOS_LIBS  = -L$${TRILINOS_DIR}/lib -L$${SUPERLU_PATH}/lib \
                              -laztecoo -lml -lifpack -lamesos -lepetra -lepetraext -lteuchos \
                               $${UMFPACK_LIBS} \
                               $${SUPERLU_LIBS}

linux-g++::TRILINOS_LIBS  = -L$${TRILINOS_DIR}/lib -L$${SUPERLU_PATH}/lib \
                            -laztecoo -lml -lifpack -lamesos -lepetra -lepetraext -lteuchos \
                            $${UMFPACK_LIBS} \
                            $${SUPERLU_LIBS}

macx-g++::TRILINOS_LIBS  = -L$${TRILINOS_DIR}/lib -L$${SUPERLU_PATH}/lib -L/opt/local/lib \
                           -laztecoo -lml -lifpack -lamesos -lepetra -lepetraext -lteuchos \
                            $${UMFPACK_LIBS} \
                            $${SUPERLU_LIBS}

######################################################################################
#                                INTEL Pardiso
######################################################################################
# Version: 11.1
# LD_LIBRARY_PATH should be set
# MKL_NUM_THREADS=Ncpu should be set
# http://software.intel.com/en-us/articles/intel-mkl-link-line-advisor/
#####################################################################################
win32-msvc2008::MKLPATH =
linux-g++::MKLPATH      = /opt/intel
macx-g++::MKLPATH       = /opt/intel

INTEL_MKL_INCLUDE = $${MKLPATH}/mkl/include
ARCH = $$QMAKE_HOST.arch

win32-msvc2008::MKL_LIBS       = $${MKLPATH}\ia32\lib
win32-msvc2008::INTEL_MKL_LIBS = -L$${MKL_LIBS} mkl_intel_c_dll.lib mkl_core_dll.lib mkl_intel_thread_dll.lib libiomp5md.lib

win32-g++-*::MKL_LIBS       =
win32-g++-*::INTEL_MKL_LIBS =

contains($$ARCH, x86) {
    linux-g++::INTEL_MKL_LIBS = -L$${MKLPATH}/lib/ia32 -L$${MKLPATH}/mkl/lib/ia32 \
                                -lmkl_rt \
                                -ldl -lpthread -lm
    linux-g++::QMAKE_LFLAGS   += -m32
    linux-g++::QMAKE_CXXFLAGS += -m32

    macx-g++::INTEL_MKL_LIBS = -L$${MKLPATH}/lib/ia32 -L$${MKLPATH}/mkl/lib/ia32 \
                               -lmkl_rt \
                               -ldl -lpthread -lm
    macx-g++::QMAKE_LFLAGS   += -m32
    macx-g++::QMAKE_CXXFLAGS += -m32
}

contains(ARCH, x86_64) {
    linux-g++::INTEL_MKL_LIBS = -L$${MKLPATH}/mkl/lib/intel64 \
                                -lmkl_rt \
                                -ldl -lpthread -lm
    linux-g++::QMAKE_LFLAGS   += -m64
    linux-g++::QMAKE_CXXFLAGS += -m64

    macx-g++::INTEL_MKL_LIBS = -L$${MKLPATH}/lib/intel64 -L$${MKLPATH}/mkl/lib/intel64 \
                               -lmkl_rt \
                               -ldl -lpthread -lm
    macx-g++::QMAKE_LFLAGS   += -m64
    macx-g++::QMAKE_CXXFLAGS += -m64
}


#####################################################################################
#                                 MPI SUPPORT
#####################################################################################
enable_mpi {
QMAKE_CXXFLAGS += -DDAE_MPI

win32-msvc2008::MPI =
unix::MPI  = 

win32-msvc2008::MPI_INCLUDE =
unix::MPI_INCLUDE  = /usr/include/mpi

win32-msvc2008::MPI_LIBDIR =
unix::MPI_LIBDIR  = 

win32-msvc2008::MPI_LIBS =
unix::MPI_LIBS           = -lboost_mpi-mt -lboost_serialization -lmpi_cxx -lmpi
}


#####################################################################################
#                                  DAE Tools
#####################################################################################
win32-msvc2008::DAE_CORE_LIB                 = cdaeCore.lib
win32-msvc2008::DAE_DATAREPORTING_LIB        = cdaeDataReporting.lib
win32-msvc2008::DAE_ACTIVITY_LIB             = cdaeActivity.lib
win32-msvc2008::DAE_IDAS_SOLVER_LIB          = cdaeIDAS_DAESolver.lib
win32-msvc2008::DAE_UNITS_LIB                = cdaeUnits.lib
win32-msvc2008::DAE_SUPERLU_SOLVER_LIB       = cdaeSuperLU_LASolver.lib
win32-msvc2008::DAE_SUPERLU_MT_SOLVER_LIB    = cdaeSuperLU_MT_LASolver.lib
win32-msvc2008::DAE_SUPERLU_CUDA_SOLVER_LIB  = cdaeSuperLU_CUDA_LASolver.lib
win32-msvc2008::DAE_BONMIN_SOLVER_LIB        = cdaeBONMIN_MINLPSolver.lib
win32-msvc2008::DAE_IPOPT_SOLVER_LIB         = cdaeIPOPT_NLPSolver.lib
win32-msvc2008::DAE_NLOPT_SOLVER_LIB         = cdaeNLOPT_NLPSolver.lib
win32-msvc2008::DAE_TRILINOS_SOLVER_LIB      = cdaeTrilinos_LASolver.lib
win32-msvc2008::DAE_INTEL_PARDISO_SOLVER_LIB = cdaeIntelPardiso_LASolver.lib
win32-msvc2008::DAE_DEALII_SOLVER_LIB        = cdaeDealII_FESolver.lib

win32-g++-*::DAE_CORE_LIB                 = -lcdaeCore
win32-g++-*::DAE_DATAREPORTING_LIB        = -lcdaeDataReporting
win32-g++-*::DAE_ACTIVITY_LIB             = -lcdaeActivity
win32-g++-*::DAE_IDAS_SOLVER_LIB          = -lcdaeIDAS_DAESolver
win32-g++-*::DAE_UNITS_LIB                = -lcdaeUnits
win32-g++-*::DAE_SUPERLU_SOLVER_LIB       = -lcdaeSuperLU_LASolver
win32-g++-*::DAE_SUPERLU_MT_SOLVER_LIB    = -lcdaeSuperLU_MT_LASolver
win32-g++-*::DAE_SUPERLU_CUDA_SOLVER_LIB  = -lcdaeSuperLU_CUDA_LASolver
win32-g++-*::DAE_BONMIN_SOLVER_LIB        = -lcdaeBONMIN_MINLPSolver
win32-g++-*::DAE_IPOPT_SOLVER_LIB         = -lcdaeIPOPT_NLPSolver
win32-g++-*::DAE_NLOPT_SOLVER_LIB         = -lcdaeNLOPT_NLPSolver
win32-g++-*::DAE_TRILINOS_SOLVER_LIB      = -lcdaeTrilinos_LASolver
win32-g++-*::DAE_INTEL_PARDISO_SOLVER_LIB = -lcdaeIntelPardiso_LASolver
win32-g++-*::DAE_DEALII_SOLVER_LIB        = -lcdaeDealII_FESolver

unix::DAE_CORE_LIB                 = -lcdaeCore
unix::DAE_DATAREPORTING_LIB        = -lcdaeDataReporting
unix::DAE_ACTIVITY_LIB             = -lcdaeActivity
unix::DAE_IDAS_SOLVER_LIB          = -lcdaeIDAS_DAESolver
unix::DAE_UNITS_LIB                = -lcdaeUnits
unix::DAE_SUPERLU_SOLVER_LIB       = -lcdaeSuperLU_LASolver
unix::DAE_SUPERLU_MT_SOLVER_LIB    = -lcdaeSuperLU_MT_LASolver
unix::DAE_SUPERLU_CUDA_SOLVER_LIB  = -lcdaeSuperLU_CUDA_LASolver
unix::DAE_BONMIN_SOLVER_LIB        = -lcdaeBONMIN_MINLPSolver
unix::DAE_IPOPT_SOLVER_LIB         = -lcdaeIPOPT_NLPSolver
unix::DAE_NLOPT_SOLVER_LIB         = -lcdaeNLOPT_NLPSolver
unix::DAE_TRILINOS_SOLVER_LIB      = -lcdaeTrilinos_LASolver
unix::DAE_INTEL_PARDISO_SOLVER_LIB = -lcdaeIntelPardiso_LASolver
unix::DAE_DEALII_SOLVER_LIB        = -lcdaeDealII_FESolver

QMAKE_LIBDIR += $${DAE_DEST_DIR} $${BOOSTLIBPATH} $${PYTHON_LIB_DIR}

#######################################################
#            Settings for installing files
#######################################################
# Removed "_numpy$${NUMPY_VERSION}" to avoid compile-time dependency on numpy versions
SOLVERS_DIR  = ../daetools-package/daetools/solvers/$${DAE_SYSTEM}_$${DAE_MACHINE}_py$${PYTHON_MAJOR}$${PYTHON_MINOR}
PYDAE_DIR    = ../daetools-package/daetools/pyDAE/$${DAE_SYSTEM}_$${DAE_MACHINE}_py$${PYTHON_MAJOR}$${PYTHON_MINOR}

win32-msvc2008::DUMMY = $$system(mkdir daetools-package\daetools\solvers\\$${DAE_SYSTEM}_$${DAE_MACHINE}_py$${PYTHON_MAJOR}$${PYTHON_MINOR})
win32-msvc2008::DUMMY = $$system(mkdir daetools-package\daetools\pyDAE\\$${DAE_SYSTEM}_$${DAE_MACHINE}_py$${PYTHON_MAJOR}$${PYTHON_MINOR})

win32-g++-*::DUMMY = $$system(mkdir -p daetools-package/daetools/solvers/$${DAE_SYSTEM}_$${DAE_MACHINE}_py$${PYTHON_MAJOR}$${PYTHON_MINOR})
win32-g++-*::DUMMY = $$system(mkdir -p daetools-package/daetools/pyDAE/$${DAE_SYSTEM}_$${DAE_MACHINE}_py$${PYTHON_MAJOR}$${PYTHON_MINOR})

unix::DUMMY = $$system(mkdir -p daetools-package/daetools/solvers/$${DAE_SYSTEM}_$${DAE_MACHINE}_py$${PYTHON_MAJOR}$${PYTHON_MINOR})
unix::DUMMY = $$system(mkdir -p daetools-package/daetools/pyDAE/$${DAE_SYSTEM}_$${DAE_MACHINE}_py$${PYTHON_MAJOR}$${PYTHON_MINOR})

STATIC_LIBS_DIR = ../daetools-package/daetools/usr/local/lib
HEADERS_DIR     = ../daetools-package/daetools/usr/local/include

#####################################################################################
#         Write compiler settings (needed to build installations packages)
#####################################################################################
# Python settings
#COMPILER_SETTINGS_FOLDER = .compiler_settings
#win32-msvc2008::COMPILER = $$system(mkdir $${COMPILER_SETTINGS_FOLDER})
#unix::COMPILER  = $$system(mkdir -p $${COMPILER_SETTINGS_FOLDER})

#COMPILER = $$system(echo $${DAE_TOOLS_MAJOR} > $${COMPILER_SETTINGS_FOLDER}/dae_major)
#COMPILER = $$system(echo $${DAE_TOOLS_MINOR} > $${COMPILER_SETTINGS_FOLDER}/dae_minor)
#COMPILER = $$system(echo $${DAE_TOOLS_BUILD} > $${COMPILER_SETTINGS_FOLDER}/dae_build)
