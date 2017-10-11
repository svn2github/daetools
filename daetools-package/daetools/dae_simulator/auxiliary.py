"""
***********************************************************************************
                              auxiliary.py
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
***********************************************************************************
"""
import os, sys, json, importlib
from time import localtime, strftime
from daetools.pyDAE import *
from daetools.pyDAE.logs import daePythonStdOutLog 
from daetools.pyDAE.data_reporters import daePlotDataReporter, daeMatlabMATFileDataReporter 

# These integers are used by the simulation loader too
daeSundialsIDAS = 0

(laSundialsLU, laSuperLU, laSuperLU_MT) = list(range(0, 3))
(laAmesos_Klu, laAmesos_Superlu, laAmesos_Umfpack, laAmesos_Lapack, laAztecOO) = list(range(3, 8))
(laPardiso, laIntelPardiso, laIntelMKL, laAmdACML, laLapack, laMagmaLapack, laSuperLU_CUDA, laCUSP) = list(range(8, 16))

(nlpIPOPT, nlpNLOPT, nlpBONMIN) = list(range(0, 3))

(TCPIPLog, PythonStdOutLog, StdOutLog, BaseLog, FileLog, DelegateLog) = list(range(0, 6))

(TCPIPDataReporter, NoOpDataReporter, DelegateDataReporter, TEXTFileDataReporter, PlotDataReporter) = list(range(0, 5))
(MatlabMATFileDataReporter, BlackHoleDataReporter) = list(range(5, 7))

def getAvailableDAESolvers():
    available_dae_solvers = []
    try:
        from daetools.pyDAE import pyIDAS
        available_dae_solvers.append(("Sundials IDAS", daeSundialsIDAS, "DAE"))
    except Exception as e:
        print(str(e))

    return available_dae_solvers

def createDAESolver(daeIndex):
    dae_solver = None

    if daeIndex == daeSundialsIDAS:
        dae_solver = daeIDAS()

    return dae_solver
    
def getAvailableNLPSolvers():
    available_nlp_solvers = []
    
    try:
        from daetools.solvers.ipopt import pyIPOPT
        available_nlp_solvers.append(("IPOPT", nlpIPOPT, "NLP"))
        if 'daetools.solvers.ipopt' in sys.modules:
            del sys.modules['daetools.solvers.ipopt']
    except Exception as e:
        pass
    
    try:
        from daetools.solvers.nlopt import pyNLOPT
        available_nlp_solvers.append(("NLOPT", nlpNLOPT, "NLP"))
        if 'daetools.solvers.nlopt' in sys.modules:
            del sys.modules['daetools.solvers.nlopt']
    except Exception as e:
        pass
    
    try:
        from daetools.solvers.bonmin import pyBONMIN
        available_nlp_solvers.append(("BONMIN", nlpBONMIN, "MINLP"))
        if 'daetools.solvers.bonmin' in sys.modules:
            del sys.modules['daetools.solvers.bonmin']
    except Exception as e:
        pass
    
    return available_nlp_solvers
    
def getAvailableLASolvers():
    available_la_solvers = []
    
    available_la_solvers.append(("Sundials LU", laSundialsLU, "dense, sequential, direct"))
    try:
        from daetools.solvers.superlu import pySuperLU
        available_la_solvers.append(("SuperLU", laSuperLU, "sparse, sequential, direct"))
        if 'daetools.solvers.superlu' in sys.modules:
            del sys.modules['daetools.solvers.superlu']
    except Exception as e:
        pass

    try:
        from daetools.solvers.superlu_mt import pySuperLU_MT
        available_la_solvers.append(("SuperLU_MT", laSuperLU_MT, "sparse, pthreads, direct"))
        if 'daetools.solvers.superlu_mt' in sys.modules:
            del sys.modules['daetools.solvers.superlu_mt']
    except Exception as e:
        pass

    try:
        from daetools.solvers.trilinos import pyTrilinos
        suppSolvers = pyTrilinos.daeTrilinosSupportedSolvers()
        if 'Amesos_Klu' in suppSolvers:
            available_la_solvers.append(("Trilinos Amesos - KLU", laAmesos_Klu, "sparse, sequential, direct"))
        if 'Amesos_Superlu' in suppSolvers:
            available_la_solvers.append(("Trilinos Amesos - SuperLU", laAmesos_Superlu, "sparse, sequential, direct"))
        if 'Amesos_Umfpack' in suppSolvers:
            available_la_solvers.append(("Trilinos Amesos - Umfpack", laAmesos_Umfpack, "sparse, sequential, direct"))
        if 'Amesos_Lapack' in suppSolvers:
            available_la_solvers.append(("Trilinos Amesos - Lapack", laAmesos_Lapack, "dense, sequential, direct"))
        if 'AztecOO' in suppSolvers:
            available_la_solvers.append(("Trilinos AztecOO - Krylov", laAztecOO, "sparse, sequential, iterative"))
        if 'daetools.solvers.trilinos' in sys.modules:
            del sys.modules['daetools.solvers.trilinos']
    except Exception as e:
        pass

    try:
        from daetools.solvers.pardiso import pyPardiso
        available_la_solvers.append(("Pardiso", laPardiso, "sparse, OpenMP, direct"))
        if 'daetools.solvers.pardiso' in sys.modules:
            del sys.modules['daetools.solvers.pardiso']
    except Exception as e:
        pass

    try:
        from daetools.solvers.intel_pardiso import pyIntelPardiso
        available_la_solvers.append(("Intel Pardiso", laIntelPardiso, "sparse, OpenMP, direct"))
        if 'daetools.solvers.intel_pardiso' in sys.modules:
            del sys.modules['daetools.solvers.intel_pardiso']
    except Exception as e:
        pass
    
    return available_la_solvers

def getAvailableDataReporters():
    available_datareporters = []
    
    available_datareporters.append(("TCPIPDataReporter", TCPIPDataReporter, ""))
    # NoOp does not do anything but DOES store the data (Local data reporter)
    available_datareporters.append(("NoOpDataReporter", NoOpDataReporter, ""))
    available_datareporters.append(("DelegateDataReporter", DelegateDataReporter, ""))
    available_datareporters.append(("TEXTFileDataReporter", TEXTFileDataReporter, ""))
    available_datareporters.append(("PlotDataReporter", PlotDataReporter, ""))
    available_datareporters.append(("MatlabMATFileDataReporter", MatlabMATFileDataReporter, ""))
    # BlackHole does not do anything and DOES NOT store the data
    available_datareporters.append(("BlackHoleDataReporter", BlackHoleDataReporter, ""))
    
    return available_datareporters
    
def createDataReporter(datareporterIndex):
    datareporter = None

    # Send all the reported data to the TCP/IP data receiver via TCP/IP
    if datareporterIndex == TCPIPDataReporter:
        datareporter = daeTCPIPDataReporter()

    # Does nothing and does not keep the reported data
    elif datareporterIndex == BlackHoleDataReporter:
        datareporter = daeBlackHoleDataReporter()

    # Does nothing but keeps the reported data
    elif datareporterIndex == NoOpDataReporter:
        datareporter = daeNoOpDataReporter()

    # Delegate all calls to the containing data reporters
    elif datareporterIndex == DelegateDataReporter:
        datareporter = daeDelegateDataReporter()

    # Saves the reported data to a plain text file
    elif datareporterIndex == TEXTFileDataReporter:
        datareporter = daeTEXTFileDataReporter()

    # Plots the reported data using matplotlib
    elif datareporterIndex == PlotDataReporter:
        datareporter = daePlotDataReporter()

    # Saves the reported data to a .mat file
    elif datareporterIndex == MatlabMATFileDataReporter:
        datareporter = daeMatlabMATFileDataReporter()
     
    else:
        raise RuntimeError("Unsupported Log selected")
    
    return datareporter

def getAvailableLogs():
    available_logs = []
    
    available_logs.append(("TCPIPLog", TCPIPLog, ""))
    available_logs.append(("PythonStdOutLog", PythonStdOutLog, ""))
    available_logs.append(("StdOutLog", StdOutLog, ""))
    available_logs.append(("BaseLog", BaseLog, ""))
    available_logs.append(("FileLog", FileLog, ""))
    available_logs.append(("DelegateLog", DelegateLog, ""))
    
    return available_logs
    
def createLog(logIndex):
    log = None
    
    if logIndex == BaseLog:
        log = daeBaseLog()

    elif logIndex == FileLog:
        log = daeFileLog('') # Log will create a temp file somewhere

    elif logIndex == DelegateLog:
        log = daeDelegateLog()

    elif logIndex == StdOutLog:
        log = daeStdOutLog()

    elif logIndex == TCPIPLog:
        log = daeTCPIPLog()

    elif logIndex == PythonStdOutLog:
        log = daePythonStdOutLog()
     
    else:
        raise RuntimeError("Unsupported Log selected")
    
    return log
   
def createLASolver(lasolverIndex):
    lasolver = None
    
    if lasolverIndex == laSundialsLU:
        pass

    elif lasolverIndex == laSuperLU:
        from daetools.solvers.superlu import pySuperLU
        lasolver = pySuperLU.daeCreateSuperLUSolver()

    elif lasolverIndex == laSuperLU_MT:
        from daetools.solvers.superlu_mt import pySuperLU_MT
        lasolver = pySuperLU_MT.daeCreateSuperLUSolver()
    
    elif lasolverIndex == laAmesos_Klu:
        from daetools.solvers.trilinos import pyTrilinos
        lasolver = pyTrilinos.daeCreateTrilinosSolver("Amesos_Klu", "")

    elif lasolverIndex == laAmesos_Superlu:
        from daetools.solvers.trilinos import pyTrilinos
        lasolver = pyTrilinos.daeCreateTrilinosSolver("Amesos_Superlu", "")

    elif lasolverIndex == laAmesos_Umfpack:
        from daetools.solvers.trilinos import pyTrilinos
        lasolver = pyTrilinos.daeCreateTrilinosSolver("Amesos_Umfpack", "")

    elif lasolverIndex == laAmesos_Lapack:
        from daetools.solvers.trilinos import pyTrilinos
        lasolver = pyTrilinos.daeCreateTrilinosSolver("Amesos_Lapack", "")

    elif lasolverIndex == laAztecOO:
        from daetools.solvers.trilinos import pyTrilinos
        lasolver = pyTrilinos.daeCreateTrilinosSolver("AztecOO", "ILUT")

    elif lasolverIndex == laPardiso:
        from daetools.solvers.pardiso import pyPardiso
        lasolver = pyPardiso.daeCreatePardisoSolver()

    elif lasolverIndex == laIntelPardiso:
        from daetools.solvers.intel_pardiso import pyIntelPardiso
        lasolver = pyIntelPardiso.daeCreateIntelPardisoSolver()
    
    elif lasolverIndex == laIntelMKL:
        from daetools.solvers import pyIntelMKL
        lasolver = pyIntelMKL.daeCreateLapackSolver()

    elif lasolverIndex == laAmdACML:
        from daetools.solvers import pyAmdACML
        lasolver = pyAmdACML.daeCreateLapackSolver()

    elif lasolverIndex == laLapack:
        from daetools.solvers import pyLapack
        lasolver = pyLapack.daeCreateLapackSolver()

    elif lasolverIndex == laMagmaLapack:
        from daetools.solvers import pyMagma
        lasolver = pyMagma.daeCreateLapackSolver()
    
    elif lasolverIndex == laSuperLU_CUDA:
        from daetools.solvers.superlu_cuda import pySuperLU_CUDA
        lasolver = pySuperLU_CUDA.daeCreateSuperLUSolver()
    
    elif lasolverIndex == laCUSP:
        from daetools.solvers import pyCUSP
        lasolver = pyCUSP.daeCreateCUSPSolver()
    
    else:
        raise RuntimeError("Unsupported LA Solver selected")
    
    return lasolver
    
def createNLPSolver(minlpsolverIndex):
    nlpsolver = None
    
    if minlpsolverIndex == nlpIPOPT:
        from daetools.solvers.ipopt import pyIPOPT
        nlpsolver = pyIPOPT.daeIPOPT()

    elif minlpsolverIndex == nlpNLOPT:
        from daetools.solvers.nlopt import pyNLOPT
        # Default algorithm
        algorithm = 'NLOPT_LD_SLSQP'
        try:
            from PyQt5 import QtCore, QtGui
            algorithms = ['NLOPT_GN_DIRECT','NLOPT_GN_DIRECT_L','NLOPT_GN_DIRECT_L_RAND','NLOPT_GN_DIRECT_NOSCAL','NLOPT_GN_DIRECT_L_NOSCAL',
                        'NLOPT_GN_DIRECT_L_RAND_NOSCAL','NLOPT_GN_ORIG_DIRECT','NLOPT_GN_ORIG_DIRECT_L','NLOPT_GD_STOGO','NLOPT_GD_STOGO_RAND',
                        'NLOPT_LD_LBFGS_NOCEDAL','NLOPT_LD_LBFGS','NLOPT_LN_PRAXIS','NLOPT_LD_VAR1','NLOPT_LD_VAR2','NLOPT_LD_TNEWTON',
                        'NLOPT_LD_TNEWTON_RESTART','NLOPT_LD_TNEWTON_PRECOND','NLOPT_LD_TNEWTON_PRECOND_RESTART','NLOPT_GN_CRS2_LM',
                        'NLOPT_GN_MLSL','NLOPT_GD_MLSL','NLOPT_GN_MLSL_LDS','NLOPT_GD_MLSL_LDS','NLOPT_LD_MMA','NLOPT_LN_COBYLA',
                        'NLOPT_LN_NEWUOA','NLOPT_LN_NEWUOA_BOUND','NLOPT_LN_NELDERMEAD','NLOPT_LN_SBPLX','NLOPT_LN_AUGLAG','NLOPT_LD_AUGLAG',
                        'NLOPT_LN_AUGLAG_EQ','NLOPT_LD_AUGLAG_EQ','NLOPT_LN_BOBYQA','NLOPT_GN_ISRES',
                        'NLOPT_AUGLAG','NLOPT_AUGLAG_EQ','NLOPT_G_MLSL','NLOPT_G_MLSL_LDS','NLOPT_LD_SLSQP']
            # Show the input box to choose the algorithm (the default is len(algorithms)-1 that is: NLOPT_LD_SLSQP)
            algorithm, ok = QtGui.QInputDialog.getItem(None, "NLOPT Algorithm", "Choose the NLOPT algorithm:", algorithms, len(algorithms)-1, False)
        except Exception as e:
            pass
        
        nlpsolver = pyNLOPT.daeNLOPT(str(algorithm))

    elif minlpsolverIndex == nlpBONMIN:
        from daetools.solvers.bonmin import pyBONMIN
        nlpsolver = pyBONMIN.daeBONMIN()

    else:
        raise RuntimeError("Unsupported (MI)NLP Solver selected")
    
    return nlpsolver

def createDAESolverByName(name):
    available_objects = getAvailableDAESolvers()
    for obj in available_objects:
        # obj[0]: name
        # obj[1]: index
        # obj[2]: description
        if obj[0] == name:
            return createDAESolver(obj[1])
    return None

def createLASolverByName(name):
    available_objects = getAvailableLASolvers()
    for obj in available_objects:
        # obj[0]: name
        # obj[1]: index
        # obj[2]: description
        if obj[0] == name:
            return createLASolver(obj[1])
    return None

def createNLPSolverByName(name):
    available_objects = getAvailableNLPSolvers()
    for obj in available_objects:
        # obj[0]: name
        # obj[1]: index
        # obj[2]: description
        if obj[0] == name:
            return createNLPSolver(obj[1])
    return None

def createDataReporterByName(name):
    available_objects = getAvailableDataReporters()
    for obj in available_objects:
        # obj[0]: name
        # obj[1]: index
        # obj[2]: description
        if obj[0] == name:
            return createDataReporter(obj[1])
    return None
    
def createLogByName(name):
    available_objects = getAvailableLogs()
    for obj in available_objects:
        # obj[0]: name
        # obj[1]: index
        # obj[2]: description
        if obj[0] == name:
            return createLog(obj[1])
    return None

def initializeSimulation(simulation, strDAESolver,
                                     strLASolver,
                                     strDataReporter,
                                     strDataReporterConnectionString,
                                     strLog,
                                     bCalculateSensitivities):
    """
    This function initializes a simulation using the strings supplied:
    DAE solver, LA solver, DataReporter, Log etc.
    """
    if not simulation:
        raise RuntimeError('Invalid simulation object')
    if not isinstance(strDAESolver, str):
        raise RuntimeError('DAE solver argument must be a string')
    if not isinstance(strLASolver, str):
        raise RuntimeError('LA solver argument must be a string')
    if not isinstance(strDataReporter, str):
        raise RuntimeError('Data reporter argument must be a string')
    if not isinstance(strDataReporterConnectionString, str):
        raise RuntimeError('Data reporter connection string argument must be a string')
    if not isinstance(strLog, str):
        raise RuntimeError('Log argument must be a string')
    
    log = createLogByName(strLog)
    if not log:
        raise RuntimeError('Cannot create log: %s' % strLog)

    daesolver = createDAESolverByName(strDAESolver)
    if not daesolver:
        raise RuntimeError('Cannot create DAE solver: %s' % strDAESolver)

    lasolver  = None
    if strLASolver:
        lasolver = createLASolverByName(strLASolver)
        if not lasolver:
            raise RuntimeError('Cannot create LA solver: %s' % strLASolver)
        daesolver.SetLASolver(lasolver)

    datareporter = createDataReporterByName(strDataReporter)
    if not datareporter:
        raise RuntimeError('Cannot create datareporter: %s' % strDataReporter)

    process_name = simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
    if not datareporter.Connect(strDataReporterConnectionString, process_name):
        raise RuntimeError("Cannot connect data reporter")

    simulation.Initialize(daesolver, datareporter, log, bCalculateSensitivities)
    
    # Save objects somewhere so they don't get destroyed after leaving the scope
    simulation.__rt_objects__ = [daesolver, lasolver, datareporter, log]

    return daesolver, lasolver, datareporter, log

def initializeSimulationJSON(simulation, jsonSettings):
    """
    This function reads all information needed to initialize a simulation
    from the jsonSettings argument, including DAE solver, LA solver, DataReporter, Log etc.
    If the data in json string are not available the defaults are used.
    """
    if not simulation:
        raise RuntimeError('Invalid simulation object')
    if not isinstance(jsonSettings, str):
        raise RuntimeError('jsonSettings argument must be a string')

    settings = json.loads(jsonSettings)

    daesolver_name    = 'Sundials IDAS'
    la_solver_name    = None
    datareporter_name = 'BlackHoleDataReporter'
    connect_string    = ''
    log_name          = 'daePythonStdOutLog'
    calculateSensitivities = False

    if 'DAESolver' in settings:
        daesolver_name = settings['DAESolver']['Name']
    if 'LASolver' in settings:
        la_solver_name = settings['LASolver']['Name']
    if 'DataReporter' in settings:
        datareporter_name = settings['DataReporter']['Name']
        connect_string    = settings['DataReporter']['ConnectString'].encode('ascii','ignore')
    if 'Log' in settings:
        log_name = settings['Log']['Name']
    if 'CalculateSensitivities' in settings:
        calculateSensitivities = settings['CalculateSensitivities']
    
    log = createLogByName(log_name)
    if not log:
        raise RuntimeError('Cannot create log: %s' % log_name)

    daesolver = createDAESolverByName(daesolver_name)
    if not daesolver:
        raise RuntimeError('Cannot create DAE solver: %s' % daesolver_name)
    lasolver = None
    if la_solver_name:
        lasolver = createLASolverByName(la_solver_name)
        if not lasolver:
            raise RuntimeError('Cannot create LA solver: %s' % la_solver_name)
        daesolver.SetLASolver(lasolver)

    datareporter = createDataReporterByName(datareporter_name)
    if not datareporter:
        raise RuntimeError('Cannot create datareporter: %s' % datareporter_name)
    
    process_name   = simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
    if not datareporter.Connect(connect_string, process_name):
        raise RuntimeError("Cannot connect datareporter")

    simulation.Initialize(daesolver, datareporter, log, calculateSensitivities, jsonSettings)

    # Save objects somewhere so they don't get destroyed after leaving the scope
    simulation.__rt_objects__ = [daesolver, lasolver, datareporter, log]

    return daesolver, lasolver, datareporter, log

def loadTutorial(tutorialName, callableObjectName, arguments):
    # Get the module name
    py_module = 'daetools.examples.%s' % tutorialName
    
    # Import the tutorial module
    simulation_module = importlib.import_module(py_module)
    
    # Check if the callable exists in the tutorial module.
    if not hasattr(simulation_module, callableObjectName):
        raise RuntimeError('The module: %s does not have callable object: %s' % (simulation_module.__name__, callableObjectName))

    # Run the callable to create and initialise the simulation.
    simulation = eval("simulation_module.%s(%s)" % (callableObjectName, arguments), globals(), locals())
    
    return simulation

def loadSimulation(directory, simulationFile, callableObjectName, arguments):
    # Add directory to sys.path if it is not already there.
    if directory not in sys.path:
        sys.path.insert(0, directory)
    
    # Get the module name
    py_module = os.path.splitext(simulationFile)[0]
    
    # Here we have two options:
    #  a) If the simulation module is already loaded - reload it.
    #     This is useful if the module changed but the changes will not be available
    #     until the python interpreter restarts or the module is reloaded.
    #  b) If the module has previously not been loaded - import it.
    #if py_module in sys.modules:
    #    simulation_module = sys.modules[py_module]
    #    importlib.reload(simulation_module)
    #else:
    #    simulation_module = importlib.import_module(py_module)
    #
    # But, there are a number of caveats of doing reload (see importlib.reload(module) documentation).
    # Therefore, we only import the module and once loaded the changes will not be reflected. 
    simulation_module = importlib.import_module(py_module)
    
    # Check if the callable exists in the simulation module.
    if not hasattr(simulation_module, callableObjectName):
        raise RuntimeError('The module: %s does not have callable object: %s' % (simulation_module.__name__, callableObjectName))

    # Run the callable to create and initialise the simulation.
    simulation = eval("simulation_module.%s(%s)" % (callableObjectName, arguments), globals(), locals())
    
    return simulation
