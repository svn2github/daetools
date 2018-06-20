#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                            tutorial12.py
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
This tutorial describes the use and available options for superLU direct linear solvers:

- Sequential: superLU
- Multithreaded (OpenMP/posix threads): superLU_MT

The model is the same as the model in tutorial 1, except for the different boundary conditions.

The temperature plot (at t=100s, x=0.5, y=*):

.. image:: _static/tutorial12-results.png
   :width: 500px
"""

import sys
from time import localtime, strftime
from daetools.pyDAE import *
from daetools.solvers.superlu import pySuperLU as superlu
#from daetools.solvers.superlu_mt import pySuperLU_MT as superlu

# Standard variable types are defined in variable_types.py
from pyUnits import m, kg, s, K, Pa, mol, J, W

class modTutorial(daeModel):
    def __init__(self, Name, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)

        self.x  = daeDomain("x", self, m, "X axis domain")
        self.y  = daeDomain("y", self, m, "Y axis domain")

        self.Qb  = daeParameter("Q_b",         W/(m**2), self, "Heat flux at the bottom edge of the plate")
        self.Qt  = daeParameter("Q_t",         W/(m**2), self, "Heat flux at the top edge of the plate")
        self.rho = daeParameter("&rho;",      kg/(m**3), self, "Density of the plate")
        self.cp  = daeParameter("c_p",         J/(kg*K), self, "Specific heat capacity of the plate")
        self.k   = daeParameter("&lambda;_p",   W/(m*K), self, "Thermal conductivity of the plate")

        self.T = daeVariable("T", temperature_t, self, "Temperature of the plate")
        self.T.DistributeOnDomain(self.x)
        self.T.DistributeOnDomain(self.y)

    def DeclareEquations(self):
        daeModel.DeclareEquations(self)

        eq = self.CreateEquation("HeatBalance", "Heat balance equation valid on the open x and y domains")
        x = eq.DistributeOnDomain(self.x, eOpenOpen)
        y = eq.DistributeOnDomain(self.y, eOpenOpen)
        eq.Residual = self.rho() * self.cp() * dt(self.T(x,y)) - self.k() * \
                      (d2(self.T(x,y), self.x) + d2(self.T(x,y), self.y))

        eq = self.CreateEquation("BC_bottom", "Neumann boundary conditions at the bottom edge (constant flux)")
        x = eq.DistributeOnDomain(self.x, eOpenOpen)
        y = eq.DistributeOnDomain(self.y, eLowerBound)
        eq.Residual = - self.k() * d(self.T(x,y), self.y) - self.Qb()

        eq = self.CreateEquation("BC_top", "Neumann boundary conditions at the top edge (constant flux)")
        x = eq.DistributeOnDomain(self.x, eOpenOpen)
        y = eq.DistributeOnDomain(self.y, eUpperBound)
        eq.Residual = - self.k() * d(self.T(x,y), self.y) - self.Qt()

        eq = self.CreateEquation("BC_left", "Neumann boundary conditions at the left edge (insulated)")
        x = eq.DistributeOnDomain(self.x, eLowerBound)
        y = eq.DistributeOnDomain(self.y, eClosedClosed)
        eq.Residual = d(self.T(x,y), self.x)

        eq = self.CreateEquation("BC_right", "Neumann boundary conditions at the right edge (insulated)")
        x = eq.DistributeOnDomain(self.x, eUpperBound)
        y = eq.DistributeOnDomain(self.y, eClosedClosed)
        eq.Residual = d(self.T(x,y), self.x)

class simTutorial(daeSimulation):
    def __init__(self):
        daeSimulation.__init__(self)
        self.m = modTutorial("tutorial12")
        self.m.Description = __doc__

    def SetUpParametersAndDomains(self):
        self.m.x.CreateStructuredGrid(20, 0, 0.1)
        self.m.y.CreateStructuredGrid(20, 0, 0.1)

        self.m.k.SetValue(401 * W/(m*K))
        self.m.cp.SetValue(385 * J/(kg*K))
        self.m.rho.SetValue(8960 * kg/(m**3))
        self.m.Qb.SetValue(1e6 * W/(m**2))
        self.m.Qt.SetValue(0 * W/(m**2))

    def SetUpVariables(self):
        for x in range(1, self.m.x.NumberOfPoints - 1):
            for y in range(1, self.m.y.NumberOfPoints - 1):
                self.m.T.SetInitialCondition(x, y, 300 * K)

def createLASolver():
    lasolver = superlu.daeCreateSuperLUSolver()
    options  = lasolver.Options

    if lasolver.Name == 'SuperLU':
        # SuperLU options:
        options.ColPerm         = superlu.COLAMD    # {NATURAL, MMD_ATA, MMD_AT_PLUS_A, COLAMD}
        #options.RowPerm         = superlu.NOROWPERM # {NOROWPERM, LargeDiag}
        #options.PrintStat       = superlu.YES       # {YES, NO}
        #options.DiagPivotThresh = 1.0               # Between 0.0 and 1.0
        #options.Equil           = superlu.NO        # {YES, NO}
        #options.PivotGrowth     = superlu.NO        # {YES, NO}
        #options.ConditionNumber = superlu.NO        # {YES, NO}

    elif lasolver.Name == 'SuperLU_MT':
        # SuperLU_MT options:
        # Leave nprocs to default (specified in daetools.cfg) or specify it manually
        #options.nprocs           = 4                 # No. of threads (= no. of CPUs/Cores)
        print('SuperLU_MT numThreads = %d' % options.nprocs)
        #options.PrintStat         = superlu.YES       # {YES, NO}
        #options.ColPerm           = superlu.COLAMD    # {NATURAL, MMD_ATA, MMD_AT_PLUS_A, COLAMD}
        #options.diag_pivot_thresh = 1.0               # Between 0.0 and 1.0
        #options.panel_size        = 8                 # Integer value
        #options.relax             = 6                 # Integer value
        #options.drop_tol          = 0.0               # Floating point value

    return lasolver

def run(**kwargs):
    simulation = simTutorial()
    lasolver = createLASolver()
    return daeActivity.simulate(simulation, reportingInterval = 10, 
                                            timeHorizon       = 1000,
                                            lasolver          = lasolver,
                                            **kwargs)

if __name__ == "__main__":
    guiRun = False if (len(sys.argv) > 1 and sys.argv[1] == 'console') else True
    run(guiRun = guiRun)
