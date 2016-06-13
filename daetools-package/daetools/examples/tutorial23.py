#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                            tutorial23.py
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
This tutorial illustrates the C++ MPI code generator.
The model is identical to the model from the tutorial11.
"""

import sys, numpy, itertools
from time import localtime, strftime
from daetools.pyDAE import *

# Standard variable types are defined in variable_types.py
from pyUnits import m, kg, s, K, Pa, mol, J, W

#from daetools.solvers.superlu import pySuperLU
from daetools.solvers.trilinos import pyTrilinos
from daetools.solvers.aztecoo_options import daeAztecOptions

# The linear solver used is iterative (GMRES); therefore decrease the abs.tol.
temperature_t.AbsoluteTolerance = 1e-2

class modTutorial(daeModel):
    def __init__(self, Name, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)

        self.x = daeDomain("x", self, m, "X axis domain")
        self.y = daeDomain("y", self, m, "Y axis domain")

        self.Qb = daeParameter("Q_b",         W/(m**2), self, "Heat flux at the bottom edge of the plate")
        self.Qt = daeParameter("Q_t",         W/(m**2), self, "Heat flux at the top edge of the plate")
        self.ro = daeParameter("&rho;",      kg/(m**3), self, "Density of the plate")
        self.cp = daeParameter("c_p",         J/(kg*K), self, "Specific heat capacity of the plate")
        self.k  = daeParameter("&lambda;_p",   W/(m*K), self, "Thermal conductivity of the plate")

        self.T = daeVariable("T", temperature_t, self)
        self.T.DistributeOnDomain(self.x)
        self.T.DistributeOnDomain(self.y)
        self.T.Description = "Temperature of the plate"

    def DeclareEquations(self):
        daeModel.DeclareEquations(self)

        # For readibility, get the adouble objects from parameters/variables
        # and create numpy arrays for T and its derivatives in tim and space
        # This will also save a lot of memory (no duplicate adouble objects in equations)
        Nx = self.x.NumberOfPoints
        Ny = self.y.NumberOfPoints
        ro = self.ro()
        cp = self.cp()
        k  = self.k()
        Qb = self.Qb()
        Qt = self.Qt()

        T      = numpy.empty((Nx,Ny), dtype=object)
        dTdt   = numpy.empty((Nx,Ny), dtype=object)
        dTdx   = numpy.empty((Nx,Ny), dtype=object)
        dTdy   = numpy.empty((Nx,Ny), dtype=object)
        d2Tdx2 = numpy.empty((Nx,Ny), dtype=object)
        d2Tdy2 = numpy.empty((Nx,Ny), dtype=object)
        for x in range(Nx):
            for y in range(Ny):
                T[x,y]      = self.T(x,y)
                dTdt[x,y]   = dt(self.T(x,y))
                dTdx[x,y]   = d (self.T(x,y), self.x, eCFDM)
                dTdy[x,y]   = d (self.T(x,y), self.y, eCFDM)
                d2Tdx2[x,y] = d2(self.T(x,y), self.x, eCFDM)
                d2Tdy2[x,y] = d2(self.T(x,y), self.y, eCFDM)

        # Get the flat list of indexes
        indexes = [(x,y) for x,y in itertools.product(range(Nx), range(Ny))]
        eq_types = numpy.empty((Nx,Ny), dtype=object)
        eq_types[ : , : ] = 'in' # inner region
        eq_types[  0, : ] = 'l'  # left boundary
        eq_types[ -1, : ] = 'r'  # right boundary
        eq_types[ : ,  0] = 'b'  # bottom boundary
        eq_types[ : , -1] = 't'  # top boundary
        print(eq_types)
        for x,y in indexes:
            eq_type = eq_types[x,y]
            #print x,y,eq_type
            eq = self.CreateEquation("HeatBalance", "")
            if eq_type == 'in':
                eq.Residual = ro*cp*dTdt[x,y] - k*(d2Tdx2[x,y] + d2Tdy2[x,y])

            elif eq_type == 'l':
                eq.Residual = dTdx[x,y]

            elif eq_type == 'r':
                eq.Residual = dTdx[x,y]

            elif eq_type == 't':
                eq.Residual = -k*dTdy[x,y] - Qt

            elif eq_type == 'b':
                eq.Residual = -k*dTdy[x,y] - Qb

            else:
                raise RuntimeError('Invalid equation type: %s' % eq_type)

class simTutorial(daeSimulation):
    def __init__(self):
        daeSimulation.__init__(self)
        self.m = modTutorial("tutorial23")
        self.m.Description = __doc__

    def SetUpParametersAndDomains(self):
        self.m.x.CreateStructuredGrid(39, 0, 0.1)
        self.m.y.CreateStructuredGrid(39, 0, 0.1)

        self.m.k.SetValue(401 * W/(m*K))
        self.m.cp.SetValue(385 * J/(kg*K))
        self.m.ro.SetValue(8960 * kg/(m**3))
        self.m.Qb.SetValue(1e6 * W/(m**2))
        self.m.Qt.SetValue(0 * W/(m**2))

    def SetUpVariables(self):
        for x in range(1, self.m.x.NumberOfPoints - 1):
            for y in range(1, self.m.y.NumberOfPoints - 1):
                self.m.T.SetInitialCondition(x, y, 300 * K)

    def DoDataPartitioning(self, equationsOverallIndexes, mapOverallBlockIndexes):
        #for kv in equationsOverallIndexes.OverallIndexes_Equations:
        #    print [v for v in kv.data()]
        #print [str(key_value) for key_value in mapOverallBlockIndexes]
        pass

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

    lasolver = pyTrilinos.daeCreateTrilinosSolver("AztecOO", "")
    #lasolver = pySuperLU.daeCreateSuperLUSolver()
    daesolver.SetLASolver(lasolver)
    daesolver.RelativeTolerance = 1e-3

    # Enable reporting of all variables
    simulation.m.SetReportingOn(True)

    # Set the time horizon and the reporting interval
    simulation.ReportingInterval = 10
    simulation.TimeHorizon = 1000

    # Connect data reporter
    simName = simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
    if(datareporter.Connect("", simName) == False):
        sys.exit()

    paramListAztec = lasolver.AztecOOOptions
    lasolver.NumIters  = 1000
    lasolver.Tolerance = 1e-3
    paramListAztec.set_int("AZ_solver",    daeAztecOptions.AZ_gmres)
    paramListAztec.set_int("AZ_kspace",    500)
    paramListAztec.set_int("AZ_scaling",   daeAztecOptions.AZ_none)
    paramListAztec.set_int("AZ_reorder",   0)
    paramListAztec.set_int("AZ_conv",      daeAztecOptions.AZ_r0)
    paramListAztec.set_int("AZ_keep_info", 1)
    paramListAztec.set_int("AZ_output",    daeAztecOptions.AZ_none) # {AZ_all, AZ_none, AZ_last, AZ_summary, AZ_warnings}

    paramListAztec.set_int("AZ_precond",         daeAztecOptions.AZ_Jacobi)
    #paramListAztec.set_int("AZ_subdomain_solve", daeAztecOptions.AZ_ilut)
    #paramListAztec.set_int("AZ_overlap",         daeAztecOptions.AZ_none)
    #paramListAztec.set_int("AZ_graph_fill",      1)

    paramListAztec.Print()

    # Initialize the simulation
    simulation.Initialize(daesolver, datareporter, log)

    # Save the model report and the runtime model report
    simulation.m.SaveModelReport(simulation.m.Name + ".xml")
    simulation.m.SaveRuntimeModelReport(simulation.m.Name + "-rt.xml")

    # Solve at time=0 (initialization)
    simulation.SolveInitial()

    #fileName = '/home/ciroki/' + simulation.m.Name + '.xpm'
    #lasolver.SaveAsXPM(str(fileName))

    # Generate code
    from daetools.code_generators.cxx_mpi import daeCodeGenerator_cxx_mpi
    cg = daeCodeGenerator_cxx_mpi()
    cg.generateSimulation(simulation, '/home/ciroki/Data/daetools/trunk/code_gen_tests/cxx-tutorial23-old', 4)
    
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
