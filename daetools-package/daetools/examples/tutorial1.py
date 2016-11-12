#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                            tutorial1.py
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
************************************************************************************
"""
__doc__ = """
This tutorial introduces several new concepts:

- Distribution domains
- Distributed parameters, variables and equations
- Boundary and initial conditions

In this example we model a simple heat conduction problem: a conduction through
a very thin, rectangular copper plate.

This example should be sufficiently complex to describe all basic DAE Tools features.
For this problem, we need a two-dimensional Cartesian grid in X and Y axis
(here, for simplicity, divided into 10 x 10 segments):

.. code-block:: none

     Y axis
        ^
        |
    Ly -| T T T T T T T T T T T
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
     0 -| B B B B B B B B B B B
        --|-------------------|-------> X axis
          0                   Lx

Points 'B' at the bottom edge of the plate (for y = 0), and the points 'T' at the
top edge of the plate (for y = Ly) represent the points where the heat is applied.

The plate is considered insulated at the left (x = 0) and the right edges (x = Lx)
of the plate (points 'L' and 'R'). To model this type of problem, we have to write
a heat balance equation for all interior points except the left, right, top and
bottom edges, where we need to define the Neumann type boundary conditions.

In this problem we have to define the following domains:

- x: X axis domain, length Lx = 0.1 m
- y: Y axis domain, length Ly = 0.1 m

the following parameters:

- rho: copper density, 8960 kg/m3
- cp: copper specific heat capacity, 385 J/(kgK)
- k:  copper heat conductivity, 401 W/(mK)
- Qb: heat flux at the bottom edge of the plate, 1E6 W/m2 (or 100 W/cm2)
- Qt: heat flux at the top edge of the plate, here set to 0 W/m2

and the following variable:

- T: the temperature of the plate, K (distributed on x and y domains)

Also, we need to write the following 5 equations:

1) Heat balance::

      rho * cp * dT(x,y) / dt = k * (d2T(x,y) / dx2 + d2T(x,y) / dy2);  for all x in: (0, Lx),
                                                                       for all y in: (0, Ly)

2) Boundary conditions for the bottom edge::

      -k * dT(x,y) / dy = Qin;  for all x in: [0, Lx],
                                and y = 0

3) Boundary conditions for the top edge::

      -k * dT(x,y) / dy = Qin;  for all x in: [0, Lx],
                                and y = Ly

4) Boundary conditions for the left edge::

      dT(x,y) / dx = 0;  for all y in: (0, Ly),
                         and x = 0

5) Boundary conditions for the right edge::

      dT(x,y) / dx = 0;  for all y in: (0, Ly),
                         and x = Ln
"""

import sys
from time import localtime, strftime
from daetools.pyDAE import *

# Standard variable types are defined in variable_types.py
from pyUnits import m, kg, s, K, Pa, mol, J, W

class modTutorial(daeModel):
    def __init__(self, Name, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)

        # Distribution domain is a general term used to define an array of different objects (parameters, variables, etc).
        # daeDomain constructor accepts three arguments:
        #  - Name: string
        #  - Parent: daeModel object (indicating the model where the domain will be added)
        #  - Units: unit object (specifying the units, such as 'm' for we are modelling a space distribution)
        #  - Description: string (optional argument; the default value is an empty string)
        # All naming conventions (introduced in Whats_the_time example) apply here as well.
        # Again, domains have to be declared as members of the new model class (like all the other objects)
        self.x = daeDomain("x", self, m, "X axis domain")
        self.y = daeDomain("y", self, m, "Y axis domain")

        # Parameter can be defined as a time invariant quantity that will not change during a simulation.
        # daeParameter constructor accepts three arguments:
        #  - Name: string
        #  - Units: unit object (specifying the units, such as 'kg', 'W', 'm/s', ...)
        #  - Parent: daeModel object (indicating the model where the domain will be added)
        #  - Description: string (optional argument; the default value is an empty string)
        # All naming conventions (introduced in whats_the_time example) apply here as well.
        self.Qb = daeParameter("Q_b",         W/(m**2), self, "Heat flux at the bottom edge of the plate")
        self.Tt = daeParameter("T_t",                K, self, "Temperature at the top edge of the plate")
        self.rho = daeParameter("&rho;",      kg/(m**3), self, "Density of the plate")
        self.cp = daeParameter("c_p",         J/(kg*K), self, "Specific heat capacity of the plate")
        self.k  = daeParameter("&lambda;_p",   W/(m*K), self, "Thermal conductivity of the plate")
       
        # In this example we need a variable T which is distributed on the domains x and y. Variables (but also other objects)
        # can be distributed by using a function DistributeOnDomain, which accepts a domain object as
        # the argument (previously declared in the constructor).
        # Also a description of the object can be set by using the property Description.
        self.T = daeVariable("T", temperature_t, self)
        self.T.DistributeOnDomain(self.x)
        self.T.DistributeOnDomain(self.y)
        self.T.Description = "Temperature of the plate"

    def DeclareEquations(self):
        daeModel.DeclareEquations(self)

        # To distribute an equation on a domain the function DistributeOnDomain can be again used.
        # However, when distributing equations the function DistributeOnDomain takes an additional argument.
        # The second argument, DomainBounds, can be either of type daeeDomainBounds or a list of integers.
        # In the former case the DomainBounds argument is a flag defining a subset of the domain points.
        # There are several flags available:
        #  - eClosedClosed: Distribute on a closed domain - analogous to: x: [ LB, UB ]
        #  - eOpenClosed: Distribute on a left open domain - analogous to: x: ( LB, UB ]
        #  - eClosedOpen: Distribute on a right open domain - analogous to: x: [ LB, UB )
        #  - eOpenOpen: Distribute on a domain open on both sides - analogous to: x: ( LB, UB )
        #  - eLowerBound: Distribute on the lower bound - only one point: x = LB
        #    This option is useful for declaring boundary conditions.
        #  - eUpperBound: Distribute on the upper bound - only one point: x = UB
        #    This option is useful for declaring boundary conditions.
        # Also DomainBounds argument can be a list (an array) of points within a domain, for example: x: {0, 3, 4, 6, 8, 10}
        # Since our heat balance equation should exclude the top, bottom, left and right edges,
        # it is distributed on the open x and y domains, thus we use the eOpenOpen flag:
        eq = self.CreateEquation("HeatBalance", "Heat balance equation. Valid on open x and y domains")
        x = eq.DistributeOnDomain(self.x, eOpenOpen)
        y = eq.DistributeOnDomain(self.y, eOpenOpen)
        # Functions d() and d2() can be used to calculate partial derivatives of the 1st and 2nd order, respectively.
        # The arguments of those functions are:
        #   1. a variable or an expression for which the derivative is sought
        #   2. a domain for which we want a derivative
        #   3. discretisation method (optional; the default is the center-finite difference method of 2nd order, eCFDM)
        #   4. options (optional; a dictionary of options: {string:string}, the default is an empty dictionary)
        #      Currently, the options accepted include:
        #        - the DiscretizationOrder (the default is 2 fro eCFDM and 1 for the others)
        # In this example we use (the default) center-finite difference method (eCFDM) of 2nd order
        # to discretize the domains x and y. Other available methods are:
        #  - eBFDM: backward-finite difference method
        #  - eFFDM: forward-finite difference method
        #  - eUpwindCCFV: cell-centered finite-volume method with flux limiter
        eq.Residual = self.rho() * self.cp() * dt(self.T(x,y)) - \
                      self.k() * (d2(self.T(x,y), self.x, eCFDM) + d2(self.T(x,y), self.y, eCFDM))

        # Boundary conditions are treated as ordinary equations, and the special eLowerBound and eUpperBound flags
        # are used to define the position of the boundary.
        # The bottom edge is placed at y = 0 coordinate, thus we use eLowerBound for the y domain:
        eq = self.CreateEquation("BC_bottom", "Neumann boundary conditions at the bottom edge (sinusoidal inlet flux)")
        x = eq.DistributeOnDomain(self.x, eClosedClosed)
        y = eq.DistributeOnDomain(self.y, eLowerBound)
        eq.Residual = - self.k() * d(self.T(x,y), self.y, eCFDM) - self.Qb()

        # The top edge is placed at y = Ly coordinate, thus we use eUpperBound for the y domain:
        eq = self.CreateEquation("BC_top", "Dirichlet boundary conditions at the top edge (constant temperature)")
        x = eq.DistributeOnDomain(self.x, eClosedClosed)
        y = eq.DistributeOnDomain(self.y, eUpperBound)
        eq.Residual = self.T(x,y) - self.Tt()

        # The left edge is placed at x = 0 coordinate, thus we use eLowerBound for the x domain:
        eq = self.CreateEquation("BC_left", "Neumann boundary conditions at the left edge (insulated)")
        x = eq.DistributeOnDomain(self.x, eLowerBound)
        y = eq.DistributeOnDomain(self.y, eOpenOpen)
        eq.Residual = d(self.T(x,y), self.x, eCFDM)

        # The right edge is placed at x = Lx coordinate, thus we use eUpperBound for the x domain:
        eq = self.CreateEquation("BC_right", "Neumann boundary conditions for the right edge (insulated)")
        x = eq.DistributeOnDomain(self.x, eUpperBound)
        y = eq.DistributeOnDomain(self.y, eOpenOpen)
        eq.Residual = d(self.T(x,y), self.x, eCFDM)
        
class simTutorial(daeSimulation):
    def __init__(self):
        daeSimulation.__init__(self)
        self.m = modTutorial("tutorial1")
        self.m.Description = __doc__
        
    def SetUpParametersAndDomains(self):
        # The function CreateStructuredGrid can be used to create a structured grid. It accepts 3 arguments:
        #  - NoIntervals: integer
        #  - LowerBound:  float
        #  - UpperBound:  float
        # Here we create 2D structured gird with 10x10 intervals in x and y domains
        # (resulting in 11x11 points in both directions).
        self.m.x.CreateStructuredGrid(20, 0, 0.1)
        self.m.y.CreateStructuredGrid(20, 0, 0.1)

        # Parameter values can be set by using a function SetValue.
        self.m.k.SetValue(401 * W/(m*K))
        self.m.cp.SetValue(385 * J/(kg*K))
        self.m.rho.SetValue(8960 * kg/(m**3))
        self.m.Qb.SetValue(1.0e5 * W/(m**2))
        self.m.Tt.SetValue(300 * K)

    def SetUpVariables(self):
        # SetInitialCondition function in the case of distributed variables can accept additional arguments
        # specifying the indexes in the domains. In this example we loop over the open x and y domains,
        # thus we start the loop with 1 and end with NumberOfPoints-1 (for both domains)
        for x in range(1, self.m.x.NumberOfPoints - 1):
            for y in range(1, self.m.y.NumberOfPoints - 1):
                self.m.T.SetInitialCondition(x, y, 300 * K)

# Use daeSimulator class
def guiRun(app):
    sim = simTutorial()
    sim.m.SetReportingOn(True)
    sim.ReportingInterval = 10
    sim.TimeHorizon       = 1000
    simulator  = daeSimulator(app, simulation=sim)
    simulator.exec_()

# Setup everything manually and run in a console
def consoleRun():
    # Create Log, Solver, DataReporter and Simulation object
    log          = daePythonStdOutLog()
    daesolver    = daeIDAS()
    datareporter = daeTCPIPDataReporter()
    simulation   = simTutorial()

    # Enable reporting of all variables
    simulation.m.SetReportingOn(True)

    # Set the time horizon and the reporting interval
    simulation.ReportingInterval = 10
    simulation.TimeHorizon = 1000

    # Connect data reporter
    simName = simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
    if(datareporter.Connect("", simName) == False):
        sys.exit()

    # Initialize the simulation
    simulation.Initialize(daesolver, datareporter, log)

    # Save the model report and the runtime model report
    simulation.m.SaveModelReport(simulation.m.Name + ".xml")
    simulation.m.SaveRuntimeModelReport(simulation.m.Name + "-rt.xml")

    # Solve at time=0 (initialization)
    simulation.SolveInitial()

    # Run
    simulation.Run()

    simulation.Finalize()

if __name__ == "__main__":
    if len(sys.argv) > 1 and (sys.argv[1] == 'console'):
        consoleRun()
    else:
        from PyQt4 import QtCore, QtGui
        app = QtGui.QApplication(sys.argv)
        guiRun(app)
