SET(CMAKE_SYSTEM_NAME Windows)

SET(COMPILER_PREFIX "x86_64-w64-mingw32")

find_program(CMAKE_RC_COMPILER NAMES ${COMPILER_PREFIX}-windres)
find_program(CMAKE_C_COMPILER NAMES ${COMPILER_PREFIX}-gcc)
find_program(CMAKE_CXX_COMPILER NAMES ${COMPILER_PREFIX}-g++)
find_program(CMAKE_Fortran_COMPILER NAMES ${COMPILER_PREFIX}-gfortran)

find_program(CMAKE_AR NAMES ${COMPILER_PREFIX}-ar)
find_program(CMAKE_NM NAMES ${COMPILER_PREFIX}-nm)
find_program(CMAKE_OBJDUMP NAMES ${COMPILER_PREFIX}-objdump)
find_program(CMAKE_OBJCOPY NAMES ${COMPILER_PREFIX}-objcopy)
find_program(CMAKE_RANLIB NAMES ${COMPILER_PREFIX}-ranlib)
find_program(CMAKE_STRIP NAMES ${COMPILER_PREFIX}-strip)
find_program(CMAKE_LINKER NAMES ${COMPILER_PREFIX}-ld)

SET(CMAKE_CXX_FLAGS     "-DNDEBUG ${DAE_COMPILER_FLAGS}")
SET(CMAKE_C_FLAGS       "-DNDEBUG ${DAE_COMPILER_FLAGS}")
SET(CMAKE_Fortran_FLAGS "-DNDEBUG ${DAE_COMPILER_FLAGS}")

SET(CMAKE_FIND_ROOT_PATH  /home/x86_64-w64-mingw32-gcc-4.6.3-2-linux-x86_64-rubenvb/${COMPILER_PREFIX})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
