#!/usr/bin/env python

import os, sys
from subprocess import Popen
try:
    from about import AboutDialog
except ImportError, e:
    print 'Cannot load about module', str(e)

try:
    import numpy
except ImportError, e:
    print '[daePlotter]: Cannot load numpy module', str(e)

try:
    from PyQt4 import QtCore, QtGui
except ImportError, e:
    print '[daePlotter]: Cannot load pyQt4 modules', str(e)

try:
    from daetools.pyDAE import *
except ImportError, e:
    print '[daePlotter]: Cannot load pyDAE module', str(e)

try:
    from dae2DPlot import dae2DPlot
except ImportError, e:
    print '[daePlotter]: Cannot load dae2DPlot module', str(e)

try:
    from dae3DPlot import dae3DPlot
except ImportError, e:
    print '[daePlotter]: Cannot load dae3DPlot module', str(e)

try:
    from daeMayavi3DPlot import daeMayavi3DPlot
except ImportError, e:
    print '[daePlotter]: Cannot load daeMayavi3DPlot module', str(e)

class daeMainWindow(QtGui.QMainWindow):
    def __init__(self, tcpipServer):
        QtGui.QMainWindow.__init__(self)
        
        self.tcpipServer = tcpipServer

        self.move(0, 0)
        self.resize(400, 200)
        self.setWindowIcon(QtGui.QIcon('images/app.png'))
        self.setWindowTitle('DAE Plotter')

        exit = QtGui.QAction(QtGui.QIcon('images/close.png'), 'Exit', self)
        exit.setShortcut('Ctrl+Q')
        exit.setStatusTip('Exit application')
        self.connect(exit, QtCore.SIGNAL('triggered()'), self.close)

        plot2D = QtGui.QAction(QtGui.QIcon('images/add-2d.png'), 'New 2D plot', self)
        plot2D.setShortcut('Ctrl+2')
        plot2D.setStatusTip('New 2D plot')
        self.connect(plot2D, QtCore.SIGNAL('triggered()'), self.slotPlot2D)

        plot3D = QtGui.QAction(QtGui.QIcon('images/add-3d.png'), 'New 3D plot', self)
        plot3D.setShortcut('Ctrl+3')
        plot3D.setStatusTip('New 3D plot')
        self.connect(plot3D, QtCore.SIGNAL('triggered()'), self.slotPlot3D)

        about = QtGui.QAction('About', self)
        about.setStatusTip('About')
        self.connect(about, QtCore.SIGNAL('triggered()'), self.slotAbout)

        self.statusBar()

        menubar = self.menuBar()
        file = menubar.addMenu('&File')
        file.addAction(exit)

        plot = menubar.addMenu('&Plot')
        plot.addAction(plot2D)
        plot.addAction(plot3D)
        
        help = menubar.addMenu('&Help')
        help.addAction(about)

        self.toolbar = self.addToolBar('Main toolbar')

        self.toolbar.addAction(plot2D)
        self.toolbar.addAction(plot3D)
        self.toolbar.addAction(plotMayavi3D)

    #@QtCore.pyqtSlot()
    def slotPlot2D(self):
        plot2d = dae2DPlot(self, self.tcpipServer)
        if plot2d.newCurve() == False:
            plot2d.close()
            del plot2d
        else:
            plot2d.show()
        
    #@QtCore.pyqtSlot()
    def slotPlot3D(self):
        plot3d = daeMayavi3DPlot(self.tcpipServer)
        if plot3d.newSurface() == False:
            del plot3d

    #@QtCore.pyqtSlot()
    def slotAbout(self):
        about = AboutDialog(self)
        about.exec_()
    
def daeStartPlotter(port = 0):
    try:
        if port == 0:
            cfg = daeGetConfig()
            port = cfg.GetInteger("daetools.datareporting.tcpipDataReceiverPort", 50000)

        tcpipServer = daeTCPIPDataReceiverServer(port)
        tcpipServer.Start()
        if(tcpipServer.IsConnected() == False):
            return

        app = QtGui.QApplication(sys.argv)
        main = daeMainWindow(tcpipServer)
        main.show()
        app.exec_()
        tcpipServer.Stop()
    
    except RuntimeError:
        return
    
if __name__ == "__main__":
    daeStartPlotter()