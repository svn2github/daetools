#include "stdafx.h"
#include "coreimpl.h"

namespace dae 
{
namespace core 
{
/*********************************************************************************************
	daeVariableType
**********************************************************************************************/
daeVariableType::daeVariableType()
{

}

daeVariableType::daeVariableType(string strName,
								 string strUnits,
								 real_t dLowerBound,
								 real_t dUpperBound,
								 real_t dInitialGuess,
								 real_t dAbsoluteTolerance)
{
	m_dInitialGuess		  = dInitialGuess;
	m_dLowerBound		  = dLowerBound;
	m_dUpperBound		  = dUpperBound;
	m_strUnits            = strUnits;
	m_strName             = strName;
	m_dAbsoluteTolerance  = dAbsoluteTolerance;
}

daeVariableType::~daeVariableType()
{
}

real_t daeVariableType::GetLowerBound(void) const
{
	return m_dLowerBound;
}

void daeVariableType::SetLowerBound(real_t dValue)
{
	m_dLowerBound = dValue;
}

real_t daeVariableType::GetUpperBound(void) const
{
	return m_dUpperBound;
}

void daeVariableType::SetUpperBound(real_t dValue)
{
	m_dUpperBound = dValue;
}

real_t daeVariableType::GetInitialGuess(void) const
{
	return m_dInitialGuess;
}

void daeVariableType::SetInitialGuess(real_t dValue)
{
	m_dInitialGuess = dValue;
}

string daeVariableType::GetUnits(void) const
{
	return m_strUnits;
}

void daeVariableType::SetUnits(string strName)
{
	m_strUnits = strName;
}

string daeVariableType::GetName(void) const
{
	return m_strName;
}

void daeVariableType::SetName(string strName)
{
	m_strName = strName;
}

real_t daeVariableType::GetAbsoluteTolerance(void) const
{
	return m_dAbsoluteTolerance;
}

void daeVariableType::SetAbsoluteTolerance(real_t dTolerance)
{
	m_dAbsoluteTolerance = dTolerance;
}

void daeVariableType::Open(io::xmlTag_t* pTag)
{
	string strName;

	strName = "Name";
	pTag->Open(strName, m_strName);

	strName = "Units";
	pTag->Open(strName, m_strUnits);

	strName = "LowerBound";
	pTag->Open(strName, m_dLowerBound);

	strName = "UpperBound";
	pTag->Open(strName, m_dUpperBound);

	strName = "InitialGuess";
	pTag->Open(strName, m_dInitialGuess);

	strName = "AbsoluteTolerance";
	pTag->Open(strName, m_dAbsoluteTolerance);
}

void daeVariableType::Save(io::xmlTag_t* pTag) const
{
	string strName;

	strName = "Name";
	pTag->Save(strName, m_strName);

	strName = "Units";
	pTag->Save(strName, m_strUnits);

	strName = "LowerBound";
	pTag->Save(strName, m_dLowerBound);

	strName = "UpperBound";
	pTag->Save(strName, m_dUpperBound);

	strName = "InitialGuess";
	pTag->Save(strName, m_dInitialGuess);

	strName = "AbsoluteTolerance";
	pTag->Save(strName, m_dAbsoluteTolerance);
}


void daeVariableType::OpenRuntime(io::xmlTag_t* /*pTag*/)
{
}

void daeVariableType::SaveRuntime(io::xmlTag_t* /*pTag*/) const
{
}

bool daeVariableType::CheckObject(vector<string>& strarrErrors) const
{
	string strError;

	bool bCheck = true;

	if(m_strName.empty())
	{
		strError = "Invalid name in variable type";
		strarrErrors.push_back(strError);
		return false;
	}

	if(m_strUnits.empty())
	{
		strError = "Invalid units in variable type [" + m_strName + "]";
		strarrErrors.push_back(strError);
		bCheck = false;
	}

	if(m_dLowerBound >= m_dUpperBound)
	{
		strError = "Invalid bounds in variable type [" + m_strName + "]";
		strarrErrors.push_back(strError);
		bCheck = false;
	}

	if(m_dInitialGuess < m_dLowerBound || m_dInitialGuess > m_dUpperBound)
	{
		strError = "Invalid initial guess in variable type [" + m_strName + "]";
		strarrErrors.push_back(strError);
		bCheck = false;
	}

	if(m_dAbsoluteTolerance <= 0 || m_dAbsoluteTolerance > 1)
	{
		strError = "Invalid absolute tolerance in variable type [" + m_strName + "]";
		strarrErrors.push_back(strError);
		bCheck = false;
	}

	return bCheck;
}

}
}
