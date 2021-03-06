/***********************************************************************************
                 DAE Tools Project: www.daetools.com
                 Copyright (C) Dragan Nikolic, 2017
************************************************************************************
DAE Tools is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License version 3 as published by the Free Software
Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with the
DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
***********************************************************************************/
#include <OpenCS/cs_evaluator.h>
using namespace cs;

namespace openmp_evaluator
{
/* Evaluate equations. */
void EvaluateEquations(const csComputeStackItem_t*         computeStacks,
                       uint32_t                            equationIndex,
                       const uint32_t*                     activeEquationSetIndexes,
                       csEvaluationContext_t               EC,
                       const real_t*                       dofs,
                       const real_t*                       values,
                       const real_t*                       timeDerivatives,
                       real_t*                             residuals);

/* Jacobian kernel functions. */
void EvaluateDerivatives(const csComputeStackItem_t*         computeStacks,
                         uint32_t                            jacobianItemIndex,
                         const uint32_t*                     activeEquationSetIndexes,
                         const csIncidenceMatrixItem_t*      computeStackJacobianItems,
                         csEvaluationContext_t               EC,
                         const real_t*                       dofs,
                         const real_t*                       values,
                         const real_t*                       timeDerivatives,
                         real_t*                             jacobian);

/* Evaluate sensitivity derivatives. */
void EvaluateSensitivityDerivatives(const csComputeStackItem_t*         computeStacks,
                                    uint32_t                            equationIndex,
                                    const uint32_t*                     activeEquationSetIndexes,
                                    csEvaluationContext_t               EC,
                                    const real_t*                       dofs,
                                    const real_t*                       values,
                                    const real_t*                       timeDerivatives,
                                    const real_t*                       svalues,
                                    const real_t*                       sdvalues,
                                          real_t*                       sresiduals);
}
