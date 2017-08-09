#include "stdafx.h"
#include "coreimpl.h"
#include "adouble.h"
#include <typeinfo>
#include "nodes_array.h"
using namespace boost;

namespace dae
{
namespace core
{
#define adCheckArrays(left, right)	if((left).GetSize() == 0 || (right).GetSize() == 0)   \
{ \
    daeDeclareAndThrowException(exInvalidCall);   \
} \
    if((left).GetSize() > 1 && (right).GetSize() > 1) \
{ \
    if((left).GetSize() != (right).GetSize()) \
    daeDeclareAndThrowException(exInvalidCall); \
}

/*********************************************************************************************
 adouble_array
**********************************************************************************************/
adouble_array::adouble_array(bool gatherInfo/* = false*/,
                             adNodeArray* node_/* = NULL*/)
{
    m_bGatherInfo = gatherInfo;

    if(gatherInfo)
    {
        if(node_)
            node = adNodeArrayPtr(node_->Clone());
    }
}

adouble_array::adouble_array(const adouble_array& a)
{
    m_bGatherInfo = false;
    if(a.getGatherInfo())
    {
        m_bGatherInfo = true;
        node  = CLONE_NODE_ARRAY(a.node);
    }
    m_arrValues = a.m_arrValues;
}

adouble_array::~adouble_array()
{
}

size_t adouble_array::GetSize(void) const
{
    return m_arrValues.size();
}

void adouble_array::Resize(size_t n)
{
    m_arrValues.clear();
    m_arrValues.resize(n);
}

std::vector<adouble>::iterator adouble_array::begin()
{
    return m_arrValues.begin();
}

std::vector<adouble>::iterator adouble_array::end()
{
    return m_arrValues.end();
}

void adouble_array::operator =(const adouble_array& a)
{
    if(this->m_bGatherInfo || a.getGatherInfo())
    {
        this->m_bGatherInfo = true;
        this->node = CLONE_NODE_ARRAY(a.node);
    }
    m_arrValues = a.m_arrValues;
}

adouble& adouble_array::operator[](size_t nIndex)
{
    // This enables operations on an array which has only one element,
    // for instance single variables, parameters and constants
    // They always return its single value (for arbitrary index)
    if(m_arrValues.size() == 1)
    {
        return m_arrValues[0];
    }
    else
    {
        if(nIndex >= m_arrValues.size())
        {
            daeDeclareException(exInvalidCall);
            e << "Invalid index [" << nIndex << "] in adouble_array::operator[] call; adouble_array size is " << GetSize();
            throw e;
        }
        return m_arrValues[nIndex];
    }
}

const adouble& adouble_array::operator[](size_t nIndex) const
{
    // This enables operations on an array which has only one element,
    // for instance single variables, parameters and constants
    // They always return its single value (for arbitrary index)
    if(m_arrValues.size() == 1)
    {
        return m_arrValues[0];
    }
    else
    {
        if(nIndex >= m_arrValues.size())
        {
            daeDeclareException(exInvalidCall);
            e << "Invalid index [" << nIndex << "] in adouble_array::operator[] call; adouble_array size is " << GetSize();
            throw e;
        }

        return m_arrValues[nIndex];
    }
}

adouble adouble_array::operator()(const daeDomainIndex& domainIndex)
{
    adouble tmp;
    tmp.setGatherInfo(true);
    tmp.node = adNodePtr(new adSetupValueInArrayAtIndexNode(domainIndex, CLONE_NODE_ARRAY(node)));
    return tmp;
}

adouble adouble_array::GetItem(size_t nIndex)
{
    // This enables operations on an array which has only one element,
    // for instance single variables, parameters and constants
    // They always return its single value (for arbitrary index)
    if(m_arrValues.size() == 1)
    {
        return m_arrValues[0];
    }
    else
    {
        if(nIndex >= m_arrValues.size())
        {
            daeDeclareException(exInvalidCall);
            e << "Invalid index [" << nIndex << "] in adouble_array::GetItem call; adouble_array size is " << GetSize();
            throw e;
        }

        return m_arrValues[nIndex];
    }
}

void adouble_array::SetItem(size_t nIndex, adouble& a)
{
    if(nIndex >= m_arrValues.size())
    {
        daeDeclareException(exInvalidCall);
        e << "Invalid index [" << nIndex << "] in adouble_array::SetItem call; adouble_array size is " << GetSize();
        throw e;
    }

    m_arrValues[nIndex] = a;
}

// In all theses functions +, -, *, / we do not need to actually calculate anything,
// just to create tmp.node pointer (adNodeArrayPtr). All calculation is done in
// adUnaryNodeArray and adBinaryNodeArray::evaluate() function.
// The calculation part is left just in case.

adouble_array adouble_array::operator -(void) const
{
    adouble_array tmp;
    if(getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eSign,
                                                       CLONE_NODE_ARRAY(node) ));
        return tmp;
    }

    size_t n = GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = -(*this)[i];
    return tmp;
}

adouble_array adouble_array::operator +(const adouble_array& a) const
{
    adouble_array tmp;
    if(getGatherInfo() || a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(ePlus,
                                                        CLONE_NODE_ARRAY(node),
                                                        CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    adCheckArrays(*this, a);

    size_t n = (GetSize() > a.GetSize() ? GetSize() : a.GetSize());
    //std::cout << "+ n = " << n << "; lsize = " << GetSize() << "; rsize = " << a.GetSize() << std::endl;

    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] + a[i];
    return tmp;
}

adouble_array adouble_array::operator +(const real_t v) const
{
    adouble_array tmp;
    if(getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(ePlus,
                                                        CLONE_NODE_ARRAY(node),
                                                        adNodeArrayPtr(new adConstantNodeArray(v, UNITS(node))) ));
        return tmp;
    }

    size_t n = GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] + v;
    return tmp;
}

adouble_array adouble_array::operator +(const adouble& a) const
{
    adouble_array tmp;
    if(getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(ePlus,
                                                        CLONE_NODE_ARRAY(node),
                                                        adNodeArrayPtr(new adSingleNodeArray(CLONE_NODE(a.node, a.getValue())) ) ));
        return tmp;
    }

    size_t n = GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] + a;
    return tmp;
}

adouble_array operator +(const adouble& a, const adouble_array& arr)
{
    adouble_array tmp;
    if(arr.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(ePlus,
                                                        adNodeArrayPtr(new adSingleNodeArray(CLONE_NODE(a.node, a.getValue())) ),
                                                        CLONE_NODE_ARRAY(arr.node)
                                                        ));
        return tmp;
    }

    size_t n = arr.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = a + arr[i];
    return tmp;
}

adouble_array operator +(const real_t v, const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(ePlus,
                                                        adNodeArrayPtr(new adConstantNodeArray(v, UNITS(a.node))),
                                                        CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = v + a[i];
    return tmp;
}

adouble_array adouble_array::operator -(const adouble_array& a) const
{
    adouble_array tmp;
    if(getGatherInfo() || a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eMinus,
                                                        CLONE_NODE_ARRAY(node),
                                                        CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    adCheckArrays(*this, a);

    size_t n = (GetSize() > a.GetSize() ? GetSize() : a.GetSize());
    //std::cout << "- n = " << n << "; lsize = " << GetSize() << "; rsize = " << a.GetSize() << std::endl;

    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] - a[i];
    return tmp;
}

adouble_array adouble_array::operator -(const real_t v) const
{
    adouble_array tmp;
    if(getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eMinus,
                                                        CLONE_NODE_ARRAY(node),
                                                        adNodeArrayPtr(new adConstantNodeArray(v, UNITS(node))) ));
        return tmp;
    }

    size_t n = GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] - v;
    return tmp;
}

adouble_array adouble_array::operator -(const adouble& a) const
{
    adouble_array tmp;
    if(getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eMinus,
                                                        CLONE_NODE_ARRAY(node),
                                                        adNodeArrayPtr(new adSingleNodeArray(CLONE_NODE(a.node, a.getValue())) ) ));
        return tmp;
    }

    size_t n = GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] - a;
    return tmp;
}

adouble_array operator -(const adouble& a, const adouble_array& arr)
{
    adouble_array tmp;
    if(arr.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eMinus,
                                                        adNodeArrayPtr(new adSingleNodeArray(CLONE_NODE(a.node, a.getValue())) ),
                                                        CLONE_NODE_ARRAY(arr.node)
                                                        ));
        return tmp;
    }

    size_t n = arr.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = a - arr[i];
    return tmp;
}

adouble_array operator -(const real_t v, const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eMinus,
                                                        adNodeArrayPtr(new adConstantNodeArray(v, UNITS(a.node))),
                                                        CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = v - a[i];
    return tmp;
}

adouble_array adouble_array::operator *(const adouble_array& a) const
{
    adouble_array tmp;
    if(getGatherInfo() || a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eMulti,
                                                        CLONE_NODE_ARRAY(node),
                                                        CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    adCheckArrays(*this, a);

    size_t n = (GetSize() > a.GetSize() ? GetSize() : a.GetSize());
    //std::cout << "* n = " << n << "; lsize = " << GetSize() << "; rsize = " << a.GetSize() << std::endl;

    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] * a[i];
    return tmp;
}

adouble_array adouble_array::operator *(const real_t v) const
{
    adouble_array tmp;
    if(getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eMulti,
                                                        CLONE_NODE_ARRAY(node),
                                                        adNodeArrayPtr(new adConstantNodeArray(v, UNITS(node))) ));
        return tmp;
    }

    size_t n = GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] * v;
    return tmp;
}

adouble_array adouble_array::operator *(const adouble& a) const
{
    adouble_array tmp;
    if(getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eMulti,
                                                        CLONE_NODE_ARRAY(node),
                                                        adNodeArrayPtr(new adSingleNodeArray(CLONE_NODE(a.node, a.getValue())) ) ));
        return tmp;
    }

    size_t n = GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] * a;
    return tmp;
}

adouble_array operator *(const adouble& a, const adouble_array& arr)
{
    adouble_array tmp;
    if(arr.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eMulti,
                                                        adNodeArrayPtr(new adSingleNodeArray(CLONE_NODE(a.node, a.getValue())) ),
                                                        CLONE_NODE_ARRAY(arr.node)
                                                        ));
        return tmp;
    }

    size_t n = arr.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = a * arr[i];
    return tmp;
}

adouble_array operator *(const real_t v, const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eMulti,
                                                        adNodeArrayPtr(new adConstantNodeArray(v, UNITS(a.node))),
                                                        CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = v * a[i];
    return tmp;
}

adouble_array adouble_array::operator /(const adouble_array& a) const
{
    adouble_array tmp;
    if(getGatherInfo() || a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eDivide,
                                                        CLONE_NODE_ARRAY(node),
                                                        CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    adCheckArrays(*this, a);

    size_t n = (GetSize() > a.GetSize() ? GetSize() : a.GetSize());
    //std::cout << "/ n = " << n << "; lsize = " << GetSize() << "; rsize = " << a.GetSize() << std::endl;

    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] / a[i];
    return tmp;
}

adouble_array adouble_array::operator /(const real_t v) const
{
    adouble_array tmp;
    if(getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eDivide,
                                                        CLONE_NODE_ARRAY(node),
                                                        adNodeArrayPtr(new adConstantNodeArray(v, UNITS(node))) ));
        return tmp;
    }

    size_t n = GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] / v;
    return tmp;
}

adouble_array adouble_array::operator /(const adouble& a) const
{
    adouble_array tmp;
    if(getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eDivide,
                                                        CLONE_NODE_ARRAY(node),
                                                        adNodeArrayPtr(new adSingleNodeArray(CLONE_NODE(a.node, a.getValue())) ) ));
        return tmp;
    }

    size_t n = GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = (*this)[i] / a;
    return tmp;
}

adouble_array operator /(const adouble& a, const adouble_array& arr)
{
    adouble_array tmp;
    if(arr.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eDivide,
                                                        adNodeArrayPtr(new adSingleNodeArray(CLONE_NODE(a.node, a.getValue())) ),
                                                        CLONE_NODE_ARRAY(arr.node)
                                                        ));
        return tmp;
    }

    size_t n = arr.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = a / arr[i];
    return tmp;
}

adouble_array operator /(const real_t v, const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eDivide,
                                                        adNodeArrayPtr(new adConstantNodeArray(v, UNITS(a.node))),
                                                        CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = v / a[i];
    return tmp;
}

/*********************************************************************************************
  d_array, d2_array, dt_array
**********************************************************************************************/
adouble_array dt_array(adouble_array& adarr)
{
    adouble_array tmp;
    tmp.setGatherInfo(true);

    adNodeArrayPtr a = adarr.node;
    if( dynamic_cast<adSetupVariableNodeArray*>(a.get()) )
    {
        adSetupVariableNodeArray* node_array = dynamic_cast<adSetupVariableNodeArray*>(a.get());
        tmp.node = adNodeArrayPtr(new adSetupTimeDerivativeNodeArray(node_array->m_pVariable,
                                                                     1,
                                                                     node_array->m_arrRanges));
    }
    else
    {
        daeDeclareException(exNotImplemented);
        e << "At the moment, dt_array function accepts only variable array arguments only";
        throw e;
    }

    return tmp;
}

adouble_array d_array(adouble_array&                            adarr,
                            daeDomain&                                domain,
                            daeeDiscretizationMethod                  eDiscretizationMethod,
                            const std::map<std::string, std::string>& mapDiscretizationOptions)
{
    adouble_array tmp;
    tmp.setGatherInfo(true);

    adNodeArrayPtr a = adarr.node;
    if( dynamic_cast<adSetupVariableNodeArray*>(a.get()) )
    {
        adSetupVariableNodeArray* node_array = dynamic_cast<adSetupVariableNodeArray*>(a.get());
        tmp.node = adNodeArrayPtr(new adSetupPartialDerivativeNodeArray(node_array->m_pVariable,
                                                                        1,
                                                                        node_array->m_arrRanges,
                                                                        &domain,
                                                                        eDiscretizationMethod,
                                                                        mapDiscretizationOptions));
    }
    else
    {
        daeDeclareException(exNotImplemented);
        e << "At the moment, dt_array function accepts only variable array arguments only";
        throw e;
    }

    return tmp;

/* Old, not working code
    tmp.node = adNodeArrayPtr(new adSetupExpressionPartialDerivativeNodeArray(&domain,
                                                                              1,
                                                                              eDiscretizationMethod,
                                                                              mapDiscretizationOptions,
                                                                              adarr.node));
*/
}

adouble_array d2_array(adouble_array&                            adarr,
                             daeDomain&                                domain,
                             daeeDiscretizationMethod                  eDiscretizationMethod,
                             const std::map<std::string, std::string>& mapDiscretizationOptions)
{
    adouble_array tmp;
    tmp.setGatherInfo(true);

    adNodeArrayPtr a = adarr.node;
    if( dynamic_cast<adSetupVariableNodeArray*>(a.get()) )
    {
        adSetupVariableNodeArray* node_array = dynamic_cast<adSetupVariableNodeArray*>(a.get());
        tmp.node = adNodeArrayPtr(new adSetupPartialDerivativeNodeArray(node_array->m_pVariable,
                                                                        2,
                                                                        node_array->m_arrRanges,
                                                                        &domain,
                                                                        eDiscretizationMethod,
                                                                        mapDiscretizationOptions));
    }
    else
    {
        daeDeclareException(exNotImplemented);
        e << "At the moment, dt_array function accepts only variable array arguments only";
        throw e;
    }

    return tmp;

/* Old, not working code
    tmp.node = adNodeArrayPtr(new adSetupExpressionPartialDerivativeNodeArray(&domain,
                                                                              2,
                                                                              eDiscretizationMethod,
                                                                              mapDiscretizationOptions,
                                                                              adarr.node));
*/
}
/*********************************************************************************************
 sum, product, integral, min, max, average
**********************************************************************************************/
// If called during declaring of equations then GatherInfo is true
// In other cases will calculate a value of the function
adouble Average(const adouble_array& a)
{
    adouble tmp;

    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodePtr(new adSetupSpecialFunctionNode(eAverage,
                                                            CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    tmp = a[0];
    for(size_t i = 1; i < a.GetSize(); i++)
        tmp = tmp + a[i];
    tmp = tmp / a.m_arrValues.size();

    return tmp;
}

adouble Sum(const adouble_array& a, bool bIsLargeArray)
{
    adouble tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodePtr(new adSetupSpecialFunctionNode(eSum,
                                                            CLONE_NODE_ARRAY(a.node),
                                                            bIsLargeArray)
                             );
        return tmp;
    }

    tmp = a[0];
    for(size_t i = 1; i < a.GetSize(); i++)
        tmp = tmp + a[i];

    return tmp;
}

adouble Product(const adouble_array& a, bool bIsLargeArray)
{
    adouble tmp;

    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodePtr(new adSetupSpecialFunctionNode(eProduct,
                                                            CLONE_NODE_ARRAY(a.node),
                                                            bIsLargeArray)
                            );
        return tmp;
    }

    tmp = a[0];
    for(size_t i = 1; i < a.GetSize(); i++)
        tmp = tmp * a[i];
    return tmp;
}

adouble Integral(const adouble_array& a)
{
    adouble tmp;
    size_t i, nCount;
    daeArrayRange range;
    vector<daeArrayRange> arrRanges;
    daeDomain* pDomain = NULL;

    if(!a.getGatherInfo())
        daeDeclareAndThrowException(exInvalidCall);

    adNodeArrayImpl* n = dynamic_cast<adNodeArrayImpl*>(a.node.get());
    if(!n)
        daeDeclareAndThrowException(exInvalidPointer);
    n->GetArrayRanges(arrRanges);

    nCount = 0;
    for(i = 0; i < arrRanges.size(); i++)
        if(arrRanges[i].m_eType == eRange)
            nCount++;

    if(nCount != 1)
    {
        daeDeclareException(exInvalidCall);
        e << "At the moment, it is possible to calculate one dimensional integrals only";
        throw e;
    }

    for(i = 0; i < arrRanges.size(); i++)
    {
        if(arrRanges[i].m_eType == eRange)
        {
            range = arrRanges[i];
            pDomain = range.m_Range.m_pDomain;
            break;
        }
    }

    tmp.setGatherInfo(true);
    adSetupIntegralNode* node = new adSetupIntegralNode(eSingleIntegral,
                                                        CLONE_NODE_ARRAY(a.node),
                                                        pDomain,
                                                        range);
    tmp.node = adNodePtr(node);
    return tmp;
}

adouble Min(const adouble_array& a)
{
    adouble tmp;

    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodePtr(new adSetupSpecialFunctionNode(eMinInArray,
                                                            CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    tmp = a[0];
    for(size_t i = 1; i < a.GetSize(); i++)
        tmp = dae::core::__min__(tmp, a[i]);
    return tmp;
}

adouble Max(const adouble_array& a)
{
    adouble tmp;

    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodePtr(new adSetupSpecialFunctionNode(eMaxInArray,
                                                            CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    tmp = a[0];
    for(size_t i = 1; i < a.GetSize(); i++)
        tmp = dae::core::__max__(tmp, a[i]);
    return tmp;
}

adouble_array exp(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eExp,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = exp(a[i]);
    return tmp;
}

adouble_array sqrt(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eSqrt,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = sqrt(a[i]);
    return tmp;
}

adouble_array log(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eLn,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = log(a[i]);
    return tmp;
}

adouble_array log10(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eLog,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = log10(a[i]);
    return tmp;
}

adouble_array abs(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eAbs,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = abs(a[i]);
    return tmp;
}

adouble_array floor(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eFloor,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = floor(a[i]);
    return tmp;
}

adouble_array ceil(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eCeil,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = ceil(a[i]);
    return tmp;
}

adouble_array sin(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eSin,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = sin(a[i]);
    return tmp;
}

adouble_array cos(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eCos,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = cos(a[i]);
    return tmp;
}

adouble_array tan(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eTan,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = tan(a[i]);
    return tmp;
}

adouble_array asin(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eArcSin,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = asin(a[i]);
    return tmp;
}

adouble_array acos(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eArcCos,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = acos(a[i]);
    return tmp;
}

adouble_array atan(const adouble_array& a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eArcTan,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = atan(a[i]);
    return tmp;
}



adouble_array sinh(const adouble_array &a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eSinh,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = sinh(a[i]);
    return tmp;
}

adouble_array cosh(const adouble_array &a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eCosh,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = cosh(a[i]);
    return tmp;
}

adouble_array tanh(const adouble_array &a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eTanh,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = tanh(a[i]);
    return tmp;
}

adouble_array asinh(const adouble_array &a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eArcSinh,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = asinh(a[i]);
    return tmp;
}

adouble_array acosh(const adouble_array &a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eArcCosh,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = acosh(a[i]);
    return tmp;
}

adouble_array atanh(const adouble_array &a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eArcTanh,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = atanh(a[i]);
    return tmp;
}

adouble_array atan2(const adouble_array &a, const adouble_array &b)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adBinaryNodeArray(eArcTan2,
                                                        CLONE_NODE_ARRAY(a.node),
                                                        CLONE_NODE_ARRAY(b.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = atan2(a[i], b[i]);
    return tmp;
}

adouble_array erf(const adouble_array &a)
{
    adouble_array tmp;
    if(a.getGatherInfo())
    {
        tmp.setGatherInfo(true);
        tmp.node = adNodeArrayPtr(new adUnaryNodeArray(eErf,
                                                       CLONE_NODE_ARRAY(a.node) ));
        return tmp;
    }

    size_t n = a.GetSize();
    tmp.Resize(n);
    for(size_t i = 0; i < n; i++)
        tmp[i] = erf(a[i]);
    return tmp;
}





string adouble_array::NodeAsPlainText(void) const
{
    string strContent;
    daeModelExportContext c;

    c.m_pModel             = NULL;
    c.m_nPythonIndentLevel = 0;
    c.m_bExportDefinition  = true;

    if(node)
        node->Export(strContent, eCDAE, c);

    return strContent;
}

string adouble_array::NodeAsLatex(void) const
{
    daeNodeSaveAsContext c;
    c.m_pModel = NULL;

    if(node)
        return node->SaveAsLatex(&c);

    return string();
}

}
}
