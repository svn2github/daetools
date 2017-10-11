#include "stdafx.h"
#include "coreimpl.h"
#include <omp.h>

namespace dae
{
namespace core
{

daeBlock::daeBlock(void)
{
    m_bInitializeMode					= false;
    m_pDataProxy						= NULL;
    m_parrTimeDerivatives               = NULL;
    m_parrValues					    = NULL;
    m_parrResidual						= NULL;
    m_pmatJacobian						= NULL;
    m_dCurrentTime						= 0;
    m_dInverseTimeStep					= 0;
    m_nNumberOfEquations                = 0;
    m_nTotalNumberOfVariables           = 0;
    m_nCurrentVariableIndexForJacobianEvaluation = ULONG_MAX;

    m_dTotalTimeForResiduals            = 0;
    m_dTotalTimeForJacobian             = 0;
    m_dTotalTimeForSensitivityResiduals = 0;

    daeConfig& cfg = daeGetConfig();

    m_bUseOpenMP      = cfg.GetBoolean("daetools.core.equations.parallelEvaluation", false);
    m_omp_num_threads = cfg.GetInteger("daetools.core.equations.numThreads", 0);
    if(m_omp_num_threads > 0)
        omp_set_num_threads(m_omp_num_threads);

/*
    m_omp_schedule           = cfg.GetString ("daetools.core.equations.schedule",         "default");
    m_omp_shedule_chunk_size = cfg.GetInteger("daetools.core.equations.scheduleChunkSize", 0);
    // If the schedule in the config file is 'default' then it is left to the implementation default
    if(m_omp_schedule == "static")
        omp_set_schedule(omp_sched_static, m_omp_shedule_chunk_size);
    else if(m_omp_schedule == "dynamic")
        omp_set_schedule(omp_sched_dynamic, m_omp_shedule_chunk_size);
    else if(m_omp_schedule == "guided")
        omp_set_schedule(omp_sched_guided, m_omp_shedule_chunk_size);
    else if(m_omp_schedule == "auto")
        omp_set_schedule(omp_sched_auto, m_omp_shedule_chunk_size);
*/

#if defined(DAE_MPI)
    m_nEquationIndexesStart = ULONG_MAX;
    m_nEquationIndexesEnd   = ULONG_MAX;
    m_nVariableIndexesStart = ULONG_MAX;
    m_nVariableIndexesEnd   = ULONG_MAX;
#endif
}

daeBlock::~daeBlock(void)
{
}

void daeBlock::Open(io::xmlTag_t* pTag)
{
    io::daeSerializable::Open(pTag);
}

void daeBlock::Save(io::xmlTag_t* pTag) const
{
    io::daeSerializable::Save(pTag);
}

void daeBlock::CalculateConditions(real_t				dTime,
                                   daeArray<real_t>&	arrValues,
                                   daeArray<real_t>&	arrTimeDerivatives,
                                   daeArray<real_t>&	arrResults)
{
    map<size_t, daeExpressionInfo>::iterator iter;

    if(m_pDataProxy->PrintInfo())
        m_pDataProxy->LogMessage(string("Evaluate conditions at time ") + toStringFormatted(dTime, -1, 15) + string("..."), 0);

    SetTime(dTime);
    m_pDataProxy->SetCurrentTime(dTime);
    SetValuesArray(&arrValues);
    SetTimeDerivativesArray(&arrTimeDerivatives);

    daeExecutionContext EC;
    EC.m_pBlock						= this;
    EC.m_pDataProxy					= m_pDataProxy;
    EC.m_eEquationCalculationMode	= eCalculate;

    size_t nFnCounter = 0;
    for(iter = m_mapExpressionInfos.begin(); iter != m_mapExpressionInfos.end(); iter++)
    {
        arrResults.SetItem(nFnCounter, (*iter).second.m_pExpression->Evaluate(&EC).getValue());
        nFnCounter++;
    }
}

void daeBlock::CalculateResiduals(real_t			dTime,
                                  daeArray<real_t>& arrValues,
                                  daeArray<real_t>& arrResiduals,
                                  daeArray<real_t>& arrTimeDerivatives)
{
    if(m_pDataProxy->PrintInfo())
        m_pDataProxy->LogMessage(string("Calculate residuals at time ") + toStringFormatted(dTime, -1, 15) + string("..."), 0);

    SetTime(dTime);
    m_pDataProxy->SetCurrentTime(dTime);
    SetValuesArray(&arrValues);
    SetTimeDerivativesArray(&arrTimeDerivatives);
    SetResidualArray(&arrResiduals);

    daeExecutionContext EC;
    EC.m_pBlock						= this;
    EC.m_pDataProxy					= m_pDataProxy;
    EC.m_dInverseTimeStep			= GetInverseTimeStep();
    EC.m_pEquationExecutionInfo		= NULL;
    EC.m_eEquationCalculationMode	= eCalculate;

    daeModel* pTopLevelModel = dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel());

    // Update equations if necessary (in general, applicable only to FE equations)
    pTopLevelModel->UpdateEquations(&EC);

// Commented out because of a seg. fault (can't find the reason).
//    double startTime, endTime;
//    bool bPrintInfo = m_pDataProxy->PrintInfo();
//    if(bPrintInfo)
//    {
//        startTime = dae::GetTimeInSeconds();
//    }

/* Old, serial code
// First calculate normal equations (non-STN)
    for(size_t i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
    {
        daeEquationExecutionInfo* pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
        pEquationExecutionInfo->Residual(EC);
    }

// Now calculate STN equations
    for(size_t i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        pSTN->CalculateResiduals(EC);
    }
*/

    // Calls PyEval_InitThreads() and PyEval_SaveThread in the constructor, and PyEval_RestoreThread in the destructor
    boost::shared_ptr<daeAllowThreads_t> _allowThreads_ = pTopLevelModel->CreateAllowThreads();
    //std::cout << typeid(*_allowThreads_).name() << std::endl;

    // OpenMP version
    std::vector<daeEquationExecutionInfo*> ptrarrEquationExecutionInfos;
    GetEquationExecutionInfos(ptrarrEquationExecutionInfos);

    #pragma omp parallel for firstprivate(EC) if (m_bUseOpenMP)
    for(int i = 0; i < ptrarrEquationExecutionInfos.size(); i++)
    {
        daeEquationExecutionInfo* pEquationExecutionInfo = ptrarrEquationExecutionInfos[i];
        pEquationExecutionInfo->Residual(EC);
    }

// Commented out because of a seg. fault (can't find the reason).
//    if(bPrintInfo)
//    {
//        endTime = dae::GetTimeInSeconds();
//        m_dTotalTimeForResiduals += (endTime - startTime);
//        m_pDataProxy->LogMessage(string("Total time for the current residuals evaluation = ") + toStringFormatted(endTime - startTime, -1, 10) + string("s"), 0);
//        m_pDataProxy->LogMessage(string("Cumulative time for all residuals evaluations = ") + toStringFormatted(m_dTotalTimeForResiduals, -1, 10) + string("s"), 0);
//    }
}

void daeBlock::CalculateJacobian(real_t				dTime,
                                 daeArray<real_t>&	arrValues,
                                 daeArray<real_t>&	arrResiduals,
                                 daeArray<real_t>&	arrTimeDerivatives,
                                 daeMatrix<real_t>&	matJacobian,
                                 real_t				dInverseTimeStep)
{
    if(m_pDataProxy->PrintInfo())
        m_pDataProxy->LogMessage(string("Calculate Jacobian at time ") + toStringFormatted(dTime, -1, 15) + string("..."), 0);

    SetTime(dTime);
    m_pDataProxy->SetCurrentTime(dTime);
    SetValuesArray(&arrValues);
    SetTimeDerivativesArray(&arrTimeDerivatives);
    SetResidualArray(&arrResiduals);
    SetJacobianMatrix(&matJacobian);
    SetInverseTimeStep(dInverseTimeStep);

    daeExecutionContext EC;
    EC.m_pBlock						= this;
    EC.m_pDataProxy					= m_pDataProxy;
    EC.m_dInverseTimeStep			= GetInverseTimeStep();
    EC.m_pEquationExecutionInfo		= NULL;
    EC.m_eEquationCalculationMode	= eCalculateJacobian;

    daeModel* pTopLevelModel = dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel());

    // Update equations if necessary (in general, applicable only to FE equations)
    pTopLevelModel->UpdateEquations(&EC);

// Commented out because of a seg. fault (can't find the reason).
//    double startTime, endTime;
//    bool bPrintInfo = m_pDataProxy->PrintInfo();
//    if(bPrintInfo)
//    {
//        startTime = dae::GetTimeInSeconds();
//    }

/* Old, serial code
// First calculate normal equations (non-STN)
    for(size_t i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
    {
        daeEquationExecutionInfo* pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
        pEquationExecutionInfo->Jacobian(EC);
    }

// Now calculate STN equations
    for(size_t i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        pSTN->CalculateJacobian(EC);
    }
*/

    // Calls PyEval_InitThreads() and PyEval_SaveThread in the constructor, and PyEval_RestoreThread in the destructor
    boost::shared_ptr<daeAllowThreads_t> _allowThreads_ = pTopLevelModel->CreateAllowThreads();

    // OpenMP version
    std::vector<daeEquationExecutionInfo*> ptrarrEquationExecutionInfos;
    GetEquationExecutionInfos(ptrarrEquationExecutionInfos);

    #pragma omp parallel for firstprivate(EC) if (m_bUseOpenMP)
    for(int i = 0; i < ptrarrEquationExecutionInfos.size(); i++)
    {
        //std::cout << "  thread id " << omp_get_thread_num() << " calculating Jacobian for equation " << i << std::endl;
        daeEquationExecutionInfo* pEquationExecutionInfo = ptrarrEquationExecutionInfos[i];
        pEquationExecutionInfo->Jacobian(EC);
    }

// Commented out because of a seg. fault (can't find the reason).
//    if(bPrintInfo)
//    {
//        endTime = dae::GetTimeInSeconds();
//        m_dTotalTimeForJacobian += (endTime - startTime);
//        m_pDataProxy->LogMessage(string("Total time for the current Jacobian evaluation = ") + toStringFormatted(endTime - startTime, -1, 10) + string("s"), 0);
//        m_pDataProxy->LogMessage(string("Cumulative time for all Jacobian evaluations = ") + toStringFormatted(m_dTotalTimeForJacobian, -1, 10) + string("s"), 0);
//    }
}

// For dynamic models
void daeBlock::CalculateSensitivityResiduals(real_t						dTime,
                                             const std::vector<size_t>& narrParameterIndexes,
                                             daeArray<real_t>&			arrValues,
                                             daeArray<real_t>&			arrTimeDerivatives,
                                             daeMatrix<real_t>&			matSValues,
                                             daeMatrix<real_t>&			matSTimeDerivatives,
                                             daeMatrix<real_t>&			matSResiduals)
{
    if(m_pDataProxy->PrintInfo())
        m_pDataProxy->LogMessage(string("Calculate sensitivity residuals at time ") + toStringFormatted(dTime, -1, 15) + string("..."), 0);

    SetTime(dTime);
    m_pDataProxy->SetCurrentTime(dTime);
    SetValuesArray(&arrValues);
    SetTimeDerivativesArray(&arrTimeDerivatives);
    m_pDataProxy->SetSensitivityMatrixes(&matSValues,
                                         &matSTimeDerivatives,
                                         &matSResiduals);

    daeExecutionContext EC;
    EC.m_pBlock						= this;
    EC.m_pDataProxy					= m_pDataProxy;
    EC.m_dInverseTimeStep			= GetInverseTimeStep(); // ??????
    EC.m_pEquationExecutionInfo		= NULL;
    EC.m_eEquationCalculationMode	= eCalculateSensitivityResiduals;

    daeModel* pTopLevelModel = dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel());

    // Update equations if necessary (in general, applicable only to FE equations)
    pTopLevelModel->UpdateEquations(&EC);

// Commented out because of a seg. fault (can't find the reason).
//    double startTime, endTime;
//    bool bPrintInfo = m_pDataProxy->PrintInfo();
//    if(bPrintInfo)
//    {
//        startTime = dae::GetTimeInSeconds();
//    }

/* Old, serial code
    for(size_t i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
    {
        daeEquationExecutionInfo* pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
        pEquationExecutionInfo->SensitivityResiduals(EC, narrParameterIndexes);
    }

// In general, neither objective function nor constraints can be within an STN
    for(size_t i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        pSTN->CalculateSensitivityResiduals(EC, narrParameterIndexes);
    }
*/

    // Calls PyEval_InitThreads() and PyEval_SaveThread in the constructor, and PyEval_RestoreThread in the destructor
    boost::shared_ptr<daeAllowThreads_t> _allowThreads_ = pTopLevelModel->CreateAllowThreads();

    // OpenMP version
    std::vector<daeEquationExecutionInfo*> ptrarrEquationExecutionInfos;
    GetEquationExecutionInfos(ptrarrEquationExecutionInfos);

    #pragma omp parallel for firstprivate(EC) if (m_bUseOpenMP)
    for(int i = 0; i < ptrarrEquationExecutionInfos.size(); i++)
    {

        daeEquationExecutionInfo* pEquationExecutionInfo = ptrarrEquationExecutionInfos[i];
        pEquationExecutionInfo->SensitivityResiduals(EC, narrParameterIndexes);
    }

    m_pDataProxy->ResetSensitivityMatrixes();

// Commented out because of a seg. fault (can't find the reason).
//    if(bPrintInfo)
//    {
//        endTime = dae::GetTimeInSeconds();
//        m_dTotalTimeForSensitivityResiduals += (endTime - startTime);
//        m_pDataProxy->LogMessage(string("Total time for the current sensitivity residuals evaluation = ") + toStringFormatted(endTime - startTime, -1, 10) + string("s"), 0);
//        m_pDataProxy->LogMessage(string("Cumulative time for all sensitivity residuals evaluations = ") + toStringFormatted(m_dTotalTimeForSensitivityResiduals, -1, 10) + string("s"), 0);
//    }
}

// For steady-state models (not used anymore)
void daeBlock::CalculateSensitivityParametersGradients(const std::vector<size_t>& narrParameterIndexes,
                                                       daeArray<real_t>&		  arrValues,
                                                       daeArray<real_t>&		  arrTimeDerivatives,
                                                       daeMatrix<real_t>&		  matSResiduals)
{
    size_t i;
    daeSTN* pSTN;
    daeEquationExecutionInfo* pEquationExecutionInfo;

    if(m_pDataProxy->PrintInfo())
        m_pDataProxy->LogMessage(string("Calculate sensitivity gradients..."), 0);

    SetValuesArray(&arrValues);
    SetTimeDerivativesArray(&arrTimeDerivatives);
    m_pDataProxy->SetSensitivityMatrixes(NULL, NULL, &matSResiduals);

    daeExecutionContext EC;
    EC.m_pBlock						= this;
    EC.m_pDataProxy					= m_pDataProxy;
    EC.m_dInverseTimeStep			= GetInverseTimeStep();
    EC.m_pEquationExecutionInfo		= NULL;
    EC.m_eEquationCalculationMode	= eCalculateSensitivityParametersGradients;

    // Update equations if necessary (in general, applicable only to FE equations)
    dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel())->UpdateEquations(&EC);

    for(i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
    {
        pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
        pEquationExecutionInfo->SensitivityParametersGradients(EC, narrParameterIndexes);
    }

// In general, neither objective function nor constraints can be within an STN
    for(i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        pSTN->CalculateSensitivityParametersGradients(EC, narrParameterIndexes);
    }

    m_pDataProxy->ResetSensitivityMatrixes();
}

void daeBlock::CalcNonZeroElements(int& NNZ)
{
    size_t i;
    daeSTN* pSTN;
    daeEquationExecutionInfo* pEquationExecutionInfo;

// First find in normal equations (non-STN)
    for(i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
    {
        pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
        NNZ += pEquationExecutionInfo->m_mapIndexes.size();
    }

// Then in STN equations
    for(i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        pSTN->CalcNonZeroElements(NNZ);
    }
}

void daeBlock::FillSparseMatrix(daeSparseMatrix<real_t>* pMatrix)
{
    size_t i;
    daeSTN* pSTN;
    daeEquationExecutionInfo* pEquationExecutionInfo;

    for(i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
    {
        pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
        pMatrix->AddRow(pEquationExecutionInfo->m_mapIndexes);
    }

    for(i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        pSTN->FillSparseMatrix(pMatrix);
    }
}

void daeBlock::FillAbsoluteTolerancesInitialConditionsAndInitialGuesses(daeArray<real_t>& arrValues,
                                                                        daeArray<real_t>& arrTimeDerivatives,
                                                                        daeArray<real_t>& arrInitialConditionsTypes,
                                                                        daeArray<real_t>& arrAbsoluteTolerances)
{
    map<size_t, size_t>::iterator iter;

    if(GetNumberOfEquations() != m_mapVariableIndexes.size())
    {
        daeDeclareException(exInvalidCall);
        e << "Number of equation is not equal to number of variables";
        throw e;
    }
    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);

    const real_t* pBlockValues            = m_pDataProxy->GetInitialValuesPointer();
    const real_t* pBlockInitialConditions = m_pDataProxy->GetInitialConditionsPointer();
    // Here I need information which variables are differential
    const real_t* pBlockIDs               = m_pDataProxy->GetVariableTypesGatheredPointer();
    const real_t* pBlockAbsoluteTolerance = m_pDataProxy->GetAbsoluteTolerancesPointer();

    for(iter = m_mapVariableIndexes.begin(); iter != m_mapVariableIndexes.end(); iter++)
    {
        arrValues.SetItem                (iter->second, pBlockValues[iter->first]);
        arrTimeDerivatives.SetItem       (iter->second, pBlockInitialConditions[iter->first]);
        arrInitialConditionsTypes.SetItem(iter->second, pBlockIDs[iter->first]);
        arrAbsoluteTolerances.SetItem    (iter->second, pBlockAbsoluteTolerance[iter->first]);
    }
}

//void daeBlock::SetAllInitialConditions(real_t value)
//{
//	daeDeclareAndThrowException(exNotImplemented);

//	if(!m_pDataProxy)
//		daeDeclareAndThrowException(exInvalidPointer);

//	size_t n = m_pDataProxy->GetTotalNumberOfVariables();
//	for(size_t i = 0; i < n; i++)
//	{
//		if(m_pDataProxy->GetVariableTypeGathered(i) == cnDifferential)
//		{
//			m_pDataProxy->SetInitialCondition(i, value, m_pDataProxy->GetInitialConditionMode());
//			m_pDataProxy->SetVariableType(i, cnDifferential);
//		}
//	}
//}

size_t daeBlock::FindVariableBlockIndex(size_t nVariableOverallIndex) const
{
    map<size_t, size_t>::const_iterator iter = m_mapVariableIndexes.find(nVariableOverallIndex);
    if(iter != m_mapVariableIndexes.end()) // if found
        return iter->second;
    else
        return ULONG_MAX;
}

bool daeBlock::IsModelDynamic() const
{
    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);

    return m_pDataProxy->IsModelDynamic();
}
/*
real_t* daeBlock::GetValuesPointer()
{
#ifdef DAE_DEBUG
    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    return m_pDataProxy->GetValue(0);
}

real_t* daeBlock::GetTimeDerivativesPointer()
{
#ifdef DAE_DEBUG
    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    return m_pDataProxy->GetTimeDerivative(0);
}

real_t* daeBlock::GetAbsoluteTolerancesPointer()
{
#ifdef DAE_DEBUG
    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    return m_pDataProxy->GetAbsoluteTolerance(0);
}

real_t* daeBlock::GetVariableTypesPointer()
{
#ifdef DAE_DEBUG
    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    return m_pDataProxy->GetVariableTypes();
}
*/

void daeBlock::CleanUpSetupData()
{
    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);
    m_pDataProxy->CleanUpSetupData();
}

void daeBlock::CreateIndexMappings(real_t* pdValues, real_t* pdTimeDerivatives)
{
    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);
    m_pDataProxy->CreateIndexMappings(m_mapVariableIndexes, pdValues, pdTimeDerivatives);
}

void daeBlock::SetBlockData(daeArray<real_t>& arrValues, daeArray<real_t>& arrTimeDerivatives)
{
// Now we use block indexes to directly access the solvers arrays and therefore no need to actually copy anything
    m_parrValues          = &arrValues;
    m_parrTimeDerivatives = &arrTimeDerivatives;

/*
#ifdef DAE_DEBUG
    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
// m_mapVariableIndexes<nOverallIndex, nBlockIndex>
    real_t* pBlockValues          = m_pDataProxy->GetValue(0);
    real_t* pBlockTimeDerivatives = m_pDataProxy->GetTimeDerivative(0);

    for(map<size_t, size_t>::iterator iter = m_mapVariableIndexes.begin(); iter != m_mapVariableIndexes.end(); iter++)
    {
        pBlockValues         [iter->first] = arrValues         [iter->second];
        pBlockTimeDerivatives[iter->first] = arrTimeDerivatives[iter->second];
    }
*/
}

/*
void daeBlock::CopyDataToSolver(daeArray<real_t>& arrValues, daeArray<real_t>& arrTimeDerivatives) const
{
#ifdef DAE_DEBUG
    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
// m_mapVariableIndexes<nOverallIndex, nBlockIndex>
    const real_t* pBlockValues          = m_pDataProxy->GetValue(0);
    const real_t* pBlockTimeDerivatives = m_pDataProxy->GetTimeDerivative(0);

    for(map<size_t, size_t>::const_iterator iter = m_mapVariableIndexes.begin(); iter != m_mapVariableIndexes.end(); iter++)
    {
        arrValues         [iter->second] = pBlockValues         [iter->first];
        arrTimeDerivatives[iter->second] = pBlockTimeDerivatives[iter->first];
    }
}
*/

void daeBlock::Initialize(void)
{
    size_t i;
    pair<size_t, size_t> uintPair;
    map<size_t, size_t>::iterator iter;
    daeSTN* pSTN;

    if(!m_pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);

    m_pDataProxy->SetBlock(this);

    if(GetNumberOfEquations() != m_mapVariableIndexes.size())
    {
        daeDeclareException(exInvalidCall);
        e << "Number of equations [" << GetNumberOfEquations() << "] is not equal to number of variables used in equations [" << m_mapVariableIndexes.size() << "]";
        throw e;
    }

// First BuildExpressions in the top level model and its children
    daeModel* pModel = dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel());
    if(!pModel)
        daeDeclareException(exInvalidPointer);
    pModel->BuildExpressions(this);

// Then BuildExpressions in all STNs
    for(i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        if(!pSTN)
            daeDeclareAndThrowException(exInvalidPointer);

        pSTN->BuildExpressions(this);
    }

/* We do not have to check for discontinuities here - it will be done during the initialization
   And, btw, this does not affect anything, it just checks for discontinuities. Properly done,
   it should call ExecuteOnConditionActions() if CheckDiscontinuities() returns true.

    pModel->CheckDiscontinuities();

    for(i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        if(!pSTN)
            daeDeclareAndThrowException(exInvalidPointer);

        pSTN->CheckDiscontinuities();
    }
*/
    RebuildExpressionMap();
}

bool daeBlock::CheckForDiscontinuities(void)
{
    size_t i;
    daeSTN* pSTN;

    if(m_dCurrentTime > 0 && m_pDataProxy->PrintInfo())
        m_pDataProxy->LogMessage(string("Checking state transitions at time ") + toStringFormatted<real_t>(m_dCurrentTime, -1, 15) + string("..."), 0);

// Achtung, Achtung!!
// Moved to daeSimulation::Integrate_xxx() functions to get it reset before every call to simulation.Integrate/daesolver.Solve
//    m_pDataProxy->SetLastSatisfiedCondition(NULL);

// First check discontinuities in the top level model
    daeModel* pModel = dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel());
    if(pModel->CheckDiscontinuities())
        return true;

// Then check conditions from STNs
    for(i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        if(pSTN->CheckDiscontinuities())
            return true;
    }

    return false;
}

daeeDiscontinuityType daeBlock::ExecuteOnConditionActions(void)
{
    size_t i;
    daeSTN* pSTN;
    daeeDiscontinuityType eResult;

    m_pDataProxy->SetReinitializationFlag(false);
    m_pDataProxy->SetCopyDataFromBlock(false);

    daeModel* pModel = dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel());
    pModel->ExecuteOnConditionActions();

    for(i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        pSTN->ExecuteOnConditionActions();
    }

// If any of the actions changed the state it has to be indicated in those flags
    if(m_pDataProxy->GetReinitializationFlag() && m_pDataProxy->GetCopyDataFromBlock())
    {
        eResult = eModelDiscontinuityWithDataChange;
    }
    else if(m_pDataProxy->GetReinitializationFlag())
    {
        eResult = eModelDiscontinuity;
    }
    else
    {
        eResult = eNoDiscontinuity;
    }

    return eResult;
}

void daeBlock::RebuildExpressionMap()
{
    size_t i;
    daeSTN* pSTN;

    m_mapExpressionInfos.clear();

// First rebuild for the top level model
    daeModel* pModel = dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel());
    pModel->AddExpressionsToBlock(this);

// Then for all othe STNs
    for(i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        pSTN->AddExpressionsToBlock(this);
    }
}

bool daeBlock::CheckOverlappingAndAddVariables(const vector<size_t>& narrVariablesInEquation)
{
    size_t i, k;
    pair<size_t, size_t> uintPair;
    map<size_t, size_t>::iterator iter;

    for(i = 0; i < narrVariablesInEquation.size(); i++)
    {
        iter = m_mapVariableIndexes.find(narrVariablesInEquation[i]);
        if(iter != m_mapVariableIndexes.end()) //if found
        {
            for(k = 0; k < narrVariablesInEquation.size(); k++)
            {
                uintPair.first  = narrVariablesInEquation[k];  // overall block
                uintPair.second = m_mapVariableIndexes.size(); // index in block
                m_mapVariableIndexes.insert(uintPair);
            }
            return true;
        }
    }

    return false;
}

void daeBlock::AddVariables(const map<size_t, size_t>& mapIndexes)
{
    pair<size_t, size_t> uintPair;
    map<size_t, size_t>::const_iterator iter;

    for(iter = mapIndexes.begin(); iter != mapIndexes.end(); iter++)
    {
        uintPair.first  = (*iter).first;				// overall index
        uintPair.second = m_mapVariableIndexes.size();	// index in block
        m_mapVariableIndexes.insert(uintPair);
    }
}

string daeBlock::GetCanonicalName(void) const
{
    return m_strName;
}

string daeBlock::GetName(void) const
{
    return m_strName;
}

void daeBlock::SetName(const string& strName)
{
    m_strName = strName;
}

size_t daeBlock::GetNumberOfRoots() const
{
    size_t nNoRoots = 0;

    return (nNoRoots + m_mapExpressionInfos.size());
}

daeDataProxy_t* daeBlock::GetDataProxy(void) const
{
    return m_pDataProxy;
}

void daeBlock::SetDataProxy(daeDataProxy_t* pDataProxy)
{
    if(!pDataProxy)
        daeDeclareAndThrowException(exInvalidPointer);
    m_pDataProxy = pDataProxy;
}

map<size_t, size_t>& daeBlock::GetVariableIndexesMap()
{
    return m_mapVariableIndexes;
}

void daeBlock::AddEquationExecutionInfo(daeEquationExecutionInfo* pEquationExecutionInfo)
{
    if(!pEquationExecutionInfo)
        daeDeclareAndThrowException(exInvalidPointer);

    m_ptrarrEquationExecutionInfos.push_back(pEquationExecutionInfo);
}

void daeBlock::GetEquationExecutionInfos(vector<daeEquationExecutionInfo*>& ptrarrEquationExecutionInfos)
{
    daeSTN* pSTN;

    ptrarrEquationExecutionInfos = m_ptrarrEquationExecutionInfos;
    for(size_t i = 0; i < m_ptrarrSTNs.size(); i++)
    {
        pSTN = m_ptrarrSTNs[i];
        pSTN->CollectEquationExecutionInfos(ptrarrEquationExecutionInfos);
    }
}

size_t daeBlock::GetNumberOfEquations() const
{
    return m_nNumberOfEquations;
}

real_t daeBlock::GetValue(size_t nBlockIndex) const
{
#ifdef DAE_DEBUG
    if(!m_parrValues)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    return (*m_parrValues).GetItem(nBlockIndex);
}

void daeBlock::SetValue(size_t nBlockIndex, real_t dValue)
{
#ifdef DAE_DEBUG
    if(!m_parrValues)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    (*m_parrValues).SetItem(nBlockIndex, dValue);
}

real_t daeBlock::GetTimeDerivative(size_t nBlockIndex) const
{
#ifdef DAE_DEBUG
    if(!m_parrTimeDerivatives)
        daeDeclareAndThrowException(exInvalidPointer);
    if(nBlockIndex >= m_nNumberOfEquations)
        daeDeclareAndThrowException(exOutOfBounds);
#endif
    return (*m_parrTimeDerivatives).GetItem(nBlockIndex);
}

void daeBlock::SetTimeDerivative(size_t nBlockIndex, real_t dTimeDerivative)
{
#ifdef DAE_DEBUG
    if(!m_parrTimeDerivatives)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    (*m_parrTimeDerivatives).SetItem(nBlockIndex, dTimeDerivative);
}

real_t daeBlock::GetResidual(size_t nEquationIndex) const
{
#ifdef DAE_DEBUG
    if(!m_parrResidual)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    return (*m_parrResidual).GetItem(nEquationIndex);
}

void daeBlock::SetResidual(size_t nEquationIndex, real_t dResidual)
{
#ifdef DAE_DEBUG
    if(!m_parrResidual)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    (*m_parrResidual).SetItem(nEquationIndex, dResidual);
}

real_t daeBlock::GetJacobian(size_t nEquationIndex, size_t nVariableindexInBlock) const
{
#ifdef DAE_DEBUG
    if(!m_pmatJacobian)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    return m_pmatJacobian->GetItem(nEquationIndex, nVariableindexInBlock);
}

void daeBlock::SetJacobian(size_t nEquationIndex, size_t nVariableindexInBlock, real_t dJacobianItem)
{
#ifdef DAE_DEBUG
    if(!m_pmatJacobian)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    m_pmatJacobian->SetItem(nEquationIndex, nVariableindexInBlock, dJacobianItem);
}

void daeBlock::SetValuesArray(daeArray<real_t>* pValues)
{
#ifdef DAE_DEBUG
    if(!pValues)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    m_parrValues = pValues;
}

daeArray<real_t>* daeBlock::GetValuesAray(void) const
{
    return m_parrValues;
}

void daeBlock::SetTimeDerivativesArray(daeArray<real_t>* pTimeDerivatives)
{
#ifdef DAE_DEBUG
    if(!pTimeDerivatives)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    m_parrTimeDerivatives = pTimeDerivatives;
}

daeArray<real_t>* daeBlock::GetTimeDerivativesArray(void) const
{
    return m_parrTimeDerivatives;
}

daeMatrix<real_t>* daeBlock::GetJacobianMatrix() const
{
    return m_pmatJacobian;
}

void daeBlock::SetJacobianMatrix(daeMatrix<real_t>* pJacobian)
{
#ifdef DAE_DEBUG
    if(!pJacobian)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    m_pmatJacobian = pJacobian;
}

daeArray<real_t>* daeBlock::GetResidualArray() const
{
    return m_parrResidual;
}

void daeBlock::SetResidualArray(daeArray<real_t>* pResidual)
{
#ifdef DAE_DEBUG
    if(!pResidual)
        daeDeclareAndThrowException(exInvalidPointer);
#endif
    m_parrResidual = pResidual;
}

real_t daeBlock::GetTime() const
{
    return m_dCurrentTime;
}

void daeBlock::SetTime(real_t dTime)
{
    m_dCurrentTime = dTime;
}

real_t daeBlock::GetInverseTimeStep() const
{
    return m_dInverseTimeStep;
}

void daeBlock::SetInverseTimeStep(real_t dInverseTimeStep)
{
    m_dInverseTimeStep = dInverseTimeStep;
}

bool daeBlock::GetInitializeMode() const
{
    return m_bInitializeMode;
}

void daeBlock::SetInitializeMode(bool bMode)
{
    m_bInitializeMode = bMode;
}

bool daeBlock::CheckObject(vector<string>& strarrErrors) const
{
    dae_capacity_check(m_ptrarrEquationExecutionInfos);
    dae_capacity_check(m_ptrarrSTNs);

    return true;
}


}
}

