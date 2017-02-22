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
TARGET = daetools_simulation
TEMPLATE = app

unix::QMAKE_CFLAGS_RELEASE   -= -O2
unix::QMAKE_CFLAGS           += -std=c99 -pedantic -O0
unix::QMAKE_CFLAGS_WARN_ON    = -Wall -Wextra \
                                -Wno-unused-parameter \
                                -Wno-unused-variable \
                                -Wno-unused-but-set-variable

INCLUDEPATH += 
QMAKE_LIBDIR += 

LIBS += -lsundials_idas -lsundials_nvecserial -lblas -llapack

SOURCES += auxiliary.c adouble.c daesolver.c simulation.c model.c main.c
HEADERS += typedefs.h auxiliary.h adouble.h daesolver.h simulation.h model.h
