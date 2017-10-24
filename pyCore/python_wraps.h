#ifndef DAE_PYTHON_WRAPS_H
#define DAE_PYTHON_WRAPS_H

#if !defined(__MINGW32__) && (defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64))
#pragma warning(disable: 4250)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#endif

#if defined(__MACH__) || defined(__APPLE__)
#include <python.h>
#endif
#include <stdint.h>
#include <string>
#include <boost/python.hpp>
//#include <boost/python/numeric.hpp>
#include <boost/python/slice.hpp>
#include <boost/python/tuple.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/python/call_method.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include "../dae_develop.h"
#include "../Core/base_logging.h"
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include "../Core/tcpiplog.h"
#include "../Core/nodes.h"
#include "../Core/nodes_array.h"

namespace daepython
{
template<typename TYPE1, typename TYPE2>
std::map<TYPE1,TYPE2> getMapFromDict(boost::python::dict d)
{
    std::map<TYPE1,TYPE2> m;

    // Get the list of pairs key:value
    boost::python::stl_input_iterator<boost::python::tuple> iter(d.attr("items")()), end;

    for(; iter != end; iter++)
    {
        boost::python::tuple t = *iter;
        TYPE1 key = boost::python::extract<TYPE1>(t[0]);
        TYPE2 val = boost::python::extract<TYPE1>(t[1]);
        m[key] = val;
    }

    return m;
}

template<typename ITEM>
boost::python::dict getDictFromObjectArray(const std::vector<ITEM>& arrObjects)
{
    boost::python::dict d;

    for(size_t i = 0; i < arrObjects.size(); i++)
        d[ arrObjects[i]->GetName() ] = boost::ref(arrObjects[i]);

    return d;
}

template<typename ITEM>
boost::python::list getListFromVector(const std::vector<ITEM>& arrItems)
{
    boost::python::list l;

    for(size_t i = 0; i < arrItems.size(); i++)
        l.append(boost::ref(arrItems[i]));

    return l;
}

template<typename ITEM, typename CAST>
boost::python::list getListFromVectorAndCastPointer(const std::vector<ITEM>& arrItems)
{
    CAST pObject;
    boost::python::list l;

    for(size_t i = 0; i < arrItems.size(); i++)
    {
        pObject = dynamic_cast<CAST>(arrItems[i]);
        l.append(boost::ref(pObject));
    }
    return l;
}

template<typename ITEM>
boost::python::list getListFromVectorByValue(const std::vector<ITEM>& arrItems)
{
    boost::python::list l;

    for(size_t i = 0; i < arrItems.size(); i++)
        l.append(arrItems[i]);

    return l;
}

template<typename ITEM>
std::string getVector__repr__(const std::vector<ITEM*>& arrItems, std::string (*__repr__)(const ITEM&))
{
    std::string str = "[";
    for(size_t i = 0; i < arrItems.size(); i++)
        str += (i == 0 ? "" : ", ") + __repr__(*arrItems[i]);
    str += "]";

    return str;
}

template<typename ITEM, typename CAST>
std::string getVectorWithCast__repr__(const std::vector<ITEM*>& arrItems, std::string (*__repr__)(const CAST&))
{
    std::string str = "[";
    for(size_t i = 0; i < arrItems.size(); i++)
    {
        CAST* ptr = dynamic_cast<CAST*>(arrItems[i]);
        str += (i == 0 ? "" : ", ") + __repr__(*ptr);
    }
    str += "]";

    return str;
}

adouble       create_adouble_from_object(boost::python::object value);
adouble_array create_adouble_array_from_object(boost::python::object value);

boost::python::list daeArray_GetValues(daeArray<real_t>& self);

/*******************************************************
    String functions
*******************************************************/
string daeVariableType__repr__(const daeVariableType& self);
string daeDomainIndex__repr__(const daeDomainIndex& self);
string daeIndexRange__repr__(const daeIndexRange& self);
string daeArrayRange__str__(const daeArrayRange& self);
string daeArrayRange__repr__(const daeArrayRange& self);
string daeDomain__str__(const daeDomain& self);
string daeDomain__repr__(const daeDomain& self);
string daeParameter__str__(const daeParameter& self);
string daeParameter__repr__(const daeParameter& self);
string daeVariable__str__(const daeVariable& self);
string daeVariable__repr__(const daeVariable& self);
string daePort__str__(const daePort& self);
string daePort__repr__(const daePort& self);
string daeEventPort__str__(const daeEventPort &self);
string daeEventPort__repr__(const daeEventPort& self);
string daeModel__str__(const daeModel& self);
string daeModel__repr__(const daeModel& self);
string daeEquation__str__(const daeEquation& self);
string daeEquation__repr__(const daeEquation& self);
string daeSTN__str__(const daeSTN& self);
string daeSTN__repr__(const daeSTN& self);
string daeIF__str__(const daeIF& self);
string daeIF__repr__(const daeIF& self);
string daeState__str__(const daeState& self);
string daeState__repr__(const daeState& self);
string daeDEDI__str__(const daeDEDI& self);
string daeDEDI__repr__(const daeDEDI& self);
string daeCondition__str__(const daeCondition& self);
string daeCondition__repr__(const daeCondition& self);
string adouble__str__(const adouble& self);
string adouble__repr__(const adouble& self);
string adNode__str__(const adNode& self);
string adNode__repr__(const adNode& self);
string adouble_array__str__(const adouble_array& self);
string adouble_array__repr__(const adouble_array& self);

string daeOptimizationVariable__str__(const daeOptimizationVariable& self);
string daeOptimizationVariable__repr__(const daeOptimizationVariable& self);

string daeObjectiveFunction__str__(const daeObjectiveFunction& self);
string daeObjectiveFunction__repr__(const daeObjectiveFunction& self);

string daeOptimizationConstraint__str__(const daeOptimizationConstraint& self);
string daeOptimizationConstraint__repr__(const daeOptimizationConstraint& self);

string daeMeasuredVariable__str__(const daeMeasuredVariable& self);
string daeMeasuredVariable__repr__(const daeMeasuredVariable& self);

string daeEventPortConnection__str__(const daeEventPortConnection& self);
string daeEventPortConnection__repr__(const daeEventPortConnection& self);

string daePortConnection__str__(const daePortConnection& self);
string daePortConnection__repr__(const daePortConnection& self);

string daeOnEventActions__str__(const daeOnEventActions& self);
string daeOnEventActions__repr__(const daeOnEventActions& self);

string daeOnConditionActions__str__(const daeOnConditionActions& self);
string daeOnConditionActions__repr__(const daeOnConditionActions& self);

string daeAction__str__(daeAction& self);
string daeAction__repr__(daeAction& self);

string daeThermoPhysicalPropertyPackage__str__(daeThermoPhysicalPropertyPackage& self);
string daeThermoPhysicalPropertyPackage__repr__(daeThermoPhysicalPropertyPackage& self);

string daeScalarExternalFunction__str__(daeScalarExternalFunction& self);
string daeScalarExternalFunction__repr__(daeScalarExternalFunction& self);

string daeVectorExternalFunction__str__(daeVectorExternalFunction& self);
string daeVectorExternalFunction__repr__(daeVectorExternalFunction& self);

/*******************************************************
    Common functions
*******************************************************/
daeDomainIndex CreateDomainIndex(boost::python::object& o);
daeArrayRange  CreateArrayRange(boost::python::object& o, daeDomain* pDomain);

boost::python::list daeVariableWrapper_GetDomainIndexes(daeVariableWrapper &self);
daeVariable* daeVariableWrapper_GetVariable(daeVariableWrapper &self);

boost::python::object pydaeGetConfig(void);
bool        GetBoolean(daeConfig& self, const std::string& strPropertyPath);
real_t      GetFloat(daeConfig& self, const std::string& strPropertyPath);
int         GetInteger(daeConfig& self, const std::string& strPropertyPath);
std::string GetString(daeConfig& self, const std::string& strPropertyPath);
bool        GetBoolean1(daeConfig& self, const std::string& strPropertyPath, const bool defValue);
real_t      GetFloat1(daeConfig& self, const std::string& strPropertyPath, const real_t defValue);
int         GetInteger1(daeConfig& self, const std::string& strPropertyPath, const int defValue);
std::string GetString1(daeConfig& self, const std::string& strPropertyPath, const std::string& defValue);

void SetBoolean(daeConfig& self, const std::string& strPropertyPath, bool value);
void SetFloat(daeConfig& self, const std::string& strPropertyPath, real_t value);
void SetInteger(daeConfig& self, const std::string& strPropertyPath, int value);
void SetString(daeConfig& self, const std::string& strPropertyPath, std::string& value);

std::string daeConfig__str__(daeConfig& self);
std::string daeConfig__repr__(daeConfig& self);
boost::python::object daeConfig__contains__(daeConfig& self, boost::python::object key);
boost::python::object daeConfig_has_key(daeConfig& self, boost::python::object key);
boost::python::object daeConfig__getitem__(daeConfig& self, boost::python::object key);
void                  daeConfig__setitem__(daeConfig& self, boost::python::object key, boost::python::object value);

/*******************************************************
    adNode...
*******************************************************/
daeParameter* adSetupParameterNode_Parameter(adSetupParameterNode& node);
boost::python::list adSetupParameterNode_Domains(adSetupParameterNode& node);

daeVariable* adSetupVariableNode_Variable(adSetupVariableNode& node);
boost::python::list adSetupVariableNode_Domains(adSetupVariableNode& node);

daeVariable* adSetupTimeDerivativeNode_Variable(adSetupTimeDerivativeNode& node);
boost::python::list adSetupTimeDerivativeNode_Domains(adSetupTimeDerivativeNode& node);

daeVariable* adSetupPartialDerivativeNode_Variable(adSetupPartialDerivativeNode& node);
daeDomain* adSetupPartialDerivativeNode_Domain(adSetupPartialDerivativeNode& node);
boost::python::list adSetupPartialDerivativeNode_Domains(adSetupPartialDerivativeNode& node);

real_t adRuntimeParameterNode_Value(adRuntimeParameterNode& node);
daeParameter* adRuntimeParameterNode_Parameter(adRuntimeParameterNode& node);
boost::python::list adRuntimeParameterNode_Domains(adRuntimeParameterNode& node);

daeVariable* adRuntimeVariableNode_Variable(adRuntimeVariableNode& node);
boost::python::list adRuntimeVariableNode_Domains(adRuntimeVariableNode& node);

daeVariable* adRuntimeTimeDerivativeNode_Variable(adRuntimeTimeDerivativeNode& node);
boost::python::list adRuntimeTimeDerivativeNode_Domains(adRuntimeTimeDerivativeNode& node);

daeDomain* adDomainIndexNode_Domain(adDomainIndexNode& node);
real_t adDomainIndexNode_Value(adDomainIndexNode& node);

boost::python::list adRuntimeSpecialFunctionForLargeArraysNode_RuntimeNodes(adRuntimeSpecialFunctionForLargeArraysNode& node);

daeScalarExternalFunction* adScalarExternalFunctionNode_ExternalFunction(adScalarExternalFunctionNode& node);
daeVectorExternalFunction* adVectorExternalFunctionNode_ExternalFunction(adVectorExternalFunctionNode& node);

//adouble adFEMatrixItemNode_Value(adFEMatrixItemNode& self);
//adouble adFEVectorItemNode_Value(adFEVectorItemNode& self);

daeVariable* daeFloatCoefficientVariableProduct_variable(daeFloatCoefficientVariableProduct& self);

boost::python::list map_Uint_daeFloatCoefficientVariableProduct_values(std::map<size_t, daeFloatCoefficientVariableProduct>& self);

/*******************************************************
    adouble
*******************************************************/
const adouble ad_true_divide1(const adouble &a, const adouble &b);
const adouble ad_true_divide2(const adouble &a, const real_t v);
const adouble ad_true_divide3(const real_t v, const adouble &a);
const adouble ad_floor_divide1(const adouble &a, const adouble &b);
const adouble ad_floor_divide2(const adouble &a, const real_t v);
const adouble ad_floor_divide3(const real_t v, const adouble &a);

const adouble ad_exp(const adouble &a);
const adouble ad_log(const adouble &a);
const adouble ad_sqrt(const adouble &a);
const adouble ad_sin(const adouble &a);
const adouble ad_cos(const adouble &a);
const adouble ad_tan(const adouble &a);
const adouble ad_asin(const adouble &a);
const adouble ad_acos(const adouble &a);
const adouble ad_atan(const adouble &a);

const adouble ad_sinh(const adouble &a);
const adouble ad_cosh(const adouble &a);
const adouble ad_tanh(const adouble &a);
const adouble ad_asinh(const adouble &a);
const adouble ad_acosh(const adouble &a);
const adouble ad_atanh(const adouble &a);
const adouble ad_atan2(const adouble &a, const adouble &b);
const adouble ad_erf(const adouble &a);

const adouble ad_pow1(const adouble &a, real_t v);
const adouble ad_pow2(const adouble &a, const adouble &b);
const adouble ad_pow3(real_t v, const adouble &a);
const adouble ad_log10(const adouble &a);

const adouble ad_abs(const adouble &a);
const adouble ad_ceil(const adouble &a);
const adouble ad_floor(const adouble &a);
const adouble ad_max1(const adouble &a, const adouble &b);
const adouble ad_max2(real_t v, const adouble &a);
const adouble ad_max3(const adouble &a, real_t v);
const adouble ad_min1(const adouble &a, const adouble &b);
const adouble ad_min2(real_t v, const adouble &a);
const adouble ad_min3(const adouble &a, real_t v);

bool ad_not(const adouble &a);
bool ad_nonzero(const adouble &a);
bool ad_bool(const adouble &a);

const adouble ad_dt(const adouble& a);

const adouble ad_d(const adouble&           a,
                   daeDomain&               domain,
                   daeeDiscretizationMethod method  = eCFDM,
                   boost::python::dict      options = boost::python::dict());
const adouble ad_d2(const adouble&           a,
                    daeDomain&               domain,
                    daeeDiscretizationMethod method  = eCFDM,
                    boost::python::dict      options = boost::python::dict());

const adouble_array ad_dt_array(adouble_array& adarr);
const adouble_array ad_d_array(adouble_array&           adarr,
                               daeDomain&               domain,
                               daeeDiscretizationMethod method  = eCFDM,
                               boost::python::dict      options = boost::python::dict());
const adouble_array ad_d2_array(adouble_array&           adarr,
                                daeDomain&               domain,
                                daeeDiscretizationMethod method  = eCFDM,
                                boost::python::dict      options = boost::python::dict());

const adouble ad_Constant_q(const quantity& q);
const adouble ad_Constant_c(real_t c);
const adouble_array adarr_Array(boost::python::list Values);
const adouble_array adarr_FromNumpyArray(boost::python::object ndValues);
const adouble_array adarr_FromList(boost::python::list lValues);

/*******************************************************
    adouble_array
*******************************************************/
bool adarr_not(const adouble_array &a);
bool adarr_nonzero(const adouble_array &a);
bool adarr_bool(const adouble_array &a);

const adouble_array adarr_true_divide1(const adouble_array &a, const adouble_array &b);
const adouble_array adarr_true_divide2(const adouble_array &a, const real_t v);
const adouble_array adarr_true_divide3(const real_t v, const adouble_array &a);
const adouble_array adarr_floor_divide1(const adouble_array &a, const adouble_array &b);
const adouble_array adarr_floor_divide2(const adouble_array &a, const real_t v);
const adouble_array adarr_floor_divide3(const real_t v, const adouble_array &a);

const adouble_array adarr_exp(const adouble_array& a);
const adouble_array adarr_sqrt(const adouble_array& a);
const adouble_array adarr_log(const adouble_array& a);
const adouble_array adarr_log10(const adouble_array& a);
const adouble_array adarr_abs(const adouble_array& a);
const adouble_array adarr_floor(const adouble_array& a);
const adouble_array adarr_ceil(const adouble_array& a);
const adouble_array adarr_sin(const adouble_array& a);
const adouble_array adarr_cos(const adouble_array& a);
const adouble_array adarr_tan(const adouble_array& a);
const adouble_array adarr_asin(const adouble_array& a);
const adouble_array adarr_acos(const adouble_array& a);
const adouble_array adarr_atan(const adouble_array& a);

const adouble_array adarr_sinh(const adouble_array &a);
const adouble_array adarr_cosh(const adouble_array &a);
const adouble_array adarr_tanh(const adouble_array &a);
const adouble_array adarr_asinh(const adouble_array &a);
const adouble_array adarr_acosh(const adouble_array &a);
const adouble_array adarr_atanh(const adouble_array &a);
const adouble_array adarr_atan2(const adouble_array &a, const adouble_array &b);
const adouble_array adarr_erf(const adouble_array &a);

const adouble adarr_sum(const adouble_array& a);
const adouble adarr_product(const adouble_array& a);
const adouble adarr_min(const adouble_array& a);
const adouble adarr_max(const adouble_array& a);
const adouble adarr_average(const adouble_array& a);
const adouble adarr_integral(const adouble_array& a);

adouble adouble_array__call__(adouble_array& a, boost::python::object index);

/*******************************************************
    adouble_array
*******************************************************/
boost::python::list daeCondition_GetExpressions(daeCondition& self);

/*******************************************************
    daeFMI2Object_t
*******************************************************/
daeParameter* daeFMI2Object_t_parameter(daeFMI2Object_t& self);
daeVariable*  daeFMI2Object_t_variable(daeFMI2Object_t& self);
daeSTN*       daeFMI2Object_t_stn(daeFMI2Object_t& self);

/*******************************************************
    daeObject
*******************************************************/
daeObject* daeObject_GetModel(daeObject& self);
string daeGetRelativeName_1(const daeObject* parent, const daeObject* child);
string daeGetRelativeName_2(const string& strParent, const string& strChild);

/*******************************************************
    daeDomain
*******************************************************/
//boost::python::object GetNumPyArrayDomain(daeDomain& domain);
//daeIndexRange FunctionCallDomain1(daeDomain& domain, int start, int end, int step);
//daeIndexRange FunctionCallDomain2(daeDomain& domain, boost::python::list l);
//daeIndexRange FunctionCallDomain3(daeDomain& domain);
boost::python::object GetDomainPoints(daeDomain& domain);
void SetDomainPoints(daeDomain& domain, boost::python::object points);
adouble_array DomainArray(daeDomain& domain, boost::python::object indexes);
boost::python::list GetDomainCoordinates(daeDomain& domain);
void CreateUnstructuredGrid(daeDomain& domain, boost::python::list coords);
void CreateStructuredGrid(daeDomain& domain, size_t nNoIntervals, real_t LB, real_t UB);
void qCreateStructuredGrid(daeDomain& domain, size_t nNoIntervals, quantity qLB, quantity qUB);

daeIndexRange* __init__daeIndexRange(daeDomain* pDomain, boost::python::list CustomPoints);
daeDomain* daeIndexRange_GetDomain(daeIndexRange& self);

/*******************************************************
    daeParameter
*******************************************************/
/*
class daeParameterWrapper : public daeParameter,
                            public boost::python::wrapper<daeParameter>
{
public:
    daeParameterWrapper(void)
    {
    }

    daeParameterWrapper(string strName, const unit& units, daeModel* pModel, string strDescription = "", boost::python::list domains = boost::python::list())
        : daeParameter(strName, units, pModel, strDescription)
    {
        daeDomain* pDomain;
        boost::python::ssize_t n = boost::python::len(domains);
        m_ptrDomains.resize(n);
        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            pDomain = boost::python::extract<daeDomain*>(domains[i]);
            m_ptrDomains[i] = pDomain;
        }
    }

    daeParameterWrapper(string strName, const unit& units, daePort* pPort, string strDescription = "", boost::python::list domains = boost::python::list())
        : daeParameter(strName, units, pPort, strDescription)
    {
        daeDomain* pDomain;
        boost::python::ssize_t n = boost::python::len(domains);
        m_ptrDomains.resize(n);
        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            pDomain = boost::python::extract<daeDomain*>(domains[i]);
            m_ptrDomains[i] = pDomain;
        }
    }

public:
    boost::python::list GetDomains(void)
    {
        return getListFromVector(m_ptrDomains);
    }

    boost::python::dict GetDomainsIndexesMap1(size_t nIndexBase)
    {
       // Returns dictionary {integer : [list of integers]}
        boost::python::dict d;
        std::map<size_t, std::vector<size_t> > mapIndexes;
        typedef std::map<size_t, std::vector<size_t> >::iterator c_iterator;

        daeParameter::GetDomainsIndexesMap(mapIndexes, nIndexBase);

        for(c_iterator iter = mapIndexes.begin(); iter != mapIndexes.end(); iter++)
            d[iter->first] = getListFromVectorByValue<size_t>(iter->second);

        return d;
    }

    real_t lGetParameterValue(boost::python::list indexes)
    {
        std::vector<size_t> narrIndexes;
        boost::python::ssize_t n = boost::python::len(indexes);
        narrIndexes.resize(n);

        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            boost::python::extract<size_t> index(indexes[i]);

            if(index.check())
                narrIndexes[i] = index();
            else
            {
                daeDeclareException(exInvalidCall);
                e << "Invalid type of index [" << i << "] in the list of indexes in GetValue for parameter " << GetCanonicalName();
                throw e;
            }
        }
        return GetValue(narrIndexes);
    }

    quantity lGetParameterQuantity(boost::python::list indexes)
    {
        std::vector<size_t> narrIndexes;
        boost::python::ssize_t n = boost::python::len(indexes);
        narrIndexes.resize(n);

        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            boost::python::extract<size_t> index(indexes[i]);

            if(index.check())
                narrIndexes[i] = index();
            else
            {
                daeDeclareException(exInvalidCall);
                e << "Invalid type of index [" << i << "] in the list of indexes in GetQuantity for parameter " << GetCanonicalName();
                throw e;
            }
        }
        return GetQuantity(narrIndexes);
    }

    real_t GetParameterValue0()
    {
        return GetValue();
    }

    real_t GetParameterValue1(size_t n1)
    {
        return GetValue(n1);
    }

    real_t GetParameterValue2(size_t n1, size_t n2)
    {
        return GetValue(n1, n2);
    }

    real_t GetParameterValue3(size_t n1, size_t n2, size_t n3)
    {
        return GetValue(n1, n2, n3);
    }

    real_t GetParameterValue4(size_t n1, size_t n2, size_t n3, size_t n4)
    {
        return GetValue(n1, n2, n3, n4);
    }

    real_t GetParameterValue5(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5)
    {
        return GetValue(n1, n2, n3, n4, n5);
    }

    real_t GetParameterValue6(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6)
    {
        return GetValue(n1, n2, n3, n4, n5, n6);
    }

    real_t GetParameterValue7(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7)
    {
        return GetValue(n1, n2, n3, n4, n5, n6, n7);
    }

    real_t GetParameterValue8(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8)
    {
        return GetValue(n1, n2, n3, n4, n5, n6, n7, n8);
    }

    quantity GetParameterQuantity0()
    {
        return GetQuantity();
    }

    quantity GetParameterQuantity1(size_t n1)
    {
        return GetQuantity(n1);
    }

    quantity GetParameterQuantity2(size_t n1, size_t n2)
    {
        return GetQuantity(n1, n2);
    }

    quantity GetParameterQuantity3(size_t n1, size_t n2, size_t n3)
    {
        return GetQuantity(n1, n2, n3);
    }

    quantity GetParameterQuantity4(size_t n1, size_t n2, size_t n3, size_t n4)
    {
        return GetQuantity(n1, n2, n3, n4);
    }

    quantity GetParameterQuantity5(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5)
    {
        return GetQuantity(n1, n2, n3, n4, n5);
    }

    quantity GetParameterQuantity6(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6)
    {
        return GetQuantity(n1, n2, n3, n4, n5, n6);
    }

    quantity GetParameterQuantity7(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7)
    {
        return GetQuantity(n1, n2, n3, n4, n5, n6, n7);
    }

    quantity GetParameterQuantity8(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8)
    {
        return GetQuantity(n1, n2, n3, n4, n5, n6, n7, n8);
    }
};
*/

daeParameter* daeParameter_init1(string strName, const unit& units, daeModel* pModel, string strDescription = "", boost::python::list domains = boost::python::list());
daeParameter* daeParameter_init2(string strName, const unit& units, daePort* pPort, string strDescription = "", boost::python::list domains = boost::python::list());

boost::python::list daeParameter_GetDomains(daeParameter& self);
boost::python::dict daeParameter_GetDomainsIndexesMap1(daeParameter& self, size_t nIndexBase);
real_t lGetParameterValue(daeParameter& self, boost::python::list indexes);
quantity lGetParameterQuantity(daeParameter& self, boost::python::list indexes);

real_t GetParameterValue0(daeParameter& self);
real_t GetParameterValue1(daeParameter& self, size_t n1);
real_t GetParameterValue2(daeParameter& self, size_t n1, size_t n2);
real_t GetParameterValue3(daeParameter& self, size_t n1, size_t n2, size_t n3);
real_t GetParameterValue4(daeParameter& self, size_t n1, size_t n2, size_t n3, size_t n4);
real_t GetParameterValue5(daeParameter& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5);
real_t GetParameterValue6(daeParameter& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6);
real_t GetParameterValue7(daeParameter& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7);
real_t GetParameterValue8(daeParameter& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8);
quantity GetParameterQuantity0(daeParameter& self);
quantity GetParameterQuantity1(daeParameter& self, size_t n1);
quantity GetParameterQuantity2(daeParameter& self, size_t n1, size_t n2);
quantity GetParameterQuantity3(daeParameter& self, size_t n1, size_t n2, size_t n3);
quantity GetParameterQuantity4(daeParameter& self, size_t n1, size_t n2, size_t n3, size_t n4);
quantity GetParameterQuantity5(daeParameter& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5);
quantity GetParameterQuantity6(daeParameter& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6);
quantity GetParameterQuantity7(daeParameter& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7);
quantity GetParameterQuantity8(daeParameter& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8);

boost::python::object GetNumPyArrayParameter(daeParameter& param);

adouble FunctionCallParameter0(daeParameter& param);
adouble FunctionCallParameter1(daeParameter& param, boost::python::object o1);
adouble FunctionCallParameter2(daeParameter& param, boost::python::object o1, boost::python::object o2);
adouble FunctionCallParameter3(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3);
adouble FunctionCallParameter4(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4);
adouble FunctionCallParameter5(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5);
adouble FunctionCallParameter6(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6);
adouble FunctionCallParameter7(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7);
adouble FunctionCallParameter8(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7, boost::python::object o8);

void lSetParameterValue(daeParameter& param, boost::python::list indexes, real_t value);
void lSetParameterQuantity(daeParameter& param, boost::python::list indexes, const quantity& value);
void SetParameterValue0(daeParameter& param, real_t value);
void SetParameterValue1(daeParameter& param, size_t n1, real_t value);
void SetParameterValue2(daeParameter& param, size_t n1, size_t n2, real_t value);
void SetParameterValue3(daeParameter& param, size_t n1, size_t n2, size_t n3, real_t value);
void SetParameterValue4(daeParameter& param, size_t n1, size_t n2, size_t n3, size_t n4, real_t value);
void SetParameterValue5(daeParameter& param, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, real_t value);
void SetParameterValue6(daeParameter& param, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, real_t value);
void SetParameterValue7(daeParameter& param, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, real_t value);
void SetParameterValue8(daeParameter& param, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, real_t value);

void SetParameterQuantity0(daeParameter& param, const quantity& q);
void SetParameterQuantity1(daeParameter& param, size_t n1, const quantity& q);
void SetParameterQuantity2(daeParameter& param, size_t n1, size_t n2, const quantity& q);
void SetParameterQuantity3(daeParameter& param, size_t n1, size_t n2, size_t n3, const quantity& q);
void SetParameterQuantity4(daeParameter& param, size_t n1, size_t n2, size_t n3, size_t n4, const quantity& q);
void SetParameterQuantity5(daeParameter& param, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, const quantity& q);
void SetParameterQuantity6(daeParameter& param, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, const quantity& q);
void SetParameterQuantity7(daeParameter& param, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, const quantity& q);
void SetParameterQuantity8(daeParameter& param, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, const quantity& q);

void SetParameterValues(daeParameter& param, real_t values);
void qSetParameterValues(daeParameter& param, const quantity& q);
void lSetParameterValues(daeParameter& param, boost::python::object nd_values);

adouble_array ParameterArray1(daeParameter& param, boost::python::object o1);
adouble_array ParameterArray2(daeParameter& param, boost::python::object o1, boost::python::object o2);
adouble_array ParameterArray3(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3);
adouble_array ParameterArray4(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4);
adouble_array ParameterArray5(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5);
adouble_array ParameterArray6(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6);
adouble_array ParameterArray7(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7);
adouble_array ParameterArray8(daeParameter& param, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7, boost::python::object o8);

/*******************************************************
    daeVariable_Wrapper
*******************************************************/
/*
class daeVariable_Wrapper : public daeVariable,
                            public boost::python::wrapper<daeVariable>
{
public:
    daeVariable_Wrapper(void)
    {
    }

    daeVariable_Wrapper(string strName, const daeVariableType& varType, daeModel* pModel, string strDescription = "", boost::python::list domains = boost::python::list())
        : daeVariable(strName, varType, pModel, strDescription)
    {
        daeDomain* pDomain;
        boost::python::ssize_t n = boost::python::len(domains);
        m_ptrDomains.resize(n);
        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            pDomain = boost::python::extract<daeDomain*>(domains[i]);
            m_ptrDomains[i] = pDomain;
        }
    }

    daeVariable_Wrapper(string strName, const daeVariableType& varType, daePort* pPort, string strDescription = "", boost::python::list domains = boost::python::list())
        : daeVariable(strName, varType, pPort, strDescription)
    {
        daeDomain* pDomain;
        boost::python::ssize_t n = boost::python::len(domains);
        m_ptrDomains.resize(n);
        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            pDomain = boost::python::extract<daeDomain*>(domains[i]);
            m_ptrDomains[i] = pDomain;
        }
    }

public:
    boost::python::list GetDomains(void)
    {
        return getListFromVector(m_ptrDomains);
    }

    daeVariableType* GetVariableType(void)
    {
        return &this->m_VariableType;
    }

    real_t lGetVariableValue(boost::python::list indexes)
    {
        std::vector<size_t> narrIndexes;
        boost::python::ssize_t n = boost::python::len(indexes);
        narrIndexes.resize(n);

        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            boost::python::extract<size_t> index(indexes[i]);

            if(index.check())
                narrIndexes[i] = index();
            else
            {
                daeDeclareException(exInvalidCall);
                e << "Invalid type of index [" << i << "] in the list of indexes in GetValue for variable " << GetCanonicalName();
                throw e;
            }
        }
        return GetValue(narrIndexes);
    }

    quantity lGetVariableQuantity(boost::python::list indexes)
    {
        std::vector<size_t> narrIndexes;
        boost::python::ssize_t n = boost::python::len(indexes);
        narrIndexes.resize(n);

        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            boost::python::extract<size_t> index(indexes[i]);

            if(index.check())
                narrIndexes[i] = index();
            else
            {
                daeDeclareException(exInvalidCall);
                e << "Invalid type of index [" << i << "] in the list of indexes in GetQuantity for variable " << GetCanonicalName();
                throw e;
            }
        }
        return GetQuantity(narrIndexes);
    }

    real_t GetVariableValue0(void)
    {
        return GetValue();
    }

    real_t GetVariableValue1(size_t n1)
    {
        return GetValue(n1);
    }

    real_t GetVariableValue2(size_t n1, size_t n2)
    {
        return GetValue(n1, n2);
    }

    real_t GetVariableValue3(size_t n1, size_t n2, size_t n3)
    {
        return GetValue(n1, n2, n3);
    }

    real_t GetVariableValue4(size_t n1, size_t n2, size_t n3, size_t n4)
    {
        return GetValue(n1, n2, n3, n4);
    }

    real_t GetVariableValue5(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5)
    {
        return GetValue(n1, n2, n3, n4, n5);
    }

    real_t GetVariableValue6(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6)
    {
        return GetValue(n1, n2, n3, n4, n5, n6);
    }

    real_t GetVariableValue7(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7)
    {
        return GetValue(n1, n2, n3, n4, n5, n6, n7);
    }

    real_t GetVariableValue8(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8)
    {
        return GetValue(n1, n2, n3, n4, n5, n6, n7, n8);
    }

    quantity GetVariableQuantity0()
    {
        return GetQuantity();
    }

    quantity GetVariableQuantity1(size_t n1)
    {
        return GetQuantity(n1);
    }

    quantity GetVariableQuantity2(size_t n1, size_t n2)
    {
        return GetQuantity(n1, n2);
    }

    quantity GetVariableQuantity3(size_t n1, size_t n2, size_t n3)
    {
        return GetQuantity(n1, n2, n3);
    }

    quantity GetVariableQuantity4(size_t n1, size_t n2, size_t n3, size_t n4)
    {
        return GetQuantity(n1, n2, n3, n4);
    }

    quantity GetVariableQuantity5(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5)
    {
        return GetQuantity(n1, n2, n3, n4, n5);
    }

    quantity GetVariableQuantity6(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6)
    {
        return GetQuantity(n1, n2, n3, n4, n5, n6);
    }

    quantity GetVariableQuantity7(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7)
    {
        return GetQuantity(n1, n2, n3, n4, n5, n6, n7);
    }

    quantity GetVariableQuantity8(size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8)
    {
        return GetQuantity(n1, n2, n3, n4, n5, n6, n7, n8);
    }

};
*/
daeVariable* daeVariable_init1(string strName, const daeVariableType& varType, daeModel* pModel, string strDescription = "", boost::python::list domains = boost::python::list());
daeVariable* daeVariable_init2(string strName, const daeVariableType& varType, daePort* pPort, string strDescription = "", boost::python::list domains = boost::python::list());


boost::python::list daeVariable_GetDomains(daeVariable& self);
daeVariableType* daeVariable_GetVariableType(daeVariable& self);
real_t daeVariable_lGetVariableValue(daeVariable& self, boost::python::list indexes);
quantity daeVariable_lGetVariableQuantity(daeVariable& self, boost::python::list indexes);
real_t daeVariable_GetVariableValue0(daeVariable& self);
real_t daeVariable_GetVariableValue1(daeVariable& self, size_t n1);
real_t daeVariable_GetVariableValue2(daeVariable& self, size_t n1, size_t n2);
real_t daeVariable_GetVariableValue3(daeVariable& self, size_t n1, size_t n2, size_t n3);
real_t daeVariable_GetVariableValue4(daeVariable& self, size_t n1, size_t n2, size_t n3, size_t n4);
real_t daeVariable_GetVariableValue5(daeVariable& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5);
real_t daeVariable_GetVariableValue6(daeVariable& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6);
real_t daeVariable_GetVariableValue7(daeVariable& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7);
real_t daeVariable_GetVariableValue8(daeVariable& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8);
quantity daeVariable_GetVariableQuantity0(daeVariable& self);
quantity daeVariable_GetVariableQuantity1(daeVariable& self, size_t n1);
quantity daeVariable_GetVariableQuantity2(daeVariable& self, size_t n1, size_t n2);
quantity daeVariable_GetVariableQuantity3(daeVariable& self, size_t n1, size_t n2, size_t n3);
quantity daeVariable_GetVariableQuantity4(daeVariable& self, size_t n1, size_t n2, size_t n3, size_t n4);
quantity daeVariable_GetVariableQuantity5(daeVariable& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5);
quantity daeVariable_GetVariableQuantity6(daeVariable& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6);
quantity daeVariable_GetVariableQuantity7(daeVariable& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7);
quantity daeVariable_GetVariableQuantity8(daeVariable& self, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8);

boost::python::object daeVariable_BlockIndexes(daeVariable& self);
boost::python::object daeVariable_Values(daeVariable& var);
boost::python::object daeVariable_TimeDerivatives(daeVariable& var);
boost::python::object daeVariable_IDs(daeVariable& var);
boost::python::object daeVariable_GatheredIDs(daeVariable& var);
boost::python::dict   daeVariable_GetDomainsIndexesMap(daeVariable& self, size_t nIndexBase);

adouble VariableFunctionCall0(daeVariable& var);
adouble VariableFunctionCall1(daeVariable& var, boost::python::object o1);
adouble VariableFunctionCall2(daeVariable& var, boost::python::object o1, boost::python::object o2);
adouble VariableFunctionCall3(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3);
adouble VariableFunctionCall4(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4);
adouble VariableFunctionCall5(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5);
adouble VariableFunctionCall6(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6);
adouble VariableFunctionCall7(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7);
adouble VariableFunctionCall8(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7, boost::python::object o8);

adouble Get_dt0(daeVariable& var);
adouble Get_dt1(daeVariable& var, boost::python::object o1);
adouble Get_dt2(daeVariable& var, boost::python::object o1, boost::python::object o2);
adouble Get_dt3(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3);
adouble Get_dt4(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4);
adouble Get_dt5(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5);
adouble Get_dt6(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6);
adouble Get_dt7(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7);
adouble Get_dt8(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7, boost::python::object o8);

adouble Get_d1(daeVariable& var, daeDomain& d, boost::python::object o1);
adouble Get_d2(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2);
adouble Get_d3(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3);
adouble Get_d4(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4);
adouble Get_d5(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5);
adouble Get_d6(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6);
adouble Get_d7(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7);
adouble Get_d8(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7, boost::python::object o8);

adouble Get_d21(daeVariable& var, daeDomain& d, boost::python::object o1);
adouble Get_d22(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2);
adouble Get_d23(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3);
adouble Get_d24(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4);
adouble Get_d25(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5);
adouble Get_d26(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6);
adouble Get_d27(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7);
adouble Get_d28(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7, boost::python::object o8);

adouble_array VariableArray1(daeVariable& var, boost::python::object o1);
adouble_array VariableArray2(daeVariable& var, boost::python::object o1, boost::python::object o2);
adouble_array VariableArray3(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3);
adouble_array VariableArray4(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4);
adouble_array VariableArray5(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5);
adouble_array VariableArray6(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6);
adouble_array VariableArray7(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7);
adouble_array VariableArray8(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7, boost::python::object o8);

adouble_array Get_dt_array1(daeVariable& var, boost::python::object o1);
adouble_array Get_dt_array2(daeVariable& var, boost::python::object o1, boost::python::object o2);
adouble_array Get_dt_array3(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3);
adouble_array Get_dt_array4(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4);
adouble_array Get_dt_array5(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5);
adouble_array Get_dt_array6(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6);
adouble_array Get_dt_array7(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7);
adouble_array Get_dt_array8(daeVariable& var, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7, boost::python::object o8);

adouble_array Get_d_array1(daeVariable& var, daeDomain& d, boost::python::object o1);
adouble_array Get_d_array2(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2);
adouble_array Get_d_array3(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3);
adouble_array Get_d_array4(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4);
adouble_array Get_d_array5(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5);
adouble_array Get_d_array6(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6);
adouble_array Get_d_array7(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7);
adouble_array Get_d_array8(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7, boost::python::object o8);

adouble_array Get_d2_array1(daeVariable& var, daeDomain& d, boost::python::object o1);
adouble_array Get_d2_array2(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2);
adouble_array Get_d2_array3(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3);
adouble_array Get_d2_array4(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4);
adouble_array Get_d2_array5(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5);
adouble_array Get_d2_array6(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6);
adouble_array Get_d2_array7(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7);
adouble_array Get_d2_array8(daeVariable& var, daeDomain& d, boost::python::object o1, boost::python::object o2, boost::python::object o3, boost::python::object o4, boost::python::object o5, boost::python::object o6, boost::python::object o7, boost::python::object o8);

void lSetVariableValue1(daeVariable& var, boost::python::list indexes, real_t value);
void lSetVariableValue2(daeVariable& var, boost::python::list indexes, const quantity& value);
void SetVariableValue0(daeVariable& var, real_t value);
void SetVariableValue1(daeVariable& var, size_t n1, real_t value);
void SetVariableValue2(daeVariable& var, size_t n1, size_t n2, real_t value);
void SetVariableValue3(daeVariable& var, size_t n1, size_t n2, size_t n3, real_t value);
void SetVariableValue4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, real_t value);
void SetVariableValue5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, real_t value);
void SetVariableValue6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, real_t value);
void SetVariableValue7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, real_t value);
void SetVariableValue8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, real_t value);

void lAssignValue1(daeVariable& var, boost::python::list indexes, real_t value);
void lAssignValue2(daeVariable& var, boost::python::list indexes, const quantity& value);
void AssignValue0(daeVariable& var, real_t value);
void AssignValue1(daeVariable& var, size_t n1, real_t value);
void AssignValue2(daeVariable& var, size_t n1, size_t n2, real_t value);
void AssignValue3(daeVariable& var, size_t n1, size_t n2, size_t n3, real_t value);
void AssignValue4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, real_t value);
void AssignValue5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, real_t value);
void AssignValue6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, real_t value);
void AssignValue7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, real_t value);
void AssignValue8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, real_t value);

void lReAssignValue1(daeVariable& var, boost::python::list indexes, real_t value);
void lReAssignValue2(daeVariable& var, boost::python::list indexes, const quantity& value);
void ReAssignValue0(daeVariable& var, real_t value);
void ReAssignValue1(daeVariable& var, size_t n1, real_t value);
void ReAssignValue2(daeVariable& var, size_t n1, size_t n2, real_t value);
void ReAssignValue3(daeVariable& var, size_t n1, size_t n2, size_t n3, real_t value);
void ReAssignValue4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, real_t value);
void ReAssignValue5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, real_t value);
void ReAssignValue6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, real_t value);
void ReAssignValue7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, real_t value);
void ReAssignValue8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, real_t value);

void lSetInitialCondition1(daeVariable& var, boost::python::list indexes, real_t value);
void lSetInitialCondition2(daeVariable& var, boost::python::list indexes, const quantity& value);
void SetInitialCondition0(daeVariable& var, real_t value);
void SetInitialCondition1(daeVariable& var, size_t n1, real_t value);
void SetInitialCondition2(daeVariable& var, size_t n1, size_t n2, real_t value);
void SetInitialCondition3(daeVariable& var, size_t n1, size_t n2, size_t n3, real_t value);
void SetInitialCondition4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, real_t value);
void SetInitialCondition5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, real_t value);
void SetInitialCondition6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, real_t value);
void SetInitialCondition7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, real_t value);
void SetInitialCondition8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, real_t value);

void lReSetInitialCondition1(daeVariable& var, boost::python::list indexes, real_t value);
void lReSetInitialCondition2(daeVariable& var, boost::python::list indexes, const quantity& value);
void ReSetInitialCondition0(daeVariable& var, real_t value);
void ReSetInitialCondition1(daeVariable& var, size_t n1, real_t value);
void ReSetInitialCondition2(daeVariable& var, size_t n1, size_t n2, real_t value);
void ReSetInitialCondition3(daeVariable& var, size_t n1, size_t n2, size_t n3, real_t value);
void ReSetInitialCondition4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, real_t value);
void ReSetInitialCondition5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, real_t value);
void ReSetInitialCondition6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, real_t value);
void ReSetInitialCondition7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, real_t value);
void ReSetInitialCondition8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, real_t value);

void lSetInitialGuess1(daeVariable& var, boost::python::list indexes, real_t value);
void lSetInitialGuess2(daeVariable& var, boost::python::list indexes, const quantity& value);
void SetInitialGuess0(daeVariable& var, real_t value);
void SetInitialGuess1(daeVariable& var, size_t n1, real_t value);
void SetInitialGuess2(daeVariable& var, size_t n1, size_t n2, real_t value);
void SetInitialGuess3(daeVariable& var, size_t n1, size_t n2, size_t n3, real_t value);
void SetInitialGuess4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, real_t value);
void SetInitialGuess5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, real_t value);
void SetInitialGuess6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, real_t value);
void SetInitialGuess7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, real_t value);
void SetInitialGuess8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, real_t value);

void qSetVariableValue0(daeVariable& var, const quantity& q);
void qSetVariableValue1(daeVariable& var, size_t n1, const quantity& q);
void qSetVariableValue2(daeVariable& var, size_t n1, size_t n2, const quantity& q);
void qSetVariableValue3(daeVariable& var, size_t n1, size_t n2, size_t n3, const quantity& q);
void qSetVariableValue4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, const quantity& q);
void qSetVariableValue5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, const quantity& q);
void qSetVariableValue6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, const quantity& q);
void qSetVariableValue7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, const quantity& q);
void qSetVariableValue8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, const quantity& q);

void qAssignValue0(daeVariable& var, const quantity& value);
void qAssignValue1(daeVariable& var, size_t n1, const quantity& value);
void qAssignValue2(daeVariable& var, size_t n1, size_t n2, const quantity& value);
void qAssignValue3(daeVariable& var, size_t n1, size_t n2, size_t n3, const quantity& value);
void qAssignValue4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, const quantity& value);
void qAssignValue5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, const quantity& value);
void qAssignValue6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, const quantity& value);
void qAssignValue7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, const quantity& value);
void qAssignValue8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, const quantity& value);

void qReAssignValue0(daeVariable& var, const quantity& value);
void qReAssignValue1(daeVariable& var, size_t n1, const quantity& value);
void qReAssignValue2(daeVariable& var, size_t n1, size_t n2, const quantity& value);
void qReAssignValue3(daeVariable& var, size_t n1, size_t n2, size_t n3, const quantity& value);
void qReAssignValue4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, const quantity& value);
void qReAssignValue5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, const quantity& value);
void qReAssignValue6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, const quantity& value);
void qReAssignValue7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, const quantity& value);
void qReAssignValue8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, const quantity& value);

void qSetInitialCondition0(daeVariable& var, const quantity& q);
void qSetInitialCondition1(daeVariable& var, size_t n1, const quantity& q);
void qSetInitialCondition2(daeVariable& var, size_t n1, size_t n2, const quantity& q);
void qSetInitialCondition3(daeVariable& var, size_t n1, size_t n2, size_t n3, const quantity& q);
void qSetInitialCondition4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, const quantity& q);
void qSetInitialCondition5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, const quantity& q);
void qSetInitialCondition6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, const quantity& q);
void qSetInitialCondition7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, const quantity& q);
void qSetInitialCondition8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, const quantity& q);

void qReSetInitialCondition0(daeVariable& var, const quantity& q);
void qReSetInitialCondition1(daeVariable& var, size_t n1, const quantity& q);
void qReSetInitialCondition2(daeVariable& var, size_t n1, size_t n2, const quantity& q);
void qReSetInitialCondition3(daeVariable& var, size_t n1, size_t n2, size_t n3, const quantity& q);
void qReSetInitialCondition4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, const quantity& q);
void qReSetInitialCondition5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, const quantity& q);
void qReSetInitialCondition6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, const quantity& q);
void qReSetInitialCondition7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, const quantity& q);
void qReSetInitialCondition8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, const quantity& q);

void qSetInitialGuess0(daeVariable& var, const quantity& q);
void qSetInitialGuess1(daeVariable& var, size_t n1, const quantity& q);
void qSetInitialGuess2(daeVariable& var, size_t n1, size_t n2, const quantity& q);
void qSetInitialGuess3(daeVariable& var, size_t n1, size_t n2, size_t n3, const quantity& q);
void qSetInitialGuess4(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, const quantity& q);
void qSetInitialGuess5(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, const quantity& q);
void qSetInitialGuess6(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, const quantity& q);
void qSetInitialGuess7(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, const quantity& q);
void qSetInitialGuess8(daeVariable& var, size_t n1, size_t n2, size_t n3, size_t n4, size_t n5, size_t n6, size_t n7, size_t n8, const quantity& q);

void AssignValues(daeVariable& var, real_t values);
void AssignValues2(daeVariable& var, boost::python::object nd_values);
void qAssignValues(daeVariable& var, const quantity& q);

void ReAssignValues(daeVariable& var, real_t values);
void ReAssignValues2(daeVariable& var, boost::python::object nd_values);
void qReAssignValues(daeVariable& var, const quantity& q);

void SetInitialConditions(daeVariable& var, real_t values);
void SetInitialConditions2(daeVariable& var, boost::python::object nd_values);
void qSetInitialConditions(daeVariable& var, const quantity& q);

void ReSetInitialConditions(daeVariable& var, real_t values);
void ReSetInitialConditions2(daeVariable& var, boost::python::object nd_values);
void qReSetInitialConditions(daeVariable& var, const quantity& q);

void SetInitialGuesses(daeVariable& var, real_t values);
void SetInitialGuesses2(daeVariable& var, boost::python::object nd_values);
void qSetInitialGuesses(daeVariable& var, const quantity& q);

/*******************************************************
    daeActionWrapper
*******************************************************/
class daeActionWrapper : public daeAction,
                         public boost::python::wrapper<daeAction>
{
public:
    daeActionWrapper(void)
    {
        m_eActionType = eUserDefinedAction;
    }

    void Execute(void)
    {
        this->get_override("Execute")();
    }
};

/*******************************************************
    daePort
*******************************************************/
/*
class daePortWrapper : public daePort,
                       public boost::python::wrapper<daePort>
{
public:
    daePortWrapper(void)
    {
    }

    daePortWrapper(string strName, daeePortType eType, daeModel* parent, string strDescription = "")
        : daePort(strName, eType, parent, strDescription)
    {
    }

public:
    std::string GetObjectClassName(void) const
    {
        boost::python::reference_existing_object::apply<const daePort*>::type converter;
        PyObject* pyobj = converter( this );
        boost::python::object obj = boost::python::object( boost::python::handle<>( pyobj ) );
        boost::python::object o_class = obj.attr("__class__");
        string name = boost::python::extract<string>(o_class.attr("__name__"));
        return name;
    }
};
*/

boost::python::list daePort_GetDomains(daePort& self);
boost::python::list daePort_GetParameters(daePort& self);
boost::python::list daePort_GetVariables(daePort& self);

boost::python::dict daePort_dictDomains(daePort& self);
boost::python::dict daePort_dictParameters(daePort& self);
boost::python::dict daePort_dictVariables(daePort& self);

/*******************************************************
    daeEventPort
*******************************************************/
boost::python::list GetEventPortEventsList(daeEventPort& self);

/*******************************************************
    daeEquation
*******************************************************/
daeDEDI* daeEquation_DistributeOnDomain1(daeEquation& self, daeDomain& rDomain, daeeDomainBounds eDomainBounds, const string& strName = string(""));
daeDEDI* daeEquation_DistributeOnDomain2(daeEquation& self, daeDomain& rDomain, boost::python::list l, const string& strName = string(""));
boost::python::list daeEquation_GetEquationExecutionInfos(daeEquation& self);
boost::python::list daeEquation_DistributedEquationDomainInfos(daeEquation& self);

/*******************************************************
    daeEquationExecutionInfo
*******************************************************/
adNode* daeEquationExecutionInfo_GetNode(daeEquationExecutionInfo& self);
boost::python::list daeEquationExecutionInfo_GetVariableIndexes(daeEquationExecutionInfo& self);
boost::python::dict daeEquationExecutionInfo_JacobianExpressions(daeEquationExecutionInfo& self);

/*******************************************************
    daeDEDI
*******************************************************/
daeDomain* daeDEDI_GetDomain(daeDEDI& self);
boost::python::list daeDEDI_GetDomainPoints(daeDEDI& self);

/*******************************************************
    daePortConnection
*******************************************************/
daeObject* daePortConnection_GetPortFrom(daePortConnection& self);
daeObject* daePortConnection_GetPortTo(daePortConnection& self);
boost::python::list daePortConnection_GetEquations(daePortConnection& self);

/*******************************************************
    daeSTN
*******************************************************/
boost::python::list daeSTN_States(daeSTN& self);
boost::python::dict daeSTN_dictStates(daeSTN& self);
std::string daeSTN_GetActiveState(daeSTN& self);
void daeSTN_SetActiveState(daeSTN& self, const std::string& strActiveState);

/*******************************************************
    daeOnEventActions
*******************************************************/
boost::python::list daeOnEventActions_Actions(const daeOnEventActions& self);
boost::python::list daeOnEventActions_UserDefinedActions(const daeOnEventActions& self);

/******************************************************************
    Opaque support or Python threads and GIL support
*******************************************************************/
class pyAllowThreads : public daeAllowThreads_t
{
public:
    pyAllowThreads(bool doNothing_ = true) : doNothing(doNothing_)
    {
        if(doNothing)
            return;

        // Initialise the python interpreter thread-state and implicitly acquire the GIL in the calling thread.
        // If the threads are already initialised do nothing.
        if(!PyEval_ThreadsInitialized())
            PyEval_InitThreads();

        // Now, having acquired the GIL in the calling thread, release it so that the spawned threads can acquire it when required.
        save = PyEval_SaveThread();
    }

    virtual ~pyAllowThreads()
    {
        if(doNothing)
            return;

        // Re-acquire the GIL in the calling thread
        PyEval_RestoreThread(save);
    }

protected:
    const bool     doNothing;
    PyThreadState* save;
};

class pyGILState : public daeGILState_t
{
public:
    pyGILState(bool doNothing_ = true) : doNothing(doNothing_)
    {
        if(doNothing)
            return;

        // Acquire the GIL
        gstate = PyGILState_Ensure();
    }

    virtual ~pyGILState()
    {
        if(doNothing)
            return;

        // Release the thread. No Python API allowed beyond this point
        PyGILState_Release(gstate);
    }

protected:
    const bool       doNothing;
    PyGILState_STATE gstate;
};

/*******************************************************
    daeModel
*******************************************************/
class daeModelWrapper : public daeModel,
                        public boost::python::wrapper<daeModel>
{
public:
    daeModelWrapper(void)
    {
    }

    daeModelWrapper(string strName, daeModel* pModel = NULL, string strDescription = "") : daeModel(strName, pModel, strDescription)
    {
    }

    void DeclareEquations(void)
    {
        if(boost::python::override f = this->get_override("DeclareEquations"))
            f();
        else
            this->daeModel::DeclareEquations();
    }
    void def_DeclareEquations(void)
    {
        this->daeModel::DeclareEquations();
    }

    void UpdateEquations(const daeExecutionContext* pExecutionContext)
    {
        if(boost::python::override f = this->get_override("UpdateEquations"))
            f(pExecutionContext);
        else
            this->daeModel::UpdateEquations(pExecutionContext);
    }
    void def_UpdateEquations(const daeExecutionContext* pExecutionContext)
    {
        this->daeModel::UpdateEquations(pExecutionContext);
    }

    /* If InitializeModel is called on daeModelWrapper object from c++ then daeModelWrapper::InitializeModel
     * will be called: if there is implementation in python it will be called - otherwise the default daeModel
     * implementation. If it is called from python on non-daeModelWrapper object (it is not python object at all)
     * the InitializeModel call of that class will be called.
     */
    void InitializeModel(const std::string& jsonInit)
    {
        if(boost::python::override f = this->get_override("InitializeModel"))
            f(jsonInit);
        else
            this->daeModel::InitializeModel(jsonInit);
    }

    boost::shared_ptr<daeAllowThreads_t> CreateAllowThreads()
    {
        boost::shared_ptr<daeAllowThreads_t> allowThreads(new pyAllowThreads(false));
        return allowThreads;
    }

    boost::shared_ptr<daeGILState_t> CreateGILState()
    {
        boost::shared_ptr<daeGILState_t> GIL(new pyGILState(false));
        return GIL;
    }
};

boost::python::dict daeModel_GetCoSimulationInterface(daeModel& self);
boost::python::dict daeModel_GetFMIInterface(daeModel& self);

void daeModel_def_InitializeModel(daeModel& self, const std::string& jsonInit);

void daeModel_ON_CONDITION(daeModel& self, const daeCondition& rCondition,
                                           boost::python::list switchToStates     = boost::python::list(),
                                           boost::python::list setVariableValues  = boost::python::list(),
                                           boost::python::list triggerEvents      = boost::python::list(),
                                           boost::python::list userDefinedActions = boost::python::list(),
                                           real_t dEventTolerance = 0.0);

void daeModel_ON_EVENT(daeModel& self, daeEventPort* pTriggerEventPort,
                                       boost::python::list switchToStates     = boost::python::list(),
                                       boost::python::list setVariableValues  = boost::python::list(),
                                       boost::python::list triggerEvents      = boost::python::list(),
                                       boost::python::list userDefinedActions = boost::python::list());

boost::python::dict daeModel_GetOverallIndex_BlockIndex_VariableNameMap(daeModel& self);

boost::python::list daeModel_GetDomains(daeModel& self);
boost::python::list daeModel_GetParameters(daeModel& self);
boost::python::list daeModel_GetVariables(daeModel& self);
boost::python::list daeModel_GetPorts(daeModel& self);
boost::python::list daeModel_GetEventPorts(daeModel& self);
boost::python::list daeModel_GetOnEventActions(daeModel& self);
boost::python::list daeModel_GetOnConditionActions(daeModel& self);
boost::python::list daeModel_GetPortArrays(daeModel& self);
boost::python::list daeModel_GetComponents(daeModel& self);
boost::python::list daeModel_GetComponentArrays(daeModel& self);
boost::python::list daeModel_GetSTNs(daeModel& self);
boost::python::list daeModel_GetEquations(daeModel& self);
boost::python::list daeModel_GetPortConnections(daeModel& self);
boost::python::list daeModel_GetEventPortConnections(daeModel& self);

boost::python::dict daeModel_dictDomains(daeModel& self);
boost::python::dict daeModel_dictParameters(daeModel& self);
boost::python::dict daeModel_dictVariables(daeModel& self);
boost::python::dict daeModel_dictPorts(daeModel& self);
boost::python::dict daeModel_dictEventPorts(daeModel& self);
boost::python::dict daeModel_dictOnEventActions(daeModel& self);
boost::python::dict daeModel_dictOnConditionActions(daeModel& self);
boost::python::dict daeModel_dictPortArrays(daeModel& self);
boost::python::dict daeModel_dictComponents(daeModel& self);
boost::python::dict daeModel_dictComponentArrays(daeModel& self);
boost::python::dict daeModel_dictSTNs(daeModel& self);
boost::python::dict daeModel_dictEquations(daeModel& self);
boost::python::dict daeModel_dictPortConnections(daeModel& self);
boost::python::dict daeModel_dictEventPortConnections(daeModel& self);

using dae::tpp::daeThermoPhysicalPropertyPackage_t;
using dae::tpp::daeeThermoPackagePhase;
using dae::tpp::daeeThermoPackageBasis;

void CapeOpen_LoadPackage(daeThermoPhysicalPropertyPackage& self,
                          const std::string& strPackageManager,
                          const std::string& strPackageName,
                          boost::python::list lCompoundIDs,
                          boost::python::list lCompoundCASNumbers,
                          boost::python::dict dAvailablePhases,
                          daeeThermoPackageBasis defaultBasis = dae::tpp::eMole,
                          boost::python::dict dOptions = boost::python::dict());

void CoolProp_LoadPackage(daeThermoPhysicalPropertyPackage& self,
                          boost::python::list lCompoundIDs,
                          boost::python::list lCompoundCASNumbers,
                          boost::python::dict dAvailablePhases,
                          daeeThermoPackageBasis defaultBasis = dae::tpp::eMole,
                          boost::python::dict dOptions = boost::python::dict());

adouble GetCompoundConstant(daeThermoPhysicalPropertyPackage& self,
                            const std::string& property,
                            const std::string& compound);

adouble GetTDependentProperty(daeThermoPhysicalPropertyPackage& self,
                              const std::string& property,
                              boost::python::object T,
                              const std::string& compound);

adouble GetPDependentProperty(daeThermoPhysicalPropertyPackage& self,
                              const std::string& property,
                              boost::python::object P,
                              const std::string& compound);

adouble CalcSinglePhaseScalarProperty(daeThermoPhysicalPropertyPackage& self,
                                      const std::string& property,
                                      boost::python::object P,
                                      boost::python::object T,
                                      boost::python::object x,
                                      const std::string& phase,
                                      daeeThermoPackageBasis basis = dae::tpp::eMole);

adouble_array CalcSinglePhaseVectorProperty(daeThermoPhysicalPropertyPackage& self,
                                            const std::string& property,
                                            boost::python::object P,
                                            boost::python::object T,
                                            boost::python::object x,
                                            const std::string& phase,
                                            daeeThermoPackageBasis basis = dae::tpp::eMole);

adouble CalcTwoPhaseScalarProperty(daeThermoPhysicalPropertyPackage& self,
                                   const std::string& property,
                                   boost::python::object P1,
                                   boost::python::object T1,
                                   boost::python::object x1,
                                   const std::string& phase1,
                                   boost::python::object P2,
                                   boost::python::object T2,
                                   boost::python::object x2,
                                   const std::string& phase2,
                                   daeeThermoPackageBasis basis = dae::tpp::eMole);

adouble_array CalcTwoPhaseVectorProperty(daeThermoPhysicalPropertyPackage& self,
                                         const std::string& property,
                                         boost::python::object P1,
                                         boost::python::object T1,
                                         boost::python::object x1,
                                         const std::string& phase1,
                                         boost::python::object P2,
                                         boost::python::object T2,
                                         boost::python::object x2,
                                         const std::string& phase2,
                                         daeeThermoPackageBasis basis);

double _GetCompoundConstant(daeThermoPhysicalPropertyPackage& self,
                            const std::string& property,
                            const std::string& compound);

double _GetTDependentProperty(daeThermoPhysicalPropertyPackage& self,
                              const std::string& property,
                              double T,
                              const std::string& compound);

double _GetPDependentProperty(daeThermoPhysicalPropertyPackage& self,
                              const std::string& property,
                              double P,
                              const std::string& compound);

double _CalcSinglePhaseScalarProperty(daeThermoPhysicalPropertyPackage& self,
                                      const std::string& property,
                                      double P,
                                      double T,
                                      boost::python::list x,
                                      const std::string& phase,
                                      daeeThermoPackageBasis basis = dae::tpp::eMole);

boost::python::list _CalcSinglePhaseVectorProperty(daeThermoPhysicalPropertyPackage& self,
                                                   const std::string& property,
                                                   double P,
                                                   double T,
                                                   boost::python::list x,
                                                   const std::string& phase,
                                                   daeeThermoPackageBasis basis = dae::tpp::eMole);

double _CalcTwoPhaseScalarProperty(daeThermoPhysicalPropertyPackage& self,
                                   const std::string& property,
                                   double P1,
                                   double T1,
                                   boost::python::list x1,
                                   const std::string& phase1,
                                   double P2,
                                   double T2,
                                   boost::python::list x2,
                                   const std::string& phase2,
                                   daeeThermoPackageBasis basis = dae::tpp::eMole);

boost::python::list _CalcTwoPhaseVectorProperty(daeThermoPhysicalPropertyPackage& self,
                                                const std::string& property,
                                                double P1,
                                                double T1,
                                                boost::python::list x1,
                                                const std::string& phase1,
                                                double P2,
                                                double T2,
                                                boost::python::list x2,
                                                const std::string& phase2,
                                                daeeThermoPackageBasis basis = dae::tpp::eMole);


class daeArrayWrapper : public daeArray<real_t>,
                        public boost::python::wrapper< daeArray<real_t> >
{
public:
    virtual ~daeArrayWrapper(void){}

public:
    real_t GetItem(size_t i) const
    {
        return this->get_override("GetItem")(i);
    }

    void SetItem(size_t i, real_t value)
    {
        this->get_override("SetItem")(i, value);
    }

    size_t GetSize(void) const
    {
        return this->get_override("GetSize")();
    }
};

class daeMatrixWrapper : public daeMatrix<real_t>,
                         public boost::python::wrapper< daeMatrix<real_t> >
{
public:
    virtual ~daeMatrixWrapper(void){}

public:
    real_t GetItem(size_t row, size_t col) const
    {
        return this->get_override("GetItem")(row, col);
    }

    void SetItem(size_t row, size_t col, real_t value)
    {
        this->get_override("SetItem")(row, col, value);
    }

    size_t GetNrows(void) const
    {
        return this->get_override("GetNrows")();
    }

    size_t GetNcols(void) const
    {
        return this->get_override("GetNcols")();
    }
};

class daeArrayWrapper_adouble : public daeArray<adouble>,
                                public boost::python::wrapper< daeArray<adouble> >
{
public:
    virtual ~daeArrayWrapper_adouble(void){}

public:
    adouble GetItem(size_t i) const
    {
        return this->get_override("GetItem")(i);
    }

    void SetItem(size_t i, adouble value)
    {
        this->get_override("SetItem")(i, value);
    }

    size_t GetSize(void) const
    {
        return this->get_override("GetSize")();
    }
};

class daeMatrixWrapper_adouble : public daeMatrix<adouble>,
                                 public boost::python::wrapper< daeMatrix<adouble> >
{
public:
    virtual ~daeMatrixWrapper_adouble(void){}

public:
    adouble GetItem(size_t row, size_t col) const
    {
        return this->get_override("GetItem")(row, col);
    }

    void SetItem(size_t row, size_t col, adouble value)
    {
        this->get_override("SetItem")(row, col, value);
    }

    size_t GetNrows(void) const
    {
        return this->get_override("GetNrows")();
    }

    size_t GetNcols(void) const
    {
        return this->get_override("GetNcols")();
    }
};

/*******************************************************
    daeSparseMatrixRowIteratorWrapper
*******************************************************/
class daeSparseMatrixRowIteratorWrapper : public daeSparseMatrixRowIterator,
                                          public boost::python::wrapper<daeSparseMatrixRowIterator>
{
public:
    daeSparseMatrixRowIteratorWrapper(void)
    {
    }

    void first()
    {
        this->get_override("first")();
    }

    void next()
    {
        this->get_override("next")();
    }

    bool isDone()
    {
        return this->get_override("isDone")();
    }

    unsigned int currentItem()
    {
        return this->get_override("currentItem")();
    }
};

/*******************************************************
    daeSparseMatrixRowIterator_python
*******************************************************/
class daeSparseMatrixRowIterator__iter__
{
public:
    daeSparseMatrixRowIterator__iter__(daeSparseMatrixRowIterator& iter)
        : m_iter(iter)
    {
    }

    unsigned int next()
    {
        if(m_iter.isDone())
        {
            PyErr_SetString(PyExc_StopIteration, "The end of the row reached.");
            boost::python::throw_error_already_set();
        }

        unsigned int item = m_iter.currentItem();
        m_iter.next();
        return item;
    }

public:
    daeSparseMatrixRowIterator& m_iter;
};

daeSparseMatrixRowIterator__iter__* daeSparseMatrixRowIterator_iter(daeSparseMatrixRowIterator& self);

/*******************************************************
    daeFiniteElementObjectWrapper
*******************************************************/
class daeFiniteElementObjectWrapper : public daeFiniteElementObject_t,
                                      public boost::python::wrapper<daeFiniteElementObject_t>
{
public:
    daeFiniteElementObjectWrapper(void)
    {
    }

    void AssembleSystem()
    {
        this->get_override("AssembleSystem")();
    }

    void ReAssembleSystem()
    {
        this->get_override("ReAssembleSystem")();
    }

    bool NeedsReAssembling()
    {
        return this->get_override("NeedsReAssembling")();
    }

    daeSparseMatrixRowIterator* RowIterator(unsigned int row) const
    {
        return this->get_override("RowIterator")(row);
    }

    dae::daeMatrix<real_t>* SystemMatrix() const
    {
        return this->get_override("SystemMatrix")();
    }

    dae::daeMatrix<real_t>* SystemMatrix_dt() const
    {
        return this->get_override("SystemMatrix_dt")();
    }

    dae::daeArray<real_t>* SystemRHS() const
    {
        return this->get_override("SystemRHS")();
    }

    std::vector<std::string> GetVariableNames() const
    {
        boost::python::list l = this->get_override("GetVariableNames")();

        std::vector<std::string> arrNames;
        boost::python::ssize_t n = boost::python::len(l);
        arrNames.resize(n);

        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            arrNames[i] = boost::python::extract<std::string>(l[i]);
        }

        return arrNames;
    }

    unsigned int GetNumberOfPointsInDomainOmega() const
    {
        return this->get_override("GetNumberOfPointsInDomainOmega")();
    }

    std::vector<unsigned int> GetDOFtoBoundaryMap()
    {
        boost::python::list l = this->get_override("GetDOFtoBoundaryMap")();

        std::vector<unsigned int> mapDOFtoBoundary;
        boost::python::ssize_t n = boost::python::len(l);
        mapDOFtoBoundary.resize(n);

        for(boost::python::ssize_t i = 0; i < n; i++)
        {
            mapDOFtoBoundary[i] = boost::python::extract<unsigned int>(l[i]);
        }

        return mapDOFtoBoundary;
    }
};

/*******************************************************
    daeState
*******************************************************/
boost::python::list daeState_GetEquations(daeState& self);
boost::python::list daeState_GetNestedSTNs(daeState& self);
boost::python::list daeState_GetOnEventActions(daeState& self);
boost::python::list daeState_GetOnConditionActions(daeState& self);

/*******************************************************
    daeSTN
*******************************************************/
boost::python::list GetStatesSTN(daeSTN& stn);

/*******************************************************
    daeIF
*******************************************************/

/*******************************************************
    daeScalarExternalFunctionWrapper
*******************************************************/
class daeScalarExternalFunctionWrapper : public daeScalarExternalFunction,
                                         public boost::python::wrapper<daeScalarExternalFunction>
{
public:
    daeScalarExternalFunctionWrapper(const string& strName, daeModel* pModel, const unit& units, boost::python::dict arguments)
        : daeScalarExternalFunction(strName, pModel, units)
    {
        string name;
        boost::python::ssize_t i, n;
        boost::python::tuple t;
        daeExternalFunctionArgumentMap_t mapArguments;

        boost::python::list items = arguments.items();
        n = boost::python::len(items);

        for(i = 0; i < n; i++)
        {
            t = boost::python::extract<boost::python::tuple>(items[i]);
            name  = boost::python::extract<string>(t[0]);
            boost::python::extract<adouble> get_adouble(t[1]);
            boost::python::extract<adouble_array> get_adouble_array(t[1]);

            if(get_adouble.check())
                mapArguments[name] = get_adouble();
            else if(get_adouble_array.check())
                mapArguments[name] = get_adouble_array();
        }
        this->daeExternalFunction_t::SetArguments(mapArguments);
    }

    boost::python::object Calculate_(boost::python::dict values)
    {
        daeDeclareAndThrowException(exNotImplemented);
        return boost::python::object();
    }

    adouble Calculate(daeExternalFunctionArgumentValueMap_t& mapValues) const
    {
        pyGILState GIL(false);

        boost::python::dict values;
        adouble ad_res;

        if(boost::python::override f = this->get_override("Calculate"))
        {
            for(daeExternalFunctionArgumentValueMap_t::iterator iter = mapValues.begin(); iter != mapValues.end(); iter++)
            {
                adouble*              ad    = boost::get<adouble>              (&iter->second);
                std::vector<adouble>* adarr = boost::get<std::vector<adouble> >(&iter->second);

                if(ad)
                {
                    values[iter->first] = *ad;
                }
                else if(adarr)
                {
                    values[iter->first] = getListFromVector(*adarr);
                }
                else
                {
                    daeDeclareAndThrowException(exInvalidCall);
                }
            }

            boost::python::object res = f(values);
            boost::python::extract<adouble> get_adouble(res);
            if(get_adouble.check())
            {
                return get_adouble();
            }
            else
            {
                daeDeclareAndThrowException(exInvalidCall);
                return adouble();
            }
        }
        else
        {
            daeDeclareAndThrowException(exInvalidCall);
            return adouble();
        }

        return ad_res;
    }
};

/*******************************************************
    daeLinearInterpolationFunctionWrapper
*******************************************************/
class daeLinearInterpolationFunctionWrapper : public daeLinearInterpolationFunction,
                                              public boost::python::wrapper<daeLinearInterpolationFunction>
{
public:
    daeLinearInterpolationFunctionWrapper(const string& strName, daeModel* pModel, const unit& units,
                                          boost::python::list x, boost::python::list y, adouble argument)
        : daeLinearInterpolationFunction(strName, pModel, units)
    {
        if(boost::python::len(x) != boost::python::len(y))
        {
            daeDeclareException(exInvalidCall);
            e << "The size of x and y data arrays does not match";
            throw e;
        }

        std::vector<real_t> x_arr;
        std::vector<real_t> y_arr;
        boost::python::ssize_t i, n;

        n = boost::python::len(x);
        for(i = 0; i < n; i++)
        {
            x_arr.push_back( boost::python::extract<real_t>(x[i]) );
            y_arr.push_back( boost::python::extract<real_t>(y[i]) );
        }

        this->InitData(x_arr, y_arr, argument);
    }
};

/*******************************************************
    daeCTypesExternalFunctionWrapper
*******************************************************/
class daeCTypesExternalFunctionWrapper : public daeCTypesExternalFunction,
                                         public boost::python::wrapper<daeCTypesExternalFunction>
{
public:
    daeCTypesExternalFunctionWrapper(const string& strName, daeModel* pModel, const unit& units, boost::python::object ctypes_fun_ptr,  boost::python::dict arguments)
        : daeCTypesExternalFunction(strName, pModel, units, NULL)
    {
        std::string name;
        boost::python::ssize_t i, n;
        boost::python::tuple t;
        daeExternalFunctionArgumentMap_t mapArguments;

        boost::python::object ctypes_addressof = boost::python::import("ctypes").attr("addressof");
        // Take the address of the ctypes._FuncPtr object and cast it to the function pointer.
        uintptr_t fun_ptr = boost::python::extract<uintptr_t>(ctypes_addressof(ctypes_fun_ptr));
        this->m_external_lib_function = *reinterpret_cast<external_lib_function*>(fun_ptr);

        boost::python::list items = arguments.items();
        n = boost::python::len(items);

        for(i = 0; i < n; i++)
        {
            t = boost::python::extract<boost::python::tuple>(items[i]);
            name  = boost::python::extract<string>(t[0]);
            boost::python::extract<adouble> get_adouble(t[1]);
            boost::python::extract<adouble_array> get_adouble_array(t[1]);

            if(get_adouble.check())
                mapArguments[name] = get_adouble();
            else if(get_adouble_array.check())
                daeDeclareAndThrowException(exNotImplemented);
        }
        this->daeExternalFunction_t::SetArguments(mapArguments);
    }
};

/*******************************************************
    daeVectorExternalFunctionWrapper
*******************************************************/
class daeVectorExternalFunctionWrapper : public daeVectorExternalFunction,
                                         public boost::python::wrapper<daeVectorExternalFunction>
{
public:
    daeVectorExternalFunctionWrapper(const string& strName, daeModel* pModel, const unit& units, size_t nNumberofArguments, boost::python::dict arguments)
        : daeVectorExternalFunction(strName, pModel, units, nNumberofArguments)
    {
        string name;
        boost::python::ssize_t i, n;
        boost::python::tuple t;
        daeExternalFunctionArgumentMap_t mapArguments;

        boost::python::list items = arguments.items();
        n = boost::python::len(items);

        for(i = 0; i < n; i++)
        {
            t = boost::python::extract<boost::python::tuple>(items[i]);
            name  = boost::python::extract<string>(t[0]);
            boost::python::extract<adouble> get_adouble(t[1]);
            boost::python::extract<adouble_array> get_adouble_array(t[1]);

            if(get_adouble.check())
                mapArguments[name] = get_adouble();
            else if(get_adouble_array.check())
                mapArguments[name] = get_adouble_array();
        }
        SetArguments(mapArguments);
    }

    boost::python::object Calculate_(boost::python::dict values)
    {
        daeDeclareAndThrowException(exNotImplemented);
        return boost::python::list();
    }

    std::vector<adouble> Calculate(daeExternalFunctionArgumentValueMap_t& mapValues) const
    {
        std::vector<adouble> arrResults;
        boost::python::list results;
        boost::python::dict values;

        if(boost::python::override f = this->get_override("Calculate"))
        {
            for(daeExternalFunctionArgumentValueMap_t::iterator iter = mapValues.begin(); iter != mapValues.end(); iter++)
            {
                adouble*              ad    = boost::get<adouble>              (&iter->second);
                std::vector<adouble>* adarr = boost::get<std::vector<adouble> >(&iter->second);

                if(ad)
                    values[iter->first] = *ad;
                else if(adarr)
                    values[iter->first] = getListFromVector(*adarr);
                else
                    daeDeclareAndThrowException(exInvalidCall);
            }

            boost::python::object res = f(values);
            results = boost::python::extract<boost::python::list>(res);

            boost::python::ssize_t n = boost::python::len(results);
            arrResults.resize(n);

            for(boost::python::ssize_t i = 0; i < n; i++)
            {
                boost::python::extract<adouble> get_adouble(results[i]);
                if(get_adouble.check())
                    arrResults[i] = get_adouble();
                else
                    daeDeclareAndThrowException(exInvalidCall);
            }
            return arrResults;
        }
        else
        {
            daeDeclareAndThrowException(exInvalidCall);
            return arrResults;
        }
    }
};


/*******************************************************
    daeOnConditionActions
*******************************************************/
daeCondition* daeOnConditionActions_Condition(daeOnConditionActions& self);
boost::python::list daeOnConditionActions_Actions(daeOnConditionActions& self);
boost::python::list daeOnConditionActions_UserDefinedActions(daeOnConditionActions& self);

/*******************************************************
    daeObjectiveFunction, daeOptimizationConstraint
*******************************************************/
boost::python::object GetGradientsObjectiveFunction(daeObjectiveFunction& o);
boost::python::object GetGradientsOptimizationConstraint(daeOptimizationConstraint& o);
boost::python::object GetGradientsMeasuredVariable(daeMeasuredVariable& o);

/*******************************************************
    daeLog
*******************************************************/
class daeLogWrapper : public daeLog_t,
                      public boost::python::wrapper<daeLog_t>
{
public:
    void Message(const string& strMessage, size_t nSeverity)
    {
        this->get_override("Message")(strMessage, nSeverity);
    }
};

class daeBaseLogWrapper : public daeBaseLog,
                          public boost::python::wrapper<daeBaseLog>
{
public:
    daeBaseLogWrapper(void)
    {
    }

    void Message(const string& strMessage, size_t nSeverity)
    {
        if(boost::python::override f = this->get_override("Message"))
            f(strMessage, nSeverity);
        else
            this->daeBaseLog::Message(strMessage, nSeverity);
    }

    void def_Message(const string& strMessage, size_t nSeverity)
    {
        this->daeBaseLog::Message(strMessage, nSeverity);
    }

    void SetProgress(real_t dProgress)
    {
        if(boost::python::override f = this->get_override("SetProgress"))
            f(dProgress);
        else
            this->daeBaseLog::SetProgress(dProgress);
    }

    void def_SetProgress(real_t dProgress)
    {
        this->daeBaseLog::SetProgress(dProgress);
    }
};

class daeDelegateLogWrapper : public daeDelegateLog,
                              public boost::python::wrapper<daeDelegateLog>
{
public:
    daeDelegateLogWrapper()
    {
    }

    void Message(const string& strMessage, size_t nSeverity)
    {
        if(boost::python::override f = this->get_override("Message"))
            f(strMessage, nSeverity);
        else
            this->daeDelegateLog::Message(strMessage, nSeverity);
    }

    void def_Message(const string& strMessage, size_t nSeverity)
    {
        this->daeDelegateLog::Message(strMessage, nSeverity);
    }

    boost::python::list GetLogs()
    {
        return getListFromVector(m_ptrarrLogs);
    }
};

class daeFileLogWrapper : public daeFileLog,
                          public boost::python::wrapper<daeFileLog>
{
public:
    daeFileLogWrapper(string strFileName) : daeFileLog(strFileName)
    {
    }

    void Message(const string& strMessage, size_t nSeverity)
    {
        if(boost::python::override f = this->get_override("Message"))
            f(strMessage, nSeverity);
        else
            this->daeFileLog::Message(strMessage, nSeverity);
    }

    void def_Message(const string& strMessage, size_t nSeverity)
    {
        this->daeFileLog::Message(strMessage, nSeverity);
    }
};

class daeStdOutLogWrapper : public daeStdOutLog,
                            public boost::python::wrapper<daeStdOutLog>
{
public:
    daeStdOutLogWrapper(void){}

    void Message(const string& strMessage, size_t nSeverity)
    {
        if(boost::python::override f = this->get_override("Message"))
            f(strMessage, nSeverity);
        else
            this->daeStdOutLog::Message(strMessage, nSeverity);
    }

    void def_Message(const string& strMessage, size_t nSeverity)
    {
        this->daeStdOutLog::Message(strMessage, nSeverity);
    }
};

class daeTCPIPLogWrapper : public daeTCPIPLog,
                           public boost::python::wrapper<daeTCPIPLog>
{
public:
    daeTCPIPLogWrapper()
    {
    }

    void Message(const string& strMessage, size_t nSeverity)
    {
        if(boost::python::override f = this->get_override("Message"))
            f(strMessage, nSeverity);
        else
            this->daeTCPIPLog::Message(strMessage, nSeverity);
    }

    void def_Message(const string& strMessage, size_t nSeverity)
    {
        this->daeTCPIPLog::Message(strMessage, nSeverity);
    }
};

class thread_locker
{
public:
    thread_locker()
    {
    //	if(thread_support::enabled())
            m_gstate = PyGILState_Ensure();
    }

    ~thread_locker()
    {
    //	if(boost::thread_support::enabled())
            PyGILState_Release(m_gstate);
    }
    PyGILState_STATE m_gstate;
};

class daeTCPIPLogServerWrapper : public daeTCPIPLogServer,
                                 public boost::python::wrapper<daeTCPIPLogServer>
{
public:
    daeTCPIPLogServerWrapper(int nPort) : daeTCPIPLogServer(nPort)
    {
    }

    virtual void MessageReceived(const char* szMessage)
    {
        thread_locker lock;
        if(boost::python::override f = this->get_override("MessageReceived"))
            f(szMessage);
        else
            daeTCPIPLogServer::MessageReceived(szMessage);
    }

    void def_MessageReceived(const char* szMessage)
    {
        daeTCPIPLogServer::MessageReceived(szMessage);
    }
};

}

#endif
