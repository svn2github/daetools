/***********************************************************************************
*                 DAE Tools Project: www.daetools.com
*                 Copyright (C) Dragan Nikolic
************************************************************************************
DAE Tools is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License version 3 as published by the Free Software
Foundation. DAE Tools is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with the
DAE Tools software; if not, see <http://www.gnu.org/licenses/>.
***********************************************************************************/
#include "compute_stack_opencl_multi.h"
#include <iostream>
#include <sstream>
#include <numeric>
#include <functional>
#include <omp.h>

daeComputeStackEvaluator_OpenCL_multi::daeComputeStackEvaluator_OpenCL_multi(const std::vector<int>&    platforms,
                                                                             const std::vector<int>&    devices,
                                                                             const std::vector<double>& taskPortions,
                                                                             std::string                buildProgramOptions)
{
    if(platforms.size() != devices.size() || platforms.size() != taskPortions.size())
        throw std::runtime_error("Invalid number of platforms/devices/taskPortions specifed");

    m_platforms    = platforms;
    m_devices      = devices;
    m_taskPortions = taskPortions;

    // Normalise the task portions.
    double totalTask = std::accumulate(m_taskPortions.begin(), m_taskPortions.end(), 0.0);
    for(size_t i = 0; i < m_platforms.size(); i++)
        m_taskPortions[i] = m_taskPortions[i] / totalTask;

    for(size_t i = 0; i < m_platforms.size(); i++)
    {
        int    platformID  = m_platforms[i];
        int    deviceID    = m_devices[i];
        double taskPortion = m_taskPortions[i];

        daeComputeStackEvaluator_OpenCL* evaluator = new daeComputeStackEvaluator_OpenCL(platformID, deviceID, buildProgramOptions);
        m_evaluators.push_back(evaluator);

        printf("OpenCL device %s:\n", evaluator->selectedDeviceName.c_str());
        printf("    platformID  = %d\n", platformID);
        printf("    deviceID    = %d\n", deviceID);
        printf("    taskPortion = %f\n", taskPortion);
    }
}

daeComputeStackEvaluator_OpenCL_multi::~daeComputeStackEvaluator_OpenCL_multi()
{
    for(size_t i = 0; i < m_evaluators.size(); i++)
    {
        daeComputeStackEvaluator_OpenCL* evaluator = m_evaluators[i];
        delete evaluator;
    }
    m_evaluators.clear();
}

void daeComputeStackEvaluator_OpenCL_multi::Initialize(bool calculateSensitivities,
                                                       size_t numberOfVariables,
                                                       size_t numberOfEquationsToProcess,
                                                       size_t numberOfDOFs,
                                                       size_t numberOfComputeStackItems,
                                                       size_t numberOfJacobianItems,
                                                       size_t numberOfJacobianItemsToProcess,
                                                       adComputeStackItem_t*    computeStacks,
                                                       uint32_t*                activeEquationSetIndexes,
                                                       adJacobianMatrixItem_t*  computeStackJacobianItems)
{
    bool                    calculateSensitivities_i;
    size_t                  numberOfVariables_i;
    size_t                  numberOfEquationsToProcess_i;
    size_t                  numberOfDOFs_i;
    size_t                  numberOfComputeStackItems_i;
    size_t                  numberOfJacobianItems_i;
    size_t                  numberOfJacobianItemsToProcess_i;
    adComputeStackItem_t*   computeStacks_i;
    uint32_t*               activeEquationSetIndexes_i;
    adJacobianMatrixItem_t* computeStackJacobianItems_i;

    size_t noEvaluators = m_evaluators.size();

    m_startEquationIndexes.clear();
    m_startJacobianIndexes.clear();
    m_noEquations.clear();
    m_noJacobianIndexes.clear();
    for(size_t i = 0; i < m_evaluators.size(); i++)
    {
        if(i == noEvaluators-1) // The last evaluator gets the remaining of equations/jacobian items
        {
            int Neqs = numberOfVariables     - std::accumulate(m_noEquations.begin(),       m_noEquations.begin() + i,       0.0);
            int Njis = numberOfJacobianItems - std::accumulate(m_noJacobianIndexes.begin(), m_noJacobianIndexes.begin() + i, 0.0);
            m_noEquations.push_back(Neqs);
            m_noJacobianIndexes.push_back(Njis);
        }
        else
        {
            m_noEquations.push_back(int(m_taskPortions[i] * numberOfVariables));
            m_noJacobianIndexes.push_back(int(m_taskPortions[i] * numberOfJacobianItems));
        }
    }

    for(size_t i = 0; i < m_evaluators.size(); i++)
    {
        int sei = std::accumulate(m_noEquations.begin(),       m_noEquations.begin() + i,       0.0);
        int sji = std::accumulate(m_noJacobianIndexes.begin(), m_noJacobianIndexes.begin() + i, 0.0);
        m_startEquationIndexes.push_back(sei);
        m_startJacobianIndexes.push_back(sji);
    }

    for(size_t i = 0; i < m_evaluators.size(); i++)
    {
        daeComputeStackEvaluator_OpenCL* evaluator = m_evaluators[i];

        printf("OpenCL device %s:\n", evaluator->selectedDeviceName.c_str());
        printf("    Equations range: [%d, %d)\n",  m_startEquationIndexes[i], m_startEquationIndexes[i]+m_noEquations[i]);
        printf("    Jacobians range: [%d, %d)\n",  m_startJacobianIndexes[i], m_startJacobianIndexes[i]+m_noJacobianIndexes[i]);

        calculateSensitivities_i         = calculateSensitivities;
        numberOfVariables_i              = numberOfVariables;
        numberOfEquationsToProcess_i     = m_noEquations[i];
        numberOfDOFs_i                   = numberOfDOFs;
        numberOfComputeStackItems_i      = numberOfComputeStackItems;
        numberOfJacobianItems_i          = numberOfJacobianItems;
        numberOfJacobianItemsToProcess_i = m_noJacobianIndexes[i];
        computeStacks_i                  = computeStacks;
        activeEquationSetIndexes_i       = activeEquationSetIndexes;
        computeStackJacobianItems_i      = computeStackJacobianItems;

        evaluator->Initialize(calculateSensitivities_i,
                              numberOfVariables_i,
                              numberOfEquationsToProcess_i,
                              numberOfDOFs_i,
                              numberOfComputeStackItems_i,
                              numberOfJacobianItems_i,
                              numberOfJacobianItemsToProcess_i,
                              computeStacks_i,
                              activeEquationSetIndexes_i,
                              computeStackJacobianItems_i);
    }
}

void daeComputeStackEvaluator_OpenCL_multi::FreeResources()
{
    size_t noEvaluators = m_evaluators.size();
    for(size_t i = 0; i < noEvaluators; i++)
    {
        daeComputeStackEvaluator_OpenCL* evaluator = m_evaluators[i];
        evaluator->FreeResources();
    }
}

/* Residual kernel function. */
void daeComputeStackEvaluator_OpenCL_multi::EvaluateResiduals(daeComputeStackEvaluationContext_t EC,
                                                              real_t*                            dofs,
                                                              real_t*                            values,
                                                              real_t*                            timeDerivatives,
                                                              real_t*                            residuals)
{
    size_t noEvaluators = m_evaluators.size();
    if(noEvaluators > 0)
        omp_set_num_threads(noEvaluators);

    #pragma omp parallel for firstprivate(EC)
    for(size_t i = 0; i < noEvaluators; i++)
    {
        daeComputeStackEvaluator_OpenCL* evaluator = m_evaluators[i];

        EC.numberOfEquations      = m_noEquations[i];
        EC.startEquationIndex     = m_startEquationIndexes[i];
        size_t startEquationIndex = m_startEquationIndexes[i];

        //printf("Calculate residuals on OpenCL device: %s\n", evaluator->selectedDeviceName.c_str());
        evaluator->EvaluateResiduals(EC,
                                     dofs,
                                     values,
                                     timeDerivatives,
                                     &residuals[startEquationIndex]);
    }
}

/* Jacobian kernel function (generic version). */
void daeComputeStackEvaluator_OpenCL_multi::EvaluateJacobian(daeComputeStackEvaluationContext_t EC,
                                                             real_t*                            dofs,
                                                             real_t*                            values,
                                                             real_t*                            timeDerivatives,
                                                             real_t*                            jacobianItems)
{
    size_t noEvaluators = m_evaluators.size();
    if(noEvaluators > 0)
        omp_set_num_threads(noEvaluators);

    #pragma omp parallel for firstprivate(EC)
    for(size_t i = 0; i < noEvaluators; i++)
    {
        daeComputeStackEvaluator_OpenCL* evaluator = m_evaluators[i];

        EC.numberOfEquations      = m_noEquations[i];
        EC.numberOfJacobianItems  = m_noJacobianIndexes[i];
        EC.startJacobianIndex     = m_startJacobianIndexes[i];
        size_t startJacobianIndex = m_startJacobianIndexes[i];

        //printf("Calculate jacobian on OpenCL device: %s\n", evaluator->selectedDeviceName.c_str());
        evaluator->EvaluateJacobian(EC,
                                    dofs,
                                    values,
                                    timeDerivatives,
                                    &jacobianItems[startJacobianIndex]);
    }
}

/* Sensitivity residual kernel function. */
void daeComputeStackEvaluator_OpenCL_multi::EvaluateSensitivityResiduals(daeComputeStackEvaluationContext_t    EC,
                                                                         real_t*                               dofs,
                                                                         real_t*                               values,
                                                                         real_t*                               timeDerivatives,
                                                                         real_t*                               svalues,
                                                                         real_t*                               sdvalues,
                                                                         real_t*                               sresiduals)
{
    size_t noEvaluators = m_evaluators.size();
    if(noEvaluators > 0)
        omp_set_num_threads(noEvaluators);

    #pragma omp parallel for firstprivate(EC)
    for(size_t i = 0; i < noEvaluators; i++)
    {
        daeComputeStackEvaluator_OpenCL* evaluator = m_evaluators[i];

        EC.numberOfEquations      = m_noEquations[i];
        EC.startEquationIndex     = m_startEquationIndexes[i];
        size_t startEquationIndex = m_startEquationIndexes[i];

        //printf("Calculate sens. residuals on OpenCL device: %s\n", evaluator->selectedDeviceName.c_str());
        evaluator->EvaluateSensitivityResiduals(EC,
                                                dofs,
                                                values,
                                                timeDerivatives,
                                                svalues,
                                                sdvalues,
                                                &sresiduals[startEquationIndex]);
    }
}

