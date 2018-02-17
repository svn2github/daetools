#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                           tutorial_cv_9.py
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
Code verification using the Method of Exact Solutions (Solid Body Rotation problem).

Reference (section 4.4.6.1 Solid Body Rotation):
    
- D. Kuzmin (2010). A Guide to Numerical Methods for Transport Equations. 
  `PDF <http://www.mathematik.uni-dortmund.de/~kuzmin/Transport.pdf>`_

Solid body rotation illustrates the ability of a numerical scheme to transport initial
data without distortion. Here, a 2D transient convection problem in a rectangular 
(0,1)x(0,1) domain is solved using the FE method:

.. code-block:: none

   dc/dt + div(u*c) = 0, in Omega = (0,1)x(0,1)

The initial conditions define a conical body which is rotated counterclockwise around
the point (0.5, 0.5) using the velocity field u = (0.5 - y, x - 0.5). 
The cone is defined by the following equation:
    
.. code-block:: none
   
   r0 = 0.15
   (x0, y0) = (0.5, 0.25)
   c(x,y,0) = 1 - (1/r0) * sqrt((x-x0)**2 + (y-y0)**2)
    
After t = 2pi the body should arrive at the starting position.

Homogeneous Dirichlet boundary conditions are prescribed at all four edges:

.. code-block:: none

   c(x,y,t) = 0.0
   
The mesh is a square (0,1)x(0,1):

.. image:: _static/square(0,1)x(0,1)-64x64.png
   :width: 300 px

The solution plot at t = 0 and t = 2pi (96x96 grid): 

.. image:: _static/tutorial_cv_9-results1.png
   :height: 400 px

.. image:: _static/tutorial_cv_9-results2.png
   :height: 400 px

Animations for 32x32 and 96x96 grids:
    
.. image:: _static/tutorial_cv_9-animation-32x32.gif
   :height: 400 px

.. image:: _static/tutorial_cv_9-animation-96x96.gif
   :height: 400 px

It can be observed that the shape of the cone is preserved. Also, since the above
equation is hyperbolic some oscillations in the solution out of the cone appear,
which are more pronounced for coarser grids.
This problem was resolved in the original example using the flux linearisation technique.

The normalised global errors and the order of accuracy plots 
(no. elements = [32x32, 64x64, 96x96, 128x128], t = 2pi):

.. image:: _static/tutorial_cv_9-results3.png
   :width: 800 px
"""

import os, sys, numpy, json, tempfile
from time import localtime, strftime
import matplotlib.pyplot as plt
from daetools.pyDAE import *
from daetools.solvers.deal_II import *
from daetools.solvers.superlu import pySuperLU

# Standard variable types are defined in variable_types.py
from pyUnits import m, kg, s, K, Pa, mol, J, W

class VelocityFunction_2D(Function_2D):
    def __init__(self, n_components = 1):
        Function_2D.__init__(self, n_components)
        self.m_velocity = Tensor_1_2D()

    def gradient(self, point, component = 0):
        self.m_velocity[0] = 0.5 - point.y
        self.m_velocity[1] = point.x - 0.5
        return self.m_velocity

    def vector_gradient(self, point):
        return [self.value(point, c) for c in range(self.n_components)]

c_t = daeVariableType("c_t", unit(),  0.0, 1E20, 0, 1e-07)

class modTutorial(daeModel):
    def __init__(self, Name, Nx, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)

        dofs = [dealiiFiniteElementDOF_2D(name='c',
                                          description='Something',
                                          fe = FE_Q_2D(1),
                                          multiplicity=1,
                                          variableType=c_t)]
        self.n_components = int(numpy.sum([dof.Multiplicity for dof in dofs]))

        meshes_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'meshes')
        mesh_file  = os.path.join(meshes_dir, 'square(0,1)x(0,1)-%dx%d.msh' % (Nx, Nx))

        # Store the object so it does not go out of scope while still in use by daetools
        self.fe_system = dealiiFiniteElementSystem_2D(meshFilename    = mesh_file,
                                                      quadrature      = QGauss_2D(3),
                                                      faceQuadrature  = QGauss_1D(3),
                                                      dofs            = dofs)

        self.fe_model = daeFiniteElementModel('SolidBodyRotation', self, 'Solid Body Rotation problem', self.fe_system)

    def DeclareEquations(self):
        daeModel.DeclareEquations(self)

        # Create some auxiliary objects for readability
        phi_i  =  phi_2D('c', fe_i, fe_q)
        phi_j  =  phi_2D('c', fe_j, fe_q)
        dphi_i = dphi_2D('c', fe_i, fe_q)
        dphi_j = dphi_2D('c', fe_j, fe_q)
        xyz    = xyz_2D(fe_q)
        JxW    = JxW_2D(fe_q)

        # The counterclockwise velocity field (0.5-y, x-0.5) Function<dim>::gradient wrapper.
        self.fun_u = VelocityFunction_2D()
        u_grad = function_gradient_2D("u", self.fun_u, xyz)

        # Boundary IDs
        left_edge   = 0
        top_edge    = 1
        right_edge  = 2
        bottom_edge = 3

        dirichletBC = {}
        dirichletBC[left_edge]   = [ 
                                    ('c',  adoubleConstantFunction_2D(adouble(0.0), self.n_components)),
                                   ]
        dirichletBC[top_edge]    = [ 
                                    ('c',  adoubleConstantFunction_2D(adouble(0.0), self.n_components)),
                                   ]
        dirichletBC[right_edge]  = [ 
                                    ('c',  adoubleConstantFunction_2D(adouble(0.0), self.n_components)),
                                   ]
        dirichletBC[bottom_edge] = [ 
                                    ('c',  adoubleConstantFunction_2D(adouble(0.0), self.n_components)),
                                   ]

        # FE weak form terms
        accumulation = (phi_i * phi_j) * JxW
        diffusion    = 0.0 * JxW
        convection   = phi_i * (u_grad * dphi_j) * JxW
        source       = 0.0 * JxW

        weakForm = dealiiFiniteElementWeakForm_2D(Aij = diffusion + convection,
                                                  Mij = accumulation,
                                                  Fi  = source,
                                                  functionsDirichletBC = dirichletBC)

        # Setting the weak form of the FE system will declare a set of equations:
        # [Mij]{dx/dt} + [Aij]{x} = {Fi} and boundary integral equations
        self.fe_system.WeakForm = weakForm

class simTutorial(daeSimulation):
    def __init__(self, Nx):
        daeSimulation.__init__(self)
        self.m = modTutorial("tutorial_cv_9", Nx)
        self.m.Description = __doc__
        self.m.fe_model.Description = __doc__

    def SetUpParametersAndDomains(self):
        pass
    
    def SetUpVariables(self):
        # Get coordinates for every DOF
        sp = self.m.fe_system.GetDOFSupportPoints()

        # Define a conical body
        (x0, y0) = (0.5, 0.25)
        r0 = 0.15
        r = lambda x,y: (1.0/r0) * numpy.sqrt((x-x0)**2 + (y-y0)**2)
        def ic(internal_index, overall_index):
            p = sp[overall_index]
            if r(p.x, p.y) <= 1.0:
                return 1 - r(p.x, p.y)
            else:
                return 0.0

        setFEInitialConditions(self.m.fe_model, self.m.fe_system, 'c', ic)

# Setup everything manually and run in a console
def simulate(Nx):
    # Create Log, Solver, DataReporter and Simulation object
    log          = daePythonStdOutLog()
    daesolver    = daeIDAS()
    datareporter = daeDelegateDataReporter()
    simulation   = simTutorial(Nx)

    daesolver.RelativeTolerance = 1E-6
    
    # Do no print progress
    log.PrintProgress = False

    lasolver = pySuperLU.daeCreateSuperLUSolver()
    daesolver.SetLASolver(lasolver)

    simName = simulation.m.Name + 'Nx=%d'%Nx + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
    results_folder = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'tutorial_cv_9-results(Nx=%d)' % Nx)

    # Create three data reporters:
    # 1. DealII
    feDataReporter = simulation.m.fe_system.CreateDataReporter()
    datareporter.AddDataReporter(feDataReporter)
    if not feDataReporter.Connect(results_folder, simName):
        sys.exit()

    # 2. TCP/IP
    tcpipDataReporter = daeTCPIPDataReporter()
    datareporter.AddDataReporter(tcpipDataReporter)
    if not tcpipDataReporter.Connect("", simName):
        sys.exit()

    # 3. Data
    dr = daeNoOpDataReporter()
    datareporter.AddDataReporter(dr)

    # Enable reporting of all variables
    simulation.m.SetReportingOn(True)

    # Set the time horizon and the reporting interval
    simulation.ReportingInterval = 2*numpy.pi / 100
    simulation.TimeHorizon = 2*numpy.pi

    # Initialize the simulation
    simulation.Initialize(daesolver, datareporter, log)

    # Save the model report and the runtime model report
    #simulation.m.fe_model.SaveModelReport(simulation.m.Name + ".xml")
    #simulation.m.fe_model.SaveRuntimeModelReport(simulation.m.Name + "-rt.xml")

    # Solve at time=0 (initialization)
    simulation.SolveInitial()

    # Run
    simulation.Run()
    simulation.Finalize()
    
    ###########################################
    #  Plots and data                         #
    ###########################################
    results = dr.Process.dictVariables
    cvar = results[simulation.m.Name + '.SolidBodyRotation.c']
    points = cvar.Domains[0].Points
    c       = cvar.Values[-1,:] # 2D array [t,omega]
    # After t = 2pi system returned to the initial position
    c_exact = cvar.Values[0,:] # 2D array [t,omega]
   
    return points, c, c_exact

def run(**kwargs):
    Nxs = numpy.array([32, 64, 96]) #, 128])
    n = len(Nxs)
    L = 1.0 - (-1.0) # = 2.0
    hs = L / Nxs
    E = numpy.zeros(n)
    C = numpy.zeros(n)
    p = numpy.zeros(n)
    E2 = numpy.zeros(n)
    
    # The normalised global errors
    for i,Nx in enumerate(Nxs):
        points, numerical_sol, manufactured_sol = simulate(int(Nx))
        E[i] = numpy.sqrt((1.0/Nx) * numpy.sum((numerical_sol-manufactured_sol)**2))

    # Order of accuracy
    for i,Nx in enumerate(Nxs):
        p[i] = numpy.log(E[i]/E[i-1]) / numpy.log(hs[i]/hs[i-1])
        C[i] = E[i] / hs[i]**p[i]
        
    C2 = 100.0 # constant for the second order slope line (to get close to the actual line)
    E2 = C2 * hs**2 # E for the second order slope
    
    fontsize = 14
    fontsize_legend = 11
    fig = plt.figure(figsize=(10,4), facecolor='white')
    fig.canvas.set_window_title('The Normalised global errors and the Orders of accuracy (Nelems = %s) (cv_9)' % Nxs.tolist())
    
    ax = plt.subplot(121)
    plt.figure(1, facecolor='white')
    plt.loglog(hs, E,  'ro', label='E(h)')
    plt.loglog(hs, E2, 'b-', label='2nd order slope')
    plt.xlabel('h', fontsize=fontsize)
    plt.ylabel('||E||', fontsize=fontsize)
    plt.legend(fontsize=fontsize_legend)
    #plt.xlim((0.04, 0.11))
        
    ax = plt.subplot(122)
    plt.figure(1, facecolor='white')
    plt.semilogx(hs[1:], p[1:],  'rs-', label='Order of Accuracy (p)')
    plt.xlabel('h', fontsize=fontsize)
    plt.ylabel('p', fontsize=fontsize)
    plt.legend(fontsize=fontsize_legend)
    #plt.xlim((0.04, 0.075))
    #plt.ylim((2.0, 2.04))
    
    plt.tight_layout()
    plt.show()
      
if __name__ == "__main__":
    run()
    
