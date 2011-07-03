#include "stdafx.h"
#include "coreimpl.h"
#include <algorithm>

namespace dae 
{
namespace core 
{
/******************************************************************
	daeFunctionWithGradients
*******************************************************************/
daeFunctionWithGradients::daeFunctionWithGradients(void)
{
	m_pModel                         = NULL;
	m_nEquationIndexInBlock          = ULONG_MAX;
	m_nVariableIndexInBlock          = ULONG_MAX;
	m_pEquationExecutionInfo         = NULL;
	m_pSimulation					 = NULL;
	m_nNumberOfOptimizationVariables = 0;
}

daeFunctionWithGradients::daeFunctionWithGradients(daeSimulation_t* pSimulation, 
												   real_t abstol, 
												   const string& strVariableName, 
												   const string& strEquationName,
												   const string& strDescription)
{
	if(!pSimulation)
		daeDeclareAndThrowException(exInvalidPointer);

	m_pModel = dynamic_cast<daeModel*>(pSimulation->GetModel());
	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer);
		
	const daeVariableType varType("gradient_function_t", "-", -1.0e+100, 1.0e+100, 0.0, abstol);

	m_nEquationIndexInBlock = ULONG_MAX;
	m_nVariableIndexInBlock = ULONG_MAX;
	
	m_pVariable	= boost::shared_ptr<daeVariable>(new daeVariable(strVariableName, varType, m_pModel, strDescription));
	m_pVariable->SetReportingOn(true);
	
	m_pEquation = m_pModel->CreateEquation(strEquationName, strDescription);
	m_pEquation->SetResidual( (*m_pVariable)() );

	m_pSimulation                    = pSimulation;
	m_pEquationExecutionInfo         = NULL;
	m_nNumberOfOptimizationVariables = 0;
	
}

daeFunctionWithGradients::~daeFunctionWithGradients(void)
{
}

void daeFunctionWithGradients::SetResidual(adouble res)
{
	if(!m_pEquation)
		daeDeclareAndThrowException(exInvalidPointer)
		
	m_pEquation->SetResidual( res - (*m_pVariable )() );
}

adouble daeFunctionWithGradients::GetResidual(void) const
{
	if(!m_pEquation)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return m_pEquation->GetResidual();
}

std::string daeFunctionWithGradients::GetName(void) const
{
	if(!m_pVariable)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return m_pVariable->GetName();
}

real_t daeFunctionWithGradients::GetValue(void) const
{
	if(!m_pVariable)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return m_pVariable->GetValue();
}
/*
 This function DOES NOT set gradients for ALL optimization variables but only for those that the obj.function depends on!!! 
 Anyway, the array 'gradients' is Nparams long, and the indexes in m_narrOptimizationVariablesIndexes are in the range (0, Nparams-1)
 matSensitivities is a (Nparams) x (Nvariables) matrix

 Here I need values for the column 'k' (marked with x below):
	0     | . . . . . . . . x . . |  -> Sensitivity of the variable with the index k on parameter 0 
	1     | . . . . . . . . x . . |  -> Sensitivity of the variable with the index k on parameter 1 
	2     | . . . . . . . . x . . |
	3     | . . . . . . . . x . . |
	4     | . . . . . . . . x . . |
	...   | ...                   |
	Np-1  | . . . . . . . . x . . |  -> Sensitivity of the variable with the index k on parameter Np-1 

 a) Steady-state model
 Index k is equal to m_nEquationIndexInBlock for the steady-state model since then we have the following equation:
	Fobj(p1,p2,...,pm) - Vobj = 0  
 and in that case the sensitivity of the variable Vobj is:
	Si(Vobj) = ∂Vobj/∂pi = ∂Fobj/∂pi since from the equation above we have: Vobj = Fobj(p1,p2,...,pm)
 therefore I can obtain the sensitivity values as:
	S(Vobj) = [matSens(0, k), matSens(1, k), ..., matSens(m-1, k)]
	where k = m_nEquationIndexInBlock

 b) Dynamic model
 If we have a dynamic model, then the sensitivity S(Vobj) has to satisfy the following equation:
	S(Vobj) * ∂(Fobj)/∂(Vobj) + dS(Vobj)/dt * ∂(Fobj)/∂(dVobj/dt) + ∂(Fobj)/∂pi = 0 
 and in that case the sensitivity of the variable Vobj is:
	S(Vobj) = [matSens(0, k), matSens(1, k), ..., matSens(m-1, k)]
	where k = index of m_pVariable in the block
	
 Exactly the same logic applies on constraints!!!
*/
void daeFunctionWithGradients::GetGradients(const daeMatrix<real_t>& matSensitivities, real_t* gradients, size_t Nparams) const
{
	size_t varIndex, paramIndex;

	if(!m_pVariable)
		daeDeclareAndThrowException(exInvalidPointer);
	if(m_narrOptimizationVariablesIndexes.size() > Nparams)
		daeDeclareAndThrowException(exInvalidCall);
	if(m_nNumberOfOptimizationVariables != Nparams)
		daeDeclareAndThrowException(exInvalidCall);

	if(m_pModel->IsModelDynamic())
		varIndex = m_nVariableIndexInBlock;
	else
		varIndex = m_nEquationIndexInBlock;
	
	::memset(gradients, 0, Nparams * sizeof(real_t));

	for(size_t j = 0; j < m_narrOptimizationVariablesIndexes.size(); j++)
	{
		paramIndex = m_narrOptimizationVariablesIndexes[j];
		gradients[paramIndex] = matSensitivities.GetItem(paramIndex, // Index of the parameter
														 varIndex);  // Index of the variable
	}
}

void daeFunctionWithGradients::GetGradients(real_t* gradients, size_t Nparams) const
{
	daeMatrix<real_t>& matSensitivities = GetSensitivitiesMatrix();
	GetGradients(matSensitivities, gradients, Nparams);
}

daeMatrix<real_t>& daeFunctionWithGradients::GetSensitivitiesMatrix(void) const
{
	if(!m_pVariable)
		daeDeclareAndThrowException(exInvalidPointer);
	if(!m_pSimulation)
		daeDeclareAndThrowException(exInvalidPointer);
	
	daeDAESolver_t* pDAESolver = m_pSimulation->GetDAESolver();
	if(!pDAESolver)
		daeDeclareAndThrowException(exInvalidPointer);

	daeMatrix<real_t>& matSens = pDAESolver->GetSensitivities();
	if(m_nNumberOfOptimizationVariables != matSens.GetNrows())
		daeDeclareAndThrowException(exInvalidCall);

	return matSens;
}

bool daeFunctionWithGradients::IsLinear(void) const
{
	if(!m_pEquationExecutionInfo)
		daeDeclareAndThrowException(exInvalidPointer)
	boost::shared_ptr<adNode> node = m_pEquationExecutionInfo->GetEquationEvaluationNode();
	if(!node)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return node->IsLinear();
}

void daeFunctionWithGradients::GetOptimizationVariableIndexes(std::vector<size_t>& narrOptimizationVariablesIndexes) const
{
	narrOptimizationVariablesIndexes = m_narrOptimizationVariablesIndexes;
}

void daeFunctionWithGradients::Initialize(const std::vector< boost::shared_ptr<daeOptimizationVariable> >& arrOptimizationVariables, daeBlock_t* pBlock)
{
	size_t i;
	boost::shared_ptr<daeOptimizationVariable> pOptVariable;
	vector<daeEquationExecutionInfo*> ptrarrEquationExecutionInfos;

	if(!m_pEquation)
		daeDeclareAndThrowException(exInvalidPointer)
	
	m_pEquation->GetEquationExecutionInfos(ptrarrEquationExecutionInfos);
	if(ptrarrEquationExecutionInfos.size() != 1)
		daeDeclareAndThrowException(exInvalidCall)
		
	m_pEquationExecutionInfo = ptrarrEquationExecutionInfos[0];
	if(!m_pEquationExecutionInfo)
		daeDeclareAndThrowException(exInvalidPointer)
		
// 1. Set the variable/equation index in the block it belongs
	m_nEquationIndexInBlock = m_pEquationExecutionInfo->GetEquationIndexInBlock();
	m_nVariableIndexInBlock = pBlock->FindVariableBlockIndex(m_pVariable->m_nOverallIndex);
	if(m_nEquationIndexInBlock == ULONG_MAX || m_nVariableIndexInBlock == ULONG_MAX)
		daeDeclareAndThrowException(exInvalidCall);

// 2. Add all optimization variables indexes to list of indexes
	m_nNumberOfOptimizationVariables = arrOptimizationVariables.size();

	m_narrOptimizationVariablesIndexes.clear();
	for(i = 0; i < arrOptimizationVariables.size(); i++)
	{
		pOptVariable = arrOptimizationVariables[i];
		m_narrOptimizationVariablesIndexes.push_back(pOptVariable->GetOptimizationVariableIndex());
	}
	std::sort(m_narrOptimizationVariablesIndexes.begin(), m_narrOptimizationVariablesIndexes.end());
}

void daeFunctionWithGradients::RemoveEquationFromModel(void)
{
	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer);
	
	m_pModel->RemoveEquation(m_pEquation);
}

bool daeFunctionWithGradients::CheckObject(vector<string>& strarrErrors) const
{
	string strError;

	bool bCheck = true;

// Do not check daeObject since this is not an usual object
//	if(!daeObject::CheckObject(strarrErrors))
//		bCheck = false;

	if(!m_pVariable)
	{	
		strError = "Invalid variable";
		strarrErrors.push_back(strError);
		return false;
	}
	if(!m_pEquation)
	{	
		strError = "Invalid equation";
		strarrErrors.push_back(strError);
		return false;
	}
	if(m_pEquation->GetEquationDefinitionMode() == eEDMUnknown ||
	   m_pEquation->GetEquationEvaluationMode() == eEEMUnknown)
	{	
		strError = "Equation residual not specified [" + m_pEquation->GetCanonicalName() + "]";
		strarrErrors.push_back(strError);
		return false;
	}
	
	return bCheck;
}

void daeFunctionWithGradients::Export(std::string& strContent, daeeModelLanguage eLanguage, daeModelExportContext& c) const
{
	
}

size_t daeFunctionWithGradients::GetNumberOfOptimizationVariables(void) const
{
	return m_nNumberOfOptimizationVariables;
}

/******************************************************************
	daeObjectiveFunction
*******************************************************************/
daeObjectiveFunction::daeObjectiveFunction(void)
{
}

daeObjectiveFunction::daeObjectiveFunction(daeSimulation_t* pSimulation, 
										   real_t abstol, 
										   size_t nEquationIndex,
										   const string& strDescription)
	  :	daeFunctionWithGradients(pSimulation, 
								 abstol, 
								 string("V_obj") + toString<size_t>(nEquationIndex + 1), 
								 string("F_obj") + toString<size_t>(nEquationIndex + 1),
								 strDescription)

{
}

daeObjectiveFunction::~daeObjectiveFunction(void)
{
}

/*
daeObjectiveFunction::daeObjectiveFunction(void)
{
	m_pModel                         = NULL;
	m_nEquationIndexInBlock          = ULONG_MAX;
	m_nVariableIndexInBlock          = ULONG_MAX;
	m_pEquationExecutionInfo         = NULL;
	m_pSimulation					 = NULL;
	m_nNumberOfOptimizationVariables = 0;
}

daeObjectiveFunction::daeObjectiveFunction(daeSimulation_t* pSimulation, real_t abstol, size_t N)
{
	m_pModel = dynamic_cast<daeModel*>(pSimulation->GetModel());
	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer);
		
	string strVName = string("V_obj") + toString<size_t>(N + 1); 
	string strFName = string("F_obj") + toString<size_t>(N + 1); 

	const daeVariableType typeObjectiveFunction("typeObjectiveFunction", "-", -1.0e+100, 1.0e+100, 0.0, abstol);
	m_nEquationIndexInBlock = ULONG_MAX;
	m_nVariableIndexInBlock = ULONG_MAX;
	m_pObjectiveVariable	= boost::shared_ptr<daeVariable>(new daeVariable(strVName, typeObjectiveFunction, m_pModel, "Objective value" + strVName));
	m_pObjectiveVariable->SetReportingOn(true);
	m_pObjectiveFunction = m_pModel->CreateEquation(strFName, "Objective function " + strFName);
	m_pEquationExecutionInfo = NULL;
	m_pSimulation = pSimulation;
	m_nNumberOfOptimizationVariables = 0;
	
	m_pObjectiveFunction->SetResidual( (*m_pObjectiveVariable)() );
}

daeObjectiveFunction::~daeObjectiveFunction(void)
{
}

void daeObjectiveFunction::SetResidual(adouble res)
{
	if(!m_pObjectiveFunction)
		daeDeclareAndThrowException(exInvalidPointer)
		
	m_pObjectiveFunction->SetResidual( res - (*m_pObjectiveVariable )() );
}

adouble daeObjectiveFunction::GetResidual(void) const
{
	if(!m_pObjectiveFunction)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return m_pObjectiveFunction->GetResidual();
}

std::string daeObjectiveFunction::GetName(void) const
{
	if(!m_pObjectiveVariable)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return m_pObjectiveVariable->GetName();
}

real_t daeObjectiveFunction::GetValue(void) const
{
	if(!m_pObjectiveVariable)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return m_pObjectiveVariable->GetValue();
}

// This function DOES NOT set gradients for ALL optimization variables but only for those that the obj.function depends on!!! 
// Anyway, the array 'gradients' is Nparams long, and the indexes in m_narrOptimizationVariablesIndexes are in the range (0, Nparams-1)
// matSensitivities is a (Nparams) x (Nvariables) matrix
//
// Here I need values for the column 'k' (marked with x below):
//	0     | . . . . . . . . x . . |  -> Sensitivity of the variable with the index k on parameter 0 
//	1     | . . . . . . . . x . . |  -> Sensitivity of the variable with the index k on parameter 1 
//	2     | . . . . . . . . x . . |
//	3     | . . . . . . . . x . . |
//	4     | . . . . . . . . x . . |
//	...   | ...                   |
//	Np-1  | . . . . . . . . x . . |  -> Sensitivity of the variable with the index k on parameter Np-1 
//
// a) Steady-state model
// Index k is equal to m_nEquationIndexInBlock for the steady-state model since then we have the following equation:
//	Fobj(p1,p2,...,pm) - Vobj = 0  
// and in that case the sensitivity of the variable Vobj is:
//	Si(Vobj) = ∂Vobj/∂pi = ∂Fobj/∂pi since from the equation above we have: Vobj = Fobj(p1,p2,...,pm)
// therefore I can obtain the sensitivity values as:
//	S(Vobj) = [matSens(0, k), matSens(1, k), ..., matSens(m-1, k)]
//	where k = m_nEquationIndexInBlock
//
// b) Dynamic model
// If we have a dynamic model, then the sensitivity S(Vobj) has to satisfy the following equation:
//	S(Vobj) * ∂(Fobj)/∂(Vobj) + dS(Vobj)/dt * ∂(Fobj)/∂(dVobj/dt) + ∂(Fobj)/∂pi = 0 
// and in that case the sensitivity of the variable Vobj is:
//	S(Vobj) = [matSens(0, k), matSens(1, k), ..., matSens(m-1, k)]
//	where k = index of m_pObjectiveVariable in the block
//	
// Exactly the same logic applies on constraints!!!
void daeObjectiveFunction::GetGradients(const daeMatrix<real_t>& matSensitivities, real_t* gradients, size_t Nparams) const
{
	size_t varIndex, paramIndex;

	if(!m_pObjectiveVariable)
		daeDeclareAndThrowException(exInvalidPointer);
	if(m_narrOptimizationVariablesIndexes.size() > Nparams)
		daeDeclareAndThrowException(exInvalidCall);
	if(m_nNumberOfOptimizationVariables != Nparams)
		daeDeclareAndThrowException(exInvalidCall);

	if(m_pModel->IsModelDynamic())
		varIndex = m_nVariableIndexInBlock;
	else
		varIndex = m_nEquationIndexInBlock;
	
	::memset(gradients, 0, Nparams * sizeof(real_t));

	for(size_t j = 0; j < m_narrOptimizationVariablesIndexes.size(); j++)
	{
		paramIndex = m_narrOptimizationVariablesIndexes[j];
		gradients[paramIndex] = matSensitivities.GetItem(paramIndex, // Index of the parameter
									                     varIndex);  // Index of the variable
	}
}

void daeObjectiveFunction::GetGradients(real_t* gradients, size_t Nparams) const
{
	daeMatrix<real_t>& matSensitivities = GetSensitivitiesMatrix();
	GetGradients(matSensitivities, gradients, Nparams);
}

daeMatrix<real_t>& daeObjectiveFunction::GetSensitivitiesMatrix(void) const
{
	if(!m_pObjectiveVariable)
		daeDeclareAndThrowException(exInvalidPointer);
	if(!m_pSimulation)
		daeDeclareAndThrowException(exInvalidPointer);
	
	daeDAESolver_t* pDAESolver = m_pSimulation->GetDAESolver();
	if(!pDAESolver)
		daeDeclareAndThrowException(exInvalidPointer);

	daeMatrix<real_t>& matSens = pDAESolver->GetSensitivities();
	if(m_nNumberOfOptimizationVariables != matSens.GetNrows())
		daeDeclareAndThrowException(exInvalidCall);

	return matSens;
}

bool daeObjectiveFunction::IsLinear(void) const
{
	if(!m_pEquationExecutionInfo)
		daeDeclareAndThrowException(exInvalidPointer)
	boost::shared_ptr<adNode> node = m_pEquationExecutionInfo->GetEquationEvaluationNode();
	if(!node)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return node->IsLinear();
}

void daeObjectiveFunction::GetOptimizationVariableIndexes(std::vector<size_t>& narrOptimizationVariablesIndexes) const
{
	narrOptimizationVariablesIndexes = m_narrOptimizationVariablesIndexes;
}

void daeObjectiveFunction::Initialize(const std::vector< boost::shared_ptr<daeOptimizationVariable> >& arrOptimizationVariables, daeBlock_t* pBlock)
{
	size_t i;
	boost::shared_ptr<daeOptimizationVariable> pOptVariable;
	vector<daeEquationExecutionInfo*> ptrarrEquationExecutionInfos;

	if(!m_pObjectiveFunction)
		daeDeclareAndThrowException(exInvalidPointer)
	
	m_pObjectiveFunction->GetEquationExecutionInfos(ptrarrEquationExecutionInfos);
	if(ptrarrEquationExecutionInfos.size() != 1)
		daeDeclareAndThrowException(exInvalidCall)
		
	m_pEquationExecutionInfo = ptrarrEquationExecutionInfos[0];
	if(!m_pEquationExecutionInfo)
		daeDeclareAndThrowException(exInvalidPointer)
		
// 1. Set the variable/equation index in the block it belongs
	m_nEquationIndexInBlock = m_pEquationExecutionInfo->GetEquationIndexInBlock();
	m_nVariableIndexInBlock = pBlock->FindVariableBlockIndex(m_pObjectiveVariable->m_nOverallIndex);
	if(m_nEquationIndexInBlock == ULONG_MAX || m_nVariableIndexInBlock == ULONG_MAX)
		daeDeclareAndThrowException(exInvalidCall);

// 2. Add all optimization variables indexes to list of indexes
	m_nNumberOfOptimizationVariables = arrOptimizationVariables.size();

	m_narrOptimizationVariablesIndexes.clear();
	for(i = 0; i < arrOptimizationVariables.size(); i++)
	{
		pOptVariable = arrOptimizationVariables[i];
		m_narrOptimizationVariablesIndexes.push_back(pOptVariable->GetOptimizationVariableIndex());
	}
	std::sort(m_narrOptimizationVariablesIndexes.begin(), m_narrOptimizationVariablesIndexes.end());
}

void daeObjectiveFunction::RemoveEquationFromModel(void)
{
	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer);
	
	m_pModel->RemoveEquation(m_pObjectiveFunction);
}

bool daeObjectiveFunction::CheckObject(vector<string>& strarrErrors) const
{
	string strError;

	bool bCheck = true;

// Do not check daeObject since this is not an usual object
//	if(!daeObject::CheckObject(strarrErrors))
//		bCheck = false;

	if(!m_pObjectiveVariable)
	{	
		strError = "Invalid objective variable";
		strarrErrors.push_back(strError);
		return false;
	}
	if(!m_pObjectiveFunction)
	{	
		strError = "Invalid objective function";
		strarrErrors.push_back(strError);
		return false;
	}
	if(m_pObjectiveFunction->GetEquationDefinitionMode() == eEDMUnknown ||
	   m_pObjectiveFunction->GetEquationEvaluationMode() == eEEMUnknown)
	{	
		strError = "Objective function residual not specified [" + m_pObjectiveFunction->GetCanonicalName() + "]";
		strarrErrors.push_back(strError);
		return false;
	}
	
	return bCheck;
}

void daeObjectiveFunction::Export(std::string& strContent, daeeModelLanguage eLanguage, daeModelExportContext& c) const
{
	
}

size_t daeObjectiveFunction::GetNumberOfOptimizationVariables(void) const
{
	return m_nNumberOfOptimizationVariables;
}
*/

/******************************************************************
	daeOptimizationConstraint
*******************************************************************/
daeOptimizationConstraint::daeOptimizationConstraint(void)
{
}

daeOptimizationConstraint::daeOptimizationConstraint(daeSimulation_t* pSimulation, 
													 bool bIsInequalityConstraint, 
													 real_t abstol, 
													 size_t nEquationIndex,
													 const string& strDescription)
	: daeFunctionWithGradients(pSimulation, 
							   abstol, 
							   string("V_constraint") + toString<size_t>(nEquationIndex + 1), 
							   string("F_constraint") + toString<size_t>(nEquationIndex + 1),
							   strDescription)
{
	m_eConstraintType = (bIsInequalityConstraint ? eInequalityConstraint : eEqualityConstraint);
}

daeOptimizationConstraint::~daeOptimizationConstraint(void)
{
}

void daeOptimizationConstraint::SetType(daeeConstraintType value)
{
	m_eConstraintType = value;	
}

daeeConstraintType daeOptimizationConstraint::GetType(void) const
{
	return m_eConstraintType;	
}

/*
daeOptimizationConstraint::daeOptimizationConstraint(void)
{
	m_pModel                         = NULL;
	m_nEquationIndexInBlock          = ULONG_MAX;
	m_nVariableIndexInBlock          = ULONG_MAX;
	m_pEquationExecutionInfo         = NULL;
	m_pSimulation					 = NULL;
	m_nNumberOfOptimizationVariables = 0;
}

daeOptimizationConstraint::daeOptimizationConstraint(daeSimulation_t* pSimulation, bool bIsInequalityConstraint, real_t abstol, size_t N, string strDescription)
{
	m_pModel = dynamic_cast<daeModel*>(pSimulation->GetModel());
	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer);
		
	const daeVariableType typeConstraint("typeConstraint", "-", -1.0e+100, 1.0e+100, 0.0, abstol);
	string strVName = string("V_constraint") + toString<size_t>(N + 1); 
	string strFName = string("F_constraint") + toString<size_t>(N + 1); 

	m_eConstraintType		= (bIsInequalityConstraint ? eInequalityConstraint : eEqualityConstraint);
	m_nEquationIndexInBlock = ULONG_MAX;
	m_nVariableIndexInBlock = ULONG_MAX;
	m_pConstraintVariable	= boost::shared_ptr<daeVariable>(new daeVariable(strVName, typeConstraint, m_pModel, strDescription));
	m_pConstraintVariable->SetReportingOn(true);
	m_pConstraintFunction	         = m_pModel->CreateEquation(strFName, strDescription);
	m_pEquationExecutionInfo		 = NULL;
	m_pSimulation					 = pSimulation;
	m_nNumberOfOptimizationVariables = 0;
	
	m_pConstraintFunction->SetResidual( (*m_pConstraintVariable)() );
}

daeOptimizationConstraint::~daeOptimizationConstraint(void)
{
}

void daeOptimizationConstraint::SetResidual(adouble res)
{
	if(!m_pConstraintFunction)
		daeDeclareAndThrowException(exInvalidPointer)
		
	m_pConstraintFunction->SetResidual( res - (*m_pConstraintVariable )() );
}

adouble daeOptimizationConstraint::GetResidual(void) const
{
	if(!m_pConstraintFunction)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return m_pConstraintFunction->GetResidual();
}

std::string daeOptimizationConstraint::GetName(void) const
{
	if(!m_pConstraintVariable)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return m_pConstraintVariable->GetName();
}

real_t daeOptimizationConstraint::GetValue(void) const
{
	if(!m_pConstraintVariable)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return m_pConstraintVariable->GetValue();
}

// This function DOES NOT set gradients for ALL optimization variables but only for those that the constraint depends on!!! 
// Anyway, the array 'gradients' is Nparams long, and the indexes in m_narrOptimizationVariablesIndexes are in the range (0, Nparams-1)
// matSensitivities is a (Nparams) x (Nvariables) matrix ... see the discussion above (daeOptimizationFunction::GetGradients)
void daeOptimizationConstraint::GetGradients(const daeMatrix<real_t>& matSensitivities, real_t* gradients, size_t Nparams) const
{
	size_t varIndex, paramIndex;

	if(!m_pConstraintVariable)
		daeDeclareAndThrowException(exInvalidPointer);
	if(m_narrOptimizationVariablesIndexes.size() > Nparams)
		daeDeclareAndThrowException(exInvalidCall);
	if(m_nNumberOfOptimizationVariables != Nparams)
		daeDeclareAndThrowException(exInvalidCall);

	if(m_pModel->IsModelDynamic())
		varIndex = m_nVariableIndexInBlock;
	else
		varIndex = m_nEquationIndexInBlock;

	::memset(gradients, 0, Nparams * sizeof(real_t));

	for(size_t j = 0; j < m_narrOptimizationVariablesIndexes.size(); j++)
	{
		paramIndex = m_narrOptimizationVariablesIndexes[j];
		gradients[paramIndex] = matSensitivities.GetItem(paramIndex, // Index of the parameter
							                             varIndex);  // Index of the variable
	}
}

void daeOptimizationConstraint::GetGradients(real_t* gradients, size_t Nparams) const
{
	daeMatrix<real_t>& matSensitivities = GetSensitivitiesMatrix();
	GetGradients(matSensitivities, gradients, Nparams);
}

daeMatrix<real_t>& daeOptimizationConstraint::GetSensitivitiesMatrix(void) const
{
	if(!m_pConstraintVariable)
		daeDeclareAndThrowException(exInvalidPointer);
	if(!m_pSimulation)
		daeDeclareAndThrowException(exInvalidPointer);
	
	daeDAESolver_t* pDAESolver = m_pSimulation->GetDAESolver();
	if(!pDAESolver)
		daeDeclareAndThrowException(exInvalidPointer);
	
	daeMatrix<real_t>& matSens = pDAESolver->GetSensitivities();
	if(m_nNumberOfOptimizationVariables != matSens.GetNrows())
		daeDeclareAndThrowException(exInvalidCall);

	return matSens;
}

bool daeOptimizationConstraint::IsLinear(void) const
{
	if(!m_pEquationExecutionInfo)
		daeDeclareAndThrowException(exInvalidPointer)
	boost::shared_ptr<adNode> node = m_pEquationExecutionInfo->GetEquationEvaluationNode();
	if(!node)
		daeDeclareAndThrowException(exInvalidPointer)
		
	return node->IsLinear();
}

void daeOptimizationConstraint::GetOptimizationVariableIndexes(std::vector<size_t>& narrOptimizationVariablesIndexes) const
{
	narrOptimizationVariablesIndexes = m_narrOptimizationVariablesIndexes;
}

void daeOptimizationConstraint::SetType(daeeConstraintType value)
{
	m_eConstraintType = value;	
}

daeeConstraintType daeOptimizationConstraint::GetType(void) const
{
	return m_eConstraintType;	
}

void daeOptimizationConstraint::Initialize(const std::vector< boost::shared_ptr<daeOptimizationVariable> >& arrOptimizationVariables, daeBlock_t* pBlock)
{
	size_t i;
	boost::shared_ptr<daeOptimizationVariable> pOptVariable;
	vector<daeEquationExecutionInfo*> ptrarrEquationExecutionInfos;

	if(!m_pConstraintFunction)
		daeDeclareAndThrowException(exInvalidPointer)
	
	m_pConstraintFunction->GetEquationExecutionInfos(ptrarrEquationExecutionInfos);
	if(ptrarrEquationExecutionInfos.size() != 1)
		daeDeclareAndThrowException(exInvalidCall)
		
	m_pEquationExecutionInfo = ptrarrEquationExecutionInfos[0];
	if(!m_pEquationExecutionInfo)
		daeDeclareAndThrowException(exInvalidPointer)
	
// 1. Set the variable/equation index in the block it belongs
	m_nEquationIndexInBlock = m_pEquationExecutionInfo->GetEquationIndexInBlock();
	m_nVariableIndexInBlock = pBlock->FindVariableBlockIndex(m_pConstraintVariable->m_nOverallIndex);
	if(m_nEquationIndexInBlock == ULONG_MAX || m_nVariableIndexInBlock == ULONG_MAX)
		daeDeclareAndThrowException(exInvalidCall);
	
// 2. Add all optimization variables indexes to list of indexes
	m_nNumberOfOptimizationVariables = arrOptimizationVariables.size();

	m_narrOptimizationVariablesIndexes.clear();
	for(i = 0; i < arrOptimizationVariables.size(); i++)
	{
		pOptVariable = arrOptimizationVariables[i];
		m_narrOptimizationVariablesIndexes.push_back(pOptVariable->GetOptimizationVariableIndex());
	}
	std::sort(m_narrOptimizationVariablesIndexes.begin(), m_narrOptimizationVariablesIndexes.end());
}

bool daeOptimizationConstraint::CheckObject(vector<string>& strarrErrors) const
{
	string strError;

	bool bCheck = true;

// Do not check daeObject since this is not an usual object
//	if(!daeObject::CheckObject(strarrErrors))
//		bCheck = false;

	if(!m_pConstraintVariable)
	{	
		strError = "Invalid constraint variable";
		strarrErrors.push_back(strError);
		return false;
	}
	if(!m_pConstraintFunction)
	{	
		strError = "Invalid constraint function";
		strarrErrors.push_back(strError);
		return false;
	}
	if(m_pConstraintFunction->GetEquationDefinitionMode() == eEDMUnknown ||
	   m_pConstraintFunction->GetEquationEvaluationMode() == eEEMUnknown)
	{	
		strError = "Constraint residual not specified [" + m_pConstraintFunction->GetCanonicalName() + "]";
		strarrErrors.push_back(strError);
		return false;
	}
	
	return bCheck;
}

void daeOptimizationConstraint::Export(std::string& strContent, daeeModelLanguage eLanguage, daeModelExportContext& c) const
{
	
}

size_t daeOptimizationConstraint::GetNumberOfOptimizationVariables(void) const
{
	return m_nNumberOfOptimizationVariables;
}
*/

/******************************************************************
	daeMeasuredVariable
*******************************************************************/
daeMeasuredVariable::daeMeasuredVariable(void)
{
}

daeMeasuredVariable::daeMeasuredVariable(daeSimulation_t* pSimulation, 
										 real_t abstol, 
										 size_t nEquationIndex,
										 const string& strDescription)
	: daeFunctionWithGradients(pSimulation, 
							   abstol, 
							   string("V_measured") + toString<size_t>(nEquationIndex + 1), 
							   string("F_measured") + toString<size_t>(nEquationIndex + 1),
							   strDescription)
{
}

daeMeasuredVariable::~daeMeasuredVariable(void)
{
}

/******************************************************************
	daeOptimizationVariable
*******************************************************************/
daeOptimizationVariable::daeOptimizationVariable(void)
{
	m_pVariable                  = NULL;
	m_dLB						 = 0;
	m_dUB						 = 0;
	m_dDefaultValue				 = 0;
	m_nOptimizationVariableIndex = ULONG_MAX;
}

daeOptimizationVariable::daeOptimizationVariable(daeVariable* pVariable, size_t nOptimizationVariableIndex, const std::vector<size_t>& narrDomainIndexes, real_t LB, real_t UB, real_t defaultValue)
{
	if(!pVariable)
		daeDeclareAndThrowException(exInvalidPointer)
			
	m_pVariable						= pVariable;
	m_dLB							= LB;
	m_dUB							= UB;
	m_dDefaultValue					= defaultValue;
	m_eType							= eContinuousVariable;
	m_narrDomainIndexes 			= narrDomainIndexes;
	m_nOptimizationVariableIndex	= nOptimizationVariableIndex;
}

daeOptimizationVariable::daeOptimizationVariable(daeVariable* pVariable, size_t nOptimizationVariableIndex, const std::vector<size_t>& narrDomainIndexes, int LB, int UB, int defaultValue)
{
	if(!pVariable)
		daeDeclareAndThrowException(exInvalidPointer)
			
	m_pVariable						= pVariable;
	m_dLB							= LB;
	m_dUB							= UB;
	m_dDefaultValue					= defaultValue;
	m_eType							= eIntegerVariable;
	m_narrDomainIndexes 			= narrDomainIndexes;
	m_nOptimizationVariableIndex	= nOptimizationVariableIndex;
}

daeOptimizationVariable::daeOptimizationVariable(daeVariable* pVariable, size_t nOptimizationVariableIndex, const std::vector<size_t>& narrDomainIndexes, bool defaultValue)
{
	if(!pVariable)
		daeDeclareAndThrowException(exInvalidPointer)
			
	m_pVariable						= pVariable;
	m_dLB							= 0;
	m_dUB							= 1;
	m_dDefaultValue					= (defaultValue ? 1 : 0);
	m_eType							= eBinaryVariable;
	m_narrDomainIndexes				= narrDomainIndexes;
	m_nOptimizationVariableIndex	= nOptimizationVariableIndex;
}

daeOptimizationVariable::~daeOptimizationVariable(void)
{
}

void daeOptimizationVariable::SetType(daeeOptimizationVariableType value)
{
	m_eType = value;	
}

daeeOptimizationVariableType daeOptimizationVariable::GetType(void) const
{
	return m_eType;	
}

void daeOptimizationVariable::SetStartingPoint(real_t value)
{
	m_dDefaultValue = value;	
}

real_t daeOptimizationVariable::GetStartingPoint(void) const
{
	return m_dDefaultValue;	
}

void daeOptimizationVariable::SetLB(real_t value)
{
	m_dLB = value;	
}

real_t daeOptimizationVariable::GetLB(void) const
{
	return m_dLB;	
}

void daeOptimizationVariable::SetUB(real_t value)
{
	m_dUB = value;	
}

real_t daeOptimizationVariable::GetUB(void) const
{
	return m_dUB;	
}

size_t daeOptimizationVariable::GetOverallIndex(void) const
{
	if(!m_pVariable)
		daeDeclareAndThrowException(exInvalidPointer)
		
	size_t index = m_pVariable->m_nOverallIndex + 
				   m_pVariable->CalculateIndex(m_narrDomainIndexes);
	return index;	
}

size_t daeOptimizationVariable::GetOptimizationVariableIndex(void) const
{
	return m_nOptimizationVariableIndex;	
}

std::string daeOptimizationVariable::GetName(void) const
{
	if(!m_pVariable)
		daeDeclareAndThrowException(exInvalidPointer)

	string strName = m_pVariable->GetName();
	if(m_narrDomainIndexes.empty())
		return strName;
	
	strName += "("; 
	for(size_t i = 0; i < m_narrDomainIndexes.size(); i++)
	{
		if(i != 0)
			strName += ",";
		strName += toString<size_t>(m_narrDomainIndexes[i]);
	}
	strName += ")";
	return strName;
}

void daeOptimizationVariable::SetValue(real_t value)
{
	if(!m_pVariable)
		daeDeclareAndThrowException(exInvalidPointer)
		
	size_t n = m_narrDomainIndexes.size();
	
	if(n == 0)
		m_pVariable->ReAssignValue(value);
	else if(n == 1)
		m_pVariable->ReAssignValue(m_narrDomainIndexes[0],
								   value);
	else if(n == 2)
		m_pVariable->ReAssignValue(m_narrDomainIndexes[0],
								   m_narrDomainIndexes[1],
								   value);
	else if(n == 3)
		m_pVariable->ReAssignValue(m_narrDomainIndexes[0],
								   m_narrDomainIndexes[1],
								   m_narrDomainIndexes[2],
								   value);
	else if(n == 4)
		m_pVariable->ReAssignValue(m_narrDomainIndexes[0],
								   m_narrDomainIndexes[1],
								   m_narrDomainIndexes[2],
								   m_narrDomainIndexes[3],
								   value);
	else if(n == 5)
		m_pVariable->ReAssignValue(m_narrDomainIndexes[0],
								   m_narrDomainIndexes[1],
								   m_narrDomainIndexes[2],
								   m_narrDomainIndexes[3],
								   m_narrDomainIndexes[4],
								   value);
	else if(n == 6)
		m_pVariable->ReAssignValue(m_narrDomainIndexes[0],
								   m_narrDomainIndexes[1],
								   m_narrDomainIndexes[2],
								   m_narrDomainIndexes[3],
								   m_narrDomainIndexes[4],
								   m_narrDomainIndexes[5],
								   value);
	else if(n == 7)
		m_pVariable->ReAssignValue(m_narrDomainIndexes[0],
								   m_narrDomainIndexes[1],
								   m_narrDomainIndexes[2],
								   m_narrDomainIndexes[3],
								   m_narrDomainIndexes[4],
								   m_narrDomainIndexes[5],
								   m_narrDomainIndexes[6],
								   value);
	else if(n == 8)
		m_pVariable->ReAssignValue(m_narrDomainIndexes[0],
								   m_narrDomainIndexes[1],
								   m_narrDomainIndexes[2],
								   m_narrDomainIndexes[3],
								   m_narrDomainIndexes[4],
								   m_narrDomainIndexes[5],
								   m_narrDomainIndexes[6],
								   m_narrDomainIndexes[7],
								   value);
	else
		daeDeclareAndThrowException(exInvalidCall)
}

real_t daeOptimizationVariable::GetValue(void) const
{
	if(!m_pVariable)
		daeDeclareAndThrowException(exInvalidPointer)
	
	size_t n = m_narrDomainIndexes.size();
	
	if(n == 0)
		return m_pVariable->GetValue();
	else if(n == 1)
		return m_pVariable->GetValue(m_narrDomainIndexes[0]);
	else if(n == 2)
		return m_pVariable->GetValue(m_narrDomainIndexes[0],
									 m_narrDomainIndexes[1]);
	else if(n == 3)
		return m_pVariable->GetValue(m_narrDomainIndexes[0],
									 m_narrDomainIndexes[1],
									 m_narrDomainIndexes[2]);
	else if(n == 4)
		return m_pVariable->GetValue(m_narrDomainIndexes[0],
									 m_narrDomainIndexes[1],
									 m_narrDomainIndexes[2],
									 m_narrDomainIndexes[3]);
	else if(n == 5)
		return m_pVariable->GetValue(m_narrDomainIndexes[0],
									 m_narrDomainIndexes[1],
									 m_narrDomainIndexes[2],
									 m_narrDomainIndexes[3],
									 m_narrDomainIndexes[4]);
	else if(n == 6)
		return m_pVariable->GetValue(m_narrDomainIndexes[0],
									 m_narrDomainIndexes[1],
									 m_narrDomainIndexes[2],
									 m_narrDomainIndexes[3],
									 m_narrDomainIndexes[4],
									 m_narrDomainIndexes[5]);
	else if(n == 7)
		return m_pVariable->GetValue(m_narrDomainIndexes[0],
									 m_narrDomainIndexes[1],
									 m_narrDomainIndexes[2],
									 m_narrDomainIndexes[3],
									 m_narrDomainIndexes[4],
									 m_narrDomainIndexes[5],
									 m_narrDomainIndexes[6]);
	else if(n == 8)
		return m_pVariable->GetValue(m_narrDomainIndexes[0],
									 m_narrDomainIndexes[1],
									 m_narrDomainIndexes[2],
									 m_narrDomainIndexes[3],
									 m_narrDomainIndexes[4],
									 m_narrDomainIndexes[5],
									 m_narrDomainIndexes[6],
									 m_narrDomainIndexes[7]);
	else
		daeDeclareAndThrowException(exInvalidCall)
				
	return 0;
}
			
bool daeOptimizationVariable::CheckObject(vector<string>& strarrErrors) const
{
	string strError;

	bool bCheck = true;

// Do not check daeObject since this is not an usual object
//	if(!daeObject::CheckObject(strarrErrors))
//		bCheck = false;

	if(!m_pVariable)
	{	
		strError = "Invalid optimization variable specified";
		strarrErrors.push_back(strError);
		return false;
	}
	if(!m_pVariable->m_pModel || !m_pVariable->m_pModel->m_pDataProxy)
	{	
		strError = "Invalid parent model in optimization variable [" + m_pVariable->GetCanonicalName() + "]";
		strarrErrors.push_back(strError);
		return false;
	}
			
	if(m_pVariable->m_ptrDomains.size() != m_narrDomainIndexes.size())
	{
		strError = "Wrong number of indexes in the optimization variable [" + m_pVariable->GetCanonicalName() + "]";
		strarrErrors.push_back(strError);
		bCheck = false;
	}
  
	size_t index = GetOverallIndex();
	int type = m_pVariable->m_pModel->m_pDataProxy->GetVariableType(index);

	if(type != cnFixed)
	{
		strError = "Optimization variable [" + m_pVariable->GetCanonicalName() + "] must be assigned (cannot be a state-variable)";
		strarrErrors.push_back(strError);
		bCheck = false;
	}
	
	if(m_dDefaultValue < m_dLB)
	{
		strError = "The default value of the optimization variable [" + m_pVariable->GetCanonicalName() + "] is lower than the lower bound";
		strarrErrors.push_back(strError);
		bCheck = false;
	}
	if(m_dDefaultValue > m_dUB)
	{
		strError = "The default value of the optimization variable [" + m_pVariable->GetCanonicalName() + "] is greater than the upper bound";
		strarrErrors.push_back(strError);
		bCheck = false;
	}

	return bCheck;
}

void daeOptimizationVariable::Export(std::string& strContent, daeeModelLanguage eLanguage, daeModelExportContext& c) const
{
	
}



}
}
