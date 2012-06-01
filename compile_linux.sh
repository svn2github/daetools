#!/bin/sh

set -e

if [ "$1" = "" ] || [ "$1" = "--help" ] || [ "$1" = "-help" ] || [ "$1" = "-h" ]; then
  echo "Usage: compile_linux [commands]"
  echo "Commands: all | core | pydae | trilinos | superlu | superlu_mt | superlu_cuda | bonmin | ipopt | nlopt"
  echo "  - all: build all daetools libraries, solvers and python wrapper modules"
  echo "  - core: build core daetools c++ libraries and python wrapper modules (no 3rd party LA/(MI)NLP solvers)"
  echo "  - pydae: build daetools core python wrapper modules only"
  echo "  - trilinos, superlu, superlu_mt, superlu_cuda, bonmin, ipopt, nlopt: build particular solver and its python wrapper module"
  return
fi

PROJECTS=$1

HOST_ARCH=`uname -m`
PLATFORM=`uname -s`
TRUNK="$( cd "$( dirname "$0" )" && pwd )"

if [ ${PLATFORM} = "Darwin" ]; then
  Ncpu=$(/usr/sbin/system_profiler -detailLevel full SPHardwareDataType | awk '/Total Number Of Cores/ {print $5};')
  SPEC=macx-g++

else
  Ncpu=`cat /proc/cpuinfo | grep processor | wc -l`
  SPEC=linux-g++
#  if [ ${HOST_ARCH} = "x86_64" ]; then
#    SPEC=linux-g++-64
#  else
#    SPEC=linux-g++-32
#  fi
fi

if [ ${Ncpu} -gt 1 ]; then
  Ncpu=$(($Ncpu+1))
fi
echo "Number of threads: ${Ncpu}"

cd ${TRUNK}

compile () {
  DIR=$1
  MAKEARG=$2
  CONFIG=$3
  echo "****************************************************************"
  echo "Compiling the project $DIR ... ($1, $2, $3)"
  echo "****************************************************************"
  
  if [ ${DIR} = "dae" ]; then
    cd ${TRUNK}
  elif [ ${DIR} = "pyCore" ]; then
    cd ${TRUNK}/pyCore
  elif [ ${DIR} = "pyActivity" ]; then
    cd ${TRUNK}/pyActivity
  elif [ ${DIR} = "pyDataReporting" ]; then
    cd ${TRUNK}/pyDataReporting
  elif [ ${DIR} = "pyIDAS" ]; then
    cd ${TRUNK}/pyIDAS
  elif [ ${DIR} = "pyUnits" ]; then
    cd ${TRUNK}/pyUnits
  elif [ ${DIR} = "pySuperLU" ]; then
    cd ${TRUNK}/pySuperLU
  elif [ ${DIR} = "pyBonmin" ]; then
    cd ${TRUNK}/pyBonmin
  elif [ ${DIR} = "pyTrilinos" ]; then
    cd ${TRUNK}/pyTrilinos
  else
    cd ${DIR}
  fi

  echo "***** EXECUTE: qmake -makefile $1.pro -r CONFIG+=release -spec ${SPEC} ${CONFIG}"
  qmake -makefile $1.pro -r CONFIG+=release -spec ${SPEC} ${CONFIG} 
  
  echo "***** EXECUTE: make clean -w"
  make clean -w
  
  echo "***** EXECUTE: make ${MAKEARG} -w"
  make ${MAKEARG} -w
  
  cd ${TRUNK}
  echo 
}

case ${PROJECTS} in
  pydae) echo Compiling only DAE python wrappers...
         compile pyCore           "-j1"
         compile pyActivity       "-j1"
         compile pyDataReporting  "-j1"
         compile pyIDAS           "-j1"
         compile pyUnits          "-j1"
         ;;

  all)  echo Compile ALL projects
        if [ ! -d ${TRUNK}/release ]; then
          mkdir ${TRUNK}/release
        fi
        cd ${TRUNK}/release
        rm -rf *
        cd ${TRUNK}

        compile dae                "-j$Ncpu"

        compile LA_SuperLU         "-j1"                     "CONFIG+=shellCompile CONFIG+=shellSuperLU"
        compile pySuperLU          "-j1"                     "CONFIG+=shellCompile CONFIG+=shellSuperLU"

        compile LA_SuperLU         "-j1"                     "CONFIG+=shellCompile CONFIG+=shellSuperLU_MT"
        compile pySuperLU          "-j1"                     "CONFIG+=shellCompile CONFIG+=shellSuperLU_MT"

        #compile LA_SUPERLU         "-j1 --file=gpuMakefile"  "CONFIG+=shellCompile CONFIG+=shellSuperLU_CUDA"
        #compile pySuperLU          "-j1 --file=gpuMakefile"  "CONFIG+=shellCompile CONFIG+=shellSuperLU_CUDA"

        #compile LA_CUSP            "-j1 --file=cudaMakefile"

        compile LA_Trilinos_Amesos "-j1"
        compile pyTrilinos         "-j1"

        compile BONMIN_MINLPSolver "-j1"                     "CONFIG+=shellCompile CONFIG+=shellBONMIN"
        compile pyBONMIN           "-j1"                     "CONFIG+=shellCompile CONFIG+=shellBONMIN"

        compile BONMIN_MINLPSolver "-j1"                     "CONFIG+=shellCompile CONFIG+=shellIPOPT"
        compile pyBONMIN           "-j1"                     "CONFIG+=shellCompile CONFIG+=shellIPOPT"

        compile NLOPT_NLPSolver    "-j1"
        compile pyNLOPT            "-j1"
        ;;

  *) echo Compile projects: [$*] 
     for PROJECT in $*
     do
       echo Compiling $PROJECT...
       if [ ${PROJECT} = "core" ]; then
         compile dae "-j$Ncpu"
       
       elif [ ${PROJECT} = "superlu" ]; then
         compile LA_SuperLU "-j1" "CONFIG+=shellCompile CONFIG+=shellSuperLU"
         compile pySuperLU  "-j1" "CONFIG+=shellCompile CONFIG+=shellSuperLU"

       elif [ ${PROJECT} = "superlu_mt" ]; then
         compile LA_SuperLU "-j1" "CONFIG+=shellCompile CONFIG+=shellSuperLU_MT"
         compile pySuperLU  "-j1" "CONFIG+=shellCompile CONFIG+=shellSuperLU_MT"

       elif [ ${PROJECT} = "superlu_cuda" ]; then
         compile LA_SUPERLU "-j1 --file=gpuMakefile" "CONFIG+=shellCompile CONFIG+=shellSuperLU_CUDA"
         compile pySuperLU  "-j1 --file=gpuMakefile" "CONFIG+=shellCompile CONFIG+=shellSuperLU_CUDA"

       elif [ ${PROJECT} = "cusp" ]; then
         compile LA_CUSP "-j1 --file=cudaMakefile"

       elif [ ${PROJECT} = "trilinos" ]; then
         compile LA_Trilinos_Amesos "-j1"
         compile pyTrilinos         "-j1"

       elif [ ${PROJECT} = "bonmin" ]; then
         compile BONMIN_MINLPSolver "-j1" "CONFIG+=shellCompile CONFIG+=shellBONMIN"
         compile pyBONMIN           "-j1" "CONFIG+=shellCompile CONFIG+=shellBONMIN"

       elif [ ${PROJECT} = "ipopt" ]; then
         compile BONMIN_MINLPSolver "-j1" "CONFIG+=shellCompile CONFIG+=shellIPOPT"
         compile pyBONMIN           "-j1" "CONFIG+=shellCompile CONFIG+=shellIPOPT"

       elif [ ${PROJECT} = "nlopt" ]; then
         compile NLOPT_NLPSolver "-j1"
         compile pyNLOPT         "-j1"

       else
         echo "Unrecognized project: ${PROJECT}"
       fi
     done
     ;;
esac

cd ${TRUNK}

