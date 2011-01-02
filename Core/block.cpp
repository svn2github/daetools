#include "stdafx.h"
#include "coreimpl.h"

namespace dae 
{
namespace core 
{

daeBlock::daeBlock(void)
{
	m_bInitializeMode					= false;
	m_pDataProxy						= NULL;
	m_parrResidual						= NULL; 
	m_pmatJacobian						= NULL; 
//	m_pmatSValues						= NULL;
//	m_pmatSTimeDerivatives				= NULL; 
//	m_pmatSResiduals					= NULL;
	m_dCurrentTime						= 0;
	m_dInverseTimeStep					= 0;
	m_nCurrentVariableIndexForJacobianEvaluation = ULONG_MAX;
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
								   vector<real_t>&		arrResults)
{
	size_t nFnCounter;
	map<size_t, daeExpressionInfo>::iterator iter;

	SetTime(dTime);
	CopyValuesFromSolver(arrValues);
	CopyTimeDerivativesFromSolver(arrTimeDerivatives);

	nFnCounter = 0;

	daeExecutionContext EC;
	EC.m_pBlock						= this;
	EC.m_pDataProxy					= m_pDataProxy;
	EC.m_eEquationCalculationMode	= eCalculate;

	for(iter = m_mapExpressionInfos.begin(); iter != m_mapExpressionInfos.end(); iter++)
	{
		if(!(*iter).second.m_pExpression)
		{	
			daeDeclareException(exInvalidPointer);
			throw e;
		}
		arrResults[nFnCounter] = (*iter).second.m_pExpression->Evaluate(&EC).getValue();
		nFnCounter++;
	}
}

void daeBlock::CalculateResiduals(real_t			dTime, 
								  daeArray<real_t>& arrValues, 
								  daeArray<real_t>& arrResiduals, 
								  daeArray<real_t>& arrTimeDerivatives)
{
	size_t i;
	daeSTN* pSTN;
	daeEquationExecutionInfo* pEquationExecutionInfo;

	SetTime(dTime);
	SetResidualArray(&arrResiduals);
	CopyValuesFromSolver(arrValues);
	CopyTimeDerivativesFromSolver(arrTimeDerivatives);	

// First calculate normal equations (non-STN)
	for(i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
	{
		pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
		if(!pEquationExecutionInfo)
			daeDeclareAndThrowException(exInvalidPointer);

		pEquationExecutionInfo->Residual();
	}

// Now calculate STN equations
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer);

		pSTN->CalculateResiduals();
	}
}

void daeBlock::CalculateJacobian(real_t				dTime, 
								 daeArray<real_t>&	arrValues, 
								 daeArray<real_t>&	arrResiduals, 
								 daeArray<real_t>&	arrTimeDerivatives, 
								 daeMatrix<real_t>&	matJacobian, 
								 real_t				dInverseTimeStep)
{
	size_t i;
	daeSTN* pSTN;
	daeEquationExecutionInfo* pEquationExecutionInfo;

	SetTime(dTime);
	SetResidualArray(&arrResiduals);
	SetJacobianMatrix(&matJacobian); 
	SetInverseTimeStep(dInverseTimeStep);
	CopyValuesFromSolver(arrValues);
	CopyTimeDerivativesFromSolver(arrTimeDerivatives);	

// First calculate normal equations (non-STN)
	for(i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
	{
		pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
		if(!pEquationExecutionInfo)
			daeDeclareAndThrowException(exInvalidPointer);
		
		pEquationExecutionInfo->Jacobian();
	}

// Now calculate STN equations
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer);

		pSTN->CalculateJacobian();
	}
}

// For dynamic models
void daeBlock::CalculateSensitivities(real_t					 dTime, 
									  const std::vector<size_t>& narrParameterIndexes,
									  daeArray<real_t>&			 arrValues, 
									  daeArray<real_t>&			 arrTimeDerivatives, 
									  daeMatrix<real_t>&		 matSValues, 
									  daeMatrix<real_t>&		 matSTimeDerivatives, 
									  daeMatrix<real_t>&		 matSResiduals)
{
	size_t i;
	daeSTN* pSTN;
	daeEquationExecutionInfo* pEquationExecutionInfo;

	SetTime(dTime);
	CopyValuesFromSolver(arrValues);
	CopyTimeDerivativesFromSolver(arrTimeDerivatives);
	
//	SetSValuesMatrix(&matSValues); 
//	SetSTimeDerivativesMatrix(&matSTimeDerivatives); 
//	SetSResidualsMatrix(&matSResiduals); 
	
	m_pDataProxy->SetSensitivityMatrixes(&matSValues,
										 &matSTimeDerivatives,
										 &matSResiduals);
	
	for(i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
	{
		pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
		if(!pEquationExecutionInfo)
			daeDeclareAndThrowException(exInvalidPointer);
		
		pEquationExecutionInfo->Sensitivities(narrParameterIndexes);
	}

// In general, neither objective function nor constraints can be within an STN
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer);

		pSTN->CalculateSensitivities(narrParameterIndexes);
	}
	
	m_pDataProxy->ResetSensitivityMatrixes();
}

// For steady-state models
void daeBlock::CalculateGradients(const std::vector<size_t>& narrParameterIndexes,
								  daeArray<real_t>&			 arrValues, 
								  daeMatrix<real_t>&		 matSResiduals)
{
	size_t i;
	daeSTN* pSTN;
	daeEquationExecutionInfo* pEquationExecutionInfo;

	CopyValuesFromSolver(arrValues);
	m_pDataProxy->SetSensitivityMatrixes(NULL, NULL, &matSResiduals);
	
	for(i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
	{
		pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
		if(!pEquationExecutionInfo)
			daeDeclareAndThrowException(exInvalidPointer);
		
		pEquationExecutionInfo->Gradients(narrParameterIndexes);
	}

// In general, neither objective function nor constraints can be within an STN
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer);

		pSTN->CalculateGradients(narrParameterIndexes);
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
void daeBlock::FillAbsoluteTolerancesArray(daeArray<real_t>& arrAbsoluteTolerances)
{
	size_t nBlockIndex, nOverallIndex;
	map<size_t, size_t>::iterator iter;

	if(GetNumberOfEquations() != m_mapVariableIndexes.size())
	{	
		daeDeclareException(exMiscellanous); 
		e << "Number of equations is not equal to number of variables";
		throw e;
	}
	if(!m_pDataProxy)
	{	
		daeDeclareException(exInvalidPointer); 
		e << "m_pDataProxy";
		throw e;
	}

	for(iter = m_mapVariableIndexes.begin(); iter != m_mapVariableIndexes.end(); iter++)
	{
		nOverallIndex = iter->first;
		nBlockIndex   = iter->second;
		arrAbsoluteTolerances.SetItem(nBlockIndex, *m_pDataProxy->GetAbsoluteTolerance(nOverallIndex));
	} 
}

void daeBlock::SetInitialConditionsAndInitialGuesses(daeArray<real_t>& arrValues, 
		                                             daeArray<real_t>& arrTimeDerivatives, 
													 daeArray<real_t>& arrInitialConditionsTypes)
{
	size_t nBlockIndex, nOverallIndex, nNumberOfEqnsVariables;
	map<size_t, size_t>::iterator iter;

	if(GetNumberOfEquations() != m_mapVariableIndexes.size())
	{	
		daeDeclareException(exInvalidCall); 
		e << "Number of equation is not equal to number of variables";
		throw e;
	}
	if(!m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer); 

	nNumberOfEqnsVariables = m_mapVariableIndexes.size();

	for(iter = m_mapVariableIndexes.begin(); iter != m_mapVariableIndexes.end(); iter++)
	{
		nOverallIndex = iter->first;
		nBlockIndex   = iter->second;

		arrValues.SetItem(nBlockIndex, *m_pDataProxy->GetValue(nOverallIndex));
		arrTimeDerivatives.SetItem(nBlockIndex, *m_pDataProxy->GetTimeDerivative(nOverallIndex));
		arrInitialConditionsTypes.SetItem(nBlockIndex, m_pDataProxy->GetVariableType(nOverallIndex));
	} 
}

size_t daeBlock::FindVariableBlockIndex(size_t nVariableOverallIndex) const
{
	map<size_t, size_t>::const_iterator iter = m_mapVariableIndexes.find(nVariableOverallIndex);
	if(iter != m_mapVariableIndexes.end()) // if found
		return iter->second;
	else
		return ULONG_MAX;
}

void daeBlock::CopyValuesFromSolver(daeArray<real_t>& arrValues)
{
	real_t dValue;
	size_t nBlockIndex, nOverallIndex, nNumberOfEqnsVariables;
	map<size_t, size_t>::iterator iter;

	if(GetNumberOfEquations() != m_mapVariableIndexes.size())
	{	
		daeDeclareException(exInvalidCall);
		e << "Number of equation is not equal to number of variables";
		throw e;
	}
	if(!m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer);

	nNumberOfEqnsVariables = m_mapVariableIndexes.size();

	for(iter = m_mapVariableIndexes.begin(); iter != m_mapVariableIndexes.end(); iter++)
	{
		nOverallIndex = iter->first;
		nBlockIndex   = iter->second;
		dValue = arrValues.GetItem(nBlockIndex);

		m_pDataProxy->SetValue(nOverallIndex, dValue);
	} 
}

void daeBlock::CopyValuesToSolver(daeArray<real_t>& arrValues)
{
	real_t dValue;
	size_t nBlockIndex, nOverallIndex, nNumberOfEqnsVariables;
	map<size_t, size_t>::iterator iter;

	if(GetNumberOfEquations() != m_mapVariableIndexes.size())
	{	
		daeDeclareException(exInvalidCall);
		e << "Number of equation is not equal to number of variables";
		throw e;
	}
	if(!m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer);

	nNumberOfEqnsVariables = m_mapVariableIndexes.size();

	for(iter = m_mapVariableIndexes.begin(); iter != m_mapVariableIndexes.end(); iter++)
	{
		nOverallIndex = iter->first;
		nBlockIndex   = iter->second;
		dValue = *m_pDataProxy->GetValue(nOverallIndex);
		arrValues.SetItem(nBlockIndex, dValue);
	} 
}

void daeBlock::CopyTimeDerivativesFromSolver(daeArray<real_t>& arrTimeDerivatives)
{
	real_t dValue;
	size_t nBlockIndex, nOverallIndex, nNumberOfEqnsVariables;
	map<size_t, size_t>::iterator iter;

	if(!m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer);
	if(GetNumberOfEquations() != m_mapVariableIndexes.size())
	{	
		daeDeclareException(exInvalidCall); 
		e << "Number of equation is not equal to number of variables";
		throw e;
	}

	nNumberOfEqnsVariables = m_mapVariableIndexes.size();

	for(iter = m_mapVariableIndexes.begin(); iter != m_mapVariableIndexes.end(); iter++)
	{
		nOverallIndex = iter->first;
		nBlockIndex   = iter->second;
		dValue = arrTimeDerivatives.GetItem(nBlockIndex);

		m_pDataProxy->SetTimeDerivative(nOverallIndex, dValue);
	} 
}

void daeBlock::CopyTimeDerivativesToSolver(daeArray<real_t>& arrTimeDerivatives)
{
	real_t dValue;
	size_t nBlockIndex, nOverallIndex, nNumberOfEqnsVariables;
	map<size_t, size_t>::iterator iter;

	if(!m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer);
	if(GetNumberOfEquations() != m_mapVariableIndexes.size())
	{	
		daeDeclareException(exInvalidCall); 
		e << "Number of equation is not equal to number of variables";
		throw e;
	}

	nNumberOfEqnsVariables = m_mapVariableIndexes.size();

	for(iter = m_mapVariableIndexes.begin(); iter != m_mapVariableIndexes.end(); iter++)
	{
		nOverallIndex = iter->first;
		nBlockIndex   = iter->second;
		dValue = *m_pDataProxy->GetTimeDerivative(nOverallIndex);
		arrTimeDerivatives.SetItem(nBlockIndex, dValue);
	} 
}

void daeBlock::Initialize(void)
{
	size_t i;
	pair<size_t, size_t> uintPair;
	map<size_t, size_t>::iterator iter;
	daeSTN* pSTN;

	if(!m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer);

	if(GetNumberOfEquations() != m_mapVariableIndexes.size())
	{	
		daeDeclareException(exInvalidCall);
		e << "Number of equations [" << GetNumberOfEquations() << "] is not equal to number of variables [" << m_mapVariableIndexes.size() << "]";
		throw e;
	}

	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer);

		pSTN->BuildExpressions(this);
	}

	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer);

		pSTN->CheckDiscontinuities();
	}
	RebuildExpressionMap();
}

daeeDiscontinuityType daeBlock::CheckDiscontinuities()
{
	size_t i;
	daeSTN* pSTN;
	daeeDiscontinuityType eResult;

	if(m_dCurrentTime > 0)
		m_pDataProxy->LogMessage(string("Checking state transitions at time [") + toStringFormatted<real_t>(m_dCurrentTime, -1, 15) + string("]..."), 0);

	eResult = eNoDiscontinuity;

// First check the global stopping condition from the DataProxy (Simulation)
	daeModel* model = dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel());
	if(!model)
	   daeDeclareAndThrowException(exInvalidPointer);
	daeCondition* pCondition = model->GetGlobalCondition();
	if(pCondition)
	{
		daeExecutionContext EC;
		EC.m_pDataProxy					= m_pDataProxy;
		EC.m_eEquationCalculationMode	= eCalculate;
	
		if(pCondition->Evaluate(&EC))
		{
			m_pDataProxy->LogMessage(string("The global condition: ") + pCondition->SaveNodeAsPlainText() + string(" is satisfied"), 0);
			return eGlobalDiscontinuity;
		}
	}

// Then check conditions from STNs
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer); 

		if(pSTN->CheckDiscontinuities())
			eResult = eModelDiscontinuity;
	}
	
	if(eResult == eModelDiscontinuity)
		RebuildExpressionMap();

	return eResult;
}

void daeBlock::RebuildExpressionMap()
{
	size_t i;
	daeSTN* pSTN;

	m_mapExpressionInfos.clear();

// First add the global stopping condition from daeDataProxy
	daeModel* model = dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel());
	if(!model)
	   daeDeclareAndThrowException(exInvalidPointer);
	daeCondition* pCondition = model->GetGlobalCondition();
	if(pCondition)
	{
		daeExpressionInfo ei;
		pair<size_t, daeExpressionInfo> pairExprInfo;
		map<size_t, daeExpressionInfo>::iterator iter;

		for(size_t i = 0; i < pCondition->m_ptrarrExpressions.size(); i++)
		{
			ei.m_pExpression      = pCondition->m_ptrarrExpressions[i];
			ei.m_pStateTransition = NULL;
			
			pairExprInfo.first	= m_mapExpressionInfos.size();				
			pairExprInfo.second	= ei;				
			m_mapExpressionInfos.insert(pairExprInfo);
		}
	}
	
// Then for all othe STNs
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer); 

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
	
// First check the global stopping condition
	daeModel* model = dynamic_cast<daeModel*>(m_pDataProxy->GetTopLevelModel());
	if(!model)
	   daeDeclareAndThrowException(exInvalidPointer);
	daeCondition* pCondition = model->GetGlobalCondition();
	if(pCondition)
		nNoRoots = 1;

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

void daeBlock::GetEquationExecutionInfo(vector<daeEquationExecutionInfo*>& ptrarrEquationExecutionInfos)
{
	ptrarrEquationExecutionInfos = m_ptrarrEquationExecutionInfos;
}

size_t daeBlock::GetNumberOfEquations() const
{
	size_t i, nNoEqns;
	daeSTN* pSTN;

	nNoEqns = m_ptrarrEquationExecutionInfos.size();
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer);

		nNoEqns += pSTN->GetNumberOfEquations();
	}

	return nNoEqns;
}

real_t daeBlock::GetADValue(size_t nIndexInBlock) const
{
	pair<size_t, size_t> uintPair;
	map<size_t, size_t>::const_iterator iter;

	iter = m_mapVariableIndexes.find(nIndexInBlock);
	if(iter == m_mapVariableIndexes.end()) 
	{	
		daeDeclareException(exInvalidCall);
		e << "Cannot find variable index in block";
		throw e;
	}

	if(m_bInitializeMode)
		return 0;

	if(m_nCurrentVariableIndexForJacobianEvaluation == nIndexInBlock)
		return 1;
	else
		return 0;
}

real_t daeBlock::GetValue(size_t nIndexInBlock) const
{
	pair<size_t, size_t> uintPair;
	map<size_t, size_t>::const_iterator iter;

	iter = m_mapVariableIndexes.find(nIndexInBlock);
	if(iter == m_mapVariableIndexes.end()) 
	{	
		daeDeclareException(exInvalidCall);
		e << "Cannot find variable index in block";
		throw e;
	}

	if(m_bInitializeMode)
		return 0;

	if(!m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer);

	return *m_pDataProxy->GetValue(iter->first);
}

real_t daeBlock::GetTimeDerivative(size_t nIndexInBlock) const
{
	pair<size_t, size_t> uintPair;
	map<size_t, size_t>::const_iterator iter;

	iter = m_mapVariableIndexes.find(nIndexInBlock);
	if(iter == m_mapVariableIndexes.end()) 
	{	
		daeDeclareException(exInvalidCall);
		e << "Cannot find variable index in block";
		throw e;
	}

	if(m_bInitializeMode)
		return 0;

	if(!m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer);

	return *m_pDataProxy->GetTimeDerivative(iter->first);
}

real_t daeBlock::GetResidual(size_t nIndex) const
{
	if(!m_parrResidual)
		daeDeclareAndThrowException(exInvalidPointer);
	return m_parrResidual->GetItem(nIndex);
}

void daeBlock::SetResidual(size_t nIndex, real_t dResidual)
{
	if(!m_parrResidual)
		daeDeclareAndThrowException(exInvalidPointer);
	m_parrResidual->SetItem(nIndex, dResidual);
}

real_t daeBlock::GetJacobian(size_t nEquationIndex, size_t nVariableindexInBlock) const
{
	if(!m_pmatJacobian)
		daeDeclareAndThrowException(exInvalidPointer); 
	return m_pmatJacobian->GetItem(nEquationIndex, nVariableindexInBlock);
}

void daeBlock::SetJacobian(size_t nEquationIndex, size_t nVariableindexInBlock, real_t dJacobianItem)
{
	if(!m_pmatJacobian)
		daeDeclareAndThrowException(exInvalidPointer); 
	m_pmatJacobian->SetItem(nEquationIndex, nVariableindexInBlock, dJacobianItem);
}

daeMatrix<real_t>* daeBlock::GetJacobianMatrix() const
{
	return m_pmatJacobian;
}

void daeBlock::SetJacobianMatrix(daeMatrix<real_t>* pJacobian)
{
	if(!pJacobian)
		daeDeclareAndThrowException(exInvalidPointer);
	m_pmatJacobian = pJacobian;
}

daeArray<real_t>* daeBlock::GetResidualArray() const
{
	return m_parrResidual;
}

void daeBlock::SetResidualArray(daeArray<real_t>* pResidual)
{
	if(!pResidual)
		daeDeclareAndThrowException(exInvalidPointer);
	m_parrResidual = pResidual;
}

//void daeBlock::SetSValuesMatrix(daeMatrix<real_t>* pSValues)
//{
//	if(!pSValues)
//		daeDeclareAndThrowException(exInvalidPointer);

//	m_pmatSValues = pSValues;
//}

//void daeBlock::SetSTimeDerivativesMatrix(daeMatrix<real_t>* pSTimeDerivatives)
//{
//	if(!pSTimeDerivatives)
//		daeDeclareAndThrowException(exInvalidPointer);

//	m_pmatSTimeDerivatives = pSTimeDerivatives; 
//}

//void daeBlock::SetSResidualsMatrix(daeMatrix<real_t>* pSResiduals)
//{
//	if(!pSResiduals)
//		daeDeclareAndThrowException(exInvalidPointer);

//	m_pmatSResiduals = pSResiduals;
//}

//daeMatrix<real_t>* daeBlock::GetSValuesMatrix(void) const
//{
//	return m_pmatSValues;
//}

//daeMatrix<real_t>* daeBlock::GetSTimeDerivativesMatrix(void) const
//{
//	return m_pmatSTimeDerivatives;
//}

//daeMatrix<real_t>* daeBlock::GetSResidualsMatrix(void) const
//{
//	return m_pmatSResiduals;
//}

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
	return true;
}


}
}

