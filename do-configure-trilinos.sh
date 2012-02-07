#!/bin/sh

BUILD=`pwd`
TRUNK=`cd ${BUILD}/../..; pwd`
export TRILINOS_HOME=${TRUNK}/trilinos
EXTRA_ARGS=$@

SSE_FLAGS="-mfpmath=sse"
SSE_TAGS=`grep -m 1 flags /proc/cpuinfo | grep -o 'sse\|sse2\|sse3\|ssse3\|sse4a\|sse4.1\|sse4.2\|sse5'`
for SSE_TAG in ${SSE_TAGS}
do
  SSE_FLAGS="${SSE_FLAGS} -m${SSE_TAG}"
done

echo $BUILD
echo $TRUNK
echo $TRILINOS_HOME
echo $SSE_FLAGS

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
  -DTPL_UMFPACK_INCLUDE_DIRS:FILEPATH=/usr/include/suitesparse \
  -DTPL_ENABLE_MPI:BOOL=OFF \
  -DDART_TESTING_TIMEOUT:STRING=600 \
  -DCMAKE_INSTALL_PREFIX:PATH=. \
  -DCMAKE_CXX_FLAGS:STRING="-DNDEBUG -fPIC ${SSE_FLAGS}" \
  -DCMAKE_C_FLAGS:STRING="-DNDEBUG -fPIC ${SSE_FLAGS}" \
  -DCMAKE_Fortran_FLAGS:STRING="-DNDEBUG -fPIC ${SSE_FLAGS}" \
  $EXTRA_ARGS \
  ${TRILINOS_HOME}
 