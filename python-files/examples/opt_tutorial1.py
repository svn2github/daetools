#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""********************************************************************************
                             opt_tutorial1.py
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

"""
"""

import sys
from daetools.pyDAE import *
from time import localtime, strftime

typeNone = daeVariableType("None", "-",  -1E20, 1E20,   1, 1e-6)

class modTutorial(daeModel):
    def __init__(self, Name, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)
        
        self.x1 = daeVariable("x1", typeNone, self)
        self.x2 = daeVariable("x2", typeNone, self)
        self.x3 = daeVariable("x3", typeNone, self)
        self.x4 = daeVariable("x4", typeNone, self)
        
        self.dummy = daeVariable("dummy", typeNone, self, "A dummy variable to satisfy the condition that there should be at least one-state variable and one equation in a model")

    def DeclareEquations(self):
        eq = self.CreateEquation("HeatBalance", "Heat balance equation. Valid on the open x and y domains")
        eq.Residual = self.dummy()

class simTutorial(daeSimulation):
    def __init__(self):
        daeSimulation.__init__(self)
        self.m = modTutorial("opt_tutorial1")
        self.m.Description = "This tutorial explains how to define a simple optimization problem. Here we use the steady-state test HS-71 ()"
          
    def SetUpParametersAndDomains(self):
        pass
    
    def SetUpVariables(self):
        self.m.x1.AssignValue(1)
        self.m.x2.AssignValue(5)
        self.m.x3.AssignValue(5)
        self.m.x4.AssignValue(1)
        
    def SetUpOptimization(self):
        # Set the objective function (min)
        self.ObjectiveFunction.Residual = self.m.x1() * self.m.x4() * (self.m.x1() + self.m.x2() + self.m.x3()) + self.m.x3()
        
        # Set the constraints (inequality, equality)
        c1 = self.CreateInequalityConstraint(25, 2E19, "Constraint 1")
        c1.Residual = self.m.x1() * self.m.x2() * self.m.x3() * self.m.x4()
        
        c2 = self.CreateEqualityConstraint(40, "Constraint 2")
        c2.Residual = self.m.x1() * self.m.x1() + self.m.x2() * self.m.x2() + self.m.x3() * self.m.x3() + self.m.x4() * self.m.x4()
                
        # Set the optimization variables and their lower and upper bounds
        self.SetContinuousOptimizationVariable(self.m.x1, 1, 5, 1);
        self.SetContinuousOptimizationVariable(self.m.x2, 1, 5, 5);
        self.SetContinuousOptimizationVariable(self.m.x3, 1, 5, 5);
        self.SetContinuousOptimizationVariable(self.m.x4, 1, 5, 1);

# Use daeSimulator class
def guiRun(app):
    sim = simTutorial()
    opt = daeOptimization()
    nlp = daeBONMIN()
    sim.m.SetReportingOn(True)
    sim.ReportingInterval = 1
    sim.TimeHorizon       = 5
    simulator = daeSimulator(app, simulation=sim, optimization=opt, nlpsolver=nlp)
    simulator.exec_()

# Setup everything manually and run in a console
def consoleRun():
    # Create Log, Solver, DataReporter and Simulation object
    log          = daePythonStdOutLog()
    daesolver    = daeIDAS()
    nlpsolver    = daeBONMIN()
    datareporter = daeTCPIPDataReporter()
    simulation   = simTutorial()
    optimization = daeOptimization()

    # Enable reporting of all variables
    simulation.m.SetReportingOn(True)

    # Set the time horizon and the reporting interval
    simulation.ReportingInterval = 1
    simulation.TimeHorizon = 5

    # Connect data reporter
    simName = simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
    if(datareporter.Connect("", simName) == False):
        sys.exit()

    # Initialize the simulation
    optimization.Initialize(simulation, nlpsolver, daesolver, datareporter, log)

    nlpsolver.SetOption('print_level', 5)
    nlpsolver.SetOption('hessian_approximation', 'limited-memory')
    nlpsolver.SetOption('tol', 1e-7)
    nlpsolver.SetOption('linear_solver', 'mumps')
    nlpsolver.SetOption('mu_strategy', 'adaptive')

    # Save the model report and the runtime model report 
    simulation.m.SaveModelReport(simulation.m.Name + ".xml")
    simulation.m.SaveRuntimeModelReport(simulation.m.Name + "-rt.xml")

    # Run
    optimization.Run()
    optimization.Finalize()
    
if __name__ == "__main__":
    runInGUI = True
    if len(sys.argv) > 1:
        if(sys.argv[1] == 'console'):
            runInGUI = False
    if runInGUI:
        from PyQt4 import QtCore, QtGui
        app = QtGui.QApplication(sys.argv)
        guiRun(app)
    else:
        consoleRun()
