#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                        tutorial_dealii_4.py
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
In this tutorial the transient heat conduction problem is solved using
the finite element method:

.. code-block:: none

   dT/dt - kappa * nabla^2(Τ) = g in Omega

The mesh is a pipe submerged into water receiving the heat of the sun at the
45 degrees from the top-left direction, the heat from the suroundings and
having the constant temperature of the inner tube.
The boundary conditions are thus:

- [at boundary ID=0] Sun shine at 45 degrees, gradient heat flux = 2 kW/m**2 in direction n = (1,-1)
- [at boundary ID=1] Outer surface where y <= -0.5, constant flux of 2 kW/m**2
- [at boundary ID=2] Inner tube: constant temperature of 300 K

The pipe mesh is given below:

.. image:: _static/pipe.png
   :width: 300 px

The temperature plot at t = 3600s:

.. image:: _static/tutorial_dealii_4-results.png
   :height: 400 px

Animation:
    
.. image:: _static/tutorial_dealii_4-animation.gif
   :height: 400 px
"""

import os, sys, numpy, json, math, tempfile
from time import localtime, strftime
from daetools.pyDAE import *
from daetools.solvers.deal_II import *
from daetools.solvers.superlu import pySuperLU

# Standard variable types are defined in variable_types.py
from pyUnits import m, kg, s, K, Pa, mol, J, W

# Neumann BC use either value() or gradient() functions.
# Dirichlet BC use vector_value() with n_component = multiplicity of the equation.
# Other functions use just the value().
#
# Nota bene:
#   This function is derived from Function_2D class and returns "double" value/gradient
class GradientFunction_2D(Function_2D):
    def __init__(self, gradient, direction, n_components = 1):
        """
        Arguments:
          gradient - float, flux magnitude
          direction - Tensor<1,dim>, unit vector
        """
        Function_2D.__init__(self, n_components)
        self.m_gradient = Tensor_1_2D()
        self.m_gradient[0] = gradient * direction[0]
        self.m_gradient[1] = gradient * direction[1]
        
    def gradient(self, point, component = 0):
        if point.x < 0 and point.y > 0:
            return self.m_gradient
        else:
            return Tensor_1_2D()

    def vector_gradient(self, point):
        return [self.gradient(point, c) for c in range(self.n_components)]

# Nota bene:
#   This function is derived from adoubleFunction_2D class and returns "adouble" value
#   In this case, it is a function of daetools parameters/variables
class BottomGradientFunction_2D(adoubleFunction_2D):
    def __init__(self, gradient, n_components = 1):
        """
        Arguments:
          gradient - adouble, flux magnitude (scalar)
        """
        adoubleFunction_2D.__init__(self, n_components)
        self.gradient = adouble(gradient)
        
    def value(self, point, component = 0):
        if point.y < -0.5:
            return self.gradient
        else:
            return adouble(0.0)

    def vector_value(self, point):
        return [self.value(point, c) for c in range(self.n_components)]

class modTutorial(daeModel):
    def __init__(self, Name, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)

        self.Q0_total = daeVariable("Q0_total", no_t, self, "Total heat passing through the boundary with id=0")
        self.Q1_total = daeVariable("Q1_total", no_t, self, "Total heat passing through the boundary with id=1")
        self.Q2_total = daeVariable("Q2_total", no_t, self, "Total heat passing through the boundary with id=2")

        meshes_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'meshes')
        mesh_file  = os.path.join(meshes_dir, 'pipe.msh')

        dofs = [dealiiFiniteElementDOF_2D(name='T',
                                          description='Temperature',
                                          fe = FE_Q_2D(1),
                                          multiplicity=1)]
        self.n_components = int(numpy.sum([dof.Multiplicity for dof in dofs]))

        # Store the object so it does not go out of scope while still in use by daetools
        self.fe_system = dealiiFiniteElementSystem_2D(meshFilename    = mesh_file,     # path to mesh
                                                      quadrature      = QGauss_2D(3),  # quadrature formula
                                                      faceQuadrature  = QGauss_1D(3),  # face quadrature formula
                                                      dofs            = dofs)          # degrees of freedom

        self.fe_model = daeFiniteElementModel('HeatConduction', self, 'Transient heat conduction through a pipe wall with an external heat flux', self.fe_system)
       
    def DeclareEquations(self):
        daeModel.DeclareEquations(self)

        # Thermo-physical properties of copper.
        rho   = 8960.0  # kg/m**3
        cp    =  385.0  # J/(kg*K)
        kappa =  401.0  # W/(m*K)

        # Thermal diffusivity (m**2/s)
        alpha        = kappa / (rho*cp)   # m**2/s

        flux_above   = 2.0E3/(rho*cp) # (W/m**2)/((kg/m**3) * (J/(kg*K))) =
        flux_beneath = 5.0E3/(rho*cp) # (W/m**2)/((kg/m**3) * (J/(kg*K))) =

        print('Thermal diffusivity = %f' % alpha)
        print('Beneath source flux = %f' % flux_beneath)
        print('Above source flux = %f x (1,-1)' % flux_above)

        # Create some auxiliary objects for readability
        phi_i  =  phi_2D('T', fe_i, fe_q)
        phi_j  =  phi_2D('T', fe_j, fe_q)
        dphi_i = dphi_2D('T', fe_i, fe_q)
        dphi_j = dphi_2D('T', fe_j, fe_q)
        dof_T  = dof_2D('T', fe_i)
        normal = normal_2D(fe_q)
        xyz    = xyz_2D(fe_q)
        JxW    = JxW_2D(fe_q)

        # deal.II Function<dim,Number> wrappers.
        self.fun_Diffusivity = ConstantFunction_2D(alpha)
        self.fun_Generation  = ConstantFunction_2D(0.0)
        # Gradient flux at boundary id=0 (Sun)
        self.fun_Flux_a = GradientFunction_2D(flux_above, direction = (-1, 1))
        # Flux as a function of daetools variables at boundary id=1 (outer tube where y < -0.5)
        self.fun_Flux_b = BottomGradientFunction_2D(flux_beneath)

        # Nota bene:
        #   For the Dirichlet BCs only the adouble versions of Function<dim> class can be used.
        #   The values allowed include constants and expressions on daeVariable/daeParameter objects.
        dirichletBC    = {}
        dirichletBC[2] = [ ('T', adoubleConstantFunction_2D( adouble(300) )) ] # at boundary id=2 (inner tube)

        surfaceIntegrals = {}
        surfaceIntegrals[0] = [(self.Q0_total(), (-kappa * (dphi_i * normal) * JxW) * dof_T)]
        surfaceIntegrals[1] = [(self.Q1_total(), (-kappa * (dphi_i * normal) * JxW) * dof_T)]
        surfaceIntegrals[2] = [(self.Q2_total(), (-kappa * (dphi_i * normal) * JxW) * dof_T)]

        # Function<dim>::value and Function<dim>::gradient wrappers
        D      = function_value_2D        ('Diffusivity', self.fun_Diffusivity, xyz)
        G      = function_value_2D        ('Generation',  self.fun_Generation,  xyz)
        Flux_a = function_gradient_2D     ('Flux_a',      self.fun_Flux_a,      xyz)
        Flux_b = function_adouble_value_2D('Flux_b',      self.fun_Flux_b,      xyz)

        # FE weak form terms
        accumulation = (phi_i * phi_j) * JxW
        diffusion    = (dphi_i * dphi_j) * D * JxW
        source       = phi_i * G * JxW
        faceFluxes   = {
                         0: phi_i * (Flux_a * normal) * JxW,
                         1: phi_i * Flux_b * JxW
                       }

        cell_Aij = diffusion
        cell_Mij = accumulation
        cell_Fi  = source
        
        weakForm = dealiiFiniteElementWeakForm_2D(Aij = cell_Aij,
                                                  Mij = cell_Mij,
                                                  Fi  = cell_Fi,
                                                  boundaryFaceFi  = faceFluxes,
                                                  functionsDirichletBC = dirichletBC,
                                                  surfaceIntegrals = surfaceIntegrals)

        print('Transient heat conduction equation:')
        print('    Aij = %s' % str(cell_Aij))
        print('    Mij = %s' % str(cell_Mij))
        print('    Fi  = %s' % str(cell_Fi))

        # Setting the weak form of the FE system will declare a set of equations:
        # [Mij]{dx/dt} + [Aij]{x} = {Fi} and boundary integral equations
        self.fe_system.WeakForm = weakForm

class simTutorial(daeSimulation):
    def __init__(self):
        daeSimulation.__init__(self)
        self.m = modTutorial("tutorial_dealii_4")
        self.m.Description = __doc__
        self.m.fe_model.Description = __doc__

    def SetUpParametersAndDomains(self):
        pass

    def SetUpVariables(self):
        # setFEInitialConditions(daeFiniteElementModel, dealiiFiniteElementSystem_xD, str, float|callable)
        setFEInitialConditions(self.m.fe_model, self.m.fe_system, 'T', 293)

def run(**kwargs):
    guiRun = kwargs.get('guiRun', False)
    
    simulation = simTutorial()

    # Create SuperLU LA solver
    lasolver = pySuperLU.daeCreateSuperLUSolver()

    # Create and setup two data reporters:
    datareporter = daeDelegateDataReporter()
    simName = simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
    if guiRun:
        results_folder = tempfile.mkdtemp(suffix = '-results', prefix = 'tutorial_deal_II_4-')
        daeQtMessage("deal.II", "The simulation results will be located in: %s" % results_folder)
    else:
        results_folder = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'tutorial_deal_II_4-results')
        print("The simulation results will be located in: %s" % results_folder)
    
    # 1. deal.II (exports only FE DOFs in .vtk format to the specified directory)
    feDataReporter = simulation.m.fe_system.CreateDataReporter()
    datareporter.AddDataReporter(feDataReporter)
    if not feDataReporter.Connect(results_folder, simName):
        sys.exit()
        
    # 2. TCP/IP
    tcpipDataReporter = daeTCPIPDataReporter()
    datareporter.AddDataReporter(tcpipDataReporter)
    if not tcpipDataReporter.Connect("", simName):
        sys.exit()

    return daeActivity.simulate(simulation, reportingInterval = 60, 
                                            timeHorizon       = 2*60*60,
                                            lasolver          = lasolver,
                                            datareporter      = datareporter,
                                            **kwargs)

if __name__ == "__main__":
    guiRun = False if (len(sys.argv) > 1 and sys.argv[1] == 'console') else True
    run(guiRun = guiRun)
