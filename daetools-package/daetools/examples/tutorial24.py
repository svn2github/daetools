#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                           tutorial24.py
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
This tutorial illustrates the code verification method using the Method of Manufactured 
Solutions:

References: 

1. G. Tryggvason. Method of Manufactured Solutions, Lecture 33: Predictivity-I, 2011.
   `PDF link <http://www3.nd.edu/~gtryggva/CFD-Course/2011-Lecture-33.pdf>`_
2. K. Salari and P. Knupp. Code Verification by the Method of Manufactured Solutions. 
   SAND2000 – 1444 (2000).
   `doi:10.2172/759450 <https://doi.org/10.2172/759450>`_
3. P.J. Roache. Fundamentals of Verification and Validation. Hermosa, 2009.
   `ISBN-10:0913478121 <http://www.isbnsearch.org/isbn/0913478121>`_

The procedure for the *transient convection-diffusion equation*:
    
.. code-block:: none

   L(f) = df/dt + f*df/dx - D*d2f/dx2 = 0

is the following:
    
1. Pick a function (q, the analytical solution): 
    
   .. code-block:: none
    
      q = A + sin(x + Ct)

2. Compute the new source term (g) for the original problem:
    
   .. code-block:: none
      
      g = dq/dt + q*dq/dx - D*d2q/dx2

3. Solve the original problem with the new source term:
    
   .. code-block:: none

      df/dt + f*df/dx - D*d2f/dx2 = g

Since L(f) = g and g = L(q), consequently we have: f = q.
Therefore, the computed numerical solution (f) should be equal to the analytical one (q).

The terms in the source g term are:

.. code-block:: none

   dq/dt   = C * cos(x + C*t)
   dq/dx   = cos(x + C*t)
   d2q/dx2 = -sin(x + C*t)

The model in this example is very similar to the model used in the tutorial 2.

The solution plot (t = 1.0 s):

.. image:: _static/tutorial24-results.png
   :width: 500px
"""

import sys, numpy
from time import localtime, strftime
from daetools.pyDAE import *

no_t = daeVariableType("no_t", dimless, -1.0e+20, 1.0e+20, 0.0, 1e-6)

class modTutorial(daeModel):
    def __init__(self, Name, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)

        self.x  = daeDomain("x", self, m, "X axis domain")

        self.A = 1.0
        self.C = 1.0
        self.D = 0.05

        self.f = daeVariable("f", no_t, self, "", [self.x])
        self.q = daeVariable("q", no_t, self, "", [self.x])

    def DeclareEquations(self):
        daeModel.DeclareEquations(self)

        # Create some auxiliary functions to make equations more readable 
        A       = self.A
        C       = self.C
        D       = self.D
        t       = Time()
        f       = lambda x:    self.f(x)
        df_dt   = lambda x: dt(self.f(x))
        df_dx   = lambda x:  d(self.f(x), self.x, eCFDM)
        d2f_dx2 = lambda x: d2(self.f(x), self.x, eCFDM)
        q       = lambda x: A + numpy.sin(x() + C*t)
        dq_dt   = lambda x: C * numpy.cos(x() + C*t)
        dq_dx   = lambda x: numpy.cos(x() + C*t)
        d2q_dx2 = lambda x: -numpy.sin(x() + C*t)
        g       = lambda x: dq_dt(x) + q(x) * dq_dx(x) - D * d2q_dx2(x)

        # Numerical solution
        eq = self.CreateEquation("f", "Numerical solution")
        x = eq.DistributeOnDomain(self.x, eOpenOpen)
        eq.Residual = df_dt(x) + f(x) * df_dx(x) - D * d2f_dx2(x) - g(x)
        eq.CheckUnitsConsistency = False

        eq = self.CreateEquation("f(0)", "Numerical solution")
        x = eq.DistributeOnDomain(self.x, eLowerBound)
        eq.Residual = f(x) - q(x)
        eq.CheckUnitsConsistency = False

        eq = self.CreateEquation("f(2pi)", "Numerical solution")
        x = eq.DistributeOnDomain(self.x, eUpperBound)
        eq.Residual = f(x) - q(x)
        eq.CheckUnitsConsistency = False

        # Analytical solution
        eq = self.CreateEquation("q", "Analytical solution")
        x = eq.DistributeOnDomain(self.x, eClosedClosed)
        eq.Residual = self.q(x) - q(x)
        eq.CheckUnitsConsistency = False

class simTutorial(daeSimulation):
    def __init__(self, Nx):
        daeSimulation.__init__(self)
        self.m = modTutorial("tutorial24")
        self.m.Description = __doc__
        
        self.Nx = Nx

    def SetUpParametersAndDomains(self):
        self.m.x.CreateStructuredGrid(self.Nx, 0, 2*numpy.pi)
        
    def SetUpVariables(self):
        Nx = self.m.x.NumberOfPoints
        xp = self.m.x.Points
        for x in range(1, Nx-1):
            self.m.f.SetInitialCondition(x, self.m.A + numpy.sin(xp[x]))
                
# Setup everything manually and run in a console
def run(Nx):
    # Create Log, Solver, DataReporter and Simulation object
    log          = daePythonStdOutLog()
    daesolver    = daeIDAS()
    datareporter = daeDelegateDataReporter()
    simulation   = simTutorial(Nx)

    # Enable reporting of all variables
    simulation.m.SetReportingOn(True)

    # Enable reporting of time derivatives for all reported variables
    simulation.ReportTimeDerivatives = True

    # Set the time horizon and the reporting interval
    simulation.ReportingInterval = 0.05
    simulation.TimeHorizon = 1

    # Connect data reporter
    simName = simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())

    # 1. TCP/IP
    tcpipDataReporter = daeTCPIPDataReporter()
    datareporter.AddDataReporter(tcpipDataReporter)
    if not tcpipDataReporter.Connect("", simName):
        sys.exit()

    # 2. Data
    dr = daeNoOpDataReporter()
    datareporter.AddDataReporter(dr)

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
    
    ###########################################
    #  Data                                   #
    ###########################################
    results = dr.Process.dictVariables
    fvar = results[simulation.m.Name + '.f']
    qvar = results[simulation.m.Name + '.q']
    times = fvar.TimeValues
    q = qvar.Values[-1, :] # 2D array [t,x]
    f = fvar.Values[-1, :] # 2D array [t,x]
    #print(times,f,q)
    
    return times,f,q

if __name__ == "__main__":
    Nx1 = 60
    Nx2 = 120
    L = 2*numpy.pi
    h1 = L / Nx1
    h2 = L / Nx2
    times1, f1, q1 = run(Nx1)
    times2, f2, q2 = run(Nx2)
    
    # The normalized global errors
    E1 = numpy.sqrt((1.0/Nx1) * numpy.sum((f1-q1)**2))
    E2 = numpy.sqrt((1.0/Nx2) * numpy.sum((f2-q2)**2))

    # Order of accuracy
    p = numpy.log(E1/E2) / numpy.log(h1/h2)
    C = E1 / h1**p
    
    print('\n\nOrder of Accuracy:')
    print('||E(h)|| is proportional to: C * (h**p)')
    print('||E(h1)|| = %e, ||E(h2)|| = %e' % (E1, E2))
    print('C = %e' % C)
    print('Order of accuracy (p) = %.2f' % p)
