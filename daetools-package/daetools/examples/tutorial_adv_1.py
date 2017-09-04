#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                            tutorial_adv_1.py
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
************************************************************************************
"""
__doc__ = """
This tutorial presents a user-defined simulation which instead of simply integrating
the system shows the pyQt graphical user interface (GUI) where the simulation can be
manipulated (a sort of interactive operating procedure).

The model in this example is the same as in the tutorial 4.

The simulation.Run() function is modifed to show the graphical user interface (GUI)
that allows to specify the input power of the heater (degree of freedom), a time period
for integration, and a reporting interval. The GUI also contains the temperature plot
updated in real time, as the simulation progresses.

The screenshot of the pyQt GUI:

.. image:: _static/tutorial_adv_1-screenshot.png
   :width: 500px
"""

import sys
from time import localtime, strftime, sleep
from os.path import join, realpath, dirname
from PyQt5 import QtCore, QtGui, QtWidgets
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from daetools.pyDAE import *
try:
    from .tutorial_adv_1_ui import Ui_InteractiveRunDialog
except Exception as e:
    from tutorial_adv_1_ui import Ui_InteractiveRunDialog

# Standard variable types are defined in variable_types.py
from pyUnits import m, kg, s, K, Pa, mol, J, W, kW

class modTutorial(daeModel):
    def __init__(self, Name, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)

        self.m     = daeParameter("m",       kg,           self, "Mass of the copper plate")
        self.cp    = daeParameter("c_p",     J/(kg*K),     self, "Specific heat capacity of the plate")
        self.alpha = daeParameter("&alpha;", W/((m**2)*K), self, "Heat transfer coefficient")
        self.A     = daeParameter("A",       m**2,         self, "Area of the plate")
        self.Tsurr = daeParameter("T_surr",  K,            self, "Temperature of the surroundings")

        self.Qin  = daeVariable("Q_in",  power_t,       self, "Power of the heater")
        self.T    = daeVariable("T",     temperature_t, self, "Temperature of the plate")

    def DeclareEquations(self):
        daeModel.DeclareEquations(self)

        eq = self.CreateEquation("HeatBalance", "Integral heat balance equation")
        eq.Residual = self.m() * self.cp() * dt(self.T()) - self.Qin() + self.alpha() * self.A() * (self.T() - self.Tsurr())

class simTutorial(daeSimulation):
    def __init__(self):
        daeSimulation.__init__(self)
        self.m = modTutorial("tutorial_adv_1")
        self.m.Description = __doc__

    def SetUpParametersAndDomains(self):
        self.m.cp.SetValue(385 * J/(kg*K))
        self.m.m.SetValue(1 * kg)
        self.m.alpha.SetValue(200 * W/((m**2)*K))
        self.m.A.SetValue(0.1 * m**2)
        self.m.Tsurr.SetValue(283 * K)

    def SetUpVariables(self):
        self.m.Qin.AssignValue(1500 * W)
        self.m.T.SetInitialCondition(283 * K)

    def Run(self):
        opDlg = InteractiveOP(self)
        opDlg.exec_()
        
class InteractiveOP(QtWidgets.QDialog):
    def __init__(self, simulation):
        QtWidgets.QDialog.__init__(self)

        self.ui = Ui_InteractiveRunDialog()
        self.ui.setupUi(self)

        self.setWindowIcon(QtGui.QIcon(join(dirname(__file__), 'daetools-48x48.png')))
        self.setWindowTitle("Advanced Tutorial 1 - An Interactive Schedule")

        self.simulation = simulation
        self.ui.powerSpinBox.setValue(self.simulation.m.Qin.GetValue())
        self.ui.reportingIntervalSpinBox.setValue(self.simulation.ReportingInterval)

        self.figure = Figure((5.0, 4.0), dpi=100, facecolor='white')
        self.canvas = FigureCanvas(self.figure)
        self.canvas.setParent(self.ui.frame)
        self.canvas.axes = self.figure.add_subplot(111)

        # Add an empty curve
        self.line, = self.canvas.axes.plot([], [])

        self.fp9  = matplotlib.font_manager.FontProperties(family='sans-serif', style='normal', variant='normal', weight='normal', size=9)
        self.fp10 = matplotlib.font_manager.FontProperties(family='sans-serif', style='normal', variant='normal', weight='bold', size=10)

        self.canvas.axes.set_xlabel('Time, s',        fontproperties=self.fp10)
        self.canvas.axes.set_ylabel('Temperature, K', fontproperties=self.fp10)

        for xlabel in self.canvas.axes.get_xticklabels():
            xlabel.set_fontproperties(self.fp9)
        for ylabel in self.canvas.axes.get_yticklabels():
            ylabel.set_fontproperties(self.fp9)

        self.canvas.axes.grid(True)

        self.ui.runButton.clicked.connect(self.integrate)

    def integrate(self):
        try:
            # Get the data from the GUI
            Qin      = float(self.ui.powerSpinBox.value())
            interval = float(self.ui.intervalSpinBox.value())
            self.simulation.ReportingInterval = float(self.ui.reportingIntervalSpinBox.value())
            self.simulation.TimeHorizon       = self.simulation.CurrentTime + interval

            if self.simulation.ReportingInterval > interval:
                QtWidgets.QMessageBox.warning(self, "tutorial_adv_1", 'Reporting interval must be lower than the integration interval')
                return

            # Disable the input boxes and buttons
            self.ui.powerSpinBox.setEnabled(False)
            self.ui.intervalSpinBox.setEnabled(False)
            self.ui.reportingIntervalSpinBox.setEnabled(False)
            self.ui.runButton.setEnabled(False)

            # Reassign the new Qin value, reinitialize the simulation and report the data
            self.simulation.m.Qin.ReAssignValue(Qin)
            self.simulation.Reinitialize()
            self.simulation.ReportData(self.simulation.CurrentTime)

            # Integrate for ReportingInterval until the TimeHorizon is reached
            # After each integration call update the plot with the new data
            # Sleep for 0.1 seconds after each integration to give some real-time impression
            time = self.simulation.IntegrateForTimeInterval(self.simulation.ReportingInterval, eDoNotStopAtDiscontinuity)
            self.simulation.ReportData(self.simulation.CurrentTime)
            self._updatePlot()
            sleep(0.1)
            
            while time < self.simulation.TimeHorizon:
                if time + self.simulation.ReportingInterval > self.simulation.TimeHorizon:
                    interval = self.simulation.TimeHorizon - time
                else:
                    interval = self.simulation.ReportingInterval

                time = self.simulation.IntegrateForTimeInterval(interval, eDoNotStopAtDiscontinuity)
                self.simulation.ReportData(self.simulation.CurrentTime)
                self._updatePlot()
                sleep(0.1)

        except Exception as e:
            QtWidgets.QMessageBox.warning(self, "tutorial_adv_1", 'Error: %s' % str(e))
            
        finally:
            # Enable the input boxes and buttons again
            self.ui.powerSpinBox.setEnabled(True)
            self.ui.intervalSpinBox.setEnabled(True)
            self.ui.reportingIntervalSpinBox.setEnabled(True)
            self.ui.runButton.setEnabled(True)

    def _updatePlot(self):
        dr = self.simulation.DataReporter
        temperature = dr.dictVariables['tutorial_adv_1.T']
        x = temperature.TimeValues
        y = temperature.Values
        self.line.set_xdata(x)
        self.line.set_ydata(y)
        self.canvas.axes.relim()
        self.canvas.axes.autoscale_view()
        self.canvas.draw()
        self.ui.currentTimeEdit.setText(str(self.simulation.CurrentTime) + ' s')
        QtWidgets.QApplication.processEvents()

def run(**kwargs):
    simulation = simTutorial()
    datareporter = daeDataReporterLocal()
    return daeActivity.simulate(simulation, reportingInterval = 1, 
                                            timeHorizon       = 1000,
                                            datareporter      = datareporter,
                                            **kwargs)

if __name__ == "__main__":
    qtApp  = QtWidgets.QApplication(sys.argv)
    guiRun = False if (len(sys.argv) > 1 and sys.argv[1] == 'console') else True
    run(guiRun = guiRun, qtApp = qtApp)
