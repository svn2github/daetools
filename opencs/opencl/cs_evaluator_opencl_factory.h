/***********************************************************************************
                 OpenCS Project: www.daetools.com
                 Copyright (C) Dragan Nikolic
************************************************************************************
OpenCS is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License version 3 as published by the Free Software
Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with the
DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
***********************************************************************************/
#if !defined(CS_OPENCL_FACTORY_H)
#define CS_OPENCL_FACTORY_H

#include <string>
#include <vector>
#include <cs_evaluator.h>

namespace cs
{
csComputeStackEvaluator_t* CreateComputeStackEvaluator(int platformID,
                                                       int deviceID,
                                                       std::string buildProgramOptions = "");

csComputeStackEvaluator_t* CreateComputeStackEvaluator_MultiDevice(const std::vector<int>&    platforms,
                                                                   const std::vector<int>&    devices,
                                                                   const std::vector<double>& taskPortions,
                                                                   std::string                buildProgramOptions = "");
}
#endif
