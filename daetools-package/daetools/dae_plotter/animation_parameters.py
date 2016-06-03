"""********************************************************************************
                            animation_params.py
                 DAE Tools: pyDAE module, www.daetools.com
                 Copyright (C) Dragan Nikolic, 2016
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
from os.path import join, realpath, dirname
import numpy
from PyQt4 import QtCore, QtGui
from daetools.pyDAE import *
from .animation_params_ui import Ui_AnimationParameters

images_dir = join(dirname(__file__), 'images')

class daeAnimationParameters(QtGui.QDialog):
    def __init__(self):
        QtGui.QDialog.__init__(self)
        self.ui = Ui_AnimationParameters()
        self.ui.setupUi(self)

        self.setWindowIcon(QtGui.QIcon(join(images_dir, 'daetools-48x48.png')))

        self.ui.buttonBox.accepted.connect(self.accept)
        self.ui.buttonBox.rejected.connect(self.reject)

