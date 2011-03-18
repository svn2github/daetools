"""********************************************************************************
                             daeSimulator.py
                 DAE Tools: pyDAE module, www.daetools.com
                 Copyright (C) Dragan Nikolic, 2010
***********************************************************************************
DAE Tools is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License version 3 as published by the Free Software
Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with the
DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
********************************************************************************"""
import os, platform, sys, tempfile
from daetools.pyDAE import *
from time import localtime, strftime
from PyQt4 import QtCore, QtGui
from Simulator_ui import Ui_SimulatorDialog
from daetools.pyDAE.WebViewDialog import WebView

class daeTextEditLog(daeBaseLog):
    def __init__(self, TextEdit, App):
        daeBaseLog.__init__(self)
        self.TextEdit = TextEdit
        self.App      = App

    def Message(self, message, severity):
        self.TextEdit.append(self.IndentString + message)
        if self.TextEdit.isVisible() == True:
            self.TextEdit.update()
        self.App.processEvents()

class daeSimulator(QtGui.QDialog):
    def __init__(self, app, **kwargs):
        QtGui.QDialog.__init__(self)
        self.ui = Ui_SimulatorDialog()
        self.ui.setupUi(self)

        font = QtGui.QFont()
        font.setPointSize(9)
        if platform.system() == 'Linux':
            font.setFamily("Monospace")
        else:
            font.setFamily("Courier New")
        self.ui.textEdit.setFont(font)
        self.setWindowTitle("DAE Tools Simulator v" + daeVersion(True))

        self.connect(self.ui.RunButton,    QtCore.SIGNAL('clicked()'), self.slotRun)
        self.connect(self.ui.ResumeButton, QtCore.SIGNAL('clicked()'), self.slotResume)
        self.connect(self.ui.PauseButton,  QtCore.SIGNAL('clicked()'), self.slotPause)
        self.connect(self.ui.MatrixButton, QtCore.SIGNAL('clicked()'), self.slotOpenSparseMatrixImage)
        self.connect(self.ui.ExportButton, QtCore.SIGNAL('clicked()'), self.slotExportSparseMatrixAsMatrixMarketFormat)

        self.app          = app
        self.simulation   = kwargs.get('simulation',   None)
        self.optimization = kwargs.get('optimization', None)
        self.datareporter = kwargs.get('datareporter', None)
        self.log          = kwargs.get('log',          None)
        self.daesolver    = kwargs.get('daesolver',    None)
        self.lasolver     = kwargs.get('lasolver',     None)
        self.nlpsolver    = kwargs.get('nlpsolver',    None)

        if self.app == None:
            raise RuntimeError('daeSimulator: app object must not be None')
        if self.simulation == None:
            raise RuntimeError('daeSimulator: simulation object must not be None')

        if self.lasolver == None:
            self.ui.LASolverComboBox.setEnabled(True)
        else:
            self.ui.LASolverComboBox.setEnabled(False)

        if self.optimization == None:
            self.ui.simulationLabel.setText('Simulation')
            self.ui.MINLPSolverComboBox.setEnabled(False)
        else:
            self.ui.simulationLabel.setText('Optimization')
            self.ui.MINLPSolverComboBox.setEnabled(True)
        self.ui.SimulationLineEdit.insert(self.simulation.m.Name)

        self.ui.ReportingIntervalDoubleSpinBox.setValue(self.simulation.ReportingInterval)
        self.ui.TimeHorizonDoubleSpinBox.setValue(self.simulation.TimeHorizon)

        cfg = daeGetConfig()
        tcpip = cfg.GetString("daetools.datareporting.tcpipDataReceiverAddress", "127.0.0.1")
        port  = cfg.GetInteger("daetools.datareporting.tcpipDataReceiverPort", 50000)
        self.ui.DataReporterTCPIPAddressLineEdit.setText( tcpip + ':' + str(port) )

    #@QtCore.pyqtSlot()
    def slotResume(self):
        self.simulation.Resume()

    #@QtCore.pyqtSlot()
    def slotPause(self):
        self.simulation.Pause()

    #@QtCore.pyqtSlot()
    def slotRun(self):
        try:
            self.ui.textEdit.clear()

            tcpipaddress                      = str(self.ui.DataReporterTCPIPAddressLineEdit.text())
            self.simulation.ReportingInterval = float(self.ui.ReportingIntervalDoubleSpinBox.value())
            self.simulation.TimeHorizon       = float(self.ui.TimeHorizonDoubleSpinBox.value())

            if self.datareporter == None:
                self.datareporter = daeTCPIPDataReporter()
                simName = self.simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
                if(self.datareporter.Connect(str(tcpipaddress), simName) == False):
                    QtGui.QMessageBox.warning(None, "DAE Tools Simulator", "Cannot connect data reporter!\nDid you forget to start daePlotter?")
                    raise RuntimeError("Cannot connect daeTCPIPDataReporter")
                    return
            if self.log == None:
                self.log = daeTextEditLog(self.ui.textEdit, self.app)
            if self.daesolver == None:
                self.daesolver = daeIDAS()

            self.lasolver = None
            lasolverIndex = self.ui.LASolverComboBox.currentIndex()
            if lasolverIndex == 0:
                pass

            elif lasolverIndex == 1:
                try:
                    import daetools.pyTrilinos as pyTrilinos
                    self.lasolver = pyTrilinos.daeCreateTrilinosSolver("Amesos_Klu", "")
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create TrilinosAmesos LA solver\nError: " + str(e))
                    return

            elif lasolverIndex == 2:
                try:
                    import daetools.pyTrilinos as pyTrilinos
                    self.lasolver = pyTrilinos.daeCreateTrilinosSolver("Amesos_Superlu", "")
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create TrilinosAmesos LA solver\nError: " + str(e))
                    return

            elif lasolverIndex == 3:
                try:
                    import daetools.pyTrilinos as pyTrilinos
                    self.lasolver = pyTrilinos.daeCreateTrilinosSolver("Amesos_Umfpack", "")
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create TrilinosAmesos LA solver\nError: " + str(e))
                    return

            elif lasolverIndex == 4:
                try:
                    import daetools.pyTrilinos as pyTrilinos
                    self.lasolver = pyTrilinos.daeCreateTrilinosSolver("Amesos_Lapack", "")
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create TrilinosAmesos LA solver\nError: " + str(e))
                    return

            elif lasolverIndex == 5:
                try:
                    import daetools.pyTrilinos as pyTrilinos
                    self.lasolver = pyTrilinos.daeCreateTrilinosSolver("AztecOO", "ILUT")
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create TrilinosAmesos LA solver\nError: " + str(e))
                    return

            elif lasolverIndex == 6:
                try:
                    import daetools.pyIntelPardiso as pyIntelPardiso
                    self.lasolver = pyIntelPardiso.daeCreateIntelPardisoSolver()
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create IntelPardiso LA solver\nError: " + str(e))
                    return

            elif lasolverIndex == 7:
                try:
                    import daetools.pyIntelMKL as pyIntelMKL
                    self.lasolver = pyIntelMKL.daeCreateLapackSolver()
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create IntelMKL LA solver\nError: " + str(e))
                    return

            elif lasolverIndex == 8:
                try:
                    import daetools.pyAmdACML as pyAmdACML
                    self.lasolver = pyAmdACML.daeCreateLapackSolver()
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create AmdACML LA solver\nError: " + str(e))
                    return

            elif lasolverIndex == 9:
                try:
                    import daetools.pyMagma as pyMagma
                    self.lasolver = pyMagma.daeCreateLapackSolver()
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create Lapack LA solver\nError: " + str(e))
                    return

            elif lasolverIndex == 10:
                try:
                    from daetools.pySuperLU import pySuperLU
                    self.lasolver = pySuperLU.daeCreateSuperLUSolver()
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create SuperLU LA solver\nError: " + str(e))
                    return

            elif lasolverIndex == 11:
                try:
                    from daetools.pySuperLU import pySuperLU_MT
                    self.lasolver = pySuperLU_MT.daeCreateSuperLUSolver()
                    self.daesolver.SetLASolver(self.lasolver)
                except Exception, e:
                    QtGui.QMessageBox.warning(None, "daeSimulator", "Cannot create SuperLU_MT LA solver\nError: " + str(e))
                    return

            else:
                raise RuntimeError("Unsupported LA Solver selected")

            self.ui.RunButton.setEnabled(False)
            if(lasolverIndex in [1, 2, 3, 5, 6, 10, 11]):
                self.ui.MatrixButton.setEnabled(True)
            if(lasolverIndex in [1, 2, 3, 5, 10, 11]):
                self.ui.ExportButton.setEnabled(True)
            self.ui.MINLPSolverComboBox.setEnabled(False)
            self.ui.DAESolverComboBox.setEnabled(False)
            self.ui.LASolverComboBox.setEnabled(False)
            self.ui.DataReporterTCPIPAddressLineEdit.setEnabled(False)
            self.ui.ReportingIntervalDoubleSpinBox.setEnabled(False)
            self.ui.TimeHorizonDoubleSpinBox.setEnabled(False)

            if self.optimization == None:
                self.simulation.Initialize(self.daesolver, self.datareporter, self.log)
                self.simulation.SolveInitial()
                self.simulation.Run()
                self.simulation.Finalize()
            else:
                if self.nlpsolver == None:
                    self.nlpsolver = daeBONMIN()
                self.optimization.Initialize(self.simulation, self.nlpsolver, self.daesolver, self.datareporter, self.log)
                self.optimization.Run()
                self.optimization.Finalize()

        except Exception, error:
            self.ui.textEdit.append(str(error))
            if self.ui.textEdit.isVisible() == True:
                self.ui.textEdit.update()
            self.app.processEvents()

    #@QtCore.pyqtSlot()
    def slotOpenSparseMatrixImage(self):
        if(self.lasolver != None):
            tmpdir = tempfile.gettempdir() + os.sep
            matName = tmpdir + self.simulation.m.Name + ".xpm"
            self.lasolver.SaveAsXPM(matName)
            url = QtCore.QUrl(matName)
            wv = WebView(url)
            wv.resize(400, 400)
            wv.setWindowTitle("Sparse matrix: " + matName)
            wv.exec_()

    #@QtCore.pyqtSlot()
    def slotExportSparseMatrixAsMatrixMarketFormat(self):
        if(self.lasolver != None):
            fileName = QtGui.QFileDialog.getSaveFileName(self, "Save File", self.simulation.m.Name +".mtx", "Matrix Market Format Files (*.mtx)")
            if(str(fileName) != ""):
                self.lasolver.SaveAsMatrixMarketFile(str(fileName), self.simulation.m.Name + " matrix", self.simulation.m.Name + " description")
