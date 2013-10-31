#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
***********************************************************************************
                         tutorial_deal_II.py
                DAE Tools: pyDAE module, www.daetools.com
                Copyright (C) Dragan Nikolic, 2013
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
deal.II classes provided by the python wrapper: 
 - Tensors of rank 1 up to three dimensions (Tensor<rank,dim,double> template): 
    * Tensor_1_1D
    * Tensor_1_2D
    * Tensor_1_3D
 - Points up to three dimensions (Point<dim,double> template):
    * Point_1D
    * Point_2D
    * Point_3D
 - Functions up to three dimensions (Function<dim> template):
    * Function_1D
    * Function_2D
    * Function_3D    
 - Solvers for scalar transport equations:
    * Convection-Diffusion Equation up to three dimensions (daeConvectionDiffusion<dim> template):
       + daeConvectionDiffusion_1D
       + daeConvectionDiffusion_2D
       + daeConvectionDiffusion_3D
    * Laplace Equation up to three dimensions
    * Poisson Equation up to three dimensions
    * Helmholtz Equation up to three dimensions
"""

import os, sys, numpy, json
from daetools.pyDAE import *
from daetools.solvers.deal_II import pyDealII
from time import localtime, strftime
from daetools.solvers.superlu import pySuperLU
from daetools.solvers.trilinos import pyTrilinos
from daetools.solvers.aztecoo_options import daeAztecOptions

# Standard variable types are defined in variable_types.py
from pyUnits import m, kg, s, K, Pa, mol, J, W

class fnConstantFunction(pyDealII.Function_2D):
    def __init__(self, val, n_components = 1):
        pyDealII.Function_2D.__init__(self, n_components)
        self.m_value = float(val)
        
    def value(self, point, component = 1):
        #print 'Point%s = %f' % (point, self.m_value)
        return self.m_value

class fnConstantVectorFunction(pyDealII.Function_2D):
    def __init__(self, values, n_components = 2):
        pyDealII.Function_2D.__init__(self, n_components)
        
        if len(values) != 2:
            raise RunttimeError('The length of the values array must be two')
        self.m_values = list(values)
        
    def value(self, point, component = 1):
        return self.m_values[component]
    
    def vector_value(self, point):
        return self.m_values

class modTutorial(daeModel):
    def __init__(self, Name, Parent = None, Description = ""):
        daeModel.__init__(self, Name, Parent, Description)
        
        # Achtung, Achtung!!
        # Diffusivity, velocity, generation, dirichletBC and neumannBC must be 
        # owned by the model for deal.II FE model keeps only references to them.
        self.functions    = {}
        self.functions['Diffusivity'] = fnConstantFunction(1.0)
        self.functions['Velocity']    = fnConstantVectorFunction([0.0, 0.0])
        self.functions['Generation']  = fnConstantFunction(1.0)
        
        self.neumannBC      = {}
        #self.neumannBC[0]   = fnConstantFunction(10)
        
        self.dirichletBC    = {}
        self.dirichletBC[0] = fnConstantFunction(100)
        self.dirichletBC[1] = fnConstantFunction(200)
        self.dirichletBC[2] = fnConstantFunction(250)
        
        self.fe = pyDealII.daeConvectionDiffusion_2D('Helmholtz', 
                                                     self, 
                                                     'Modified deal.II step-7 example (steady-state Helmholtz equation)',
                                                     meshFilename      = os.path.join(os.path.dirname(__file__), 'meshes', "ex49fine.msh"),
                                                     # deal.II related arguments (all are mandatory):
                                                     quadratureFormula = 'QGauss',
                                                     polynomialOrder   = 1,
                                                     outputDirectory   = os.path.join(os.path.dirname(__file__), 'results'),
                                                     functions         = self.functions,
                                                     dirichletBC       = self.dirichletBC,
                                                     neumannBC         = self.neumannBC)
        
    def DeclareEquations(self):
        daeModel.DeclareEquations(self)
        
        # 1a. Default assemble
        #self.fe.AssembleSystem()
        
        # 1b. Use expressions for cell constributions
        
        # 1c. User-defined assemble
        
        for cell in self.fe:
            # Get face_values and friends:
            #  - face_values will be automatically reinitialized
            #  - matrices/vectors will be automatically zeroed
            #  - local_dof_indices will be automatically filled in with the cell dofs
            fe_values         = cell.fe_values
            cell_matrix       = cell.cell_matrix
            cell_rhs          = cell.cell_rhs
            system_matrix     = cell.system_matrix
            system_rhs        = cell.system_rhs
            local_dof_indices = cell.local_dof_indices
            print 'local_dof_indices = %s' % str(list(local_dof_indices))

            for q in range(cell.n_q_points):
                for i in range(cell.dofs_per_cell):
                    for j in range(cell.dofs_per_cell):
                        cell_matrix.add(i,j, (
                                               (fe_values.shape_grad(i,q) * fe_values.shape_grad(j,q)) * 1.0 # diffusivity 
                                               +
                                               fe_values.shape_value(i,q) * fe_values.shape_value(j,q)                                             
                                             )
                                             * fe_values.JxW(q))
                    cell_rhs.add(i, fe_values.shape_value(i,q) * 1.0 * fe_values.JxW(q))
            
            print '1'
            for f, face in enumerate(cell.faces):
                # Get the fe_face_values (will be automatically reinitialized with the current face)
                print 'fstart = %d' % f
                if face.at_boundary:
                    print 'Face boundary at: ', face.boundary_id
                print 'at'
                if face.at_boundary and face.boundary_id in self.neumannBC:
                    boundary_id    = face.boundary_id
                    print 'fface_values 1'
                    fe_face_values = face.fe_values
                    print 'fface_values 2'
                    for q in range(face.n_q_points):
                        for i in range(cell.dofs_per_cell):
                            cell_rhs.add(i, 0.0
                                            *
                                            fe_face_values.shape_value(i, q_point)
                                            *
                                            fe_face_values.JxW(q_point))
                print 'fend'
            print '2'
            for i in range(cell.dofs_per_cell):
                for j in range(cell.dofs_per_cell):
                    system_matrix.add(local_dof_indices[i], local_dof_indices[j], cell_matrix(i,j))
                system_rhs.add(local_dof_indices[i], cell_rhs[i])
            print '3'
        
        self.fe.CondenseHangingNodeConstraints()
        for bid, fn in self.dirichletBC.items():
            self.fe.InterpolateAndApplyBoundaryValues(bid, fn)
            
        print 'Done assembling'
        
        # 2. Generate equations
        self.fe.GenerateEquations()
        print 'Done generating equations'
        
class simTutorial(daeSimulation):
    def __init__(self):
        daeSimulation.__init__(self)
        self.m = modTutorial("tutorial_deal_II")
        self.m.Description = __doc__
        
    def SetUpParametersAndDomains(self):
        pass

    def SetUpVariables(self):
        pass
    
# Use daeSimulator class
def guiRun(app):
    simulation = simTutorial()
    datareporter = daeDelegateDataReporter()
    tcpipDataReporter = daeTCPIPDataReporter()
    feDataReporter    = daeDealIIDataReporter(simulation.m.fe.DataOut)
    datareporter.AddDataReporter(tcpipDataReporter)
    datareporter.AddDataReporter(feDataReporter)

    # Connect TCP/IP data reporter
    simName = simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
    if(tcpipDataReporter.Connect("", simName) == False):
        sys.exit()

    simulation.m.SetReportingOn(True)
    simulation.ReportingInterval = 10
    simulation.TimeHorizon       = 1000
    simulator  = daeSimulator(app, simulation=simulation, datareporter = datareporter)
    simulator.exec_()

# Setup everything manually and run in a console
def consoleRun():
    # Create Log, Solver, DataReporter and Simulation object
    log          = daePythonStdOutLog()
    daesolver    = daeIDAS()
    datareporter = daeDelegateDataReporter()
    simulation   = simTutorial()
    lasolver = pySuperLU.daeCreateSuperLUSolver()
    daesolver.SetLASolver(lasolver)
    """
    lasolver = pyTrilinos.daeCreateTrilinosSolver("AztecOO", "")
    lasolver.NumIters  = 500
    lasolver.Tolerance = 1e-6
    paramListAztec = lasolver.AztecOOOptions
    paramListAztec.set_int("AZ_solver",    daeAztecOptions.AZ_gmres)
    paramListAztec.set_int("AZ_kspace",    500)
    paramListAztec.set_int("AZ_scaling",   daeAztecOptions.AZ_none)
    paramListAztec.set_int("AZ_reorder",   0)
    paramListAztec.set_int("AZ_conv",      daeAztecOptions.AZ_r0)
    paramListAztec.set_int("AZ_keep_info", 1)
    paramListAztec.set_int("AZ_output",    daeAztecOptions.AZ_all) # {AZ_all, AZ_none, AZ_last, AZ_summary, AZ_warnings}
    paramListAztec.set_int("AZ_precond",         daeAztecOptions.AZ_dom_decomp)
    paramListAztec.set_int("AZ_subdomain_solve", daeAztecOptions.AZ_ilut)
    paramListAztec.set_int("AZ_overlap",         daeAztecOptions.AZ_none)
    paramListAztec.set_int("AZ_graph_fill",      1)
    paramListAztec.Print()
    daesolver.SetLASolver(lasolver)
    """
    
    # Create two data reporters: TCP/IP and DealII
    tcpipDataReporter = daeTCPIPDataReporter()
    feDataReporter    = daeDealIIDataReporter(simulation.m.fe.DataOut)
    datareporter.AddDataReporter(tcpipDataReporter)
    datareporter.AddDataReporter(feDataReporter)

    # Connect TCP/IP data reporter
    simName = simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
    if(tcpipDataReporter.Connect("", simName) == False):
        sys.exit()

    # Enable reporting of all variables
    simulation.m.SetReportingOn(True)

    # Set the time horizon and the reporting interval
    simulation.ReportingInterval = 10
    simulation.TimeHorizon = 1000

    # Initialize the simulation
    simulation.Initialize(daesolver, datareporter, log)

    # Save the model report and the runtime model report
    #simulation.m.fe.SaveModelReport(simulation.m.fe.Name + ".xml")
    #simulation.m.fe.SaveRuntimeModelReport(simulation.m.fe.Name + "-rt.xml")

    # Solve at time=0 (initialization)
    simulation.SolveInitial()

    # Run
    simulation.Run()
    simulation.Finalize()
    """
    print ''
    class fun1(pyDealII.Function_2D):
        def __init__(self, n_components = 1):
            pyDealII.Function_2D.__init__(self, n_components)
            
        def value(self, point, component = 1):
            return 1.2
    
    p = pyDealII.Point_2D(1.0, 2.0)
    f1 = fun1()
    print f1.n_components
    print f1.dimension
    m = pyDealII.map_Uint_Function_2D()
    m[0] = f1
    print m[0]
    print m[0].value(p)
    return
    
    t = pyDealII.Tensor_1_2D()
    print t.dimension, t.rank
    t[0] = 5
    print t, repr(t), t.norm()
    print t==t
    print t*t
    
    t = pyDealII.Point_3D()
    print t.dimension, t.rank
    t[0] = 1
    t[1] = 2
    t[2] = 3
    print t.x, t.y, t.z
    print t, repr(t)
    print t*2
    print t*t
    print t.square()
    t2 = (t*2)
    print t.distance(t2)
    """

if __name__ == "__main__":
    if len(sys.argv) > 1 and (sys.argv[1] == 'console'):
        consoleRun()
    else:
        from PyQt4 import QtCore, QtGui
        app = QtGui.QApplication(sys.argv)
        guiRun(app)
