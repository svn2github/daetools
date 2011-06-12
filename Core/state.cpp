#include "stdafx.h"
#include "coreimpl.h"

namespace dae 
{
namespace core 
{
/*********************************************************************************************
	daeState
**********************************************************************************************/
daeState::daeState()
{
	m_pModel  = NULL;
	m_pSTN    = NULL;
}

daeState::~daeState()
{
}

void daeState::Open(io::xmlTag_t* pTag)
{
	string strName;

	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer); 

	m_ptrarrEquations.EmptyAndFreeMemory();
	m_ptrarrStateTransitions.EmptyAndFreeMemory();
	m_ptrarrSTNs.EmptyAndFreeMemory();

	m_ptrarrEquations.SetOwnershipOnPointers(true);
	m_ptrarrStateTransitions.SetOwnershipOnPointers(true);
	m_ptrarrSTNs.SetOwnershipOnPointers(true);

	daeObject::Open(pTag);

	daeSetModelAndCanonicalNameDelegate<daeObject> del(this, m_pModel);

	strName = "Equations";
	pTag->OpenObjectArray(strName, m_ptrarrEquations, &del);

	strName = "STNs";
	pTag->OpenObjectArray(strName, m_ptrarrSTNs, &del);

	strName = "StateTransitions";
	pTag->OpenObjectArray(strName, m_ptrarrStateTransitions, &del);

	strName = "STN";
	daeFindSTNByID stndel(m_pModel);
	m_pSTN = pTag->OpenObjectRef<daeSTN>(strName, &stndel);
}

void daeState::Save(io::xmlTag_t* pTag) const
{
	string strName;

	daeObject::Save(pTag);

	strName = "Equations";
	pTag->SaveObjectArray(strName, m_ptrarrEquations);

	strName = "STNs";
	pTag->SaveObjectArray(strName, m_ptrarrSTNs);

	strName = "StateTransitions";
	pTag->SaveObjectArray(strName, m_ptrarrStateTransitions);

	strName = "STN";
	pTag->SaveObjectRef(strName, m_pSTN);
}

void daeState::Export(std::string& strContent, daeeModelLanguage eLanguage, daeModelExportContext& c) const
{
	string strExport, strEquations, strStateTransitions, strSTNs;
	boost::format fmtFile;

	if(c.m_bExportDefinition)
	{
	}
	else
	{
		if(eLanguage == ePYDAE)
		{
			strExport = c.CalculateIndent(c.m_nPythonIndentLevel) + "self.STATE(\"%1%\")\n" + 
						"%2%"+
						"%3%"+
						"%4%\n";
			ExportObjectArray(m_ptrarrEquations,        strEquations,        eLanguage, c);
			ExportObjectArray(m_ptrarrSTNs,             strSTNs,             eLanguage, c);
			ExportObjectArray(m_ptrarrStateTransitions, strStateTransitions, eLanguage, c);
			
			fmtFile.parse(strExport);
			fmtFile % m_strShortName
					% strEquations 
					% strSTNs 
					% strStateTransitions;
		}
		else if(eLanguage == eCDAE)
		{
			strExport = c.CalculateIndent(c.m_nPythonIndentLevel) + "STATE(\"%1%\");\n" + 
						"%2%"+
						"%3%"+
						"%4%\n";
			ExportObjectArray(m_ptrarrEquations,        strEquations,        eLanguage, c);
			ExportObjectArray(m_ptrarrSTNs,             strSTNs,             eLanguage, c);
			ExportObjectArray(m_ptrarrStateTransitions, strStateTransitions, eLanguage, c);
			
			fmtFile.parse(strExport);
			fmtFile % m_strShortName
					% strEquations 
					% strSTNs 
					% strStateTransitions;
		}
		else
		{
			daeDeclareAndThrowException(exNotImplemented); 
		}
	}
	
	strContent += fmtFile.str();
}

void daeState::OpenRuntime(io::xmlTag_t* pTag)
{
	daeObject::OpenRuntime(pTag);
}

void daeState::SaveRuntime(io::xmlTag_t* pTag) const
{
	string strName;

	daeObject::SaveRuntime(pTag);
	
	strName = "Equations";
	pTag->SaveRuntimeObjectArray(strName, m_ptrarrEquations);

	strName = "STNs";
	pTag->SaveRuntimeObjectArray(strName, m_ptrarrSTNs);

	strName = "StateTransitions";
	pTag->SaveRuntimeObjectArray(strName, m_ptrarrStateTransitions);

	strName = "STN";
	pTag->SaveObjectRef(strName, m_pSTN);
}

void daeState::Create(const string& strName, daeSTN* pSTN)
{
	m_pModel		= pSTN->m_pModel;
	m_pSTN			= pSTN;
	m_strShortName	= strName;
}

void daeState::InitializeStateTransitions(void)
{
	size_t i;
	daeSTN *pSTN;
	daeStateTransition *pST;
	
	for(i = 0; i < m_ptrarrStateTransitions.size(); i++)
	{
		pST = m_ptrarrStateTransitions[i];
		if(!pST)
			daeDeclareAndThrowException(exInvalidPointer);
	
		pST->Initialize();
	}
	
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer);
		pSTN->InitializeStateTransitions();
	}
}

void daeState::InitializeDEDIs(void)
{
	size_t i;
	daeSTN *pSTN;
	daeEquation* pEquation;
	
	for(i = 0; i < m_ptrarrEquations.size(); i++)
	{
		pEquation = m_ptrarrEquations[i];
		if(!pEquation)
			daeDeclareAndThrowException(exInvalidPointer);

		pEquation->InitializeDEDIs();
	}
	
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		if(!pSTN)
			daeDeclareAndThrowException(exInvalidPointer);
		pSTN->InitializeDEDIs();
	}
}

daeSTN* daeState::GetSTN(void) const
{
	return m_pSTN;
}

void daeState::SetSTN(daeSTN* pSTN)
{
	m_pSTN = pSTN;
}

void daeState::SetModelAndCanonicalName(daeObject* pObject)
{
	if(!pObject)
		daeDeclareAndThrowException(exInvalidPointer);
	string strName;
	strName = m_strCanonicalName + "." + pObject->m_strShortName;
	pObject->m_strCanonicalName = strName;
	pObject->m_pModel = m_pModel;
}

void daeState::AddEquation(daeEquation* pEquation)
{
	if(!pEquation)
		daeDeclareAndThrowException(exInvalidPointer);
	SetModelAndCanonicalName(pEquation);
	m_ptrarrEquations.push_back(pEquation);
}

//daeEquation* daeState::AddEquation(const string& strEquationExpression)
//{
//	daeEquation* pEquation = new daeEquation();
//	string strName = "Equation_" + toString<size_t>(m_ptrarrEquations.size());
//	pEquation->SetName(strName);
//	AddEquation(pEquation);
//	return pEquation;
//}

size_t daeState::GetNumberOfEquations(void) const
{
	return m_ptrarrEquations.size();
}

size_t daeState::GetNumberOfStateTransitions(void) const
{
	return m_ptrarrStateTransitions.size();
}

size_t daeState::GetNumberOfSTNs(void) const
{
	return m_ptrarrSTNs.size();
}

void daeState::AddNestedSTN(daeSTN* pSTN)
{
	if(!pSTN)
		daeDeclareAndThrowException(exInvalidPointer);
	SetModelAndCanonicalName(pSTN);
	pSTN->SetParentState(this);
	m_ptrarrSTNs.push_back(pSTN);
}

void daeState::AddStateTransition(daeStateTransition* pStateTransition)
{
	if(!pStateTransition)
		daeDeclareAndThrowException(exInvalidPointer);
	SetModelAndCanonicalName(pStateTransition);
	m_ptrarrStateTransitions.push_back(pStateTransition);
}
	
void daeState::GetStateTransitions(vector<daeStateTransition_t*>& ptrarrStateTransitions)
{
	ptrarrStateTransitions.clear();
	for(size_t i = 0; i < m_ptrarrStateTransitions.size(); i++)
		ptrarrStateTransitions.push_back(m_ptrarrStateTransitions[i]);
}
	
void daeState::GetEquations(vector<daeEquation_t*>& ptrarrEquations)
{
	ptrarrEquations.clear();
	for(size_t i = 0; i < m_ptrarrEquations.size(); i++)
		ptrarrEquations.push_back(m_ptrarrEquations[i]);
}
	
void daeState::GetNestedSTNs(vector<daeSTN_t*>& ptrarrSTNs)
{
	ptrarrSTNs.clear();
	for(size_t i = 0; i < m_ptrarrSTNs.size(); i++)
		ptrarrSTNs.push_back(m_ptrarrSTNs[i]);
}

void daeState::CalcNonZeroElements(int& NNZ)
{
	size_t i;
	daeSTN* pSTN;
	daeState* pState;
	daeEquationExecutionInfo* pEquationExecutionInfo;

// First find in normal equations (non-STN)
	for(i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
	{
		pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
		NNZ += pEquationExecutionInfo->m_mapIndexes.size();
	}

// Then in nested STNs
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		pSTN->CalcNonZeroElements(NNZ);
	}
}

void daeState::FillSparseMatrix(daeSparseMatrix<real_t>* pMatrix)
{
	size_t i;
	daeSTN* pSTN;
	daeEquationExecutionInfo* pEquationExecutionInfo;

// First find in normal equations (non-STN)
	for(i = 0; i < m_ptrarrEquationExecutionInfos.size(); i++)
	{
		pEquationExecutionInfo = m_ptrarrEquationExecutionInfos[i];
		pMatrix->AddRow(pEquationExecutionInfo->m_mapIndexes);
	}

// Then in nested STNs
	for(i = 0; i < m_ptrarrSTNs.size(); i++)
	{
		pSTN = m_ptrarrSTNs[i];
		pSTN->FillSparseMatrix(pMatrix);
	}
}

bool daeState::CheckObject(vector<string>& strarrErrors) const
{
	string strError;

	bool bCheck = true;

// Check base class	
	if(!daeObject::CheckObject(strarrErrors))
		bCheck = false;

// Check parent STN pointer	
	if(!m_pSTN)
	{
		strError = "Invalid parent state transition network in state [" + GetCanonicalName() + "]";
		strarrErrors.push_back(strError);
		bCheck = false;
	}
	
// Check number of equations	
	if(m_ptrarrEquations.size() == 0)
	{
		strError = "Invalid number of equations in state [" + GetCanonicalName() + "]";
		strarrErrors.push_back(strError);
		bCheck = false;
	}

// Check equations	
	if(m_ptrarrEquations.size() > 0)
	{
		daeEquation* pEquation;	
		for(size_t i = 0; i < m_ptrarrEquations.size(); i++)
		{
			pEquation = m_ptrarrEquations[i];
			if(!pEquation)
			{
				strError = "Invalid equation in state [" + GetCanonicalName() + "]";
				strarrErrors.push_back(strError);
				bCheck = false;
				continue;
			}
			
			if(!pEquation->CheckObject(strarrErrors))
				bCheck = false;
		}
	}

// Check state transitions	
	if(m_ptrarrStateTransitions.size() > 0)
	{
		daeStateTransition* pStateTransition;	
		for(size_t i = 0; i < m_ptrarrStateTransitions.size(); i++)
		{
			pStateTransition = m_ptrarrStateTransitions[i];
			if(!pStateTransition)
			{
				strError = "Invalid equation in state [" + GetCanonicalName() + "]";
				strarrErrors.push_back(strError);
				bCheck = false;
				continue;
			}
			
			if(!pStateTransition->CheckObject(strarrErrors))
				bCheck = false;
		}
	}

// Check nested STNs	
	if(m_ptrarrSTNs.size() > 0)
	{
		daeSTN* pSTN;	
		for(size_t i = 0; i < m_ptrarrSTNs.size(); i++)
		{
			pSTN = m_ptrarrSTNs[i];
			if(!pSTN)
			{
				strError = "Invalid equation in state [" + GetCanonicalName() + "]";
				strarrErrors.push_back(strError);
				bCheck = false;
				continue;
			}
			
			if(!pSTN->CheckObject(strarrErrors))
				bCheck = false;
		}
	}

	return bCheck;
}

}
}
