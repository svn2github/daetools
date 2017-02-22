#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
************************************************************************************
                           membrane_variable_types.py
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
"""

from daetools.pyDAE import *
from pyUnits import unit, m, kg, s, K, Pa, mol, J, W

#-----------------------------------------------------------------------------------------------------------------------------
#                                               Name                           Units             LB       UB   DefVal  AbsTol
#-----------------------------------------------------------------------------------------------------------------------------
time_t                       = daeVariableType("time_t",                       s,               0.0, 1.0e+20,     0.0, 1e-05)
length_t                     = daeVariableType("length_t",                     m,               0.0, 1.0e+05,     0.5, 1e-05)
area_t                       = daeVariableType("area_t",                       m**2,            0.0, 1.0e+05,     1.0, 1e-05)
specific_area_t              = daeVariableType("specific_area_t",              m**(-1),         0.0, 1.0e+05,     0.5, 1e-05)
volume_t                     = daeVariableType("volume_t",                     m**3,            0.0, 1.0e+05,     1.0, 1e-06)
velocity_t                   = daeVariableType("velocity_t",                   m/s,        -1.0e+10, 1.0e+10,   0.001, 1e-05)
pressure_t                   = daeVariableType("pressure_t",                   Pa,          1.0e+02, 1.0e+10, 1.0e+05, 1e-05)
temperature_t                = daeVariableType("temperature_t",                K,               0.0, 1.0e+05,     300, 1e-05)

fraction_t                   = daeVariableType("fraction_t",                   unit(),         -0.1,       2,    0.25, 1e-05)
no_t                         = daeVariableType("no_t",                         unit(),     -1.0e+20, 1.0e+20,     1.0, 1e-05)
recovery_t                   = daeVariableType("recovery_t",                   unit(),         -0.1,       2,     0.5, 1e-04)
selectivity_t                = daeVariableType("selectivity_t",                unit(),         -0.1,       2,     1.0, 1e-04)
moles_t                      = daeVariableType("moles_t",                      mol,             0.0, 1.0e+20,     1.0, 1e-05)
molar_flux_t                 = daeVariableType("molar_flux_t",           mol/((m**2) * s), -1.0e+20, 1.0e+20,    0.01, 1e-06)
molar_concentration_t        = daeVariableType("molar_concentration_t",        mol/(m**3),      0.0, 1.0e+10,    10.0, 1e-05)
molar_flowrate_t             = daeVariableType("molar_flowrate_t",             mol/s,      -1.0e+10, 1.0e+10,     0.0, 1e-05)

volume_flowrate_t            = daeVariableType("volume_flowrate_t",            (m**3)/s,   -1.0e+10, 1.0e+10,  1.0e-8, 1e-08)

heat_t                       = daeVariableType("heat_t",                       J,          -1.0e+20, 1.0e+20, 1.0e+03, 1e-05)
heat_flux_t                  = daeVariableType("heat_flux_t",                  W/(m**2),   -1.0e+20, 1.0e+20,     0.0, 1e-05)
heat_transfer_coefficient_t  = daeVariableType("heat_transfer_coefficient_t",  W/((m**2)*K),      0, 1.0e+10, 1.0e+02, 1e-05)
power_t                      = daeVariableType("power_t",                      W,          -1.0e+20, 1.0e+20,     0.0, 1e-05)

specific_heat_capacity_t     = daeVariableType("specific_heat_capacity_t",     J/(kg*K),        0.0, 1.0e+20, 1.0e+03, 1e-05)
density_t                    = daeVariableType("density_t",                    kg/(m**3),       0.0, 1.0e+20, 1.0e+03, 1e-05)
specific_heat_conductivity_t = daeVariableType("specific_heat_conductivity_t", W/(m*K),         0.0, 1.0e+10,     1.0, 1e-05)
dynamic_viscosity_t          = daeVariableType("dynamic_viscosity_t",          Pa*s,            0.0, 1.0e+10, 1.0e-03, 1e-06)
diffusivity_t                = daeVariableType("diffusivity_t",                (m**2)/s,        0.0, 1.0e+05, 1.0e-08, 1e-05)

amount_adsorbed_t            = daeVariableType("amount_adsorbed_t",            mol/kg,     -1.0e+20, 1.0e+20,     1.0, 1e-05)

gij_t                        = daeVariableType("gij_t",                        unit(),     -1.0e+20, 1.0e+20,     1.0, 1e-06)
BQsat_t                      = daeVariableType("BQsat_t",                      unit(),     -1.0e+20, 1.0e+20,     1.0, 1e-05)
Gij_dTheta_t                 = daeVariableType("Gij_dTheta",                  m**(-1),     -1.0e+20, 1.0e+20,    -1.0, 1e-05)
J_theta_t                    = daeVariableType("J_theta",                   kg/(m**4),     -1.0e+20, 1.0e+20,     0.0, 1e-05)
