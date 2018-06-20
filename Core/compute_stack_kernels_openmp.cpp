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
#include "compute_stack_kernels_openmp.h"

namespace openmp_evaluator
{
/* Residual kernel function .*/
void EvaluateResiduals(const csComputeStackItem_t*         computeStacks,
                       uint32_t                            equationIndex,
                       const uint32_t*                     activeEquationSetIndexes,
                       csEvaluationContext_t               EC,
                       const real_t*                       dofs,
                       const real_t*                       values,
                       const real_t*                       timeDerivatives,
                       real_t*                             residuals)
{
    /* Locate the current equation stack in the array of all compute stacks. */
    uint32_t firstIndex                      = activeEquationSetIndexes[equationIndex];
    const csComputeStackItem_t* computeStack = &computeStacks[firstIndex];

    /* Evaluate the compute stack (scaling is included). */
    adouble_t res_cs = evaluateComputeStack(computeStack,
                                            EC,
                                            dofs,
                                            values,
                                            timeDerivatives,
                                            NULL,
                                            NULL);

    /* Set the value in the residuals array. */
    residuals[equationIndex] = res_cs.m_dValue;
}

/* Jacobian kernel functions. */
void EvaluateJacobian(const csComputeStackItem_t*        computeStacks,
                      uint32_t                           jacobianItemIndex,
                      const uint32_t*                    activeEquationSetIndexes,
                      const csJacobianMatrixItem_t*      computeStackJacobianItems,
                      csEvaluationContext_t              EC,
                      const real_t*                      dofs,
                      const real_t*                      values,
                      const real_t*                      timeDerivatives,
                      real_t*                            jacobian)
{
    /* Locate the current equation stack in the array of all compute stacks. */
    csJacobianMatrixItem_t jacobianItem      = computeStackJacobianItems[jacobianItemIndex];
    uint32_t firstIndex                      = activeEquationSetIndexes[jacobianItem.equationIndex];
    const csComputeStackItem_t* computeStack = &computeStacks[firstIndex];

    /* Set the overall index for jacobian evaluation. */
    EC.jacobianIndex = jacobianItem.overallIndex;

    /* Evaluate the compute stack (scaling is included). */
    adouble_t jac_cs = evaluateComputeStack(computeStack,
                                               EC,
                                               dofs,
                                               values,
                                               timeDerivatives,
                                               NULL,
                                               NULL);

    /* Set the value in the jacobian array. */
    jacobian[jacobianItemIndex] = jac_cs.m_dDeriv;
}

/* Residual kernel function .*/
void EvaluateSensitivityResiduals(const csComputeStackItem_t*        computeStacks,
                                  uint32_t                           equationIndex,
                                  const uint32_t*                    activeEquationSetIndexes,
                                  csEvaluationContext_t              EC,
                                  const real_t*                      dofs,
                                  const real_t*                      values,
                                  const real_t*                      timeDerivatives,
                                  const real_t*                      svalues,
                                  const real_t*                      sdvalues,
                                        real_t*                      sresiduals)
{
    /* Locate the current equation stack in the array of all compute stacks. */
    uint32_t firstIndex                      = activeEquationSetIndexes[equationIndex];
    const csComputeStackItem_t* computeStack = &computeStacks[firstIndex];

    /* Evaluate the compute stack (scaling is included). */
    adouble_t res_cs = evaluateComputeStack(computeStack,
                                            EC,
                                            dofs,
                                            values,
                                            timeDerivatives,
                                            svalues,
                                            sdvalues);

    /* Set the value in the sensitivity array matrix (here we access the data using its row pointers). */
    sresiduals[equationIndex] = res_cs.m_dDeriv;
}

}
