"""********************************************************************************
                             save_video.py
                 DAE Tools: pyDAE module, www.daetools.com
                 Copyright (C) Dragan Nikolic
***********************************************************************************
DAE Tools is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License version 3 as published by the Free Software
Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with the
DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
********************************************************************************"""
import sys, numpy
from os.path import join, realpath, dirname
from PyQt5 import QtCore, QtGui, QtWidgets
from daetools.pyDAE import *
from .save_video_ui import Ui_SaveVideo

images_dir = join(dirname(__file__), 'images')

class daeSavePlot2DVideo(QtWidgets.QDialog):
    def __init__(self):
        QtWidgets.QDialog.__init__(self)
        self.ui = Ui_SaveVideo()
        self.ui.setupUi(self)

        self.setWindowIcon(QtGui.QIcon(join(images_dir, 'daetools-48x48.png')))

        self.ui.buttonBox.accepted.connect(self.accept)
        self.ui.buttonBox.rejected.connect(self.reject)
        self.ui.buttonFilename.clicked.connect(self.slotOpenFilename)

    def slotOpenFilename(self):
        filename, ok = QtWidgets.QFileDialog.getSaveFileName(self, "Save video/sequence of images", self.ui.lineeditFilename.text(),
                                                            "Videos (*.avi *.flv *.mp4 *.mpg *.ogv *.webm *.wmv);;Images (*.png *.jpeg *.tiff *.bmp);;All Files (*)")
        if ok:
            self.ui.lineeditFilename.setText(filename)

