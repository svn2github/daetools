*****************
Getting DAE Tools
*****************
..
    Copyright (C) Dragan Nikolic, 2013
    DAE Tools is free software; you can redistribute it and/or modify it under the
    terms of the GNU General Public License version 3 as published by the Free Software
    Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with the
    DAE Tools software; if not, see <http://www.gnu.org/licenses/>.

**DAE Tools** (pyDAE module) is installed in ``daetools`` folder within ``site-packages`` (or ``dist-packages``)
folder under python (tipically ``/usr/local/lib/pythonXY/Lib`` or ``C:\PythonX.Y\Lib``).
The structure of the folders is the following:

* ``daetools``

  * ``code_generators``
  * ``dae_plotter``
  * ``dae_simulator``
  * ``docs``
  * ``examples``
  * ``pyDAE``
  * ``solvers``
  * ``unit_tests``

System requirements
===================

Supported platforms:
    
* GNU/Linux (i686, x86_64, arm)
* Windows (32 bit and 32 bit version of DAE Tools on 64 bit)
* MacOS (x86, x86_64)

Supported python versions:

* 2.6 (some older GNU/Linux distributions only)
* 2.7

Supported numpy versions:
    
* GNU/Linux (1.5, 1.6, 1.7)
* Windows (1.6)
* MacOS (1.6)

Mandatory packages:

* Python (2.7.x): `<http://www.python.org>`_
* Numpy (1.5.x, 1.6.x, 1.7.x): `<http://numpy.scipy.org>`_
* Scipy: `<http://www.scipy.org>`_
* Matplotlib: `<http://matplotlib.sourceforge.net>`_
* pyQt4 (4.x): `<http://www.riverbankcomputing.co.uk/software/pyqt>`_

Optional packages (3rd party linear solvers):

* Intel Pardiso (proprietary)
* AMD ACML (proprietary)

For more information on how to install packages please refer to the documentation for the specific library.
By default all versions (GNU/Linux, Windows and MacOS) come with the Sundials dense LU and Lapack linear
solvers, SuperLU, Trilinos Amesos (with built-in support for KLU, SuperLU and Lapack linear solvers),
Trilinos AztecOO (with built-in support for Ifpack and ML preconditioners), NLOPT and IPOPT/BONMIN
(with MUMPS linear solver and  PORD ordering). Standalone SuperLU_MT is available on GNU/Linux and
MacOS versions only. Additional linear solvers: AMD ACML and Intel Pardiso must be downloaded separately
and compiled from source since they are not free software.

Getting the packages
====================

The instalation files can be downloaded from: `<https://sourceforge.net/projects/daetools/files>`_

.. note:: From the version 1.2.1 **DAE Tools** use distutils to distribute python packages and extensions.

The naming convention of the installation files:

``daetools-major.minor.build-platform-architecture-python_version.tar.gz``

where ``major.minor.build`` represents the version (``1.2.1`` for instance), ``architecture`` could be ``i686``, ``x86_64``
or ``universal``, and ``python_version`` can be ``py26``, ``py27`` etc. An example:
``daetools-1.2.1-gnu_linux-x86_64-py27.tar.gz`` is the version 1.2.1 for 64 bit GNU/Linux with python 2.7.

For the other platforms, architectures and python versions not listed in `System requirements`_
daetools must be compiled from the source.
The source code can be downloaded either from the subversion tree or from the folder with a particular version
(``daetools-1.2.1-source.tar.gz`` for instance).

Installation
============

GNU/Linux
---------

First install the mandatory packages: python 2.7, numpy 1.5/1.6/1.7, scipy, matplotlib and pyqt4.

In Debian GNU/Linux and derivatives use the Synaptic Package Manager or type the following commands:
    
.. code-block:: bash

    sudo apt-get install python-numpy python-scipy python-matplotlib python-qt4 mayavi2


In Red Hat and derivatives use the package manager or type the following commands:
    
.. code-block:: bash

    sudo yum install numpy scipy python-matplotlib PyQt4 Mayavi

Then unpack the downloaded archive, cd to the ``daetools-X.Y.Z`` folder and install **DAE Tools** by typing
the following shell command:

.. code-block:: bash

    sudo python setup.py install


MacOS
-----

First install the mandatory packages: **python 2.7**, **numpy 1.6**, **scipy**, **matplotlib** and **pyqt4**.
As a starting point the following links can be used:

* Python 2.7: `<http://www.python.org/ftp/python/2.7.3/python-2.7.3-macosx10.6.dmg>`_
* Numpy: `<http://sourceforge.net/projects/numpy/files/NumPy/1.6.2/numpy-1.6.2-py2.7-python.org-macosx10.6.dmg/download>`_
* Scipy: `<http://sourceforge.net/projects/scipy/files/scipy/0.10.1/scipy-0.10.1-py2.7-python.org-macosx10.6.dmg/download>`_
* Matplotlib: `<http://sourceforge.net/projects/matplotlib/files/matplotlib/matplotlib-1.1.0/matplotlib-1.1.0-py2.7-python.org-macosx10.6.dmg/download>`_
* PyQt4: `<http://www.riverbankcomputing.co.uk/static/Downloads/PyQt4 download section>`_

Then unpack the downloaded archive, cd to the ``daetools-X.Y.Z`` folder and install **DAE Tools** by typing
the following shell command:

.. code-block:: bash

    sudo python setup.py install


Windows
-------

**DAE Tools** is compiled and tested on a 32-bit Windows XP and Windows 7. In order to use **DAE Tools** on
64-bit versions of Windows the 32-bit versions of python, pyqt, numpy and scipy packages should be installed.
First install the mandatory packages: python 2.7, numpy 1.6, scipy, matplotlib and pyqt4.
As a starting point the following links can be used:

* Python 2.7: `<http://www.python.org/ftp/python/2.7.3/python-2.7.3.msi>`_
* Numpy: `<http://sourceforge.net/projects/numpy/files/NumPy/1.6.2/numpy-1.6.2-win32-superpack-python2.7.exe/download>`_
* Scipy: `<http://sourceforge.net/projects/scipy/files/scipy/0.10.1/scipy-0.10.1-win32-superpack-python2.7.exe/download>`_
* Matplotlib: `<http://sourceforge.net/projects/matplotlib/files/matplotlib/matplotlib-1.1.0/matplotlib-1.1.0.win32-py2.7.exe/download>`_
* PyQt4: `<http://www.riverbankcomputing.co.uk/static/Downloads/PyQt4 download section>`_

To be able to create 3D plots you need to install Mayavi2 package.  Alternatively you can install everything needed
through `Python(x,y) <http://www.pythonxy.com>`_.
Finally, install **DAE Tools** by double clicking the file daetools_x.x-x-win32_py27.exe and follow the instructions.
To uninstall use the uninstall program in ``Start`` -> ``All Programs`` -> ``DAE Tools`` -> ``Uninstall``.

..
    Additional linear equation solvers (proprietary)
    ------------------------------------------------
    Optionally you can also install proprietary `AMD ACML <http://www.amd.com/acml>`_ and
    `Intel MKL <http://software.intel.com/en-us/intel-mkl/>`_ libraries.
    Please follow the installation procedures in the documentation. **pyAmdACML** and **pyIntelMKL/pyIntelPardiso**
    modules are compiled against ACML 4.4.0 and MKL 10.2.5.035 respectively. Also have a look on the licensing
    conditions (**these libraries are not** `**free software** <http://www.gnu.org/philosophy/free-sw.html>`_).

    In order to use AMD ACML and Intel MKL libraries you have to do some additional configuration.
    You can follow the instructions in the corresponding package documentation or do a quick setup as described below:

    #**GNU/Linux**: setup for a single user<br /> Copy `<acml_mkl_bashrc this file>`_ to your home folder,
    edit it so that it reflects your installation and add the line. $HOME/acml_mkl_bashrc  at the end of $HOME/.bashrc file
    #**GNU/Linux**: setup for all users<br /> Subject to your machine architecture and library versions
    (here **x86_64** GNU/Linux with **ACML v4.4.0** and **MKL v10.2.5.035**), put the following lines in
    /etc/ld.so.conf and execute ldconfig: /opt/intel/mkl/10.2.5.035/lib/em64t /opt/acml4.4.0/gfortran64_mp/lib
    #**Windows XP**:<br /> If not already added, add the following line to your **PATH** environment variable
    (Control Panel -> System): c:\AMD\acml4.4.0\ifort32_mp\lib;c:\Intel\MKL\10.2.5.035\ia32\bin\

    
Compiling from source
===============================

To compile the **DAE Tools** the following is needed:
    
* Installed python, numpy, and scipy modules
* Compiled third party libraries and DAE/LA/NLP solvers: Sundials IDAS, Bonmin, NLopt, Trilinos, SuperLU, SuperLU_MT,
  Blas/Lapack

All **DAE Tools** modules are developed using the QtCreator/QMake cross-platform integrated development environment.
The source code can be downloaded from the SourceForge website or checked out from the
`DAE Tools subversion repository <https://svn.code.sf.net/p/daetools/code>`_:

.. code-block:: bash

    svn checkout svn://svn.code.sf.net/p/daetools/code daetools


GNU/Linux and MacOS
-------------------

.. _the_easy_way:

.. rubric:: The easy way

First, install all the necessary dependencies by executing ``install_dependencies_linux.sh`` shell script located
in the ``trunk`` directory. It will check the OS you are running (currently Debian, Ubuntu, Linux Mint, CentOS and
Fedora are supported but other can be easily added) and install all necessary packages needed for **DAE Tools**
development.

.. code-block:: bash

    # 'lsb_release' command might be missing on some GNU/Linux platforms
    # and has to be installed before proceeding.
    # On Debian based systems:
    # sudo apt-get install lsb-release
    # On red Hat based systems:
    # sudo yum install redhat-lsb

    cd daetools/trunk
    sh install_dependencies_linux.sh


Then, compile the third party libraries by executing ``compile_libraries_linux.sh`` shell script located in the
``trunk`` directory. The script will download all necessary source archives from the **DAE Tools** SourceForge web-site,
unpack them, apply changes and compile them. If all dependencies are installed there should not be problems compiling
the libraries.

.. code-block:: bash

    sh compile_libraries_linux.sh


.. note:: There is a bug in Sundials IDAS library. When compiling fails, go to the folder ``trunk/idas`` and change the line 24
          (or somewhere around it) in the ``Makefile``:

          .. code-block:: bash

              top_builddir =
              to:
              top_builddir = .

.. note:: There are known problems to compile the older bonmin and trilinos libraries using GNU GCC 4.6. This has been fixed
          in bonmin 1.5+ and trilinos 10.8+ versions. Therefore, either GCC 4.5 and below or the recent
          versions of bonmin/trilinos libraries should be used.

Finally, compile the **DAE Tools** libraries and python modules by executing ``compile_linux.sh`` shell script located
in the ``trunk`` directory. The script accepts one argument specifying projects that should be compiled. Any of the
following is accepted: ``all``, ``core``, ``pydae``, ``solvers``, ``superlu``, ``superlu_mt``, ``superlu_cuda``,
``cusp``, ``trilinos``, ``bonmin``, ``ipopt``, and ``nlopt``. If ``all`` is specified the script will compile
``dae``, ``superlu``, ``superlu_mt``, ``trilinos``, ``bonmin``, ``ipopt``, and ``nlopt`` projects.

.. code-block:: bash

    sh compile_linux.sh all
    # Or for instance:
    # sh compile_linux.sh dae superlu nlopt


All python extensions should be placed in ``trunk/daetools-package/daetools/pyDAE`` and
``trunk/daetools-package/daetools/solvers`` folders.
**DAE Tools** can be now installed by using the folowing commands:
    
.. code-block:: bash

    cd daetools/trunk/daetools-package
    sudo python setup.py install


.. _from_qtcreator_ide:

.. rubric:: From QtCreator IDE

DAE Tools can also be compiled from within QtCreator IDE. First install dependencies and compile third party libraries
(as explained in :ref:`The easy way <the_easy_way>`) and then do the following:
    
* Do not do the shadow build. Uncheck it (for all projects) and build everything in the release folder
* Choose the right specification file for your platform (usually it is done automatically by the IDE, but double-check it):
    
 * for GNU/Linux use ``-spec linux-g++``
 * for MacOS use ``-spec macx-g++``

* Compile the ``dae`` project (you can add the additional Make argument ``-jN`` to speed-up the compilation process,
  where N is the number of processors plus one; for instance on the quad-core machine you can use ``-j5``)
* Compile ``SuperLU/SuperLU_MT/SuperLU_CUDA`` and ``Bonmin/Ipopt`` solvers.
  ``SuperLU/SuperLU_MT/SuperLU_CUDA`` and ``Bonmin/Ipopt`` share the same code and the same project file so some
  hacking is needed. Here are the instructions how to compile them:
    
 * Compiling ``libcdaeBONMIN_MINLPSolver.a`` and ``pyBONMIN.so``:
 
   * Set ``CONFIG += BONMIN`` in ``BONMIN_MINLPSolver.pro``, run ``qmake`` and then compile
   * Set ``CONFIG += BONMIN`` in ``pyBONMIN.pro``, run ``qmake`` and then compile
  
 * Compiling ``libcdaeIPOPT_NLPSolver.a`` and ``pyIPOPT.so``:
 
   * Set ``CONFIG += IPOPT`` in ``BONMIN_MINLPSolver.pro``, run ``qmake`` and then compile
   * Set ``CONFIG += IPOPT`` in ``pyBONMIN.pro``, run ``qmake`` and then compile
  
 * Compiling ``libcdaeSuperLU_LASolver.a`` and ``pySuperLU.so``:
 
   * Set ``CONFIG += SuperLU`` in ``LA_SuperLU.pro``, run ``qmake`` and then compile
   * Set ``CONFIG += SuperLU`` in ``pySuperLU.pro``, run ``qmake`` and then compile
  
 * Compiling ``libcdaeSuperLU_MT_LASolver.a`` and ``pySuperLU_MT.so``:
 
   * Set ``CONFIG += SuperLU_MT`` in ``LA_SuperLU.pro``, run ``qmake`` and then compile
   * Set ``CONFIG += SuperLU_MT`` in ``pySuperLU.pro``, run ``qmake`` and then compile
  
 * Compiling ``libcdaeSuperLU_CUDA_LASolver.a`` and ``pySuperLU_CUDA.so``:
 
   * Set ``CONFIG += SuperLU_CUDA`` in ``LA_SuperLU.pro``, run ``qmake`` and then compile
   * Set ``CONFIG += SuperLU_CUDA`` in ``pySuperLU.pro``, run ``qmake`` and then compile

* Compile the ``LA_Trilinos_Amesos`` project

Windows
-------

Necessary tools: `QtCreator <http://qt.nokia.com/products/developer-tools>`_,
`Microsoft VC++ <http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=14597>`_
and `G95 Fortran <http://www.g95.org>`_ compiler (Mumps only).

.. note:: Compiling all third party libraries and **DAE Tools** projects requires a mental gymnastics
          impossible to describe by any human language so that the pre-compiled libraries are provided in the downloads
          section (`windows libraries <https://sourceforge.net/projects/daetools/files/windows%20libraries>`_).

**DAE Tools** should be compiled from within QtCreator IDE:
    
* Unpack the downloaded archive ``bonmin-trilinos-idas-superlu-nlopt-mumps-g95-msvc-win32.zip`` into the 
  ``daetools/trunk`` folder. All libraries are compiled with MS VC++ 2008 Express edition (the most likely other
  versions of MS VC++ will also work). Mumps Fortran 95 files are compiled with G95 Fortran compiler.

* Path to ``libf95.a`` and ``libgcc.a`` libraries should be set in ``dae.pri`` config file.
  For instance, if G95 is installed in ``c:\g95`` set the ``G95_LIBDIR`` variable to:
  ``G95_LIBDIR = c:\g95\lib\gcc-lib\i686-pc-mingw32\4.1.2``
 
* Follow the instructions for compiling **DAE Tools** described in :ref:`From QtCreator IDE <from_qtcreator_ide>` section above.

.. note:: superlu_mt and superlu_cuda cannot be compiled on Windows at the moment.

DAE Tools can be installed by using the folowing commands:

.. code-block:: bash

    cd daetools/trunk/daetools-package
    sudo python setup.py install

