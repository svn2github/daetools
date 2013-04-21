**************
Module pyCore
**************

.. py:module:: pyCore
.. py:currentmodule:: pyCore

Overview
==========
Trt mrt.

Autodifferentiation and equation evaluation tree support
========================================================

Classes
-------

.. autosummary::
    adouble
    adouble_array
    daeCondition

.. autoclass:: pyCore.adouble
    :members:

.. autoclass:: pyCore.adouble_array
    :members:

    .. automethod:: __len__
    .. automethod:: __getitem__
    .. automethod:: __setitem__

.. autoclass:: pyCore.daeCondition
    :members:

    .. method:: __or__((daeCondition)self, (daeCondition)right) -> daeCondition

    Logical operator ``or``
    
    .. method:: __and__((daeCondition)self, (daeCondition)right) -> daeCondition

    Logical operator ``and``

Mathematical functions
----------------------
.. autosummary::
    :nosignatures:

    Exp
    Log
    Log10
    Sqrt
    Sin
    Cos
    Tan
    ASin
    ACos
    ATan
    Sinh
    Cosh
    Tanh
    ASinh
    ACosh
    ATanh
    ATan2
    Ceil
    Floor
    Pow
    Abs
    Min
    Max

.. autofunction:: pyCore.Exp
.. autofunction:: pyCore.Log
.. autofunction:: pyCore.Log10
.. autofunction:: pyCore.Sqrt
.. autofunction:: pyCore.Sin
.. autofunction:: pyCore.Cos
.. autofunction:: pyCore.Tan
.. autofunction:: pyCore.ASin
.. autofunction:: pyCore.ACos
.. autofunction:: pyCore.ATan
.. autofunction:: pyCore.Sinh
.. autofunction:: pyCore.Cosh
.. autofunction:: pyCore.Tanh
.. autofunction:: pyCore.ASinh
.. autofunction:: pyCore.ACosh
.. autofunction:: pyCore.ATanh
.. autofunction:: pyCore.ATan2
.. autofunction:: pyCore.Ceil
.. autofunction:: pyCore.Floor
.. autofunction:: pyCore.Pow
.. autofunction:: pyCore.Abs
.. autofunction:: pyCore.Min
.. autofunction:: pyCore.Max

Modelling concepts
====================
...

Enumerations
------------
.. autosummary::
    daeeDomainType
    daeeParameterType
    daeePortType
    daeeDiscretizationMethod
    daeeDomainBounds
    daeeInitialConditionMode
    daeeDomainIndexType
    daeeRangeType
    daeIndexRangeType
    daeeOptimizationVariableType
    daeeModelLanguage
    daeeConstraintType
    daeeUnaryFunctions
    daeeBinaryFunctions
    daeeSpecialUnaryFunctions
    daeeLogicalUnaryOperator
    daeeLogicalBinaryOperator
    daeeConditionType
    daeeActionType
    daeeEquationType
    daeeModelType

.. autoclass:: pyCore.daeeDomainType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeParameterType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeePortType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeDiscretizationMethod
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeDomainBounds
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeInitialConditionMode
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeDomainIndexType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeRangeType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeIndexRangeType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeOptimizationVariableType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeModelLanguage
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeConstraintType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeUnaryFunctions
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeBinaryFunctions
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeSpecialUnaryFunctions
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeLogicalUnaryOperator
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeLogicalBinaryOperator
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeConditionType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeActionType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeEquationType
    :members:
    :undoc-members:
    :exclude-members: names, values

.. autoclass:: pyCore.daeeModelType
    :members:
    :undoc-members:
    :exclude-members: names, values

   
Classes
--------
.. autosummary::
    daeVariableType
    daeDomain
    daeParameter
    daeVariable
    daeModel
    daeSTN
    daeIF
    daeEquation
    daeState
    daeStateTransition
    daePort
    daeEventPort
    daePortConnection
    daeScalarExternalFunction
    daeVectorExternalFunction
    daeDomainIndex
    daeIndexRange
    daeArrayRange
    daeDEDI
    daeAction
    daeOptimizationVariable
    daeObjectiveFunction
    daeOptimizationConstraint
    daeMeasuredVariable
    daeEquationExecutionInfo


.. autoclass:: pyCore.daeVariableType
    :members:
    :undoc-members:

    .. automethod:: __init__
    
.. autoclass:: pyCore.daeObject
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeDomain
    :members:
    :undoc-members:

    .. automethod:: __init__
    .. automethod:: __getitem__
    .. automethod:: __call__

.. autoclass:: pyCore.daeParameter
    :members:
    :undoc-members:
    :exclude-members: GetValue, SetValue, GetQuantity, SetValues, __call__, array

    .. method:: GetValue((daeParameter)self, [(int)index1[, ...[, (int)index8]]]) -> float

        Gets the value of the parameter at the specified domain indexes. How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the parameter is distributed on.
    
    .. method:: GetQuantity((daeParameter)self, [(int)index1[, ...[, (int)index8]]]) -> quantity

        Gets the value of the parameter at the specified domain indexes as the ``quantity`` object (with value and units).
        How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the parameter is distributed on.

    .. method:: SetValue((daeParameter)self, [(int)index1[, ...[, (int)index8]]], (object)value) -> None

        Sets the value of the parameter at the specified domain indexes (as ``float`` or ``quantity``). How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the parameter is distributed on.

    .. method:: SetValues((daeParameter)self, (float)values) -> None

        Sets all values of the parameter (as ``float`` or ``quantity``).
    
    .. method:: array((daeParameter)self, [(object)index1[, ...[, (object)index8]]]) -> adouble_array

        Gets the array of parameter's values at the specified domain indexes (used to build equation residuals only). How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the parameter is distributed on. Argument types can be
        one of the following:

        * :py:class:`~pyCore.daeIndexRange` object
        * plain integer (to select a single index from a domain)
        * python ``list`` (to select a list of indexes from a domain)
        * python ``slice`` (to select a range of indexes from a domain: start_index, end_index, step)
        * character ``'*'`` (to select all points from a domain)
        * integer ``-1`` (to select all points from a domain)
        * empty python list ``[]`` (to select all points from a domain)

    .. method:: __call__((daeParameter)self, [(int)index1[, ...[, (int)index8]]]) -> adouble

        Gets the value of the parameter at the specified domain indexes (used to build equation residuals only). How many arguments ``index1``, ..., ``index8`` are used
        depends on the number of domains that the parameter is distributed on.

.. autoclass:: pyCore.daeVariable
    :members:
    :undoc-members:
    :exclude-members: GetValue, SetValue, GetQuantity, SetValues, AssignValue, ReAssignValue, SetInitialGuess, SetInitialCondition, ReSetInitialCondition,
                      AssignValues, ReAssignValues, SetInitialConditions, ReSetInitialConditions, SetInitialGuesses, SetAbsoluteTolerances,
                      __call__, d, d2, dt, array, d_array, d2_array, dt_array

    .. method:: GetValue((daeVariable)self, [(int)index1[, ...[, (int)index8]]]) -> float

        Gets the value of the variable at the specified domain indexes. How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the variable is distributed on.

    .. method:: GetQuantity((daeVariable)self, [(int)index1[, ...[, (int)index8]]]) -> quantity

        Gets the value of the variable at the specified domain indexes as the ``quantity`` object (with value and units).
        How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the variable is distributed on.

    .. method:: SetValue((daeVariable)self, [(int)index1[, ...[, (int)index8]]], (object)value) -> None

        Sets the value of the variable at the specified domain indexes (as ``float`` or ``quantity``). How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the variable is distributed on.

    .. method:: SetValues((daeVariable)self, (object)values) -> None

        Sets all values of the variable (as ``float`` or ``quantity``).

    .. method:: AssignValue((daeVariable)self, [(int)index1[, ...[, (int)index8]]], (object)value) -> None

    .. method:: AssignValues((daeVariable)self, (object)values) -> None

    .. method:: ReAssignValue((daeVariable)self, [(int)index1[, ...[, (int)index8]]], (object)value) -> None

    .. method:: ReAssignValues((daeVariable)self, (object)values) -> None

    .. method:: SetInitialCondition((daeVariable)self, [(int)index1[, ...[, (int)index8]]], (object)initialCondition) -> None

    .. method:: SetInitialConditions((daeVariable)self, (object)initialConditions) -> None

    .. method:: ReSetInitialCondition((daeVariable)self, [(int)index1[, ...[, (int)index8]]], (object)initialCondition) -> None

    .. method:: ReSetInitialConditions((daeVariable)self, (object)initialConditions) -> None

    .. method:: SetInitialGuess((daeVariable)self, [(int)index1[, ...[, (int)index8]]], (object)initialGuess) -> None

    .. method:: SetInitialGuesses((daeVariable)self, (object)initialGuesses) -> None

    .. method:: SetAbsoluteTolerances((daeVariable)self, (object)tolerances) -> None

    .. method:: array((daeVariable)self, [(object)index1[, ...[, (object)index8]]]) -> adouble_array

        Gets the array of variable's values at the specified domain indexes (used to build equation residuals only). How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the variable is distributed on. Argument types are the same
        as those described in :py:meth:`pyCore.daeParameter.array`

    .. method:: d_array((daeVariable)self, [(object)index1[, ...[, (object)index8]]]) -> adouble_array

        Gets the array of partial derivatives at the specified domain indexes (used to build equation residuals only). How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the variable is distributed on. Argument types are the same
        as those described in :py:meth:`pyCore.daeParameter.array`.

    .. method:: d2_array((daeVariable)self, [(object)index1[, ...[, (object)index8]]]) -> adouble_array

        Gets the array of partial derivatives of the second order at the specified domain indexes (used to build equation residuals only). How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the variable is distributed on. Argument types are the same
        as those described in :py:meth:`pyCore.daeParameter.array`.

    .. method:: dt_array((daeVariable)self, [(object)index1[, ...[, (object)index8]]]) -> adouble_array

        Gets the array of time derivatives at the specified domain indexes (used to build equation residuals only). How many arguments ``index1, ..., index8`` are used
        depends on the number of domains that the variable is distributed on. Argument types are the same
        as those described in :py:meth:`pyCore.daeParameter.array`.

    .. method:: __call__((daeVariable)self, [(int)index1[, ...[, (int)index8]]]) -> adouble

        Gets the value of the variable at the specified domain indexes (used to build equation residuals only). How many arguments ``index1``, ..., ``index8`` are used
        depends on the number of domains that the variable is distributed on.

    .. method:: d((daeVariable)self, (daeDomain)domain, [(int)index1[, ...[, (int)index8]]]) -> adouble

        Gets the partial derivative of the variable at the specified domain indexes (used to build equation residuals only). How many arguments ``index1``, ..., ``index8`` are used
        depends on the number of domains that the variable is distributed on.

    .. method:: d2((daeVariable)self, (daeDomain)domain, [(int)index1[, ...[, (int)index8]]]) -> adouble

        Gets the partial derivative of second order of the variable at the specified domain indexes (used to build equation residuals only). How many arguments ``index1``, ..., ``index8`` are used
        depends on the number of domains that the variable is distributed on.

    .. method:: dt((daeVariable)self, [(int)index1[, ...[, (int)index8]]]) -> adouble

        Gets the time derivative of the variable at the specified domain indexes (used to build equation residuals only). How many arguments ``index1``, ..., ``index8`` are used
        depends on the number of domains that the variable is distributed on.


.. autoclass:: pyCore.daeModel
    :members:
    :undoc-members:

    .. automethod:: __init__
    
.. autoclass:: pyCore.daeSTN
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeIF
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeEquation
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeState
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeStateTransition
    :members:
    :undoc-members:

.. autoclass:: pyCore.daePort
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeEventPort
    :members:
    :undoc-members:

.. autoclass:: pyCore.daePortConnection
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeScalarExternalFunction
    :members:
    :undoc-members:

    .. automethod:: __call__

.. autoclass:: pyCore.daeVectorExternalFunction
    :members:
    :undoc-members:

    .. automethod:: __call__

.. autoclass:: pyCore.daeDomainIndex
    :members:
    :undoc-members:

    .. automethod:: __init__

.. autoclass:: pyCore.daeIndexRange
    :members:
    :undoc-members:

    .. automethod:: __init__

.. autoclass:: pyCore.daeArrayRange
    :members:
    :undoc-members:

    .. automethod:: __init__

.. autoclass:: pyCore.daeDEDI
    :members:
    :undoc-members:

    .. automethod:: __call__

.. autoclass:: pyCore.daeAction
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeOptimizationVariable
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeObjectiveFunction
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeOptimizationConstraint
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeMeasuredVariable
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeEquationExecutionInfo
    :members:
    :undoc-members:


Logging
--------
.. autosummary::
    :nosignatures:

    daeLog_t
    daeBaseLog
    daeFileLog
    daeStdOutLog
    daeTCPIPLog
    daeTCPIPLogServer

.. autoclass:: pyCore.daeLog_t
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeBaseLog
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeFileLog
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeStdOutLog
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeTCPIPLog
    :members:
    :undoc-members:

.. autoclass:: pyCore.daeTCPIPLogServer
    :members:
    :undoc-members:

Functions
---------
.. autosummary::
    :nosignatures:

    d
    dt
    Time
    Constant
    Array
    Sum
    Product
    Integral
    Average

.. autofunction:: pyCore.d
.. autofunction:: pyCore.dt
.. autofunction:: pyCore.Time
.. autofunction:: pyCore.Constant
.. autofunction:: pyCore.Array
.. autofunction:: pyCore.Sum
.. autofunction:: pyCore.Product
.. autofunction:: pyCore.Integral
.. autofunction:: pyCore.Average


Auxiliary classes
-----------------
.. autosummary::
    :nosignatures:

    daeVariableWrapper
    daeConfig

.. autoclass:: pyCore.daeVariableWrapper
    :members:
    :undoc-members:

    .. automethod:: __init__

.. autoclass:: pyCore.daeConfig
    :members:
    :undoc-members:

    .. automethod:: __contains__
    .. automethod:: __getitem__
    .. automethod:: __setitem__

Auxiliary functions
----------------------
.. autosummary::
    :nosignatures:
    
    daeGetConfig
    daeVersion
    daeVersionMajor
    daeVersionMinor
    daeVersionBuild

.. autofunction:: pyCore.daeGetConfig
.. autofunction:: pyCore.daeVersion
.. autofunction:: pyCore.daeVersionMajor
.. autofunction:: pyCore.daeVersionMinor
.. autofunction:: pyCore.daeVersionBuild

Global constants
----------------------
.. autosummary::
    :nosignatures:
    
    cnAlgebraic
    cnDifferential
    cnAssigned

.. autodata:: pyCore.cnAlgebraic

.. autodata:: pyCore.cnDifferential

.. autodata:: pyCore.cnAssigned
