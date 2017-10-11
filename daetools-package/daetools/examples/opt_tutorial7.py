#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                            opt_tutorial7.py
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
This tutorial introduces monitoring optimization progress.
"""

import sys
from time import localtime, strftime, sleep
from daetools.pyDAE import *
from daetools.solvers.ipopt import pyIPOPT
from daetools.dae_simulator.optimization_progress_monitor import daeOptimizationProgressMonitor

# Standard variable types are defined in variable_types.py

class modTutorial(daeModel):
    def __init__(self, Name, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)

        self.x1 = daeVariable("x1", no_t, self)
        self.x2 = daeVariable("x2", no_t, self)
        self.x3 = daeVariable("x3", no_t, self)
        self.x4 = daeVariable("x4", no_t, self)

        self.dummy = daeVariable("dummy", no_t, self, "A dummy variable to satisfy the condition that there should be at least one-state variable and one equation in a model")

    def DeclareEquations(self):
        daeModel.DeclareEquations(self)

        eq = self.CreateEquation("Dummy")
        eq.Residual = self.dummy()

class simTutorial(daeSimulation):
    def __init__(self):
        daeSimulation.__init__(self)
        self.m = modTutorial("opt_tutorial7")
        self.m.Description = __doc__

    def SetUpParametersAndDomains(self):
        pass

    def SetUpVariables(self):
        self.m.x1.AssignValue(1)
        self.m.x2.AssignValue(5)
        self.m.x3.AssignValue(5)
        self.m.x4.AssignValue(1)

    def SetUpOptimization(self):
        # Set the objective function (minimization).
        # The objective function can be accessed by using ObjectiveFunction property which always returns the 1st obj. function,
        # for in general case more than one obj. function. can be defined, so ObjectiveFunctions[0] can be used as well:
        #       self.ObjectiveFunctions[0].Residual = ...
        # Obviously defining more than one obj. function has no meaning when using opt. software such as Ipopt, Bonmin or Nlopt
        # which cannot do the multi-objective optimization. The number of obj. functions can be defined in the function
        # optimization.Initialize by using the named argument NumberOfObjectiveFunctions (the default is 1).
        # Other obj. functions can be obtained by using ObjectiveFunctions[i] property.
        self.ObjectiveFunction.Residual = self.m.x1() * self.m.x4() * (self.m.x1() + self.m.x2() + self.m.x3()) + self.m.x3()

        # Set the constraints (inequality, equality)
        # Constraints are in the following form:
        #  - Inequality: g(i) <= 0
        #  - Equality: h(i) = 0
        self.c1 = self.CreateInequalityConstraint("Constraint 1") # g(x) >= 25:  25 - x1*x2*x3*x4 <= 0
        self.c1.Residual = 25 - self.m.x1() * self.m.x2() * self.m.x3() * self.m.x4()

        self.c2 = self.CreateEqualityConstraint("Constraint 2") # h(x) == 40
        self.c2.Residual = self.m.x1() * self.m.x1() + self.m.x2() * self.m.x2() + self.m.x3() * self.m.x3() + self.m.x4() * self.m.x4() - 40

        # Set the optimization variables, their lower/upper bounds and the starting point
        # The optimization variables can be stored and used later to get the optimization results or
        # to interact with some 3rd party software.
        self.x1 = self.SetContinuousOptimizationVariable(self.m.x1, 1, 5, 2);
        self.x2 = self.SetContinuousOptimizationVariable(self.m.x2, 1, 5, 2);
        self.x3 = self.SetContinuousOptimizationVariable(self.m.x3, 1, 5, 2);
        self.x4 = self.SetContinuousOptimizationVariable(self.m.x4, 1, 5, 2);

class optTutorial(daeOptimization):
    def __init__(self, app):
        daeOptimization.__init__(self)

        self.app = app
        self.monitor = daeOptimizationProgressMonitor()
        self.monitor.show()

    def Initialize(self, simulation, nlpsolver, daesolver, datareporter, log):
        daeOptimization.Initialize(self, simulation, nlpsolver, daesolver, datareporter, log)

        opt_vars    = self.Simulation.OptimizationVariables
        constraints = self.Simulation.Constraints
        fobj        = self.Simulation.ObjectiveFunction

        n   = 3
        Nov = len(opt_vars)
        Nc  = len(constraints)
        m = max(Nov, Nc)

        self.f_plot   = None
        self.c_plots  = []
        self.ov_plots = []

        self.f_plot = self.monitor.addSubplot(n, m, 1, 'Fobj')

        for i, ov in enumerate(opt_vars):
            plot_no = m + 1 + i
            self.ov_plots.append( self.monitor.addSubplot(n, m, plot_no, ov.Name) )
        
        for i, c in enumerate(constraints):
            plot_no = 2*m + 1 + i
            self.c_plots.append( self.monitor.addSubplot(n, m, plot_no, c.Name) )

        try:
            # Some matplotlib versions do not support it
            self.monitor.figure.tight_layout()
        except Exception as e:
            pass
        
    def StartIterationRun(self, iteration):
        pass

    def EndIterationRun(self, iteration):
        opt_vars    = self.Simulation.OptimizationVariables
        constraints = self.Simulation.Constraints
        fobj        = self.Simulation.ObjectiveFunction

        subplot, line = self.f_plot
        self.monitor.addIteration(subplot, line, fobj.Value)

        for ov, (subplot, line) in zip(opt_vars, self.ov_plots):
            self.monitor.addIteration(subplot, line, ov.Value)

        for c, (subplot, line) in zip(constraints, self.c_plots):
            self.monitor.addIteration(subplot, line, c.Value)

        self.monitor.redraw()
        self.app.processEvents()
        sleep(0.5)

def setOptions(nlpsolver):
    # 1) Set the options manually
    try:
        nlpsolver.SetOption('print_level', 0)
        nlpsolver.SetOption('tol', 1e-7)
        nlpsolver.SetOption('mu_strategy', 'adaptive')

        # Print options loaded at pyIPOPT startup and the user set options:
        #nlpsolver.PrintOptions()
        #nlpsolver.PrintUserOptions()

        # ClearOptions can clear all options:
        #nlpsolver.ClearOptions()
    except Exception as e:
        print(str(e))

def run(**kwargs):
    qtApp = kwargs.get('qtApp', None)

    simulation   = simTutorial()
    optimization = optTutorial(qtApp)
    # Achtung! Achtung! NLP solver options can only be set after optimization.Initialize()
    # Otherwise seg. fault occurs for some reasons.
    nlpsolver    = pyIPOPT.daeIPOPT()
    return daeActivity.optimize(simulation, reportingInterval       = 1, 
                                            timeHorizon             = 1,
                                            optimization            = optimization,
                                            nlpsolver               = nlpsolver,
                                            nlpsolver_setoptions_fn = setOptions,
                                            reportSensitivities     = True,
                                            **kwargs)

if __name__ == "__main__":
    app = daeCreateQtApplication(sys.argv)
    guiRun_ = False if (len(sys.argv) > 1 and sys.argv[1] == 'console') else True
    run(guiRun = guiRun_, qtApp = app)
