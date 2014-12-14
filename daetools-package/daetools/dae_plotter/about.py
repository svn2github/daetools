"""********************************************************************************
                             about.py
                 DAE Tools: pyDAE module, www.daetools.com
                 Copyright (C) Dragan Nikolic, 2014
***********************************************************************************
DAE Tools is free software; you can redistribute it and/or modify it under the 
terms of the GNU General Public License version 3 as published by the Free Software 
Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with the
DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
********************************************************************************"""
import sys
from PyQt4 import QtCore, QtGui
from daetools.pyDAE import *

python_major = sys.version_info[0]
from .about_ui import Ui_About

class daeAboutDialog(QtGui.QDialog):
    def __init__(self, process):
        QtGui.QDialog.__init__(self)
        self.ui = Ui_About()
        self.ui.setupUi(self)
        self.setWindowTitle("DAE Tools Project v" + daeVersion(True))
