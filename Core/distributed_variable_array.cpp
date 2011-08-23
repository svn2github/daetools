#include "stdafx.h"
#include "coreimpl.h"
#include "nodes_array.h"
using namespace boost;

namespace dae 
{
namespace core 
{

daeExecutionContext::daeExecutionContext()
{
	m_pDataProxy										= NULL;
	m_pBlock											= NULL;
	m_dInverseTimeStep									= 0;
	m_pEquationExecutionInfo							= NULL;
	m_eEquationCalculationMode							= eECMUnknown;
	m_nCurrentVariableIndexForJacobianEvaluation		= ULONG_MAX;
	m_nCurrentParameterIndexForSensitivityEvaluation	= ULONG_MAX;
	m_nIndexInTheArrayOfCurrentParameterForSensitivityEvaluation = ULONG_MAX;
}

size_t daeVariable::GetNumberOfPoints() const
{
	vector<daeDomain*>::size_type i;
	daeDomain* pDomain;
	size_t nTotalNumberOfVariables = 1;
	for(i = 0; i < m_ptrDomains.size(); i++)
	{
		pDomain = m_ptrDomains[i];
		if(!pDomain)
			daeDeclareAndThrowException(exInvalidPointer); 
		if(pDomain->m_nNumberOfPoints == 0)
		{	
			daeDeclareException(exInvalidCall); 
			e << "Number of points in domain [" << pDomain->GetCanonicalName() << "] in variable [" << m_strCanonicalName << "] must not be zero; did you forget to initialize it?";
			throw e;
		}
		nTotalNumberOfVariables *= pDomain->m_nNumberOfPoints;
	}
	return nTotalNumberOfVariables;
}

size_t daeVariable::CalculateIndex(const std::vector<size_t>& narrDomainIndexes) const
{
	size_t* indexes;
	size_t i, N, index;

	N = narrDomainIndexes.size();
	if(N == 0)
	{
		index = CalculateIndex(NULL, 0);
	}
	else
	{
		indexes = new size_t[N];
		for(i = 0; i < N; i++)
			indexes[i] = narrDomainIndexes[i];
		
		index = CalculateIndex(indexes, N);
		delete[] indexes;
	}
	return index;
}

size_t daeVariable::CalculateIndex(const size_t* indexes, const size_t N) const
{
	size_t		i, j, nIndex, temp;
	daeDomain	*pDomain;

	if(m_ptrDomains.size() != N)
	{	
		daeDeclareException(exInvalidCall); 
		e << "Illegal number of domains, parameter " << m_strCanonicalName;
		throw e;
	}

// Check the pointers and the bounds first
	for(i = 0; i < N; i++)
	{
		pDomain = m_ptrDomains[i];
		if(!pDomain)
			daeDeclareAndThrowException(exInvalidPointer); 
		if(indexes[i] >= pDomain->GetNumberOfPoints())
			daeDeclareAndThrowException(exOutOfBounds); 
	}

// Calculate the index
	nIndex = 0;
	for(i = 0; i < N; i++)
	{
		temp = indexes[i];
		for(j = i+1; j < N; j++)
			temp *= m_ptrDomains[j]->GetNumberOfPoints();
		nIndex += temp;
	}

	return nIndex;
}

void daeVariable::Fill_adouble_array(vector<adouble>& arrValues, const daeArrayRange* ranges, size_t* indexes, const size_t N, size_t currentN) const
{
	if(currentN == N) // create and add adouble to the vector
	{
		dae_push_back(arrValues, Create_adouble(indexes, N));
	}
	else // continue iterating
	{
		const daeArrayRange& r = ranges[currentN];
		
		vector<size_t> narrPoints;
	// If the size is 1 it is going to call Fill_adouble_array() only once
	// Otherwise, narrPoints.size() times
		r.GetPoints(narrPoints);
		for(size_t i = 0; i < narrPoints.size(); i++)
		{
			indexes[currentN] = narrPoints[i]; // Wasn't it bug below??? It should be narrPoints[i]!!
			Fill_adouble_array(arrValues, ranges, indexes, N, currentN + 1);
		}
		
//		if(r.m_eType == eRangeConstantIndex)
//		{
//			indexes[currentN] = r.m_nIndex;
//			Fill_adouble_array(arrValues, ranges, indexes, N, currentN + 1);
//		}
//		else if(r.m_eType == eRangeDomainIterator)
//		{
//			indexes[currentN] = r.m_pDEDI->GetCurrentIndex();
//			Fill_adouble_array(arrValues, ranges, indexes, N, currentN + 1);
//		}
//		else if(r.m_eType == eRange)
//		{
//			vector<size_t> narrPoints;
//			r.m_Range.GetPoints(narrPoints);
//			for(size_t i = 0; i < narrPoints.size(); i++)
//			{
//				indexes[currentN] = i; // BUG !!!!!!!! Shouldn't it be narrPoints[i]???
//				Fill_adouble_array(arrValues, ranges, indexes, N, currentN + 1);
//			}
//		}
	}
}

void daeVariable::Fill_dt_array(vector<adouble>& arrValues, const daeArrayRange* ranges, size_t* indexes, const size_t N, size_t currentN) const
{
	if(currentN == N) // create and add adouble to the vector
	{
		dae_push_back(arrValues, Calculate_dt(indexes, N));
	}
	else // continue iterating
	{
		const daeArrayRange& r = ranges[currentN];
		
		vector<size_t> narrPoints;
	// If the size is 1 it is going to call Fill_dt_array() only once
	// Otherwise, narrPoints.size() times
		r.GetPoints(narrPoints);
		for(size_t i = 0; i < narrPoints.size(); i++)
		{
			indexes[currentN] = narrPoints[i]; // Wasn't it bug below??? It should be narrPoints[i] !!
			Fill_dt_array(arrValues, ranges, indexes, N, currentN + 1);
		}
		
//		if(r.m_eType == eRangeConstantIndex)
//		{
//			indexes[currentN] = r.m_nIndex;
//			Fill_dt_array(arrValues, ranges, indexes, N, currentN + 1);
//		}
//		else if(r.m_eType == eRangeDomainIterator)
//		{
//			indexes[currentN] = r.m_pDEDI->GetCurrentIndex();
//			Fill_dt_array(arrValues, ranges, indexes, N, currentN + 1);
//		}
//		else if(r.m_eType == eRange)
//		{
//			vector<size_t> narrPoints;
//			r.m_Range.GetPoints(narrPoints);
//			for(size_t i = 0; i < narrPoints.size(); i++)
//			{
//				indexes[currentN] = i; // BUG !!!!!!!! Shouldn't it be narrPoints[i]???
//				Fill_dt_array(arrValues, ranges, indexes, N, currentN + 1);
//			}
//		}
	}
}

void daeVariable::Fill_partial_array(vector<adouble>& arrValues, size_t nOrder, const daeDomain_t& rDomain, const daeArrayRange* ranges, size_t* indexes, const size_t N, size_t currentN) const
{
	if(currentN == N) // create and add adouble to the vector
	{
		dae_push_back(arrValues, partial(nOrder, rDomain, indexes, N));
	}
	else // continue iterating
	{
		const daeArrayRange& r = ranges[currentN];
		
		vector<size_t> narrPoints;
	// If the size is 1 it is going to call Fill_partial_array() only once
	// Otherwise, narrPoints.size() times
		r.GetPoints(narrPoints);
		for(size_t i = 0; i < narrPoints.size(); i++)
		{
			indexes[currentN] = narrPoints[i]; // Wasn't it bug below??? It should be narrPoints[i] !!
			Fill_partial_array(arrValues, nOrder, rDomain, ranges, indexes, N, currentN + 1);
		}
		
//		if(r.m_eType == eRangeConstantIndex)
//		{
//			indexes[currentN] = r.m_nIndex;
//			Fill_partial_array(arrValues, nOrder, rDomain, ranges, indexes, N, currentN + 1);
//		}
//		else if(r.m_eType == eRangeDomainIterator)
//		{
//			indexes[currentN] = r.m_pDEDI->GetCurrentIndex();
//			Fill_partial_array(arrValues, nOrder, rDomain, ranges, indexes, N, currentN + 1);
//		}
//		else if(r.m_eType == eRange)
//		{
//			vector<size_t> narrPoints;
//			r.m_Range.GetPoints(narrPoints);
//			for(size_t i = 0; i < narrPoints.size(); i++)
//			{
//				indexes[currentN] = i; // BUG !!!!!!!! Shouln't it be narrPoints[i]???
//				Fill_partial_array(arrValues, nOrder, rDomain, ranges, indexes, N, currentN + 1);
//			}
//		}
	}
}

adouble_array daeVariable::Create_adouble_array(const daeArrayRange* ranges, const size_t N) const
{
	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer); 
	if(!m_pModel->m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer); 

// First create all adoubles (according to the ranges sent)
// The result is array of values, and if GetGatherMode flag is set 
// also the adNode* in each adouble
	adouble_array varArray;
	size_t* indexes = new size_t[N];
	Fill_adouble_array(varArray.m_arrValues, ranges, indexes, N, 0);
	delete[] indexes;

// Now I should create adNodeArray* node
// I rely here on adNode* in each of adouble in varArray.m_arrValues 
// created above
	if(m_pModel->m_pDataProxy->GetGatherInfo())
	{
		adRuntimeVariableNodeArray* node = new adRuntimeVariableNodeArray();
		varArray.node = shared_ptr<adNodeArray>(node);
		varArray.setGatherInfo(true);
		node->m_pVariable = const_cast<daeVariable*>(this);
		
		size_t size = varArray.m_arrValues.size();
		if(size == 0)
			daeDeclareAndThrowException(exInvalidCall); 
		
		node->m_ptrarrVariableNodes.resize(size);
		for(size_t i = 0; i < size; i++)
			node->m_ptrarrVariableNodes[i] = varArray.m_arrValues[i].node;
		node->m_arrRanges.resize(N);
		for(size_t i = 0; i < N; i++)
			node->m_arrRanges[i] = ranges[i];
	}
	
	return varArray;
}

adouble_array daeVariable::Calculate_dt_array(const daeArrayRange* ranges, const size_t N) const
{
	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer); 
	if(!m_pModel->m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer); 

	adouble_array varArray;
	size_t* indexes = new size_t[N];
	Fill_dt_array(varArray.m_arrValues, ranges, indexes, N, 0);
	delete[] indexes;

	if(m_pModel->m_pDataProxy->GetGatherInfo())
	{
		adRuntimeTimeDerivativeNodeArray* node = new adRuntimeTimeDerivativeNodeArray();
		varArray.node = shared_ptr<adNodeArray>(node);
		varArray.setGatherInfo(true);
		node->m_pVariable = const_cast<daeVariable*>(this);
		node->m_nDegree   = 1;
		
		size_t size = varArray.m_arrValues.size();
		if(size == 0)
			daeDeclareAndThrowException(exInvalidCall); 
		
		node->m_ptrarrTimeDerivativeNodes.resize(size);
		for(size_t i = 0; i < size; i++)
			node->m_ptrarrTimeDerivativeNodes[i] = varArray.m_arrValues[i].node;
		node->m_arrRanges.resize(N);
		for(size_t i = 0; i < N; i++)
			node->m_arrRanges[i] = ranges[i];
	}
	return varArray;
}

adouble_array daeVariable::partial_array(const size_t nOrder, const daeDomain_t& rDomain, const daeArrayRange* ranges, const size_t N) const
{
	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer); 
	if(!m_pModel->m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer); 

	adouble_array varArray;
	size_t* indexes = new size_t[N];
	Fill_partial_array(varArray.m_arrValues, nOrder, rDomain, ranges, indexes, N, 0);
	delete[] indexes;

	if(m_pModel->m_pDataProxy->GetGatherInfo())
	{
		adRuntimePartialDerivativeNodeArray* node = new adRuntimePartialDerivativeNodeArray();
		varArray.node = shared_ptr<adNodeArray>(node);
		varArray.setGatherInfo(true);
		node->m_pVariable = const_cast<daeVariable*>(this);
		const daeDomain* pDomain = dynamic_cast<const daeDomain*>(&rDomain);
		node->m_pDomain = const_cast<daeDomain*>(pDomain);
		node->m_nDegree = nOrder;
		
		size_t size = varArray.m_arrValues.size();
		if(size == 0)
			daeDeclareAndThrowException(exInvalidCall); 
		
		node->m_ptrarrPartialDerivativeNodes.resize(size);
		for(size_t i = 0; i < size; i++)
			node->m_ptrarrPartialDerivativeNodes[i] = varArray.m_arrValues[i].node;
		node->m_arrRanges.resize(N);
		for(size_t i = 0; i < N; i++)
			node->m_arrRanges[i] = ranges[i];
	}
	return varArray;
}

adouble_array daeVariable::CreateSetupVariableArray(const daeArrayRange* ranges, const size_t N) const
{
	adouble_array varArray;

	if(m_ptrDomains.size() != N)
	{	
		daeDeclareException(exInvalidCall); 
		e << "Invalid variable array call for [" << m_strCanonicalName << "], number of domains is " << m_ptrDomains.size() << " - but only " << N << " is given";
		throw e;
	}
	if(!m_pModel)
	{	
		daeDeclareException(exInvalidPointer); 
		e << "Invalid parent model in variable [" << m_strCanonicalName << "]";
		throw e;
	}
// Check if domains in indexes correspond to domains here
	for(size_t i = 0; i < N; i++)
	{
		if(ranges[i].m_eType == eRangeDomainIndex)
		{
			if(ranges[i].m_domainIndex.m_eType == eDomainIterator ||
			   ranges[i].m_domainIndex.m_eType == eIncrementedDomainIterator)
			{
				if(m_ptrDomains[i] != ranges[i].m_domainIndex.m_pDEDI->m_pDomain)
				{
					daeDeclareException(exInvalidCall);
					e << "You cannot create daeArrayRange with the domain [" << ranges[i].m_domainIndex.m_pDEDI->m_pDomain->GetCanonicalName() 
					  << "]; you must use the domain [" << m_ptrDomains[i]->GetCanonicalName() << "] as " << i+1 << ". range argument "
					  << "in variable [" << m_strCanonicalName << "] in function array()";
					throw e;
				}
			}
		}
		else if(ranges[i].m_eType == eRange)
		{
			if(m_ptrDomains[i] != ranges[i].m_Range.m_pDomain)
			{
				daeDeclareException(exInvalidCall);
				e << "You cannot create daeArrayRange with the domain [" << ranges[i].m_Range.m_pDomain->GetCanonicalName() 
				  << "]; you must use the domain [" << m_ptrDomains[i]->GetCanonicalName() << "] as " << i+1 << ". range argument "
				  << "in variable [" << m_strCanonicalName << "] in function array()";
				throw e;
			}
		}
	}

	adSetupVariableNodeArray* node = new adSetupVariableNodeArray();
	varArray.node = shared_ptr<adNodeArray>(node);
	varArray.setGatherInfo(true);

	node->m_pVariable = const_cast<daeVariable*>(this);
	node->m_arrRanges.resize(N);
	for(size_t i = 0; i < N; i++)
		node->m_arrRanges[i] = ranges[i];

	return varArray;
}

adouble_array daeVariable::CreateSetupTimeDerivativeArray(const daeArrayRange* ranges, const size_t N) const
{
	adouble_array varArray;

	if(m_ptrDomains.size() != N)
	{	
		daeDeclareException(exInvalidCall); 
		e << "Invalid time derivative array call for [" << m_strCanonicalName << "], number of domains is " << m_ptrDomains.size() << " - but only " << N << " is given";
		throw e;
	}
	if(!m_pModel)
	{	
		daeDeclareException(exInvalidPointer); 
		e << "Invalid parent model in variable [" << m_strCanonicalName << "]";
		throw e;
	}
// Check if domains in indexes correspond to domains here
	for(size_t i = 0; i < N; i++)
	{
		if(ranges[i].m_eType == eRangeDomainIndex)
		{
			if(ranges[i].m_domainIndex.m_eType == eDomainIterator ||
			   ranges[i].m_domainIndex.m_eType == eIncrementedDomainIterator)
			{
				if(m_ptrDomains[i] != ranges[i].m_domainIndex.m_pDEDI->m_pDomain)
				{
					daeDeclareException(exInvalidCall);
					e << "You cannot create daeArrayRange with the domain [" << ranges[i].m_domainIndex.m_pDEDI->m_pDomain->GetCanonicalName() 
					  << "]; you must use the domain [" << m_ptrDomains[i]->GetCanonicalName() << "] as " << i+1 << ". range argument "
					  << "in variable [" << m_strCanonicalName << "] in function dt_array()";
					throw e;
				}
			}
		}
		else if(ranges[i].m_eType == eRange)
		{
			if(m_ptrDomains[i] != ranges[i].m_Range.m_pDomain)
			{
				daeDeclareException(exInvalidCall);
				e << "You cannot create daeArrayRange with the domain [" << ranges[i].m_Range.m_pDomain->GetCanonicalName() 
				  << "]; you must use the domain [" << m_ptrDomains[i]->GetCanonicalName() << "] as " << i+1 << ". range argument "
				  << "in variable [" << m_strCanonicalName << "] in function dt_array()";
				throw e;
			}
		}
	}

	adSetupTimeDerivativeNodeArray* node = new adSetupTimeDerivativeNodeArray();
	varArray.node = shared_ptr<adNodeArray>(node);
	varArray.setGatherInfo(true);

	node->m_nDegree = 1;
	node->m_pVariable = const_cast<daeVariable*>(this);
	node->m_arrRanges.resize(N);
	for(size_t i = 0; i < N; i++)
		node->m_arrRanges[i] = ranges[i];

	return varArray;
}

adouble_array daeVariable::CreateSetupPartialDerivativeArray(const size_t nOrder, const daeDomain_t& rDomain, const daeArrayRange* ranges, const size_t N) const
{
	adouble_array varArray;

	if(m_ptrDomains.size() != N)
	{	
		daeDeclareException(exInvalidCall); 
		e << "Invalid partial derivative array call for [" << m_strCanonicalName << "], number of domains is " << m_ptrDomains.size() << " - but only " << N << " is given";
		throw e;
	}
	if(!m_pModel)
	{	
		daeDeclareException(exInvalidPointer); 
		e << "Invalid parent model in variable [" << m_strCanonicalName << "]";
		throw e;
	}
// Check if domains in indexes correspond to domains here
	for(size_t i = 0; i < N; i++)
	{
		if(ranges[i].m_eType == eRangeDomainIndex)
		{
			if(ranges[i].m_domainIndex.m_eType == eDomainIterator ||
			   ranges[i].m_domainIndex.m_eType == eIncrementedDomainIterator)
			{
				if(m_ptrDomains[i] != ranges[i].m_domainIndex.m_pDEDI->m_pDomain)
				{
					daeDeclareException(exInvalidCall);
					e << "You cannot create daeArrayRange with the domain [" << ranges[i].m_domainIndex.m_pDEDI->m_pDomain->GetCanonicalName() 
					  << "]; you must use the domain [" << m_ptrDomains[i]->GetCanonicalName() << "] as " << i+1 << ". range argument "
					  << "in variable [" << m_strCanonicalName << "] in function d/d2_array()";
					throw e;
				}
			}
		}
		else if(ranges[i].m_eType == eRange)
		{
			if(m_ptrDomains[i] != ranges[i].m_Range.m_pDomain)
			{
				daeDeclareException(exInvalidCall);
				e << "You cannot create daeArrayRange with the domain [" << ranges[i].m_Range.m_pDomain->GetCanonicalName() 
				  << "]; you must use the domain [" << m_ptrDomains[i]->GetCanonicalName() << "] as " << i+1 << ". range argument "
				  << "in variable [" << m_strCanonicalName << "] in function d/d2_array()";
				throw e;
			}
		}
	}

	adSetupPartialDerivativeNodeArray* node = new adSetupPartialDerivativeNodeArray();
	varArray.node = shared_ptr<adNodeArray>(node);
	varArray.setGatherInfo(true);

	node->m_nDegree = nOrder;
	node->m_pVariable = const_cast<daeVariable*>(this);
	const daeDomain* pDomain = dynamic_cast<const daeDomain*>(&rDomain);
	node->m_pDomain = const_cast<daeDomain*>(pDomain);
	node->m_arrRanges.resize(N);
	for(size_t i = 0; i < N; i++)
		node->m_arrRanges[i] = ranges[i];

	return varArray;
}

adouble daeVariable::Create_adouble(const size_t* indexes, const size_t N) const
{
	adouble tmp;
	size_t nIndex, nIndexWithinVariable;
	daeExecutionContext* pExecutionContext;

	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer); 
	if(!m_pModel->m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer); 

	nIndexWithinVariable = CalculateIndex(indexes, N);
	nIndex = m_nOverallIndex + nIndexWithinVariable;

/**************************************************************************************
  New code; just we need is to add variable indexes to the equation
**************************************************************************************/
	if(m_pModel->m_pDataProxy->GetVariableType(nIndex) != cnFixed)
	{
		if(m_pModel->m_pExecutionContextForGatherInfo)
		{
			pExecutionContext = m_pModel->m_pExecutionContextForGatherInfo;
			if(!pExecutionContext)
				daeDeclareAndThrowException(exInvalidPointer); 
	
			daeEquationExecutionInfo* pEquationExecutionInfo = pExecutionContext->m_pEquationExecutionInfo;
		// StateTransitions CreateRuntimeNode call ends up here 
		// In that case we dont have pEquationExecutionInfo nor we need it
			if(pEquationExecutionInfo && pExecutionContext->m_eEquationCalculationMode == eGatherInfo)
			{
				pEquationExecutionInfo->AddVariableInEquation(nIndex);
			}
		}
	}
	
/**************************************************************************************
  Old code; all this seems non necessary (we dont need values in this function)
  Just we need is to add variable indexes to the equation
***************************************************************************************   
	if(m_pModel->m_pDataProxy->GetVariableType(nIndex) == cnFixed)
	{
		tmp.setValue(GetValueAt(nIndex));
		tmp.setDerivative(0); // Since it is assigned value
	}
	else
	{
		// If it is NULL then we have already initialized 
		if(m_pModel->m_pExecutionContextForGatherInfo)
		{
			pExecutionContext = m_pModel->m_pExecutionContextForGatherInfo;
		}
		else
		{
			pExecutionContext = m_pModel->m_pDataProxy->GetExecutionContext(nIndex);
		}
		if(!pExecutionContext)
		{	
			daeDeclareException(exInvalidPointer); 
			e << "Cannot find ExecutionContext for variable: " << m_strCanonicalName << ", index: " << nIndex;
			throw e;
		}

		if(pExecutionContext->m_eEquationCalculationMode == eCalculate)
		{
			tmp.setValue(GetValueAt(nIndex));
			tmp.setDerivative(0); // No need for it
		}
		else if(pExecutionContext->m_eEquationCalculationMode == eCalculateJacobian)
		{
			tmp.setValue(GetValueAt(nIndex));
			tmp.setDerivative(GetADValueAt(nIndex));
		}
		else if(pExecutionContext->m_eEquationCalculationMode == eGatherInfo)
		{
		// Here we dont need any calculation
			//tmp.setValue(0);
			//tmp.setDerivative(0);

			daeEquationExecutionInfo* pEquationExecutionInfo = pExecutionContext->m_pEquationExecutionInfo;
			if(!pEquationExecutionInfo)
				daeDeclareAndThrowException(exInvalidPointer); 
			pEquationExecutionInfo->AddVariableInEquation(nIndex);
		}
		else if(pExecutionContext->m_eEquationCalculationMode == eCreateFunctionsIFsSTNs)
		{
		// Here we dont need any calculation
			//tmp.setValue(0);
			//tmp.setDerivative(0);
		}
		else if(pExecutionContext->m_eEquationCalculationMode == eCalculateSensitivities)
		{
			daeDeclareAndThrowException(exInvalidCall)
		}
		else if(pExecutionContext->m_eEquationCalculationMode == eCalculateGradients)
		{
			daeDeclareAndThrowException(exInvalidCall)
		}
		else
		{
			daeDeclareException(exMiscellanous); 
			e << "Unknown function evaluation mode";
			throw e;
		}
	}
*/

	if(m_pModel->m_pDataProxy->GetGatherInfo())
	{
		adRuntimeVariableNode* node = new adRuntimeVariableNode();
		node->m_pVariable     = const_cast<daeVariable*>(this);
		node->m_nOverallIndex = nIndex;
		node->m_pdValue       = m_pModel->m_pDataProxy->GetValue(nIndex);
		if(N > 0)
		{
			node->m_narrDomains.resize(N);
			for(size_t i = 0; i < N; i++)
				node->m_narrDomains[i] = indexes[i];
		}
		tmp.node = shared_ptr<adNode>(node);
		tmp.setGatherInfo(true);
	}
	return tmp;
}

adouble daeVariable::CreateSetupVariable(const daeDomainIndex* indexes, const size_t N) const
{
	if(m_ptrDomains.size() != N)
	{	
		daeDeclareException(exInvalidCall); 
		e << "Invalid get value call for [" << m_strCanonicalName << "], number of domains is " << m_ptrDomains.size() << " - but only " << N << " is given";
		throw e;
	}
	if(!m_pModel)
	{	
		daeDeclareException(exInvalidPointer); 
		e << "Invalid parent model in variable [" << m_strCanonicalName << "]";
		throw e;
	}
// Check if domains in indexes correspond to domains here
	for(size_t i = 0; i < N; i++)
	{
		if(indexes[i].m_eType == eDomainIterator ||
		   indexes[i].m_eType == eIncrementedDomainIterator)
		{
			if(m_ptrDomains[i] != indexes[i].m_pDEDI->m_pDomain)
			{
				daeDeclareException(exInvalidCall);
				e << "You cannot create daeDomainIndex with the domain [" << indexes[i].m_pDEDI->m_pDomain->GetCanonicalName() 
				  << "]; you must use domain [" << m_ptrDomains[i]->GetCanonicalName() << "] as " << i+1 << ". index argument "
				  << "in variable [" << m_strCanonicalName << "] in operator()";
				throw e;
			}
		}
	}

	adouble tmp;
	adSetupVariableNode* node = new adSetupVariableNode();
	node->m_pVariable = const_cast<daeVariable*>(this);

	if(N > 0)
	{
		node->m_arrDomains.resize(N);
		for(size_t i = 0; i < N; i++)
			node->m_arrDomains[i] = indexes[i];
	}
	tmp.node = shared_ptr<adNode>(node);
	tmp.setGatherInfo(true);
	return tmp;
}

adouble daeVariable::Calculate_dt(const size_t* indexes, const size_t N) const
{
	adouble tmp;
	size_t nIndex;
	daeExecutionContext* pExecutionContext;

	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer); 
	if(!m_pModel->m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer); 

	nIndex = m_nOverallIndex + CalculateIndex(indexes, N);

	if(m_pModel->m_pDataProxy->GetVariableType(nIndex) == cnFixed)
	{	
		daeDeclareException(exInvalidCall); 
		e << "Differential variable [" << m_strCanonicalName << "] cannot be fixed";
		throw e;
	}

/*********************************************************************************************
  New code; just I need is to set variable type and to add variable indexes to the equation
**********************************************************************************************/
	if(m_pModel->m_pExecutionContextForGatherInfo)
	{
		pExecutionContext = m_pModel->m_pExecutionContextForGatherInfo;
		if(!pExecutionContext)
			daeDeclareAndThrowException(exInvalidPointer); 

		m_pModel->m_pDataProxy->SetVariableTypeGathered(nIndex, cnDifferential);
		
		daeEquationExecutionInfo* pEquationExecutionInfo = pExecutionContext->m_pEquationExecutionInfo;
	// StateTransitions::CreateRuntimeNode call may end up here 
	// In that case we dont have pEquationExecutionInfo nor we need it
		if(pEquationExecutionInfo)
		{
			if(pExecutionContext->m_eEquationCalculationMode == eGatherInfo)
			{
				pEquationExecutionInfo->AddVariableInEquation(nIndex);
			}
			else
			{
			// If the mode is eGatherInfo I dont have to check whether variable is differential 
			// since I dont know the variable types yet! Otherwise I do the check.
				if(m_pModel->m_pDataProxy->GetVariableTypeGathered(nIndex) != cnDifferential)
				{	
					daeDeclareException(exInvalidCall); 
					e << "Cannot get time derivative for non differential variable [" << m_strCanonicalName;
					throw e;
				}
			}
		}
	}
	
/**************************************************************************************
  Old code; all this seems non necessary (we dont need values in this function)
***************************************************************************************   
	if(m_pModel->m_pExecutionContextForGatherInfo)
		pExecutionContext = m_pModel->m_pExecutionContextForGatherInfo;
	else
		pExecutionContext = m_pModel->m_pDataProxy->GetExecutionContext(nIndex);
	if(!pExecutionContext)
		daeDeclareAndThrowException(exInvalidPointer); 

	// If the mode is eGatherInfo I dont have to check whether variable is differential 
	// since I dont know the variable types yet !!
	if(pExecutionContext->m_eEquationCalculationMode != eGatherInfo)
	{
		if(m_pModel->m_pDataProxy->GetVariableTypeGathered(nIndex) != cnDifferential)
		{	
			daeDeclareException(exInvalidCall); 
			e << "Cannot get time derivative for non differential variable [" << m_strCanonicalName;
			throw e;
		}
	}

	if(pExecutionContext->m_eEquationCalculationMode == eGatherInfo)
	{
		daeEquationExecutionInfo* pEquationExecutionInfo = pExecutionContext->m_pEquationExecutionInfo;
		if(!pEquationExecutionInfo)
			daeDeclareAndThrowException(exInvalidPointer); 

		m_pModel->m_pDataProxy->SetVariableTypeGathered(nIndex, cnDifferential);
		pEquationExecutionInfo->AddVariableInEquation(nIndex);
	// Here we dont need any calculation
		//tmp.setValue(0);
		//tmp.setDerivative(0);
	}
	else if(pExecutionContext->m_eEquationCalculationMode == eCalculateJacobian)
	{
		tmp.setValue(*m_pModel->m_pDataProxy->GetTimeDerivative(nIndex));
		if(pExecutionContext->m_nCurrentVariableIndexForJacobianEvaluation == nIndex)
			tmp.setDerivative(pExecutionContext->m_dInverseTimeStep);
		else
			tmp.setDerivative(0);
	}
	else if(pExecutionContext->m_eEquationCalculationMode == eCalculate)
	{
		tmp.setValue(*m_pModel->m_pDataProxy->GetTimeDerivative(nIndex));
		tmp.setDerivative(0); // No need for it
	}
	else if(pExecutionContext->m_eEquationCalculationMode == eCreateFunctionsIFsSTNs)
	{
	// Here we dont need any calculation
		//tmp.setValue(0);
		//tmp.setDerivative(0);
	}
	else if(pExecutionContext->m_eEquationCalculationMode == eCalculateSensitivities)
	{
		daeDeclareAndThrowException(exInvalidCall) 
	}
	else if(pExecutionContext->m_eEquationCalculationMode == eCalculateGradients)
	{
		daeDeclareAndThrowException(exInvalidCall)
	}
	else
	{
		// Unknown state
		daeDeclareException(exInvalidCall);
		e << "Unknown function evaluation mode for variable [" << m_strCanonicalName;
		throw e;
	}
*/
	
	if(m_pModel->m_pDataProxy->GetGatherInfo())
	{
		adRuntimeTimeDerivativeNode* node = new adRuntimeTimeDerivativeNode();
		node->m_pVariable = const_cast<daeVariable*>(this);
		node->m_nOverallIndex = nIndex;
		node->m_nDegree = 1;
		node->m_pdTimeDerivative = m_pModel->m_pDataProxy->GetTimeDerivative(nIndex);
		if(N > 0)
		{
			node->m_narrDomains.resize(N);
			for(size_t i = 0; i < N; i++)
				node->m_narrDomains[i] = indexes[i];
		}
		tmp.node = shared_ptr<adNode>(node);
		tmp.setGatherInfo(true);
	}
	return tmp;
}

adouble daeVariable::CreateSetupTimeDerivative(const daeDomainIndex* indexes, const size_t N) const
{
	if(m_ptrDomains.size() != N)
	{	
		daeDeclareException(exInvalidCall); 
		e << "Invalid time derivative call for [" << m_strCanonicalName << "], number of domains is " << m_ptrDomains.size() << " - but only " << N << " is given";
		throw e;
	}
	if(!m_pModel)
	{	
		daeDeclareException(exInvalidPointer); 
		e << "Invalid parent model in variable [" << m_strCanonicalName << "]";
		throw e;
	}
// Check if domains in indexes correspond to domains here
	for(size_t i = 0; i < N; i++)
	{
		if(indexes[i].m_eType == eDomainIterator ||
		   indexes[i].m_eType == eIncrementedDomainIterator)
		{
			if(m_ptrDomains[i] != indexes[i].m_pDEDI->m_pDomain)
			{
				daeDeclareException(exInvalidCall);
				e << "You cannot create daeDomainIndex with the domain [" << indexes[i].m_pDEDI->m_pDomain->GetCanonicalName() 
				  << "]; you must use domain [" << m_ptrDomains[i]->GetCanonicalName() << "] as " << i+1 << ". index argument "
				  << "in variable [" << m_strCanonicalName << "] in function dt()";
				throw e;
			}
		}
	}

	adouble tmp;
	adSetupTimeDerivativeNode* node = new adSetupTimeDerivativeNode();
	node->m_pVariable = const_cast<daeVariable*>(this);
	node->m_nDegree = 1;

	if(N > 0)
	{
		node->m_arrDomains.resize(N);
		for(size_t i = 0; i < N; i++)
			node->m_arrDomains[i] = indexes[i];
	}
	tmp.node = shared_ptr<adNode>(node);
	tmp.setGatherInfo(true);
	return tmp;
}

adouble daeVariable::partial(const size_t nOrder, const daeDomain_t& D, const size_t* indexes, const size_t N) const
{
	adouble tmp;
	vector<adouble> V;
	size_t i, nNoPoints, nFixedDomain, nIndex;

	if(!m_pModel)
		daeDeclareAndThrowException(exInvalidPointer);
	if(!m_pModel->m_pDataProxy)
		daeDeclareAndThrowException(exInvalidPointer);
	if(m_ptrDomains.size() == 0)
	{	
		daeDeclareException(exInvalidCall);
		e << "Cannot get partial derivative for non distributed variable [" << m_strCanonicalName;
		throw e;
	}
	if(N != m_ptrDomains.size())
	{
		daeDeclareException(exInvalidCall);
		e << "Illegal number of domains, variable " << m_strCanonicalName;
		throw e;
	}
	if(D.GetType() != eDistributed)
	{	
		daeDeclareException(exInvalidCall);
		e << "Cannot get partial derivative for non-distributed domain [" << D.GetCanonicalName() << "], variable [" << m_strCanonicalName << "]";
		throw e;
	}

// Find which domain is fixed
	nFixedDomain = ULONG_MAX;
	for(i = 0; i < m_ptrDomains.size(); i++)
	{
		if(m_ptrDomains[i] == &D)
			nFixedDomain = i;
	}
	if(nFixedDomain == ULONG_MAX)
	{	
		daeDeclareException(exInvalidCall);
		e << "Illegal domain for partial derivative, variable [" << m_strCanonicalName << "]";
		throw e;
	}

// Find index of the point for which we are trying to find derivative
	nIndex    = indexes[nFixedDomain];
	nNoPoints = D.GetNumberOfPoints();

	const daeDomain* pDomain = dynamic_cast<const daeDomain*>(&D);
	if(!pDomain)
		daeDeclareAndThrowException(exInvalidPointer);

	daePartialDerivativeVariable pdv(nOrder, *this, *pDomain, nFixedDomain, N, indexes);
	tmp = pDomain->partial(pdv);

	if(m_pModel->m_pDataProxy->GetGatherInfo())
	{
		adRuntimePartialDerivativeNode* node = new adRuntimePartialDerivativeNode();
		node->pardevnode = tmp.node;
		node->m_nOverallIndex = nIndex;
		const daeDomain* pDomain = dynamic_cast<const daeDomain*>(&D);
		node->m_pDomain = const_cast<daeDomain*>(pDomain);
		node->m_pVariable = const_cast<daeVariable*>(this);
		node->m_nDegree = nOrder;
		if(N > 0)
		{
			node->m_narrDomains.resize(N);
			for(size_t i = 0; i < N; i++)
				node->m_narrDomains[i] = indexes[i];
		}
		tmp.node = shared_ptr<adNode>(node);
		tmp.setGatherInfo(true);
	}
	return tmp;
}

adouble daeVariable::CreateSetupPartialDerivative(const size_t nOrder, const daeDomain_t& D, const daeDomainIndex* indexes, const size_t N) const
{
	if(m_ptrDomains.size() != N)
	{	
		daeDeclareException(exInvalidCall); 
		e << "Invalid partial derivative call for [" << m_strCanonicalName << "], number of domains is " << m_ptrDomains.size() << " - but only " << N << " is given";
		throw e;
	}
	if(!m_pModel)
	{	
		daeDeclareException(exInvalidPointer); 
		e << "Invalid parent model in variable [" << m_strCanonicalName << "]";
		throw e;
	}
// Check if domains in indexes correspond to domains here
	for(size_t i = 0; i < N; i++)
	{
		if(indexes[i].m_eType == eDomainIterator ||
		   indexes[i].m_eType == eIncrementedDomainIterator)
		{
			if(m_ptrDomains[i] != indexes[i].m_pDEDI->m_pDomain)
			{
				daeDeclareException(exInvalidCall);
				e << "You cannot create daeDomainIndex with the domain [" << indexes[i].m_pDEDI->m_pDomain->GetCanonicalName() 
				  << "]; you must use domain [" << m_ptrDomains[i]->GetCanonicalName() << "] as " << i+1 << ". index argument " 
				  << "in variable [" << m_strCanonicalName << "] in function d/d2()";
				throw e;
			}
		}
	}

	adouble tmp;
	adSetupPartialDerivativeNode* node = new adSetupPartialDerivativeNode();
	const daeDomain* pDomain = dynamic_cast<const daeDomain*>(&D);
	node->m_pDomain = const_cast<daeDomain*>(pDomain);
	node->m_pVariable = const_cast<daeVariable*>(this);
	node->m_nDegree = nOrder;

	if(N > 0)
	{
		node->m_arrDomains.resize(N);
		for(size_t i = 0; i < N; i++)
			node->m_arrDomains[i] = indexes[i];
	}
	tmp.node = shared_ptr<adNode>(node);
	tmp.setGatherInfo(true);
	return tmp;
}


}
}
