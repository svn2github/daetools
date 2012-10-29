import sys, numpy, math, traceback
from daetools.pyDAE import *

class daeExpressionFormatter(object):
    def __init__(self):
        # Equation and condition node formatting settings

        # Index base in arrays:
        #  - Modelica, Fortran use 1
        #  - daetools, python, c/c++ use 0
        self.indexBase = 0

        self.useFlattenedNamesForAssignedVariables = False
        self.IDs      = {}
        self.indexMap = {}
        
        # Use relative names (relative to domains/parameters/variables model) or full canonical names
        # If we are in model root.comp1 then variables' names could be:
        #   if useRelativeNames is True:
        #       name = 'comp2.Var' (relative to parent comp1)
        #   else:
        #       name = 'root.comp1.comp2.Var' (full canonical name)
        self.useRelativeNames = True

        self.flattenIdentifiers = False

        self.domain                   = '{domain}[{index}]'

        self.parameter                = '{parameter}({indexes})'
        self.parameterIndexStart      = ''
        self.parameterIndexEnd        = ''
        self.parameterIndexDelimiter  = ','

        self.variable                 = '{variable}({indexes})'
        self.variableIndexStart       = ''
        self.variableIndexEnd         = ''
        self.variableIndexDelimiter   = ','

        self.assignedVariable         = '{variable}'
        
        # String format for the time derivative, ie. der(variable[1,2]) in Modelica
        # daetools use: variable.dt(1,2), gPROMS $variable(1,2) ...
        self.derivative               = '{variable}.dt({indexes})'
        self.derivativeIndexStart     = ''
        self.derivativeIndexEnd       = ''
        self.derivativeIndexDelimiter = ','

        # Constants
        self.constant = '{value}'
        
        # Logical operators
        self.AND   = '{leftValue} and {rightValue}'
        self.OR    = '{leftValue} or {rightValue}'
        self.NOT   = 'not {value}'

        self.EQ    = '{leftValue} == {rightValue}'
        self.NEQ   = '{leftValue} != {rightValue}'
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

        # Internal data: model will be set by the analyzer
        self.modelCanonicalName = None

    """
    formatQuantity(), formatQuantity(), and formatNumpyArray() are commonly defined in derived classes
    """
    def formatQuantity(self, quantity):
        # Formats constants/quantities in equations that have a value and units
        return '{{{0} {1}}}'.format(quantity.value, self.formatUnits(quantity.units))

    def formatUnits(self, units):
        # Format: m kg^2/(s^2) meaning m * kg**2 / s**2
        positive = []
        negative = []
        for u, exp in units.unitDictionary.items():
            if exp >= 0:
                if exp == 1:
                    positive.append('{0}'.format(u))
                elif int(exp) == exp:
                    positive.append('{0}^{1}'.format(u, int(exp)))
                else:
                    positive.append('{0}^{1}'.format(u, exp))

        for u, exp in units.unitDictionary.items():
            if exp < 0:
                if exp == -1:
                    negative.append('{0}'.format(u))
                elif int(exp) == exp:
                    negative.append('{0}^{1}'.format(u, int(math.fabs(exp))))
                else:
                    negative.append('{0}^{1}'.format(u, math.fabs(exp)))

        sPositive = ' '.join(positive)
        if len(negative) == 0:
            sNegative = ''
        elif len(negative) == 1:
            sNegative = '/' + ' '.join(negative)
        else:
            sNegative = '/(' + ' '.join(negative) + ')'

        return sPositive + sNegative

    def formatNumpyArray(self, arr):
        if isinstance(arr, (numpy.ndarray, list)):
            return '[' + ', '.join([self.formatNumpyArray(val) for val in arr]) + ']'
        else:
            return str(arr)
            
    def formatIdentifier(self, identifier):
        # Removes illegal characters from domains/parameters/variables/ports/models/... names
        return identifier.replace('&', '').replace(';', '').replace('(', '_').replace(')', '_').replace(',', '_').replace(' ', '')

    def flattenIdentifier(self, identifier):
        # Removes illegal characters from domains/parameters/variables/ports/models/... names
        return identifier.replace('.', '_').replace('(', '_').replace(')', '_').replace('[', '_').replace(']', '_').replace(',', '_').replace(' ', '')

    def formatDomain(self, domainCanonicalName, index, value):
        # ACHTUNG, ACHTUNG!! Take care of indexing of the domain index
        if self.useRelativeNames:
            name = daeGetRelativeName(self.modelCanonicalName, domainCanonicalName)
        else:
            name = domainCanonicalName

        if self.flattenIdentifiers:
            name = self.flattenIdentifier(name)

        indexes = str(index + self.indexBase)

        res = self.domain.format(domain = name, index = indexes, value = value)
        return res

    def formatParameter(self, parameterCanonicalName, domainIndexes, value):
        # ACHTUNG, ACHTUNG!! Take care of indexing of the domainIndexes
        if self.useRelativeNames:
            name = daeGetRelativeName(self.modelCanonicalName, parameterCanonicalName)
        else:
            name = parameterCanonicalName

        if self.flattenIdentifiers:
            name = self.flattenIdentifier(name)
        
        domainindexes = ''
        if len(domainIndexes) > 0:
            domainindexes = self.parameterIndexStart + self.parameterIndexDelimiter.join([str(di+self.indexBase) for di in domainIndexes]) + self.parameterIndexEnd
        
        res = self.parameter.format(parameter = name, indexes = domainindexes, value = value)
        return res

    def formatVariable(self, variableCanonicalName, domainIndexes, overallIndex):
        # ACHTUNG, ACHTUNG!! Take care of indexing of the overallIndex and the domainIndexes
        overall_ = overallIndex + self.indexBase
        if overallIndex in self.indexMap:
            block_ = self.indexMap[overallIndex] + self.indexBase
        else:
            block_ = -1

        if self.useFlattenedNamesForAssignedVariables and (self.IDs[overallIndex] == cnAssigned):
            name = daeGetRelativeName(self.modelCanonicalName, variableCanonicalName)
            name = self.flattenIdentifier(name)

            domainindexes = ''
            if len(domainIndexes) > 0:
                domainindexes = '_' + '_'.join([str(di+self.indexBase) for di in domainIndexes]) + '_'

            res = self.assignedVariable.format(variable = name+domainindexes, overallIndex = overall_, blockIndex = block_)

        else:
            if self.useRelativeNames:
                name = daeGetRelativeName(self.modelCanonicalName, variableCanonicalName)
            else:
                name = variableCanonicalName

            if self.flattenIdentifiers:
                name = self.flattenIdentifier(name)

            domainindexes = ''
            if len(domainIndexes) > 0:
                domainindexes = self.variableIndexStart + self.variableIndexDelimiter.join([str(di+self.indexBase) for di in domainIndexes]) + self.variableIndexEnd

            res = self.variable.format(variable = name, indexes = domainindexes, overallIndex = overall_, blockIndex = block_)

        return res

    def formatTimeDerivative(self, variableCanonicalName, domainIndexes, overallIndex, order):
        # ACHTUNG, ACHTUNG!! Take care of indexing of the overallIndex and the domainIndexes
        if self.useRelativeNames:
            name = daeGetRelativeName(self.modelCanonicalName, variableCanonicalName)
        else:
            name = variableCanonicalName

        if self.flattenIdentifiers:
            name = self.flattenIdentifier(name)

        overall_ = overallIndex + self.indexBase
        if overallIndex in self.indexMap:
            block_ = self.indexMap[overallIndex] + self.indexBase
        else:
            block_ = -1

        domainindexes = ''
        if len(domainIndexes) > 0:
            domainindexes = self.derivativeIndexStart + self.derivativeIndexDelimiter.join([str(di+self.indexBase) for di in domainIndexes]) + self.derivativeIndexEnd
        
        res = self.derivative.format(variable = name, indexes = domainindexes, overallIndex = overall_, blockIndex = block_)
        return res

    def formatRuntimeConditionNode(self, node):
        res = ''
        if isinstance(node, condUnaryNode):
            value = '(' + self.formatRuntimeConditionNode(node.Node) + ')'
            if node.LogicalOperator == eNot:
                res = self.NOT.format(value = value)
                #res = '{0}{1}'.format(self.NOT, n)
            else:
                raise RuntimeError('Not supported unary logical operator')

        elif isinstance(node, condBinaryNode):
            leftValue  = '(' + self.formatRuntimeConditionNode(node.LNode) + ')'
            rightValue = '(' + self.formatRuntimeConditionNode(node.RNode) + ')'

            if node.LogicalOperator == eAnd:
                res = self.AND.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.AND, right)

            elif node.LogicalOperator == eOr:
                res = self.OR.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.OR, right)

            else:
                raise RuntimeError('Not supported binary logical operator')

        elif isinstance(node, condExpressionNode):
            leftValue  = '(' + self.formatRuntimeNode(node.LNode) + ')'
            rightValue = '(' + self.formatRuntimeNode(node.RNode) + ')'

            if node.ConditionType == eNotEQ: # !=
                res = self.NEQ.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.NEQ, right)

            elif node.ConditionType == eEQ: # ==
                res = self.EQ.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.EQ, right)

            elif node.ConditionType == eGT: # >
                res = self.GT.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.GT, right)

            elif node.ConditionType == eGTEQ: # >=
                res = self.GTEQ.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.GTEQ, right)

            elif node.ConditionType == eLT: # <
                res = self.LT.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.LT, right)

            elif node.ConditionType == eLTEQ: # <=
                res = self.LTEQ.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.LTEQ, right)

            else:
                raise RuntimeError('Not supported condition type')
        else:
            raise RuntimeError('Not supported condition node: {0}'.format(type(node)))

        return res

    def formatRuntimeNode(self, node):
        res = ''
        if isinstance(node, adConstantNode):
            value = node.Quantity.value
            units = self.formatUnits(node.Quantity.units)
            res = self.constant.format(value = value, units = units)

        elif isinstance(node, adTimeNode):
            res = self.TIME

        elif isinstance(node, adUnaryNode):
            value = '(' + self.formatRuntimeNode(node.Node) + ')'

            if node.Function == eSign:
                res = self.SIGN.format(value = value)
                #res = '{0}{1}'.format(self.SIGN, n)

            elif node.Function == eSqrt:
                res = self.SQRT.format(value = value)
                #res = '{0}({1})'.format(self.SQRT, n)

            elif node.Function == eExp:
                res = self.EXP.format(value = value)
                #res = '{0}({1})'.format(self.EXP, n)

            elif node.Function == eLog:
                res = self.LOG10.format(value = value)
                #res = '{0}({1})'.format(self.LOG10, n)

            elif node.Function == eLn:
                res = self.LOG.format(value = value)
                #res = '{0}({1})'.format(self.LOG, n)

            elif node.Function == eAbs:
                res = self.ABS.format(value = value)
                #res = '{0}({1})'.format(self.ABS, n)

            elif node.Function == eSin:
                res = self.SIN.format(value = value)
                #res = '{0}({1})'.format(self.SIN, n)

            elif node.Function == eCos:
                res = self.COS.format(value = value)
                #res = '{0}({1})'.format(self.COS, n)

            elif node.Function == eTan:
                res = self.TAN.format(value = value)
                #res = '{0}({1})'.format(self.TAN, n)

            elif node.Function == eArcSin:
                res = self.ASIN.format(value = value)
                #res = '{0}({1})'.format(self.ASIN, n)

            elif node.Function == eArcCos:
                res = self.ACOS.format(value = value)
                #res = '{0}({1})'.format(self.ACOS, n)

            elif node.Function == eArcTan:
                res = self.ATAN.format(value = value)
                #res = '{0}({1})'.format(self.ATAN, n)

            elif node.Function == eCeil:
                res = self.CEIL.format(value = value)
                #res = '{0}({1})'.format(self.CEIL, n)

            elif node.Function == eFloor:
                res = self.FLOOR.format(value = value)
                #res = '{0}({1})'.format(self.FLOOR, n)

            else:
                raise RuntimeError('Not supported unary function')

        elif isinstance(node, adBinaryNode):
            leftValue  = '(' + self.formatRuntimeNode(node.LNode) + ')'
            rightValue = '(' + self.formatRuntimeNode(node.RNode) + ')'

            if node.Function == ePlus:
                res = self.PLUS.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.PLUS, right)

            elif node.Function == eMinus:
                res = self.MINUS.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.MINUS, right)

            elif node.Function == eMulti:
                res = self.MULTI.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.MULTI, right)

            elif node.Function == eDivide:
                res = self.DIVIDE.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0} {1} {2}'.format(left, self.DIVIDE, right)

            elif node.Function == ePower:
                res = self.POWER.format(leftValue = leftValue, rightValue = rightValue)
                #if self.POWER == 'pow': # use function pow()
                #    res = 'pow({0}, {1})'.format(left, right)
                #else:
                #    res = '{0} {1} {2}'.format(left, self.POWER, right)

            elif node.Function == eMin:
                res = self.MIN.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0}({1}, {2})'.format(self.MIN, left, right)

            elif node.Function == eMax:
                res = self.MAX.format(leftValue = leftValue, rightValue = rightValue)
                #res = '{0}({1}, {2})'.format(self.MAX, left, right)

            else:
                raise RuntimeError('Not supported binary function')

        elif isinstance(node, adScalarExternalFunctionNode):
            raise RuntimeError('External functions are not supported')

        elif isinstance(node, adVectorExternalFunctionNode):
            raise RuntimeError('External functions are not supported')

        elif isinstance(node, adDomainIndexNode):
            res = self.formatDomain(self.formatIdentifier(node.Domain.CanonicalName), node.Index, node.Value)

        elif isinstance(node, adRuntimeParameterNode):
            res = self.formatParameter(self.formatIdentifier(node.Parameter.CanonicalName), node.DomainIndexes, node.Value)

        elif isinstance(node, adRuntimeVariableNode):
            res = self.formatVariable(self.formatIdentifier(node.Variable.CanonicalName), node.DomainIndexes, node.OverallIndex)

        elif isinstance(node, adRuntimeTimeDerivativeNode):
            res = self.formatTimeDerivative(self.formatIdentifier(node.Variable.CanonicalName), node.DomainIndexes, node.OverallIndex, node.Order)

        else:
            raise RuntimeError('Not supported node')

        return res
