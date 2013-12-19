#!/bin/bash
# -*- coding: utf-8 -*-

set -e
usage()
{
cat << EOF
usage: $0 [options] libs/solvers

This script compiles third party libraries/solvers necessary to build daetools.

Typical usage (configure and then build all libraries/solvers):
    ./compile_libraries_linux.sh all

OPTIONS:
   -h | --help                      Show this message.
   
   Control options (if not set default is: --clean and --build):
   --configure                 Configure library/solver.
   --build                     Build library/solver.
   --clean                     Clean library/solver.
   
   Python options (if not set use system's default python). One of the following:
        --with-python-binary        Path to python binary to use.
        --with-python-version       Version of the system's python.
                                    Format: major.minor (i.e 2.7).

SOLVERS:
    all              All libraries and solvers.
                     Equivalent to: boost ref_blas_lapack umfpack idas trilinos superlu superlu_mt bonmin nlopt
    
    Individual libraries/solvers:
    boost            Boost libraries (system, thread, python)
    ref_blas_lapack  reference BLAS and Lapack libraries
    openblas         OpenBLAS library
    umfpack          Umfpack solver
    idas             IDAS solver
    trilinos         Trilinos Amesos and AztecOO solvers
    superlu          SuperLU solver
    superlu_mt       SuperLU solver
    
    bonmin           Bonmin solver
    nlopt            NLopt solver 

    libmesh          libMesh FE library 
    deal.ii          deal.II FE library
EOF
}

# Default python binary:
PYTHON=`python -c "import sys; print sys.executable"`

args=`getopt -a -o "h" -l "help,with-python-binary:,with-python-version:,configure,build,clean:" -n "compile_libraries_linux" -- $*`

DO_CONFIGURE="no"
DO_BUILD="no"
DO_CLEAN="no"

# Process options
for i; do
  case "$i" in
    -h|--help)  usage
                exit 1
                ;;
                  
    --with-python-binary)  PYTHON=`$2 -c "import sys; print sys.executable"`
                           shift ; shift 
                           ;;
                            
    --with-python-version )  PYTHON=`python$2 -c "import sys; print sys.executable"`
                             shift ; shift 
                             ;;
                                    
    --configure) DO_CONFIGURE="yes"
                    shift
                    ;;
                         
    --build) DO_BUILD="yes"
                shift
                ;;

    --clean) DO_CLEAN="yes"
                shift
                ;;

    --) shift; break 
       ;;
  esac
done

TRUNK="$( cd "$( dirname "$0" )" && pwd )"
HOST_ARCH=`uname -m`
PLATFORM=`uname -s`

if [ ${PLATFORM} = "Darwin" ]; then
  Ncpu=$(/usr/sbin/system_profiler -detailLevel full SPHardwareDataType | awk '/Total Number Of Cores/ {print $5};')
else
  Ncpu=`cat /proc/cpuinfo | grep processor | wc -l`
fi

PYTHON_MAJOR=`${PYTHON} -c "import sys; print(sys.version_info[0])"`
PYTHON_MINOR=`${PYTHON} -c "import sys; print(sys.version_info[1])"`
PYTHON_VERSION=${PYTHON_MAJOR}.${PYTHON_MINOR}
PYTHON_INCLUDE_DIR=`${PYTHON} -c "import distutils.sysconfig; print(distutils.sysconfig.get_python_inc())"`
PYTHON_SITE_PACKAGES_DIR=`${PYTHON} -c "import distutils.sysconfig; print(distutils.sysconfig.get_python_lib())"`
PYTHON_LIB_DIR=`${PYTHON} -c "import sys; print(sys.prefix)"`/lib

# daetools specific compiler flags
DAE_COMPILER_FLAGS="-fPIC"
BOOST_MACOSX_FLAGS=

if [ ${PLATFORM} = "Darwin" ]; then
  DAE_COMPILER_FLAGS="${DAE_COMPILER_FLAGS} -arch i386 -arch x86_64"
  BOOST_MACOSX_FLAGS="macosx-version-min=10.5 architecture=x86 address-model=32_64"
  
  if type "wget" > /dev/null ; then
    echo "wget found"
  else
    echo "cURL have problems to get files from Source Forge: geting wget instead..."
    curl -O ftp://ftp.gnu.org/gnu/wget/wget-1.13.tar.gz
    tar -xvzf wget-1.13.tar.gz
    cd wget-1.13
    ./configure --with-ssl=openssl
    make
    sudo make install
  fi

else
  if [ ${HOST_ARCH} != "x86_64" ]; then
    DAE_COMPILER_FLAGS="${DAE_COMPILER_FLAGS} -mfpmath=sse"
    SSE_TAGS=`grep -m 1 flags /proc/cpuinfo | grep -o 'sse\|sse2\|sse3\|ssse3\|sse4a\|sse4.1\|sse4.2\|sse5'`
    for SSE_TAG in ${SSE_TAGS}
    do
      DAE_COMPILER_FLAGS="${DAE_COMPILER_FLAGS} -m${SSE_TAG}"
    done
  fi
fi

if [ ${Ncpu} -gt 1 ]; then
  Ncpu=$(($Ncpu+1))
fi

export DAE_COMPILER_FLAGS

DAE_UMFPACK_INSTALL_DIR="${TRUNK}/umfpack/build"
export DAE_UMFPACK_INSTALL_DIR

vBOOST=1.52.0
vBOOST_=1_52_0
vBONMIN=1.7.0
vLAPACK=3.4.1
vSUPERLU=4.1
vSUPERLU_MT=2.0
vNLOPT=2.2.4
vIDAS=1.1.0
vTRILINOS=10.8.0
vUMFPACK=5.6.2
vAMD=2.3.1
vMETIS=5.1.0
vCHOLMOD=2.1.2
vCAMD=2.3.1
vCOLAMD=2.8.0
vCCOLAMD=2.8.0
vSUITESPARSE_CONFIG=4.2.1
vOPENBLAS=0.2.8
vLIBMESH=0.9.2.2
vDEALII=8.0.0

BOOST_BUILD_ID=daetools-py${PYTHON_MAJOR}${PYTHON_MINOR}
BOOST_PYTHON_BUILD_ID=

BOOST_HTTP=http://sourceforge.net/projects/boost/files/boost
LAPACK_HTTP=http://www.netlib.org/lapack
DAETOOLS_HTTP=http://sourceforge.net/projects/daetools/files/gnu-linux-libs
IDAS_HTTP=${DAETOOLS_HTTP}
BONMIN_HTTP=http://www.coin-or.org/download/source/Bonmin
SUPERLU_HTTP=http://crd.lbl.gov/~xiaoye/SuperLU
TRILINOS_HTTP=http://trilinos.sandia.gov/download/files
NLOPT_HTTP=http://ab-initio.mit.edu/nlopt
UMFPACK_HTTP=http://www.cise.ufl.edu/research/sparse/umfpack
AMD_HTTP=http://www.cise.ufl.edu/research/sparse/amd
METIS_HTTP=http://glaros.dtc.umn.edu/gkhome/fetch/sw/metis
CHOLMOD_HTTP=http://www.cise.ufl.edu/research/sparse/cholmod
CAMD_HTTP=http://www.cise.ufl.edu/research/sparse/camd
COLAMD_HTTP=http://www.cise.ufl.edu/research/sparse/colamd
CCOLAMD_HTTP=http://www.cise.ufl.edu/research/sparse/ccolamd
SUITESPARSE_CONFIG_HTTP=http://www.cise.ufl.edu/research/sparse/UFconfig
LIBMESH_HTTP=http://sourceforge.net/projects/libmesh/files/libmesh
DEALII_HTTP=https://dealii.googlecode.com/files

# If no option is set use defaults
if [ "${DO_CONFIGURE}" = "no" -a "${DO_BUILD}" = "no" -a "${DO_CLEAN}" = "no" ]; then
    DO_CONFIGURE="yes"
    DO_BUILD="yes"
    DO_CLEAN="no"
fi

if [ -z "$@" ]; then
    # If no project is specified compile all
    usage
    exit
else
    # Check if requested solver exist
    for solver in "$@"
    do
    case "$solver" in
        all)              ;;
        boost)            ;;
        ref_blas_lapack)  ;;
        openblas)         ;;
        umfpack)          ;;
        idas)             ;;
        trilinos)         ;;
        superlu)          ;;
        superlu_mt)       ;;
        bonmin)           ;;
        nlopt)            ;; 
        libmesh)          ;;
        deal.ii)          ;;
        *) echo Unrecognized solver: "$solver"
        exit
        ;;
    esac
    done
fi
echo ""
echo "###############################################################################"
echo "Proceed with the following options:"
echo "  - Python:                       ${PYTHON}"
echo "  - Python include dir:           ${PYTHON_INCLUDE_DIR}"
echo "  - Python site-packages dir:     ${PYTHON_SITE_PACKAGES_DIR}"
echo "  - Python lib dir:               ${PYTHON_LIB_DIR}"
echo "  - Platform:                     $PLATFORM"
echo "  - Architecture:                 $HOST_ARCH"
echo "  - Additional compiler flags:    ${DAE_COMPILER_FLAGS}"
echo "  - Number of threads:            ${Ncpu}"
echo "  - Projects to compile:          $@"
echo "     + Configure:  [$DO_CONFIGURE]"
echo "     + Build:      [$DO_BUILD]"
echo "     + Clean:      [$DO_CLEAN]"
echo "###############################################################################"
echo ""

# ACHTUNG! cd to TRUNK (in case the script is called from some other folder)
cd "${TRUNK}"

#######################################################
#                       BOOST                         #
#######################################################
configure_boost() 
{
  if [ -e boost ]; then
    rm -r boost
  fi
  echo ""
  echo "[*] Setting-up boost"
  echo ""
  if [ ! -e boost_${vBOOST_}.tar.gz ]; then
    wget ${BOOST_HTTP}/${vBOOST}/boost_${vBOOST_}.tar.gz
  fi
  tar -xzf boost_${vBOOST_}.tar.gz
  mv boost_${vBOOST_} boost
  cd boost
  sh bootstrap.sh

  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_boost() 
{
  cd boost
  echo ""
  echo "[*] Building boost"
  echo ""
  
  BOOST_USER_CONFIG=~/user-config.jam
  echo "using python"                           >  ${BOOST_USER_CONFIG}
  echo "    : ${PYTHON_MAJOR}.${PYTHON_MINOR}"  >> ${BOOST_USER_CONFIG}
  echo "    : ${PYTHON}"                        >> ${BOOST_USER_CONFIG}
  echo "    : ${PYTHON_INCLUDE_DIR}"            >> ${BOOST_USER_CONFIG}
  echo "    : ${PYTHON_LIB_DIR}"                >> ${BOOST_USER_CONFIG}
  echo "    : <toolset>gcc"                     >> ${BOOST_USER_CONFIG}
  echo "    ;"                                  >> ${BOOST_USER_CONFIG}
  
  ./bjam --build-dir=./build --debug-building --layout=system --buildid=${BOOST_BUILD_ID} \
         --with-date_time --with-system --with-filesystem --with-regex --with-serialization --with-thread --with-python \
         variant=release link=shared threading=multi runtime-link=shared ${BOOST_MACOSX_FLAGS}

  cp -a stage/lib/libboost_python-${BOOST_BUILD_ID}${BOOST_PYTHON_BUILD_ID}* ../daetools-package/solibs
  cp -a stage/lib/libboost_system-${BOOST_BUILD_ID}${BOOST_PYTHON_BUILD_ID}* ../daetools-package/solibs
  cp -a stage/lib/libboost_thread-${BOOST_BUILD_ID}${BOOST_PYTHON_BUILD_ID}* ../daetools-package/solibs
  cp -a stage/lib/libboost_filesystem-${BOOST_BUILD_ID}${BOOST_PYTHON_BUILD_ID}* ../daetools-package/solibs
  
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"
}

clean_boost()
{
  echo ""
  echo "[*] Cleaning boost..."
  echo ""
  cd boost
  ./bjam --clean
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#                   OpenBLAS                          #
#######################################################
configure_openblas() 
{
  if [ -e openblas ]; then
    rm -r openblas
  fi
  
  echo ""
  echo "Setting-up openblas..."
  echo ""
  if [ ! -e openblas-${vOPENBLAS}.tar.gz ]; then
    wget ${DAETOOLS_HTTP}/openblas-${vOPENBLAS}.tar.gz
  fi
  if [ ! -e Makefile-openblas.rule ]; then
    wget ${DAETOOLS_HTTP}/Makefile-openblas.rule
  fi
  tar -xzf openblas-${vOPENBLAS}.tar.gz
  cp Makefile-openblas.rule openblas/Makefile.rule
  cd openblas
  mkdir build
  cd "${TRUNK}"
  
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_openblas() 
{
  cd openblas
  echo ""
  echo "[*] Building openblas..."
  echo ""
  make -j${Ncpu} libs
  make 
  make prefix=build install
  cp -a libopenblas_daetools* ../daetools-package/solibs
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"
}

clean_openblas()
{
  echo ""
  echo "[*] Cleaning openblas..."
  echo ""
  cd openblas
  make clean
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#            Reference BLAS and LAPACK                #
#######################################################
configure_ref_blas_lapack() 
{
  if [ -e lapack ]; then
    rm -r lapack
  fi
  
  echo ""
  echo "[*] Setting-up reference blas & lapack..."
  echo ""
  if [ ! -e lapack-${vLAPACK}.tgz ]; then
    wget ${LAPACK_HTTP}/lapack-${vLAPACK}.tgz
  fi
  if [ ! -e daetools_lapack_make.inc ]; then
    wget ${DAETOOLS_HTTP}/daetools_lapack_make.inc
  fi
  tar -xzf lapack-${vLAPACK}.tgz
  mv lapack-${vLAPACK} lapack
  cp daetools_lapack_make.inc lapack/make.inc
  cd "${TRUNK}"
  
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_ref_blas_lapack() 
{
  cd lapack
  echo ""
  echo "[*] Building reference blas & lapack..."
  echo ""
  make -j${Ncpu} lapacklib
  make -j${Ncpu} blaslib
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"
}

clean_lapack()
{
  echo ""
  echo "[*] Cleaning reference blas & lapack..."
  echo ""
  cd lapack
  make clean
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#                      UMFPACK                        #
#######################################################
configure_umfpack() 
{
  if [ -e umfpack ]; then
    rm -rf umfpack
  fi
  echo ""
  echo "[*] Setting-up umfpack and friends..."
  echo ""
  if [ ! -e SuiteSparse_config-${vSUITESPARSE_CONFIG}.tar.gz ]; then
    wget ${SUITESPARSE_CONFIG_HTTP}/SuiteSparse_config-${vSUITESPARSE_CONFIG}.tar.gz
  fi 
  if [ ! -e CHOLMOD-${vCHOLMOD}.tar.gz ]; then
    wget ${CHOLMOD_HTTP}/CHOLMOD-${vCHOLMOD}.tar.gz
  fi
  if [ ! -e AMD-${vAMD}.tar.gz ]; then
    wget ${AMD_HTTP}/AMD-${vAMD}.tar.gz
  fi
  if [ ! -e CAMD-${vCAMD}.tar.gz ]; then
    wget ${CAMD_HTTP}/CAMD-${vCAMD}.tar.gz
  fi
  if [ ! -e COLAMD-${vCOLAMD}.tar.gz ]; then
    wget ${COLAMD_HTTP}/COLAMD-${vCOLAMD}.tar.gz
  fi
  if [ ! -e CCOLAMD-${vCCOLAMD}.tar.gz ]; then
    wget ${CCOLAMD_HTTP}/CCOLAMD-${vCCOLAMD}.tar.gz
  fi
  if [ ! -e UMFPACK-${vUMFPACK}.tar.gz ]; then
    wget ${UMFPACK_HTTP}/UMFPACK-${vUMFPACK}.tar.gz
  fi
  if [ ! -e SuiteSparse_config.mk ]; then
    wget ${DAETOOLS_HTTP}/SuiteSparse_config.mk
  fi
  #if [ ! -e metis-${vMETIS}.tar.gz ]; then
  #  wget ${METIS_HTTP}/metis-${vMETIS}.tar.gz
  #fi
  #if [ ! -e metis.h ]; then
  #  wget ${DAETOOLS_HTTP}/metis.h
  #fi
  #if [ ! -e Makefile-CHOLMOD.patch ]; then
  #  wget ${DAETOOLS_HTTP}/Makefile-CHOLMOD.patch
  #fi
  
  mkdir umfpack
  cd umfpack
  tar -xzf ../SuiteSparse_config-${vSUITESPARSE_CONFIG}.tar.gz
  tar -xzf ../CHOLMOD-${vCHOLMOD}.tar.gz
  tar -xzf ../AMD-${vAMD}.tar.gz
  tar -xzf ../CAMD-${vCAMD}.tar.gz
  tar -xzf ../COLAMD-${vCOLAMD}.tar.gz
  tar -xzf ../CCOLAMD-${vCCOLAMD}.tar.gz
  tar -xzf ../UMFPACK-${vUMFPACK}.tar.gz
  cp ../SuiteSparse_config.mk SuiteSparse_config

  #tar -xzf ../metis-${vMETIS}.tar.gz
  #cp ../metis.h metis-${vMETIS}/include
  # Apply Metis 5.1.0 patch for CHOLMOD
  #cd CHOLMOD/Lib
  #patch < ../../../Makefile-CHOLMOD.patch
  
  mkdir build
  mkdir build/lib
  mkdir build/include
  cd "${TRUNK}"
  
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_umfpack() 
{
#  cd umfpack/metis-${vMETIS}
#  echo "[*] Building metis..."
#  echo "make config prefix=${DAE_UMFPACK_INSTALL_DIR} shared=0 -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}""
#  make config prefix=${DAE_UMFPACK_INSTALL_DIR} -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}"
#  make install
#  echo ""
#  echo "[*] Done!"
#  echo ""
#  cd "${TRUNK}"

  cd umfpack/SuiteSparse_config
  echo ""
  echo "[*] Building suitesparseconfig..."
  echo ""
  #echo "make cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library"
  make cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library
  make install
  make clean
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"

  cd umfpack/AMD
  echo ""
  echo "[*] Building amd..."
  echo ""
  #echo "make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library"
  make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library
  make install
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"

  cd umfpack/CAMD
  echo ""
  echo "[*] Building camd..."
  echo ""
  #echo "make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library"
  make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library
  make install
  echo "[*] Done!"
  cd "${TRUNK}"

  cd umfpack/COLAMD
  echo ""
  echo "[*] Building colamd..."
  echo ""
  #echo "make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library"
  make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library
  make install
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"

  cd umfpack/CCOLAMD
  echo ""
  echo "[*] Building ccolamd..."
  echo ""
  #echo "make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library"
  make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library
  make install
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"

  cd umfpack/CHOLMOD
  echo ""
  echo "[*] Building cholmod..."
  echo ""
  #echo "make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library"
  make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library
  make install
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"

  cd umfpack/UMFPACK
  echo ""
  echo "[*] Building umfpack..."
  echo ""
  #echo "make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library"
  make -j${Ncpu} cc=gcc F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" CPPFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}" library
  make install
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"
}

clean_umfpack()
{
  echo ""
  echo "[*] Cleaning umfpack..."
  echo ""
  #cd umfpack/metis-${vMETIS}
  #make clean
  #cd "${TRUNK}"
  
  cd umfpack/SuiteSparse_config
  make clean
  cd "${TRUNK}"
  
  cd umfpack/AMD
  make clean
  cd "${TRUNK}"
  
  cd umfpack/CAMD
  make clean
  cd "${TRUNK}"
  
  cd umfpack/COLAMD
  make clean
  cd "${TRUNK}"
  
  cd umfpack/CCOLAMD
  make clean
  cd "${TRUNK}"
  
  cd umfpack/CHOLMOD
  make clean
  cd "${TRUNK}"
  
  cd umfpack/UMFPACK
  make clean
  cd "${TRUNK}"
  
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#                       IDAS                          #
#######################################################
configure_idas() 
{
  if [ -e idas ]; then
    rm -r idas
  fi
  echo ""
  echo "[*] Setting-up idas..."
  echo ""
  if [ ! -e idas-${vIDAS}.tar.gz ]; then
    wget ${IDAS_HTTP}/idas-${vIDAS}.tar.gz
  fi
  if [ ! -e idasMakefile.in.patch ]; then
    wget ${DAETOOLS_HTTP}/idasMakefile.in.patch
  fi
  tar -xzf idas-${vIDAS}.tar.gz
  mv idas-${vIDAS} idas
  cd idas
  patch < ../idasMakefile.in.patch
  mkdir build
  ./configure --prefix=${TRUNK}/idas/build --with-pic --disable-mpi --enable-examples --enable-static=yes --enable-shared=no --enable-lapack F77=gfortran CFLAGS="${DAE_COMPILER_FLAGS} -O3" FFLAGS="${DAE_COMPILER_FLAGS}"
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_idas() 
{
  cd idas
  echo ""
  echo "[*] Building idas..."
  echo ""
  make 
  make install
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"
}

clean_idas()
{
  echo ""
  echo "[*] Cleaning idas..."
  echo ""
  cd idas
  make clean
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#                     SUPERLU                         #
#######################################################
configure_superlu() 
{
  if [ -e superlu ]; then
    rm -r superlu
  fi
  echo ""
  echo "[*] Setting-up superlu..."
  echo ""
  if [ ! -e superlu_${vSUPERLU}.tar.gz ]; then
    wget ${SUPERLU_HTTP}/superlu_${vSUPERLU}.tar.gz
  fi
  if [ ! -e superlu_makefiles.tar.gz ]; then
    wget ${DAETOOLS_HTTP}/superlu_makefiles.tar.gz
  fi
  tar -xzf superlu_${vSUPERLU}.tar.gz
  mv SuperLU_${vSUPERLU} superlu
  cd superlu
  tar -xzf ../superlu_makefiles.tar.gz
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_superlu() 
{
  cd superlu
  echo ""
  echo "[*] Building superlu..."
  echo ""
  make superlulib
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"
}

clean_superlu()
{
  echo ""
  echo "[*] Cleaning superlu..."
  echo ""
  cd superlu
  make cleanlib
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#                    SUPERLU_MT                       #
#######################################################
configure_superlu_mt() 
{
  if [ -e superlu_mt ]; then
    rm -r superlu_mt
  fi
  echo ""
  echo "[*] Setting-up superlu_mt..."
  echo ""
  if [ ! -e superlu_mt_${vSUPERLU_MT}.tar.gz ]; then
    wget ${SUPERLU_HTTP}/superlu_mt_${vSUPERLU_MT}.tar.gz
  fi
  if [ ! -e superlu_mt_makefiles.tar.gz ]; then
    wget ${DAETOOLS_HTTP}/superlu_mt_makefiles.tar.gz
  fi
  tar -xzf superlu_mt_${vSUPERLU_MT}.tar.gz
  mv SuperLU_MT_${vSUPERLU_MT} superlu_mt
  cd superlu_mt
  tar -xzf ../superlu_mt_makefiles.tar.gz
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_superlu_mt() 
{
  cd superlu_mt
  echo ""
  echo "[*] Building superlu_mt..."
  echo ""
  make lib
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"
}

clean_superlu_mt()
{
  echo ""
  echo "[*] Cleaning superlu_mt..."
  echo ""
  cd superlu_mt
  make cleanlib
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#                      BONMIN                         #
#######################################################
configure_bonmin() 
{
  if [ -e bonmin ]; then
    rm -r bonmin
  fi
  echo ""
  echo "[*] Setting-up bonmin..."
  echo ""
  if [ ! -e Bonmin-${vBONMIN}.zip ]; then
    wget ${BONMIN_HTTP}/Bonmin-${vBONMIN}.zip
  fi
  unzip Bonmin-${vBONMIN}.zip
  rm -rf bonmin/Bonmin-${vBONMIN}
  mv Bonmin-${vBONMIN} bonmin
  cd bonmin/ThirdParty/Mumps
  sh get.Mumps
  cd ../..
  mkdir -p build
  cd build
  ../configure --disable-dependency-tracking --enable-shared=no --enable-static=yes ARCHFLAGS="${DAE_COMPILER_FLAGS}" CFLAGS="${DAE_COMPILER_FLAGS}" CXXFLAGS="${DAE_COMPILER_FLAGS}" FFLAGS="${DAE_COMPILER_FLAGS}" LDFLAGS="${DAE_COMPILER_FLAGS}"
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_bonmin() 
{
  cd bonmin/build
  echo "[*] Building bonmin..."
  make -j${Ncpu}
  make test
  make install
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"
}

clean_bonmin()
{
  echo ""
  echo "[*] Cleaning bonmin..."
  echo ""
  cd bonmin/build
  make clean
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#                      NLOPT                          #
#######################################################
configure_nlopt() 
{
  if [ -e nlopt ]; then
    rm -r nlopt
  fi
  echo ""
  echo "[*] Setting-up nlopt..."
  echo ""
  if [ ! -e nlopt-${vNLOPT}.tar.gz ]; then
    wget ${NLOPT_HTTP}/nlopt-${vNLOPT}.tar.gz
  fi
  tar -xzf nlopt-${vNLOPT}.tar.gz
  mv nlopt-${vNLOPT} nlopt
  cd nlopt
  mkdir build
  cd build
  ../configure --disable-dependency-tracking -prefix=${TRUNK}/nlopt/build CFLAGS="${DAE_COMPILER_FLAGS}" CXXFLAGS="${DAE_COMPILER_FLAGS}" FFLAGS="${DAE_COMPILER_FLAGS}"
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_nlopt() 
{
  cd nlopt/build
  echo "[*] Building nlopt..."
  make
  make install
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"
}

clean_nlopt()
{
  echo ""
  echo "[*] Cleaning nlopt..."
  echo ""
  cd nlopt/build
  make clean
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#                   TRILINOS                          #
#######################################################
configure_trilinos() 
{
  if [ -e trilinos ]; then
    rm -r trilinos
  fi
  
  echo "[*] Setting-up trilinos..."
  if [ ! -e trilinos-${vTRILINOS}-Source.tar.gz ]; then
    wget ${TRILINOS_HTTP}/trilinos-${vTRILINOS}-Source.tar.gz
  fi
  tar -xzf trilinos-${vTRILINOS}-Source.tar.gz
  mv trilinos-${vTRILINOS}-Source trilinos
  cd trilinos
  mkdir build
  cd build

  export TRILINOS_HOME="${TRUNK}/trilinos"
  EXTRA_ARGS=

  echo $TRILINOS_HOME

  if [ ${PLATFORM} = "Darwin" ]; then
    UMFPACK_INCLUDE_DIR="/opt/local/include/ufsparse"
  else
    UMFPACK_INCLUDE_DIR="${DAE_UMFPACK_INSTALL_DIR}/include"
  fi
  
  cmake \
    -DCMAKE_BUILD_TYPE:STRING=RELEASE \
    -DBUILD_SHARED_LIBS:BOOL=OFF \
    -DTrilinos_ENABLE_Amesos:BOOL=ON \
    -DTrilinos_ENABLE_Epetra:BOOL=ON \
    -DTrilinos_ENABLE_AztecOO:BOOL=ON \
    -DTrilinos_ENABLE_ML:BOOL=ON \
    -DTrilinos_ENABLE_Ifpack:BOOL=ON \
    -DTrilinos_ENABLE_Teuchos:BOOL=ON \
    -DAmesos_ENABLE_SuperLU:BOOL=ON \
    -DIfpack_ENABLE_SuperLU:BOOL=ON \
    -DTPL_SuperLU_INCLUDE_DIRS:FILEPATH=${TRUNK}/superlu/SRC \
    -DTPL_SuperLU_LIBRARIES:STRING=superlu_4.1 \
    -DTPL_ENABLE_UMFPACK:BOOL=ON \
    -DTPL_UMFPACK_INCLUDE_DIRS:FILEPATH=${UMFPACK_INCLUDE_DIR} \
    -DTPL_UMFPACK_LIBRARIES:STRING=umfpack \
    -DTPL_ENABLE_MPI:BOOL=OFF \
    -DDART_TESTING_TIMEOUT:STRING=600 \
    -DCMAKE_INSTALL_PREFIX:PATH=. \
    -DCMAKE_CXX_FLAGS:STRING="-DNDEBUG ${DAE_COMPILER_FLAGS}" \
    -DCMAKE_C_FLAGS:STRING="-DNDEBUG ${DAE_COMPILER_FLAGS}" \
    -DCMAKE_Fortran_FLAGS:STRING="-DNDEBUG ${DAE_COMPILER_FLAGS}" \
    $EXTRA_ARGS \
    ${TRILINOS_HOME}
  
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_trilinos() 
{
  cd trilinos/build
  echo "[*] Building trilinos..."
  make -j${Ncpu}
  make install
  echo ""
  echo "[*] Done!"
  echo ""
  cd "${TRUNK}"
}

clean_trilinos()
{
  echo ""
  echo "[*] Cleaning trilinos..."
  echo ""
  cd trilinos/build
  make clean
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#                     libMesh                         #
#######################################################
configure_libmesh() 
{
  if [ -e libmesh ]; then
    rm -r libmesh
  fi
  echo ""
  echo "[*] Setting-up libmesh..."
  echo ""
  if [ ! -e libmesh-${vLIBMESH}.tar.gz ]; then
    wget ${LIBMESH_HTTP}/${vLIBMESH}/libmesh-${vLIBMESH}.tar.gz
  fi
  
  tar -xzf libmesh-${vLIBMESH}.tar.gz
  mv libmesh-${vLIBMESH} libmesh
  cd libmesh
  mkdir build
  export BOOST_ROOT="${TRUNK}/boost"
  ./configure --prefix="${TRUNK}/libmesh/build" --with-boost="${TRUNK}/boost"

  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_libmesh() 
{
  cd libmesh
  echo ""
  echo "[*] Building libmesh..."
  echo ""
  make -j${Ncpu}
  make install
  cd "${TRUNK}"
}

clean_libmesh()
{
  echo ""
  echo "[*] Cleaning libmesh..."
  echo ""
  cd libmesh
  make clean
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

#######################################################
#                     deal.II                         #
#######################################################
configure_dealii() 
{
  if [ -e deal.II ]; then
    rm -r deal.II
  fi
  echo ""
  echo "[*] Setting-up deal.II..."
  echo ""
  if [ ! -e deal.II-${vDEALII}.tar.gz ]; then
    wget ${DEALII_HTTP}/deal.II-${vDEALII}.tar.gz
  fi
  
  tar -xzf deal.II-${vDEALII}.tar.gz
  cd deal.II
  mkdir build
  cmake \
    -DCMAKE_BUILD_TYPE:STRING=RELEASE \
    -DDEAL_II_PACKAGE_NAME:STRING=deal.II-daetools \
    -DCMAKE_INSTALL_PREFIX="${TRUNK}/deal.II/build" \
    -DDEAL_II_WITH_THREADS=OFF \
    -DDEAL_II_WITH_MPI=OFF \
    -DDEAL_II_COMPONENT_PARAMETER_GUI=OFF \
    -DDEAL_II_COMPONENT_MESH_CONVERTER=ON 

  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}

compile_dealii() 
{
  cd deal.II
  echo ""
  echo "[*] Building deal.II..."
  echo ""
  make -j${Ncpu} install
  
  cp -a build/lib/libdeal_II-daetools.so.${vDEALII} ../daetools-package/solibs/libdeal_II-daetools.so.${vDEALII}
  
  cd "${TRUNK}"
}

clean_dealii()
{
  echo ""
  echo "[*] Cleaning deal.II..."
  echo ""
  cd deal.II
  make clean
  cd "${TRUNK}"
  echo ""
  echo "[*] Done!"
  echo ""
}


#######################################################
#                Actual work                          #
#######################################################
for solver in "$@"
do
  case "$solver" in
    all)              if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_boost
                        configure_ref_blas_lapack
                        #configure_openblas
                        configure_umfpack
                        configure_idas
                        configure_superlu
                        configure_superlu_mt
                        configure_trilinos
                        configure_bonmin
                        configure_nlopt
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_boost
                        compile_ref_blas_lapack
                        #compile_openblas
                        compile_umfpack
                        compile_idas
                        compile_superlu
                        compile_superlu_mt
                        compile_trilinos
                        compile_bonmin
                        compile_nlopt
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_boost
                        clean_ref_blas_lapack
                        #clean_openblas
                        clean_umfpack
                        clean_idas
                        clean_superlu
                        clean_superlu_mt
                        clean_trilinos
                        clean_bonmin
                        clean_nlopt
                      fi
                      ;;
    
    boost)            if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_boost
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_boost
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_boost
                      fi
                      ;;
                      
    ref_blas_lapack)  if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_ref_blas_lapack
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_ref_blas_lapack
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_ref_blas_lapack
                      fi
                      ;;
                      
    openblas)         if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_openblas
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_openblas
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_openblas
                      fi
                      ;;
                      
    umfpack)          if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_umfpack
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_umfpack
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_umfpack
                      fi
                      ;;
                      
    idas)             if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_idas
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_idas
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_idas
                      fi
                      ;;
                      
    trilinos)         if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_trilinos
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_trilinos
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_trilinos
                      fi
                      ;;
                      
    superlu)          if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_superlu
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_superlu
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_superlu
                      fi
                      ;;
                      
    superlu_mt)       if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_superlu_mt
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_superlu_mt
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_superlu_mt
                      fi
                      ;;
                      
    bonmin)           if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_bonmin
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_bonmin
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_bonmin
                      fi
                      ;;
                      
    nlopt)            if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_nlopt
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_nlopt
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_nlopt
                      fi
                      ;; 
                      
    libmesh)          if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_libmesh
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_libmesh
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_libmesh
                      fi
                      ;; 
                      
    deal.ii)          if [ "${DO_CONFIGURE}" = "yes" ]; then
                        configure_dealii
                      fi
                      
                      if [ "${DO_BUILD}" = "yes" ]; then 
                        compile_dealii
                      fi
                      
                      if [ "${DO_CLEAN}" = "yes" ]; then 
                        clean_dealii
                      fi
                      ;; 
                      
    *) echo Unrecognized solver: "$solver"
       exit
       ;;
  esac
done

cd "${TRUNK}"
