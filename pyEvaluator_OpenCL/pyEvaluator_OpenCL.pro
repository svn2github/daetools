include(../dae.pri)
QT -= core
QT -= gui

TARGET = pyEvaluator_OpenCL
TEMPLATE = lib
CONFIG += shared

# Debugging options
#QMAKE_CFLAGS += -g -O0
#QMAKE_CXXFLAGS += -g -O0

INCLUDEPATH += $${BOOSTDIR} \
               $${PYTHON_INCLUDE_DIR} \
               $${PYTHON_SITE_PACKAGES_DIR} \
               $${OPEN_CS_INCLUDE}

QMAKE_LIBDIR += $${PYTHON_LIB_DIR}

LIBS += $${SOLIBS_RPATH}

unix::LIBS += $${OPEN_CS_LIBS} \
              $${INTEL_OPENCL_LIBS} \
              $${BOOST_PYTHON_LIB} \
              $${BOOST_LIBS}
# Important: quotes around $${NVIDIA_OPENCL_LIBS}
#            for in windows the path includes empty spaces.
win32-msvc2015::LIBS += "$${NVIDIA_OPENCL_LIBS}" \
                        $${BOOST_PYTHON_LIB} \
                        $${BOOST_LIBS}

SOURCES += stdafx.cpp \
           dllmain.cpp \
           dae_python.cpp \

HEADERS += stdafx.h

#######################################################
#                Install files
#######################################################
QMAKE_POST_LINK = $${COPY_FILE} \
                  $${DAE_DEST_DIR}/$${SHARED_LIB_PREFIX}$${TARGET}$${SHARED_LIB_POSTFIX}.$${SHARED_LIB_APPEND} \
                  $${PYDAE_DIR}/$${TARGET}.$${PYTHON_EXTENSION_MODULE_EXT}
