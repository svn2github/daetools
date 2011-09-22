#!/usr/bin/env python

import json
import nineml
from nineml.abstraction_layer.testing_utils import RecordValue, TestableComponent
from nineml.abstraction_layer import ComponentClass
import os, sys
from time import localtime, strftime, time
from daetools.pyDAE import *
from nineml_daetools_bridge import *
from nineml_tex_report import *

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

class daeSimulationInputData:
    def __init__(self):
        self.parser = ExpressionParser()

        # Dictionaries 'canonical/relative name' : floating-point-value
        self.parameters         = {}
        self.initial_conditions = {}
        # Dictionaries: 'canonical/relative name' : 'expression'
        self.analog_port_expressions = {}
        self.event_port_expressions  = {}
        # Dictionary: 'canonical/relative name' : string
        self.active_regimes      = {}
        # Dictionary: 'canonical/relative name' : boolean
        self.variables_to_report = {}

        self.timeHorizon       = 0.0
        self.reportingInterval = 0.0

        #self.daeSolver    = daeIDAS()
        #self.laSolver     = pySuperLU.daeCreateSuperLUSolver()
        #self.log          = daePythonStdOutLog()
        #self.dataReporter = daeTCPIPDataReporter()

    def dumpJSON(self, sort = True, indent = 2):
        data = {}
        data['timeHorizon']               = self.timeHorizon
        data['reportingInterval']         = self.reportingInterval
        data['parameters']                = self.parameters
        data['initial_conditions']        = self.initial_conditions
        data['analog_port_expressions']   = self.analog_port_expressions
        data['event_port_expressions']    = self.event_port_expressions
        data['active_regimes']            = self.active_regimes
        data['variables_to_report']       = self.variables_to_report

        return json.dumps(data, sort_keys = sort, indent = indent)

    def loadJSON(self, jsonContent):
        data = json.loads(jsonContent)
        print data

        if data.has_key('timeHorizon'):
            self.timeHorizon = float(data['timeHorizon'])

        if data.has_key('reportingInterval'):
            self.reportingInterval = float(data['reportingInterval'])

        if data.has_key('parameters'):
            temp = data['parameters']
            if isinstance(temp, dict):
                self.parameters = temp

        if data.has_key('initial_conditions'):
            temp = data['initial_conditions']
            if isinstance(temp, dict):
                self.initial_conditions = temp

        if data.has_key('analog_ports_expressions'):
            temp = data['analog_ports_expressions']
            if isinstance(temp, dict):
                self.analog_ports_expressions = temp

        if data.has_key('event_ports_expressions'):
            temp = data['event_ports_expressions']
            if isinstance(temp, dict):
                self.event_ports_expressions = temp

        if data.has_key('active_regimes'):
            temp = data['active_regimes']
            if isinstance(temp, list):
                self.active_regimes = temp

        if data.has_key('variables_to_report'):
            temp = data['variables_to_report']
            if isinstance(temp, list):
                self.variables_to_report = temp

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        print 'timeHorizon:', timeHorizon
        print 'reportingInterval:', reportingInterval
        print 'parameters:'
        printDictionary(self.parameters)
        print 'initial_conditions:'
        printDictionary(self.initial_conditions)
        print 'active_regimes:'
        printDictionary(self.active_regimes)
        print 'analog_ports_expressions:'
        printDictionary(self.analog_ports_expressions)
        print 'event_ports_expressions:'
        printDictionary(self.event_ports_expressions)
        print 'variables_to_report:'
        printDictionary(self.variables_to_report)

class ninemlTesterDataReporter(daeDataReporterLocal):
    def __init__(self):
        daeDataReporterLocal.__init__(self)
        self.ProcessName = ""

    def createReportData(self, testName, testNotes, tmp_folder = '/tmp'):
        fp8  = mpl.font_manager.FontProperties(family='sans-serif', style='normal', variant='normal', weight='normal', size=8)
        fp9  = mpl.font_manager.FontProperties(family='sans-serif', style='normal', variant='normal', weight='normal', size=9)
        fp11 = mpl.font_manager.FontProperties(family='sans-serif', style='normal', variant='normal', weight='normal', size=11)

        font = {'family' : 'sans-serif',
                'weight' : 'normal',
                'size'   : 8}
        mpl.rc('font', **font)
        params = {'axes.labelsize':  9,
                  'text.fontsize':   8,
                  'xtick.labelsize': 8,
                  'ytick.labelsize': 8,
                  'text.usetex': True}
        mpl.rcParams.update(params)

        plots = []
        for i, var in enumerate(self.Process.Variables):
            fileName   = var.Name
            fileName   = fileName.replace('.', '')
            fileName   = fileName.replace('_', '')
            fileName   = fileName + '.png'
            filePath   = tmp_folder + '/' + fileName
            title      = var.Name.split('.')[-1] + ' = f(t)'
            xAxisLabel = 't'
            yAxisLabel = var.Name
            xPoints    = var.TimeValues
            yPoints    = var.Values.reshape(len(var.Values))

            fig = plt.figure(figsize=(4, 3), dpi=(300))
            ax = fig.add_subplot(111)
            ax.plot(xPoints, yPoints)
            ax.set_title(title)
            ax.set_xlabel(xAxisLabel)
            #ax.set_ylabel(yAxisLabel)
            fig.savefig(filePath, dpi=(300))

            plots.append((var.Name, xPoints, yPoints, fileName, filePath))

        return (testName, testNotes, plots)

    def Connect(self, ConnectionString, ProcessName):
        return True

    def Disconnect(self):
        return True

    def IsConnected(self):
        return True

class nineml_daetools_simulation(daeSimulation):
    def __init__(self, model, **kwargs):
        daeSimulation.__init__(self)
        self.m = model

        dictIdentifiers, dictFunctions = getNineMLDictionaries(self.m)
        self.parser = ExpressionParser(dictIdentifiers, dictFunctions)

        # These dictionaries may contain unicode strings (if the input originated from the web form)
        # Therefore, str(...) should be used whenever a string is expected
        self._parameters               = kwargs.get('parameters',               {})
        self._initial_conditions       = kwargs.get('initial_conditions',       {})
        self._active_regimes           = kwargs.get('active_regimes',           {})
        self._variables_to_report      = kwargs.get('variables_to_report',      {})
        self._analog_ports_expressions = kwargs.get('analog_ports_expressions', {})
        self._event_ports_expressions  = kwargs.get('event_ports_expressions',  {})

        self.TimeHorizon               = float(kwargs.get('timeHorizon', 0.0))
        self.ReportingInterval         = float(kwargs.get('reportingInterval', 0.0))

    def SetUpParametersAndDomains(self):
        for paramName, value in self._parameters.items():
            paramName = str(paramName)
            parameter = getObjectFromCanonicalName(self.m, paramName, look_for_parameters = True)
            if parameter == None:
                raise RuntimeError('Could not locate parameter {0}'.format(paramName))
            self.Log.Message('  --> Set the parameter: {0} to: {1}'.format(paramName, value), 0)
            parameter.SetValue(value)
            
    def SetUpVariables(self):
        for varName, value in self._initial_conditions.items():
            varName = str(varName)
            variable = getObjectFromCanonicalName(self.m, varName, look_for_variables = True)
            if variable == None:
                raise RuntimeError('Could not locate variable {0}'.format(varName))
            self.Log.Message('  --> Set the variable: {0} to: {1}'.format(varName, value), 0)
            variable.SetInitialCondition(value)

        for portName, expression in self._analog_ports_expressions.items():
            portName = str(portName)
            if expression == None or expression == '':
                raise RuntimeError('An expression for the value of the analog port {0} cannot be empty'.format(portName))
            port = getObjectFromCanonicalName(self.m, portName, look_for_ports = True, look_for_reduceports = True)
            if port == None:
                raise RuntimeError('Could not locate port {0}'.format(portName))
            self.Log.Message('port: {0} = {1}'.format(portName, self.parser.parse(expression)), 0)

        for modelName, stateName in self._active_regimes.items():
            modelName = str(modelName)
            stateName = str(stateName)
            stn = getObjectFromCanonicalName(self.m, modelName + '.' + nineml_daetools_bridge.ninemlSTNRegimesName, look_for_stns = True)
            if stn == None:
                raise RuntimeError('Could not locate STN {0}'.format(nineml_daetools_bridge.ninemlSTNRegimesName))

            self.Log.Message('  --> Set the active state in the model: {0} to: {1}'.format(modelName, stateName), 0)
            stn.ActiveState = stateName

        self.m.SetReportingOn(False)
        for varName, value in self._variables_to_report.items():
            varName = str(varName)
            if value:
                variable = getObjectFromCanonicalName(self.m, varName, look_for_variables = True)
                if variable == None:
                    raise RuntimeError('Could not locate variable {0}'.format(varName))
                self.Log.Message('  --> Report the variable: {0}'.format(varName), 0)
                variable.ReportingOn = True
        
    def Run(self):
        spikeinput = getObjectFromCanonicalName(self.m, 'iaf_1coba.cobaExcit.spikeinput', look_for_eventports = True)

        dt = 0.1
        while self.CurrentTime < self.TimeHorizon:
            spikeinput.ReceiveEvent(0.0)
            self.Reinitialize()
            self.ReportData(self.CurrentTime)

            targetTime = self.CurrentTime + dt
            if targetTime > self.TimeHorizon:
                targetTime = self.TimeHorizon

            while self.CurrentTime < targetTime:
                t = self.NextReportingTime
                self.Log.Message('Integrating from {0} to {1} ...'.format(self.CurrentTime, t), 0)
                self.IntegrateUntilTime(t, eDoNotStopAtDiscontinuity)
                self.ReportData(self.CurrentTime)
          
if __name__ == "__main__":
    parameters = {
        'cobaExcit.q' : 3.0,
        'cobaExcit.tau' : 5.0,
        'cobaExcit.vrev' : 0.0,
        'iaf.cm' : 1,
        'iaf.gl' : 50,
        'iaf.taurefrac' : 0.008,
        'iaf.vreset' : -0.060,
        'iaf.vrest' : -0.060,
        'iaf.vthresh' : -0.040
    }
    initial_conditions = {
        'cobaExcit.g' : 0.0,
        'iaf.V' : -0.060,
        'iaf.tspike' : -1E99
    }
    analog_ports_expressions = {}
    event_ports_expressions = {}
    active_regimes = {
        'cobaExcit' : 'cobadefaultregime',
        'iaf' : 'subthresholdregime'
    }
    variables_to_report = {
        'cobaExcit.I' : True,
        'iaf.V' : True
    }
    timeHorizon       = 1
    reportingInterval = 0.001

    # Load the Component:
    nineml_comp  = TestableComponent('hierachical_iaf_1coba')()
    if not nineml_comp:
        raise RuntimeError('Cannot load NineML component')

    # Create Log, Solver, DataReporter and Simulation object
    log          = daeBaseLog()
    daesolver    = daeIDAS()

    model = nineml_daetools_bridge(nineml_comp.name, nineml_comp, None, '')
    simulation = nineml_daetools_simulation(model, timeHorizon              = timeHorizon,
                                                   reportingInterval        = reportingInterval,
                                                   parameters               = parameters,
                                                   initial_conditions       = initial_conditions,
                                                   active_regimes           = active_regimes,
                                                   analog_ports_expressions = analog_ports_expressions,
                                                   event_ports_expressions  = event_ports_expressions,
                                                   variables_to_report      = variables_to_report)
    datareporter = ninemlTesterDataReporter()

    # Set the time horizon and the reporting interval
    simulation.ReportingInterval = reportingInterval
    simulation.TimeHorizon       = timeHorizon

    # Connect data reporter
    simName = simulation.m.Name + strftime(" [%d.%m.%Y %H:%M:%S]", localtime())
    if(datareporter.Connect("", simName) == False):
        sys.exit()

    # Initialize the simulation
    simulation.Initialize(daesolver, datareporter, log)

    # Save the model reports for all models
    #simulation.m.SaveModelReport(simulation.m.Name + ".xml")
    #iaf  = findObjectInModel(simulation.m, 'iaf', look_for_models = True)
    #iaf.SaveModelReport(iaf.Name + ".xml")
    #coba = findObjectInModel(simulation.m, 'cobaExcit', look_for_models = True)
    #coba.SaveModelReport(coba.Name + ".xml")

    # Solve at time=0 (initialization)
    simulation.SolveInitial()

    # Run
    simulation.Run()
    simulation.Finalize()

    inspector = nineml_component_inspector()
    inspector.inspect(nineml_comp)

    tests_data = []
    tests_data.append(datareporter.createReportData('Dummy test', 'Dummy test notes', '.'))

    createLatexReport(inspector, tests_data, 'nineml-tex-template.tex', 'coba_iaf.tex')

    res = createPDF('coba_iaf.tex')
    subprocess.call(['evince', 'coba_iaf.pdf'], shell=False)
