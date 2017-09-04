#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                            tutorial10.py
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
This tutorial introduces the following concepts:

- Initialization files
- Domains which bounds depend on parameter values
- Evaluation of integrals

In this example we use the same conduction problem as in the tutorial 1.
"""

import os, sys
from time import localtime, strftime
from daetools.pyDAE import *

# Standard variable types are defined in variable_types.py
from pyUnits import m, kg, s, K, Pa, mol, J, W

class modTutorial(daeModel):
    def __init__(self, Name, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)

        self.x  = daeDomain("x", self, m, "X axis domain")
        self.y  = daeDomain("y", self, m, "Y axis domain")

        self.dx = daeParameter("&delta;_x", m, self, "Width of the plate")
        self.dy = daeParameter("&delta;_y", m, self, "Height of the plate")

        self.Qb  = daeParameter("Q_b",         W/(m**2), self, "Heat flux at the bottom edge of the plate")
        self.Tt  = daeParameter("T_t",                K, self, "Temperature at the top edge of the plate")
        self.rho = daeParameter("&rho;",      kg/(m**3), self, "Density of the plate")
        self.cp  = daeParameter("c_p",         J/(kg*K), self, "Specific heat capacity of the plate")
        self.k   = daeParameter("&lambda;_p",   W/(m*K), self, "Thermal conductivity of the plate")

        self.Q_int = daeVariable("Q_int", heat_flux_t, self, "The heat flux")

        self.T = daeVariable("T", temperature_t, self, "Temperature of the plate")
        self.T.DistributeOnDomain(self.x)
        self.T.DistributeOnDomain(self.y)

        # Data needed to calculate the area of a semi-circle
        self.c  = daeDomain("c", self, m, "Domain for a circle")
        self.semicircle = daeVariable("SemiCircle", length_t, self, "Semi-circle")
        self.semicircle.DistributeOnDomain(self.c)
        self.area = daeVariable("area", area_t, self, "Area of the semi-circle")

    def DeclareEquations(self):
        daeModel.DeclareEquations(self)

        # All equations are written so that they use only functions d() and dt()
        eq = self.CreateEquation("HeatBalance", "Heat balance equation. Valid on the open x and y domains")
        x = eq.DistributeOnDomain(self.x, eOpenOpen)
        y = eq.DistributeOnDomain(self.y, eOpenOpen)
        eq.Residual = dt( self.rho() * self.cp() * self.T(x, y) ) - \
                      self.k() * ( d ( d ( self.T(x, y), self.x ), self.x ) + \
                                   d ( d ( self.T(x, y), self.y ), self.y ) )

        eq = self.CreateEquation("BC_bottom", "Neumann boundary conditions at the bottom edge (constant flux)")
        x = eq.DistributeOnDomain(self.x, eOpenOpen)
        y = eq.DistributeOnDomain(self.y, eLowerBound)
        eq.Residual = - self.k() * d(self.T(x,y), self.y, eCFDM) - self.Qb()

        eq = self.CreateEquation("BC_top", "Dirichlet boundary conditions at the top edge (constant temperature)")
        x = eq.DistributeOnDomain(self.x, eOpenOpen)
        y = eq.DistributeOnDomain(self.y, eUpperBound)
        eq.Residual = self.T(x,y) - self.Tt()

        eq = self.CreateEquation("BC_left", "Neumann boundary conditions at the left edge (insulated)")
        x = eq.DistributeOnDomain(self.x, eLowerBound)
        y = eq.DistributeOnDomain(self.y, eClosedClosed)
        eq.Residual = d(self.T(x,y), self.x, eCFDM)

        eq = self.CreateEquation("BC_right", "Neumann boundary conditions at the right edge (insulated)")
        x = eq.DistributeOnDomain(self.x, eUpperBound)
        y = eq.DistributeOnDomain(self.y, eClosedClosed)
        eq.Residual = d(self.T(x,y), self.x, eCFDM)

        # Here we have a function that calculates integral of heat fluxes at y = 0: integral(-k * (dT/dy) / dx)|y=0
        # The result should be 1E6 W/m2, which is equal to the input flux
        eq = self.CreateEquation("Q_int", "Integral of the heat flux per x domain; just an example of the integral function")
        eq.Residual = self.Q_int() - Integral( -self.k() * self.T.d_array(self.y, '*', 0) / self.dx() )

        # To check the integral function we can create a semi-circle around domain c with 100 intervals and bounds: -1 to +1
        # The variable semicircle will be a cirle with the radius 1 and coordinates: (0,0)
        # The semi-circle area is equal to: 0.5 * integral(circle * dc) = 1.56913425555
        # which is not exactly equal to r*r*pi/2 = 1.570795 because of the discretization
        eq = self.CreateEquation("Semicircle", "Semi-circle around domain c")
        c = eq.DistributeOnDomain(self.c, eClosedClosed)
        eq.Residual = self.semicircle(c) - Sqrt( Constant(1.0 * m**2) - c()**2 )

        eq = self.CreateEquation("Area", "Area of the semi-circle")
        eq.Residual = self.area() - Integral( self.semicircle.array('*') )

class simTutorial(daeSimulation):
    def __init__(self):
        daeSimulation.__init__(self)
        self.m = modTutorial("tutorial10")
        self.m.Description = __doc__

    def SetUpParametersAndDomains(self):
        # The domain for a semi-circle
        self.m.c.CreateStructuredGrid(100, -1, 1)

        self.m.dx.SetValue(0.1 * m)
        self.m.dy.SetValue(0.1 * m)

        # Domain bounds can depend on input parameters:
        self.m.x.CreateStructuredGrid(10, 0, self.m.dx.GetValue())
        self.m.y.CreateStructuredGrid(10, 0, self.m.dy.GetValue())

        self.m.k.SetValue(401 * W/(m*K))
        self.m.cp.SetValue(385 * J/(kg*K))
        self.m.rho.SetValue(8960 * kg/(m**3))
        self.m.Qb.SetValue(1e6 * W/(m**2))
        self.m.Tt.SetValue(300 * K)

    def SetUpVariables(self):
        self.m.T.SetInitialGuesses(250*K)
        for x in range(1, self.m.x.NumberOfPoints - 1):
            for y in range(1, self.m.y.NumberOfPoints - 1):
                self.m.T.SetInitialCondition(x, y, 300 * K)

def loadInitFile(simulation, log):
    # Load initialization file previously saved after the successful initialization phase.
    # The function LoadInitializationValues requires a call to daeSimulation.Reinitialize, 
    # however here the system has not been solved initially (at t = 0) and the values and  
    # initial conditions will be copied automatically to DAE solver during SolveInitial call.
    log.Message('T before loading:', 0)
    log.Message(str(simulation.m.T.npyValues), 0)
    init_file = os.path.join(os.path.realpath(os.path.dirname(__file__)), 'tutorial10.init')
    simulation.LoadInitializationValues(init_file)
    log.Message('T after loading:', 0)
    log.Message(str(simulation.m.T.npyValues), 0)
    
def saveInitFile(simulation, log):
    log.Message('T after SolveInitial:', 0)
    log.Message(str(simulation.m.T.npyValues), 0)
    
    # Save the initialization file that can be used during later initialization
    init_file = os.path.join(os.path.realpath(os.path.dirname(__file__)), 'tutorial10.init')
    simulation.StoreInitializationValues(init_file)

def run(**kwargs):
    simulation = simTutorial()
    return daeActivity.simulate(simulation, reportingInterval            = 10, 
                                            timeHorizon                  = 500,
                                            run_after_simulation_init_fn = loadInitFile,
                                            run_before_simulation_fn     = saveInitFile,
                                            **kwargs)

if __name__ == "__main__":
    guiRun = False if (len(sys.argv) > 1 and sys.argv[1] == 'console') else True
    run(guiRun = guiRun)
