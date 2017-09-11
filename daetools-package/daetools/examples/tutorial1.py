#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                            tutorial1.py
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
This tutorial introduces several new concepts:

- Distribution domains
- Distributed parameters, variables and equations
- Setting boundary conditions (Neumann and Dirichlet type)
- Setting initial conditions

In this example we model a simple heat conduction problem: a conduction through
a very thin, rectangular copper plate.

For this problem, we need a two-dimensional Cartesian grid (x,y)
(here, for simplicity, divided into 10 x 10 segments):

.. code-block:: none

     y axis
        ^
        |
    Ly -| L T T T T T T T T T R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
        | L + + + + + + + + + R
     0 -| L B B B B B B B B B R
        --|-------------------|-------> x axis
          0                   Lx

Points 'B' at the bottom edge of the plate (for y = 0), and the points 'T' at the
top edge of the plate (for y = Ly) represent the points where the heat is applied.

The plate is considered insulated at the left (x = 0) and the right edges (x = Lx)
of the plate (points 'L' and 'R'). To model this type of problem, we have to write
a heat balance equation for all interior points except the left, right, top and
bottom edges, where we need to specify boundary conditions.

In this problem we have to define two distribution domains:

- x (x axis, length Lx = 0.1 m)
- y (y axis, length Ly = 0.1 m)

the following parameters:

- rho: copper density, 8960 kg/m3
- cp:  copper specific heat capacity, 385 J/(kgK)
- k:   copper heat conductivity, 401 W/(mK)
- Qb:  heat flux at the bottom edge, 1E6 W/m2 (or 100 W/cm2)
- Tt:  temperature at the top edge, 300 K

and a single variable:

- T: the temperature of the plate (distributed on x and y domains)

The model consists of 5 equations (1 distributed equation + 4 boundary conditions):

1) Heat balance::

    rho * cp * dT(x,y)/dt = k * [d2T(x,y)/dx2 + d2T(x,y)/dy2];  x in (0, Lx), y in (0, Ly)

2) Neumann boundary conditions at the bottom edge::

    -k * dT(x,y)/dy = Qb;  x in (0, Lx), y = 0

3) Dirichlet boundary conditions at the top edge::

    T(x,y) = Tt;  x in (0, Lx), y = Ly

4) Neumann boundary conditions at the left edge (insulated)::

    dT(x,y)/dx = 0;  y in [0, Ly], x = 0

5) Neumann boundary conditions at the right edge (insulated)::

    dT(x,y)/dx = 0;  y in [0, Ly], x = Lx

The temperature plot (at t=100s, x=0.5, y=*):

.. image:: _static/tutorial1-results.png
   :width: 500px
"""

import os, sys, tempfile
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
        self.Qb  = daeParameter("Q_b",         W/(m**2), self, "Heat flux at the bottom edge of the plate")
        self.Tt  = daeParameter("T_t",                K, self, "Temperature at the top edge of the plate")
        self.rho = daeParameter("&rho;",      kg/(m**3), self, "Density of the plate")
        self.cp  = daeParameter("c_p",         J/(kg*K), self, "Specific heat capacity of the plate")
        self.k   = daeParameter("&lambda;_p",   W/(m*K), self, "Thermal conductivity of the plate")
       
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
        eq = self.CreateEquation("HeatBalance", "Heat balance equation valid on open x and y domains")
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
        eq = self.CreateEquation("BC_bottom", "Neumann boundary conditions at the bottom edge (constant flux)")
        x = eq.DistributeOnDomain(self.x, eOpenOpen)
        y = eq.DistributeOnDomain(self.y, eLowerBound)
        eq.Residual = - self.k() * d(self.T(x,y), self.y, eCFDM) - self.Qb()

        # The top edge is placed at y = Ly coordinate, thus we use eUpperBound for the y domain:
        eq = self.CreateEquation("BC_top", "Dirichlet boundary conditions at the top edge (constant temperature)")
        x = eq.DistributeOnDomain(self.x, eOpenOpen)
        y = eq.DistributeOnDomain(self.y, eUpperBound)
        eq.Residual = self.T(x,y) - self.Tt()

        # The left edge is placed at x = 0 coordinate, thus we use eLowerBound for the x domain:
        eq = self.CreateEquation("BC_left", "Neumann boundary conditions at the left edge (insulated)")
        x = eq.DistributeOnDomain(self.x, eLowerBound)
        y = eq.DistributeOnDomain(self.y, eClosedClosed)
        eq.Residual = d(self.T(x,y), self.x, eCFDM)

        # The right edge is placed at x = Lx coordinate, thus we use eUpperBound for the x domain:
        eq = self.CreateEquation("BC_right", "Neumann boundary conditions at the right edge (insulated)")
        x = eq.DistributeOnDomain(self.x, eUpperBound)
        y = eq.DistributeOnDomain(self.y, eClosedClosed)
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

def run(**kwargs):
    simulation = simTutorial()
    return daeActivity.simulate(simulation, reportingInterval = 5, 
                                            timeHorizon       = 500,
                                            **kwargs)

if __name__ == "__main__":
    guiRun = False if (len(sys.argv) > 1 and sys.argv[1] == 'console') else True
    run(guiRun = guiRun)
    
