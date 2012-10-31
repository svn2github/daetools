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
QT -= core gui
TARGET = daetools_ansic_simulation
TEMPLATE = app

unix::QMAKE_CFLAGS += -ansi -pedantic
unix::QMAKE_CXXFLAGS += -ansi -pedantic

INCLUDEPATH += 
QMAKE_LIBDIR += 

LIBS += -lsundials_idas -lsundials_nvecserial -lblas -llapack

SOURCES += auxiliary.c adouble.c main.c
HEADERS += adouble.h typedefs.h auxiliary.h daetools_model.h
