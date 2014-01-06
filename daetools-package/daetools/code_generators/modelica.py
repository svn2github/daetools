import os, sys, numpy, math, traceback
from daetools.pyDAE import *
from .formatter import daeExpressionFormatter
from .analyzer import daeCodeGeneratorAnalyzer
from .code_generator import daeCodeGenerator


portTemplate = """\
connector %(port)s
%(variables)s
end %(port)s;

"""

modelTemplate = """\
class %(model)s
/* Import libs */
  import Modelica.Math.*;
%(parameters)s
%(variables)s
%(ports)s
%(components)s

equation
%(port_connections)s
%(equations)s
%(stns)s
end %(model)s;

"""

wrapperTemplate = """\
class %(model)s_simulation
/* CODE GENERATOR WARNINGS:
%(warnings)s
*/
  annotation(experiment(StartTime = %(start_time)s, StopTime = %(end_time)s, Tolerance = %(tolerance)s));
  
%(model_instance)s

%(initial_equation)s
end %(model)s_simulation;

"""


class daeExpressionFormatter_Modelica(daeExpressionFormatter):
    def __init__(self):
        daeExpressionFormatter.__init__(self)

        # Index base in arrays
        self.indexBase = 1

        self.useFlattenedNamesForAssignedVariables = False
        self.IDs      = {}
        self.indexMap = {}

        # Use relative names and do not flatten identifiers
        self.useRelativeNames   = True
        self.flattenIdentifiers = False

        self.domain                   = '{domain}[{index}]'

        self.parameter                = '{parameter}{indexes}'
        self.parameterIndexStart      = '['
        self.parameterIndexEnd        = ']'
        self.parameterIndexDelimiter  = ','

        self.variable                 = '{variable}{indexes}'
        self.variableIndexStart       = '['
        self.variableIndexEnd         = ']'
        self.variableIndexDelimiter   = ','

        self.assignedVariable         = '{variable}'
        
        self.feMatrixItem             = '{value}'
        self.feVectorItem             = '{value}'

        # String format for the time derivative, ie. der(variable[1,2]) in Modelica
        # daetools use: variable.dt(1,2), gPROMS $variable(1,2) ...
        self.derivative               = 'der({variable}{indexes})'
        self.derivativeIndexStart     = '['
        self.derivativeIndexEnd       = ']'
        self.derivativeIndexDelimiter = ','

        # Constants
        self.constant = '{value}'

        # External functions
        self.scalarExternalFunction = '{name}()'
        self.vectorExternalFunction = '{name}()'

        # Logical operators
        self.AND   = '{leftValue} and {rightValue}'
        self.OR    = '{leftValue} or {rightValue}'
        self.NOT   = 'not {value}'

        self.EQ    = '{leftValue} == {rightValue}'
        self.NEQ   = '{leftValue} <> {rightValue}'
        self.LT    = '{leftValue} < {rightValue}'
        self.LTEQ  = '{leftValue} <= {rightValue}'
        self.GT    = '{leftValue} > {rightValue}'
        self.GTEQ  = '{leftValue} >= {rightValue}'

        # Mathematical operators
        self.SIGN   = '-{value}'

        self.PLUS   = '{leftValue} + {rightValue}'
        self.MINUS  = '{leftValue} - {rightValue}'
        self.MULTI  = '{leftValue} * {rightValue}'
        self.DIVIDE = '{leftValue} / {rightValue}'
        self.POWER  = '{leftValue} ^ {rightValue}'

        # Mathematical functions
        self.SIN    = 'sin({value})'
        self.COS    = 'cos({value})'
        self.TAN    = 'tan({value})'
        self.ASIN   = 'asin({value})'
        self.ACOS   = 'acos({value})'
        self.ATAN   = 'atan({value})'
        self.EXP    = 'exp({value})'
        self.SQRT   = 'sqrt({value})'
        self.LOG    = 'log({value})'
        self.LOG10  = 'log10({value})'
        self.FLOOR  = 'floor({value})'
        self.CEIL   = 'ceil({value})'
        self.ABS    = 'abs({value})'

        self.MIN    = 'min({leftValue}, {rightValue})'
        self.MAX    = 'max({leftValue}, {rightValue})'

        # Current time in simulation
        self.TIME   = 'time'

    def formatNumpyArray(self, arr):
        if isinstance(arr, (numpy.ndarray, list)):
            return '{' + ', '.join([self.formatNumpyArray(val) for val in arr]) + '}'
        else:
            return str(arr)

    def formatQuantity(self, quantity):
        # Formats constants/quantities in equations that have a value and units
        return str(quantity.value)

    def formatUnits(self, units):
        # Format: m.kg2/s-2 meaning m * kg**2 / s**2
        positive = []
        negative = []
        for u, exp in list(units.toDict().items()):
            if exp >= 0:
                if exp == 1:
                    positive.append('{0}'.format(u))
                elif int(exp) == exp:
                    positive.append('{0}{1}'.format(u, int(exp)))
                else:
                    positive.append('{0}{1}'.format(u, exp))

        for u, exp in list(units.toDict().items()):
            if exp < 0:
                if exp == -1:
                    negative.append('{0}'.format(u))
                elif int(exp) == exp:
                    negative.append('{0}{1}'.format(u, int(math.fabs(exp))))
                else:
                    negative.append('{0}{1}'.format(u, math.fabs(exp)))

        sPositive = '.'.join(positive)
        if len(negative) == 0:
            sNegative = ''
        elif len(negative) == 1:
            sNegative = '/' + '.'.join(negative)
        else:
            sNegative = '/(' + '.'.join(negative) + ')'

        return sPositive + sNegative
        
class daeCodeGenerator_Modelica(daeCodeGenerator):
    def __init__(self):
        self.wrapperInstanceName = ''
        self.defaultIndent       = '  '
        self.warnings            = []
        self.topLevelModel       = None
        self.simulation          = None
        
        self.exprFormatter = daeExpressionFormatter_Modelica()
        self.analyzer      = daeCodeGeneratorAnalyzer()

    def generateModel(self, model, filename = None):
        if not model:
            raise RuntimeError('Invalid model object')

        self.parametersValues        = {}
        self.assignedVariables       = {}
        self.assignedDistrVariables  = {}
        self.initialConditions       = {}
        self.initiallyActiveStates   = {}
        self.warnings                = []
        self.simulation              = None
        self.topLevelModel           = model

        indent   = 1
        s_indent = indent * self.defaultIndent

        data = self.analyzer.analyzeModel(model)
        result = self._processModel(data, indent)

        if filename:
            f = open(filename, "w")
            f.write(result)
            f.close()

        if len(self.warnings) > 0:
            print('CODE GENERATOR WARNINGS:')
            print('\n'.join(self.warnings))

        return result

    def generatePort(self, port, filename = None):
        if not port:
            raise RuntimeError('Invalid port object')

        self.parametersValues        = {}
        self.assignedVariables = {}
        self.initialConditions       = {}
        self.initiallyActiveStates   = {}
        self.warnings                = []
        self.simulation              = None
        self.topLevelModel           = port.Model

        indent   = 1
        s_indent = indent * self.defaultIndent

        data = self.analyzer.analyzePort(port)
        result = self._processPort(data, indent)

        if filename:
            f = open(filename, "w")
            f.write(result)
            f.close()

        if len(self.warnings) > 0:
            print('CODE GENERATOR WARNINGS:')
            print('\n'.join(self.warnings))

        return result

    def generateSimulation(self, simulation, directory):
        if not simulation:
            raise RuntimeError('Invalid simulation object')
        if not os.path.isdir(directory):
            os.makedirs(directory)

        self.parametersValues        = {}
        self.assignedVariables       = {}
        self.assignedDistrVariables  = {}
        self.initialConditions       = {}
        self.initiallyActiveStates   = {}
        self.warnings                = []
        self.simulation              = simulation
        self.topLevelModel           = simulation.m
        
        # Achtung, Achtung!!
        # wrapperInstanceName should not be stripped 
        # of illegal characters, since it is used to get relative names
        self.wrapperInstanceName     = simulation.m.Name

        indent   = 1
        s_indent = indent * self.defaultIndent

        self.analyzer.analyzeSimulation(simulation)

        # First generate individual ports and models used in the simulation
        for port_class, info in self.analyzer.ports:
            code = self._processPort(info['data'], indent)
            filename = os.path.join(directory, '%s.mo' % port_class)
            f = open(filename, "w")
            f.write(code)
            f.close()

        for model_class, info in self.analyzer.models:
            code = self._processModel(info['data'], indent)
            filename = os.path.join(directory, '%s.mo' % model_class)
            f = open(filename, "w")
            f.write(code)
            f.close()

        # Next, generate a wrapper model where the simulation's runtime information are specified
        self._generateRuntimeInformation(self.analyzer.runtimeInformation)

        warnings = '\n'.join(self.warnings)
        
        model_instance = s_indent + '{0} {1}('.format(self.topLevelModel.__class__.__name__, self.exprFormatter.formatIdentifier(self.wrapperInstanceName))
        indent = ' ' * len(model_instance)

        params              = ['{0} = {1}'               .format(key, value) for key, value in list(self.parametersValues.items())]
        assigned_vars       = ['{0}(fixed = true) = {1}' .format(key, value) for key, value in list(self.assignedVariables.items())]
        assigned_distr_vars = ['{0}{1} = {2};'           .format(s_indent, key, value) for key, value in list(self.assignedDistrVariables.items())]
        init_conds          = ['{0}{1} := {2};'          .format(s_indent, key, value) for key, value in list(self.initialConditions.items())]
        init_states         = ['{0}{1} := {2};'          .format(s_indent, key, value) for key, value in list(self.initiallyActiveStates.items())]

        params_and_assigned_vars = params + assigned_vars
        params_and_assigned_vars.sort(key=str.lower)
        assigned_distr_vars.sort(key=str.lower)
        init_conds.sort(key=str.lower)
        init_states.sort(key=str.lower)

        join_format = ',\n%s' % indent
        arguments = join_format.join(params_and_assigned_vars)

        initial_equations = ''
        if len(assigned_distr_vars) > 0:
            initial_equations  = 'equation\n/* Assigned distributed variables */\n' + '\n'.join(assigned_distr_vars)

        if len(init_states) > 0 or len(init_conds) > 0:
            initial_equations += '\n\ninitial algorithm\n'

        if len(init_states) > 0:
            initial_equations += '\n/* Initially active states */\n' + '\n'.join(init_states)

        if len(init_conds) > 0:
            initial_equations += '\n/* Initial conditions */\n' + '\n'.join(init_conds)

        model_instance = model_instance + arguments + ');'
        dictModel = {
                    'model'            : self.topLevelModel.__class__.__name__,
                    'model_instance'   : model_instance,
                    'initial_equation' : initial_equations,
                    'start_time'       : 0.0,
                    'end_time'         : self.simulation.TimeHorizon,
                    'tolerance'        : daeGetConfig().GetFloat('daetools.IDAS.relativeTolerance', 1e-5),
                    'warnings'         : warnings
                    }
        resultWrapper = wrapperTemplate % dictModel

        filename = os.path.join(directory, 'simulation-%s.mo' % self.topLevelModel.Name)
        f = open(filename, "w")
        f.write(resultWrapper)
        f.close()

        if len(self.warnings) > 0:
            print('CODE GENERATOR WARNINGS:')
            print(warnings)

    def _processPort(self, data, indent):
        sVariables  = []
        s_indent = indent * self.defaultIndent

        if len(data['Domains']) > 0:
            raise RuntimeError('Modelica ports cannot contain domains')

        if len(data['Parameters']) > 0:
            raise RuntimeError('Modelica ports cannot contain parameters')

        # Variables
        for variable in data['Variables']:
            if len(variable['Domains']) > 0:
                raise RuntimeError('Modelica ports cannot contain distributed variables')

            name        = self.exprFormatter.formatIdentifier(variable['Name'])
            units       = self.exprFormatter.formatUnits(data['VariableTypes'][variable['Type']]['Units'] if variable['Type'] in data['VariableTypes'] else '')
            domains     = ''
            attributes  = '(unit="{units}")'.format(units = units)
            description = variable['Description']

            varTemplate = s_indent + 'Real{domains} {name}{attributes} "{description}";'
            sVariables.append(varTemplate.format(name = name,
                                                 domains = domains,
                                                 attributes = attributes,
                                                 description = description))

        dictPort = {
                     'port'      : data['Class'],
                     'variables' : '\n'.join(sVariables)
                   }
        result = portTemplate % dictPort
        return result
    
    def _processModel(self, data, indent):
        sParameters         = []
        sVariables          = []
        sPorts              = []
        sComponents         = []
        sPortConnections    = []
        sEquations          = []
        sSTNs               = []
        sOnConditionActions = []
        sOnEventActions     = []

        s_indent = indent * self.defaultIndent

        # Achtung, Achtung!!
        # exprFormatter.modelCanonicalName should not be stripped 
        # of illegal characters, since it is used to get relative names
        self.exprFormatter.modelCanonicalName = data['CanonicalName']
        
        # These concepts are not supported by the Modelica code-generator:
        #  - EventPortConnections
        #  - EventPorts
        #  - PortArrays
        #  - ComponentArrays
        #  - OnEventActions
        if len(data['EventPortConnections']) > 0:
            raise RuntimeError('Modelica code generator: EventPortConnections are not supported')
        if len(data['EventPorts']) > 0:
            raise RuntimeError('Modelica code generator: EventPorts are not supported')
        if len(data['PortArrays']) > 0:
            raise RuntimeError('Modelica code generator: PortArrays are not supported')
        if len(data['ComponentArrays']) > 0:
            raise RuntimeError('Modelica code generator: ComponentArrays are not supported')
        if len(data['OnEventActions']) > 0:
            raise RuntimeError('Modelica code generator: OnEventActions are not supported')
        
        # Domains
        for domain in data['Domains']:
            name        = self.exprFormatter.formatIdentifier(domain['Name'])
            units       = self.exprFormatter.formatUnits(domain['Units'])
            noPoints    = domain['NumberOfPoints']
            domains     = '[{name}_np]'.format(name = name)
            attributes  = '(unit="{units}")'.format(units = units)
            description = domain['Description']
            
            domTemplate   = s_indent + 'parameter Integer {name}_np "Number of points in domain {name}";'
            paramTemplate = s_indent + 'parameter Real{domains} {name}{attributes} "{description}";\n'
            
            sParameters.append(domTemplate.format(name = name,
                                                  noPoints = noPoints))
            sParameters.append(paramTemplate.format(name = name,
                                                    domains = domains,
                                                    attributes = attributes,
                                                    description = description))
                                                                                                        
        # Parameters
        for parameter in data['Parameters']:
            name        = self.exprFormatter.formatIdentifier(parameter['Name'])
            units       = self.exprFormatter.formatUnits(parameter['Units'])
            domains = ''
            if len(parameter['Domains']) > 0:
                domains = '[{0}]'.format(','.join(self.exprFormatter.formatIdentifier(d)+'_np' for d in parameter['Domains']))
            attributes  = '(unit="{units}")'.format(units = units)
            description = parameter['Description']

            paramTemplate = s_indent + 'parameter Real{domains} {name}{attributes} "{description}";'
            sParameters.append(paramTemplate.format(name = name,
                                                    domains = domains,
                                                    attributes = attributes,
                                                    description = description))

        # Variables
        for variable in data['Variables']:
            name        = self.exprFormatter.formatIdentifier(variable['Name'])
            units       = self.exprFormatter.formatUnits(data['VariableTypes'][variable['Type']]['Units'] if variable['Type'] in data['VariableTypes'] else '')
            if len(variable['Domains']) > 0:
                domains = '[{0}]'.format(','.join(self.exprFormatter.formatIdentifier(d)+'_np' for d in variable['Domains']))
                if variable['RuntimeHint'] == 'differential':
                    attributes  = '(start=0.0, unit="{units}")'.format(units = units)
                else:
                    attributes  = '(unit="{units}")'.format(units = units)
            else:
                domains = ''
                if variable['RuntimeHint'] == 'differential':
                    attributes  = '(start=0.0, unit="{units}")'.format(units = units)
                else:
                    attributes  = '(unit="{units}")'.format(units = units)
            description = variable['Description']

            varTemplate = s_indent + 'Real{domains} {name}{attributes} "{description}";'
            sVariables.append(s_indent + '/* type hint: {0} variable */'.format(variable['RuntimeHint']))
            sVariables.append(varTemplate.format(name = name,
                                                 domains = domains,
                                                 attributes = attributes,
                                                 description = description))
        
        # Equations
        sEquations.extend(self._processEquations(data['Equations'], indent))

        # PortConnections
        for port_connection in data['PortConnections']:
            portconnTemplate = s_indent + 'connect({portFrom}, {portTo});'
            sPortConnections.append(portconnTemplate.format(portFrom = port_connection['PortFrom'],
                                                            portTo = port_connection['PortTo']))

        # Ports
        for port in data['Ports']:
            class_      = port['Class']
            name        = port['Name']
            if port['Type'] == 'eInletPort':
                portType = 'inlet'
            elif port['Type'] == 'eOutletPort':
                portType = 'outlet'
            else:
                raise RuntimeError('Invalid port type: %s (must be either inlet or outlet)' % name)
            description = port['Description']

            portTemplate = s_indent + '{class_} {name} "{description}";'
            sPorts.append(s_indent + '/* type: {0} port */'.format(portType))
            sPorts.append(portTemplate.format(class_ = class_,
                                              name = name,
                                              description = description))

        # StateTransitionNetworks
        sSTNs.extend(self._processSTNs(data['STNs'], indent, sVariables))

        # OnConditionActions
        sOnConditionActions.extend(self._processOnConditionActions(data['OnConditionActions'], indent))

        # OnEventActions
        sOnEventActions.extend(self._processOnEventActions(data['OnEventActions'], indent))

        # Components
        for component in data['Components']:
            class_      = component['Class']
            name        = component['Name']
            description = component['Description']

            compTemplate = s_indent + '{class_} {name} "{description}";'
            sComponents.append(compTemplate.format(class_ = class_,
                                                   name = name,
                                                   description = description))
                
        # Put all together                                                                    
        _ports            = ('\n/* Ports */      \n' if len(sPorts)      else '') + '\n'.join(sPorts)
        _parameters       = ('\n/* Parameters */ \n' if len(sParameters) else '') + '\n'.join(sParameters)
        _variables        = ('\n/* Variables */  \n' if len(sVariables)  else '') + '\n'.join(sVariables)
        _components       = ('\n/* Components */ \n' if len(sComponents) else '') + '\n'.join(sComponents)
        _port_connections =                                                         '\n'.join(sPortConnections)
        _equations        =                                                         '\n'.join(sEquations)
        _stns             =                                                         '\n'.join(sSTNs)
        dictModel = {
                      'model'            : data['Class'],
                      'ports'            : _ports,
                      'parameters'       : _parameters,
                      'variables'        : _variables,
                      'components'       : _components,
                      'port_connections' : _port_connections,
                      'equations'        : _equations,
                      'stns'             : _stns
                    }
        result = modelTemplate % dictModel
        return result

    def _processOnConditionActions(self, on_condition_actions):
        pass
    
    def _processOnEventActions(self, OnEventActions):
        if len(OnEventActions > 0):
            raise RuntimeError()
        
    def _processEquations(self, Equations, indent):
        sEquations = []
        s_indent = indent * self.defaultIndent
        eqTemplate = s_indent + '{residual} = 0;'
        for equation in Equations:
            description = equation['Description']
            sEquations.append(s_indent + '/* {0} */'.format(description))
            for eeinfo in equation['EquationExecutionInfos']:
                residual = self.exprFormatter.formatRuntimeNode(eeinfo['ResidualRuntimeNode'])
                sEquations.append(eqTemplate.format(residual = residual))
        return sEquations
                    
    def _processSTNs(self, STNs, indent, sVariables):
        sSTNs  = []
        sWhens = []
        s_indent  = indent * self.defaultIndent
        
        for stn in STNs:
            nStates = len(stn['States'])
            if stn['Class'] == 'daeIF':
                for i, state in enumerate(stn['States']):
                    """
                    relativeName    = daeGetRelativeName(self.wrapperInstanceName, stn['CanonicalName'])
                    relativeName    = self.exprFormatter.formatIdentifier(relativeName)
                    stnVariableName = self.exprFormatter.flattenIdentifier(relativeName) + '_ifstn'
                    description     = stn['Description']
                    states          = ', '.join(st['Name'] for st in stn['States'])
                    activeState     = stn['ActiveState']

                    varTemplate = 'char* {name}; /* States: [{states}] ({description}) */'
                    self.stnDefs.append(varTemplate.format(name = stnVariableName,
                                                        states = states,
                                                        description = description))
                    varTemplate = '_m_->{name} = "{activeState}";'
                    self.initiallyActiveStates.append(varTemplate.format(name = stnVariableName,
                                                                        activeState = activeState))
                    """
                    
                    # Not all states have state_transitions ('else' state has no state transitions)
                    on_condition_action = None
                    if i == 0:
                        # There is only one OnConditionAction in IF
                        on_condition_action = state['OnConditionActions'][0]
                        condition = self.exprFormatter.formatRuntimeConditionNode(on_condition_action['ConditionRuntimeNode'])
                        sSTNs.append(s_indent + 'if {0} then'.format(condition))

                    elif (i > 0) and (i < nStates - 1):
                        # There is only one OnConditionAction in IF
                        on_condition_action = state['OnConditionActions'][0]
                        condition = self.exprFormatter.formatRuntimeConditionNode(on_condition_action['ConditionRuntimeNode'])
                        sSTNs.append(s_indent + 'else if {0} then'.format(condition))

                    else:
                        sSTNs.append(s_indent + 'else')

                    # Process equations
                    sSTNs.extend(self._processEquations(state['Equations'], indent+1))
                    
                    # Process nested STNs (if any)
                    sSTNs.extend(self._processSTNs(state['NestedSTNs'], indent+1, sVariables))

                sSTNs.append(s_indent + 'end if;')

            elif stn['Class'] == 'daeSTN':
                stnVariableName = stn['Name'] + '_stn'
                activeState     = stn['ActiveState']
                
                sVariables.append(s_indent + '/* State transition network */')
                sVariables.append(s_indent + 'String {0};'.format(stnVariableName))

                nStates = len(stn['States'])
                for i, state in enumerate(stn['States']):
                    if i == 0:
                        sSTNs.append(s_indent + '/* STN {0}: {1}*/'.format(stnVariableName, ', '.join([st['Name'] for st in stn['States']])))
                        sSTNs.append(s_indent + 'if {0} == "{1}" then'.format(stnVariableName, state['Name']))
                    elif (i > 0) and (i < nStates - 1):
                        sSTNs.append(s_indent + 'elseif {0} == "{1}" then'.format(stnVariableName, state['Name']))
                    else:
                        sSTNs.append(s_indent + 'else')

                    sSTNs.extend(self._processEquations(state['Equations'], indent+1))

                    for state_transition in state['StateTransitions']:
                        condition = self.exprFormatter.formatRuntimeConditionNode(state_transition['ConditionRuntimeNode'])
                        if len(sWhens) == 0:
                            sWhens.append(s_indent + '/* State transitions of {0} */'.format(stnVariableName))
                            sWhens.append(s_indent + 'when ({0} == "{1}") and ({2}) then'.format(stnVariableName, state['Name'], condition))
                        else:
                            sWhens.append(s_indent + 'elsewhen ({0} == "{1}") and ({2}) then'.format(stnVariableName, state['Name'], condition))
                        sWhens.extend(self._processActions(state_transition['Actions'], indent+1))

                    if len(state['NestedSTNs']) > 0:
                        raise RuntimeError('Modelica code cannot be generated for nested state transition networks')

                if len(sWhens) > 0:
                    sWhens.append(s_indent + 'end when;')

                sSTNs.append(s_indent + 'end if;')
                sSTNs.extend(sWhens)

        return sSTNs

    def _processActions(self, Actions, indent):
        sActions = []
        s_indent  = indent * self.defaultIndent

        for action in Actions:
            if action['Type'] == 'eChangeState':
                stnVariableName = action['STN'] + '_stn'
                stateTo         = action['StateTo']
                sActions.append(s_indent + '{0} = "{1}";'.format(stnVariableName, stateTo))

            elif action['Type'] == 'eSendEvent':
                self.warnings.append('Modelica code cannot be generated for SendEvent actions - the model will not work as expected!!')

            elif action['Type'] == 'eReAssignOrReInitializeVariable':
                relativeName = daeGetRelativeName(self.wrapperInstanceName, action['VariableWrapper'].Variable.CanonicalName)
                relativeName = self.exprFormatter.formatIdentifier(relativeName)
                domainIndexes = action['VariableWrapper'].DomainIndexes
                node          = action['RuntimeNode']
                strDomainIndexes = ''
                if len(domainIndexes) > 0:
                    strDomainIndexes = '[' + ','.join() + ']'
                variableName = relativeName + strDomainIndexes
                value = self.exprFormatter.formatRuntimeNode(node)
                sActions.append(s_indent + 'reinit({0}, {1});'.format(variableName, value))

            elif action['Type'] == 'eUserDefinedAction':
                self.warnings.append('Modelica code cannot be generated for UserDefined actions - the model will not work as expected!!')

            else:
                pass
       
        return sActions

    def _generateRuntimeInformation(self, runtimeInformation):
        for domain in runtimeInformation['Domains']:
            relativeName = daeGetRelativeName(self.wrapperInstanceName, domain['CanonicalName'])
            relativeName = self.exprFormatter.formatIdentifier(relativeName)
            self.parametersValues[relativeName+'_np'] = str(domain['NumberOfPoints'])
            self.parametersValues[relativeName]       = self.exprFormatter.formatNumpyArray(domain['Points']) # Numpy array
            
        for parameter in runtimeInformation['Parameters']:
            relativeName = daeGetRelativeName(self.wrapperInstanceName, parameter['CanonicalName'])
            relativeName = self.exprFormatter.formatIdentifier(relativeName)
            if parameter['NumberOfPoints'] == 1:
                self.parametersValues[relativeName] = str(parameter['Values'])
            else:
                self.parametersValues[relativeName] = self.exprFormatter.formatNumpyArray(parameter['Values']) # Numpy array

        for variable in runtimeInformation['Variables']:
            canonicalName = self.exprFormatter.formatIdentifier(variable['CanonicalName'])
            relativeName = daeGetRelativeName(self.wrapperInstanceName, variable['CanonicalName'])
            relativeName = self.exprFormatter.formatIdentifier(relativeName)
            n = variable['NumberOfPoints']
            if n == 1:
                ID    = int(variable['IDs'])        # cnDifferential, cnAssigned or cnAlgebraic
                value = float(variable['Values'])   # numpy float

                if ID == cnDifferential:
                    self.initialConditions[canonicalName] = value
                elif ID == cnAssigned:
                    self.assignedVariables[relativeName] = value

            else:
                for i in range(0, n):
                    domIndexes = tuple(variable['DomainsIndexesMap'][i])  # list of integers
                    ID         = int(variable['IDs'][domIndexes])         # cnDifferential, cnAssigned or cnAlgebraic
                    value      = float(variable['Values'][domIndexes])    # numpy float
                    # ACHTUNG, ACHTUNG!!
                    # Modelica uses 1-based indexing
                    name = canonicalName + '[' + ','.join([str(index + self.exprFormatter.indexBase) for index in domIndexes]) + ']'

                    if ID == cnDifferential:
                        self.initialConditions[name] = value
                    elif ID == cnAssigned:
                        self.equationsForAssignedVariables[name] = value

        for stn in runtimeInformation['STNs']:
            if stn['Class'] == 'daeSTN':
                stnVariableName = stn['CanonicalName'] + '_stn'
                self.initiallyActiveStates[stnVariableName] = '"' + stn['ActiveState'] + '"'
