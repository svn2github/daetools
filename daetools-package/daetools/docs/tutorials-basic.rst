***************
Basic Tutorials
***************
..
    Copyright (C) Dragan Nikolic
    DAE Tools is free software; you can redistribute it and/or modify it under the
    terms of the GNU General Public License version 3 as published by the Free Software
    Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with the
    DAE Tools software; if not, see <http://www.gnu.org/licenses/>.


.. rubric:: The key modelling concepts in **DAE Tools**:

=======================   =================================================================
:ref:`whats_the_time`     |witt|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial1`          |t_1|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial2`          |t_2|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial3`          |t_3|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial6`          |t_6|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial10`         |t_10|
=======================   =================================================================

.. rubric:: Support for discrete systems:

=======================   =================================================================
:ref:`tutorial4`          |t_4|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial5`          |t_5|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial13`         |t_13|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial15`         |t_15|
=======================   =================================================================

.. rubric:: The simulation options:

=======================   =================================================================
:ref:`tutorial7`          |t_7|
=======================   =================================================================

.. rubric:: Data reporting:

=======================   =================================================================
:ref:`tutorial8`          |t_8|
=======================   =================================================================

.. rubric:: DAE and LA solvers:

=======================   =================================================================
:ref:`tutorial9`          |t_9|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial11`         |t_11|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial12`         |t_12|
=======================   =================================================================

.. rubric:: External functions:

=======================   =================================================================
:ref:`tutorial14`         |t_14|
=======================   =================================================================

.. rubric:: Logging:

=======================   =================================================================
:ref:`tutorial17`         |t_17|
=======================   =================================================================

.. rubric:: Interoperability with NumPy:

=======================   =================================================================
:ref:`tutorial16`         |t_16|
-----------------------   -----------------------------------------------------------------
:ref:`tutorial18`         |t_18| 
=======================   =================================================================

.. rubric:: Thermophysical property packages:

=======================   =================================================================
:ref:`tutorial19`         |t_19|
=======================   =================================================================

.. rubric:: Sensitivity analysis:

=======================   =================================================================
:ref:`tutorial20`         |t_20|
=======================   =================================================================


.. |witt| replace:: Importing DAE Tools pyDAE module(s), units and variable types
                    developing models, setting up a simulation, declaring auxiliary
                    objects (DAE solver, data reporter, log), simulation run-time
                    options, running a smulation.

.. |t_1| replace:: Using distribution domains, distributing parameters/variables/equations on domains,
                   using derivative functions (dt, d, d2), setting boundary and initial conditions.

.. |t_2| replace:: Using arrays (discrete distribution domains), specifying degrees of freedom, setting initial guesses.

.. |t_3| replace:: Declaring arrays of variable values (Array function) and using functions that operate on them arrays,
                   using the Constant function, making non-uniform grids.

.. |t_4| replace:: Declaring discontinuous equations (symmetrical state transition networks: daeIF statements).

.. |t_5| replace:: Declaring discontinuous equations (non-symmetrical state transition networks: daeSTN statements).

.. |t_6| replace:: Using ports, making port connections, declaring components (instances of other models).

.. |t_7| replace:: Making user-defined operating procedures, resetting the values of degrees of freedom and
                   resetting initial conditions.

.. |t_8| replace:: Using data reporters to write the results into files (Matlab, MS Excel, JSON, XML, HDF5, VTK, Pandas),
                   developing custom data reporters.

.. |t_9| replace:: Using available linear equations solvers (SuperLU, SuperLU_MT, Trilinos Amesos, IntelPardiso, Pardiso).

.. |t_10| replace:: Saving and restoring initialisation files, evaluating integrals.

.. |t_11| replace:: Using iterative linear equations solvers (Trilinos AztecOO) and preconditioners
                    (built-in AztecOO, Ifpack, ML).

.. |t_12| replace:: Using SuperLU and SuperLU_MT solvers and their options.

.. |t_13| replace:: Using event ports, handling events using ON_CONDITION() and ON_EVENT() functions,
                    and declaring user defined actions.

.. |t_14| replace:: Declaring and using external functions.

.. |t_15| replace:: Declaring nested state transitions.

.. |t_16| replace:: Using DAE Tools variables and NumPy functions to solve a simple stationary 1D heat conduction
                    by manually assembling Finite Element stiffness matrix and load vector.

.. |t_17| replace:: Using TCPIP Log and TCPIPLogServer.

.. |t_18| replace:: Using DAE Tools variables and NumPy functions to generate and solve a simple ODE system.

.. |t_19| replace:: Using thermophysical property packages in DAE Tools.

.. |t_20| replace:: Using the sensitivity analysis in DAE Tools.


.. begin-command

.. _whats_the_time:

What's the time? (AKA: Hello world!)
====================================

.. automodule:: daetools.examples.whats_the_time
   :no-members:
   :no-undoc-members:
   
.. rubric:: Files

=====================   =================================================================
Model report            `whats_the_time.xml <../examples/whats_the_time.xml>`_
Runtime model report    `whats_the_time-rt.xml <../examples/whats_the_time-rt.xml>`_
Source code             `whats_the_time.py <../examples/whats_the_time.html>`_
=====================   =================================================================

.. _tutorial1:
    
Tutorial 1
==========

.. automodule:: daetools.examples.tutorial1
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial1.xml <../examples/tutorial1.xml>`_
Runtime model report    `tutorial1-rt.xml <../examples/tutorial1-rt.xml>`_
Source code             `tutorial1.py <../examples/tutorial1.html>`_
=====================   =================================================================


.. _tutorial2:

Tutorial 2
==========

.. automodule:: daetools.examples.tutorial2
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial2.xml <../examples/tutorial2.xml>`_
Runtime model report    `tutorial2-rt.xml <../examples/tutorial2-rt.xml>`_
Source code             `tutorial2.py <../examples/tutorial2.html>`_
=====================   =================================================================


.. _tutorial3:

Tutorial 3
==========

.. automodule:: daetools.examples.tutorial3
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial3.xml <../examples/tutorial3.xml>`_
Runtime model report    `tutorial3-rt.xml <../examples/tutorial3-rt.xml>`_
Source code             `tutorial3.py <../examples/tutorial3.html>`_
=====================   =================================================================


.. _tutorial4:

Tutorial 4
==========

.. automodule:: daetools.examples.tutorial4
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial4.xml <../examples/tutorial4.xml>`_
Runtime model report    `tutorial4-rt.xml <../examples/tutorial4-rt.xml>`_
Source code             `tutorial4.py <../examples/tutorial4.html>`_
=====================   =================================================================


.. _tutorial5:

Tutorial 5
==========

.. automodule:: daetools.examples.tutorial5
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial5.xml <../examples/tutorial5.xml>`_
Runtime model report    `tutorial5-rt.xml <../examples/tutorial5-rt.xml>`_
Source code             `tutorial5.py <../examples/tutorial5.html>`_
=====================   =================================================================


.. _tutorial6:

Tutorial 6
==========

.. automodule:: daetools.examples.tutorial6
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial6.xml <../examples/tutorial6.xml>`_
Runtime model report    `tutorial6-rt.xml <../examples/tutorial6-rt.xml>`_
Source code             `tutorial6.py <../examples/tutorial6.html>`_
=====================   =================================================================


.. _tutorial7:

Tutorial 7
==========

.. automodule:: daetools.examples.tutorial7
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial7.xml <../examples/tutorial7.xml>`_
Runtime model report    `tutorial7-rt.xml <../examples/tutorial7-rt.xml>`_
Source code             `tutorial7.py <../examples/tutorial7.html>`_
=====================   =================================================================


.. _tutorial8:

Tutorial 8
==========

.. automodule:: daetools.examples.tutorial8
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial8.xml <../examples/tutorial8.xml>`_
Runtime model report    `tutorial8-rt.xml <../examples/tutorial8-rt.xml>`_
Source code             `tutorial8.py <../examples/tutorial8.html>`_
=====================   =================================================================


.. _tutorial9:

Tutorial 9
==========

.. automodule:: daetools.examples.tutorial9
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial9.xml <../examples/tutorial9.xml>`_
Runtime model report    `tutorial9-rt.xml <../examples/tutorial9-rt.xml>`_
Source code             `tutorial9.py <../examples/tutorial9.html>`_
=====================   =================================================================


.. _tutorial10:

Tutorial 10
===========

.. automodule:: daetools.examples.tutorial10
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial10.xml <../examples/tutorial10.xml>`_
Runtime model report    `tutorial10-rt.xml <../examples/tutorial10-rt.xml>`_
Source code             `tutorial10.py <../examples/tutorial10.html>`_
=====================   =================================================================


.. _tutorial11:

Tutorial 11
===========

.. automodule:: daetools.examples.tutorial11
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial11.xml <../examples/tutorial11.xml>`_
Runtime model report    `tutorial11-rt.xml <../examples/tutorial11-rt.xml>`_
Source code             `tutorial11.py <../examples/tutorial11.html>`_
=====================   =================================================================


.. _tutorial12:

Tutorial 12
===========

.. automodule:: daetools.examples.tutorial12
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial12.xml <../examples/tutorial12.xml>`_
Runtime model report    `tutorial12-rt.xml <../examples/tutorial12-rt.xml>`_
Source code             `tutorial12.py <../examples/tutorial12.html>`_
=====================   =================================================================


.. _tutorial13:

Tutorial 13
===========

.. automodule:: daetools.examples.tutorial13
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial13.xml <../examples/tutorial13.xml>`_
Runtime model report    `tutorial13-rt.xml <../examples/tutorial13-rt.xml>`_
Source code             `tutorial13.py <../examples/tutorial13.html>`_
=====================   =================================================================


.. _tutorial14:

Tutorial 14
===========

.. automodule:: daetools.examples.tutorial14
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial14.xml <../examples/tutorial14.xml>`_
Runtime model report    `tutorial14-rt.xml <../examples/tutorial14-rt.xml>`_
Source code             `tutorial14.py <../examples/tutorial14.html>`_
=====================   =================================================================


.. _tutorial15:

Tutorial 15
===========

.. automodule:: daetools.examples.tutorial15
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial15.xml <../examples/tutorial15.xml>`_
Runtime model report    `tutorial15-rt.xml <../examples/tutorial15-rt.xml>`_
Source code             `tutorial15.py <../examples/tutorial15.html>`_
=====================   =================================================================


.. _tutorial16:

Tutorial 16
===========

.. automodule:: daetools.examples.tutorial16
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial16.xml <../examples/tutorial16.xml>`_
Runtime model report    `tutorial16-rt.xml <../examples/tutorial16-rt.xml>`_
Source code             `tutorial16.py <../examples/tutorial16.html>`_
=====================   =================================================================


.. _tutorial17:

Tutorial 17
===========

.. automodule:: daetools.examples.tutorial17
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial17.xml <../examples/tutorial17.xml>`_
Runtime model report    `tutorial17-rt.xml <../examples/tutorial17-rt.xml>`_
Source code             `tutorial17.py <../examples/tutorial17.html>`_
=====================   =================================================================


.. _tutorial18:

Tutorial 18
===========

.. automodule:: daetools.examples.tutorial18
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial18.xml <../examples/tutorial18.xml>`_
Runtime model report    `tutorial18-rt.xml <../examples/tutorial18-rt.xml>`_
Source code             `tutorial18.py <../examples/tutorial18.html>`_
=====================   =================================================================


.. _tutorial19:

Tutorial 19
===========

.. automodule:: daetools.examples.tutorial19
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial19.xml <../examples/tutorial19.xml>`_
Runtime model report    `tutorial19-rt.xml <../examples/tutorial19-rt.xml>`_
Source code             `tutorial19.py <../examples/tutorial19.html>`_
=====================   =================================================================


.. _tutorial20:

Tutorial 20
===========

.. automodule:: daetools.examples.tutorial20
   :no-members:
   :no-undoc-members:

.. rubric:: Files

=====================   =================================================================
Model report            `tutorial20.xml <../examples/tutorial20.xml>`_
Runtime model report    `tutorial20-rt.xml <../examples/tutorial20-rt.xml>`_
Source code             `tutorial20.py <../examples/tutorial20.html>`_
=====================   =================================================================


.. end-command
