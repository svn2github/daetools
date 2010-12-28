#include "stdafx.h"
#include "coreimpl.h"
#include "nodes.h"
#include <limits>
using namespace boost;


namespace dae 
{
namespace core 
{
inline real_t fmin(const real_t &x, const real_t &y) 
{
    if(x < y) 
		return x;
    else 
		return y;
}

inline real_t fmax(const real_t &x, const real_t &y ) 
{
    if(x > y) 
		return x;
    else 
		return y;
}

inline real_t makeNaN() 
{
    if(std::numeric_limits<real_t>::has_quiet_NaN)
		return std::numeric_limits<real_t>::quiet_NaN();
	else// (std::numeric_limits<real_t>::has_signaling_NaN)
		return std::numeric_limits<real_t>::signaling_NaN();
}

/*********************************************************************************************
	adouble
**********************************************************************************************/
adouble::adouble() 
{
	m_dValue = 0.0;
    m_dDeriv = 0.0;
	m_bGatherInfo = false;
}

adouble::adouble(const real_t value)
{
	m_dValue = value;
    m_dDeriv = 0.0;
	m_bGatherInfo = false;
}

adouble::adouble(const real_t value, real_t deriv)
{
	m_dValue = value;
    m_dDeriv = deriv;
	m_bGatherInfo = false;
}

adouble::adouble(const adouble& a)
{
	m_bGatherInfo = false;
	if(a.getGatherInfo())
	{
		m_bGatherInfo = true;
		node  = shared_ptr<adNode>( (a.node ? a.node->Clone() : new adConstantNode(a.m_dValue)) );  
	}
	m_dValue = a.m_dValue;
    m_dDeriv = a.m_dDeriv;
}

adouble::~adouble()
{
}

const adouble adouble::operator - () const 
{
    adouble tmp;
	if(m_bGatherInfo)
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode(eSign, 
			                          CLONE_NODE(node, m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue =- m_dValue;
    tmp.m_dDeriv =- m_dDeriv;
    return tmp;
}

const adouble adouble::operator + () const 
{
	adouble tmp;
	if(m_bGatherInfo)
	{
		tmp.m_bGatherInfo = true;
		tmp.node = CLONE_NODE(node, m_dValue);
	    return tmp;
	}
    tmp.m_dValue = m_dValue;
    tmp.m_dDeriv = m_dDeriv;
	return tmp;
}

//const adouble adouble::operator + (const real_t v) const 
//{
//    adouble tmp;
//	if(adouble::bIsParsing)
//	{
//		tmp.node = shared_ptr<adNode>(new adBinaryNode(ePlus, 
//			                                           CLONE_NODE(node, m_dValue), 
//									                   shared_ptr<adNode>(new adConstantNode(v)) ));
//	    return tmp;
//	}
//
//    tmp.m_dValue = m_dValue + v;
//    tmp.m_dDeriv = m_dDeriv;
//    return tmp;
//}

const adouble operator +(const adouble& a, const real_t v) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(ePlus, 
			                                           CLONE_NODE(a.node, a.m_dValue), 
									                   shared_ptr<adNode>(new adConstantNode(v)) ));
	    return tmp;
	}

    tmp.m_dValue = a.m_dValue + v;
    tmp.m_dDeriv = a.m_dDeriv;
	return tmp;
}

const adouble adouble::operator + (const adouble& a) const 
{
    adouble tmp;
	if(m_bGatherInfo || a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(ePlus, 
			                          CLONE_NODE(node, m_dValue), 
					                  CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}

    tmp.m_dValue = m_dValue + a.m_dValue;
    tmp.m_dDeriv = m_dDeriv + a.m_dDeriv;
    return tmp;
}

const adouble operator +(const real_t v, const adouble& a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(ePlus, 
													   shared_ptr<adNode>(new adConstantNode(v)), 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}

    tmp.m_dValue = v + a.m_dValue;
    tmp.m_dDeriv = a.m_dDeriv;
	return tmp;
}

//const adouble adouble::operator - (const real_t v) const 
//{
//    adouble tmp;
//	if(adouble::bIsParsing)
//	{
//		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMinus, 
//													   CLONE_NODE(node, m_dValue), 
//													   shared_ptr<adNode>(new adConstantNode(v)) ));
//	    return tmp;
//	}
//    tmp.m_dValue = m_dValue - v;
//    tmp.m_dDeriv = m_dDeriv;
//    return tmp;
//}

const adouble operator -(const adouble& a, const real_t v) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMinus, 
													   CLONE_NODE(a.node, a.m_dValue), 
													   shared_ptr<adNode>(new adConstantNode(v)) ));
	    return tmp;
	}
    tmp.m_dValue = a.m_dValue - v;
    tmp.m_dDeriv = a.m_dDeriv;
    return tmp;
}

const adouble adouble::operator - (const adouble& a) const 
{
    adouble tmp;
	if(m_bGatherInfo || a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMinus, 
													   CLONE_NODE(node, m_dValue), 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}

	tmp.m_dValue = m_dValue - a.m_dValue;
    tmp.m_dDeriv = m_dDeriv - a.m_dDeriv;
    return tmp;
}

const adouble operator - (const real_t v, const adouble& a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMinus, 
													   shared_ptr<adNode>(new adConstantNode(v)), 
													   CLONE_NODE(a.node, a.m_dValue)));
	    return tmp;
	}

	tmp.m_dValue = v - a.m_dValue;
    tmp.m_dDeriv =   - a.m_dDeriv;
    return tmp;
}

//const adouble adouble::operator * (const real_t v) const 
//{
//    adouble tmp;
// 	if(adouble::bIsParsing)
//	{
//		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMulti, 
//														CLONE_NODE(node, m_dValue),
//														shared_ptr<adNode>(new adConstantNode(v)) ));  
//	    return tmp;
//	}
//
//    tmp.m_dValue = m_dValue * v;
//    tmp.m_dDeriv = m_dDeriv * v;
//    return tmp;
//}

const adouble operator *(const adouble& a, const real_t v) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMulti, 
														CLONE_NODE(a.node, a.m_dValue),
														shared_ptr<adNode>(new adConstantNode(v)) ));  
	    return tmp;
	}

    tmp.m_dValue = a.m_dValue * v;
    tmp.m_dDeriv = a.m_dDeriv * v;
    return tmp;
}
  
const adouble adouble::operator * (const adouble& a) const 
{
    adouble tmp;
	if(m_bGatherInfo || a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode( eMulti, 
														CLONE_NODE(node,   m_dValue),
														CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}

	tmp.m_dValue = m_dValue * a.m_dValue;
    tmp.m_dDeriv = m_dDeriv * a.m_dValue + m_dValue * a.m_dDeriv;
    return tmp;
}

const adouble operator * (const real_t v, const adouble& a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(	eMulti, 
														shared_ptr<adNode>(new adConstantNode(v)),
														CLONE_NODE(a.node, a.m_dValue) )); 
	    return tmp;
	}
    tmp.m_dValue = v * a.m_dValue;
    tmp.m_dDeriv = v * a.m_dDeriv;
    return tmp;
}

//const adouble adouble::operator / (const real_t v) const 
//{
//    adouble tmp;
// 	if(adouble::bIsParsing)
//	{
//		tmp.node = shared_ptr<adNode>(new adBinaryNode( eDivide, 
//														CLONE_NODE(node, m_dValue),  
//														shared_ptr<adNode>(new adConstantNode(v)) )); 
//	    return tmp;
//	}
//    tmp.m_dValue = m_dValue / v;
//    tmp.m_dDeriv = m_dDeriv / v;
//    return tmp;
//}

const adouble operator /(const adouble& a, const real_t v) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode( eDivide, 
														CLONE_NODE(a.node, a.m_dValue),  
														shared_ptr<adNode>(new adConstantNode(v)) )); 
	    return tmp;
	}
    tmp.m_dValue = a.m_dValue / v;
    tmp.m_dDeriv = a.m_dDeriv / v;
    return tmp;
}

const adouble adouble::operator / (const adouble& a) const 
{
    adouble tmp;
	if(m_bGatherInfo || a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode( eDivide, 
														CLONE_NODE(node,   m_dValue),
														CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue = m_dValue / a.m_dValue;
    tmp.m_dDeriv = (m_dDeriv * a.m_dValue - m_dValue * a.m_dDeriv) / (a.m_dValue * a.m_dValue);
    return tmp;
}

const adouble operator / (const real_t v, const adouble& a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(	eDivide, 
														shared_ptr<adNode>(new adConstantNode(v)), 
														CLONE_NODE(a.node, a.m_dValue) ));  
	    return tmp;
	}
    tmp.m_dValue = v / a.m_dValue;
    tmp.m_dDeriv = (-v * a.m_dDeriv) / (a.m_dValue * a.m_dValue);
    return tmp;
}

const adouble pow(const adouble &a, real_t v) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(	ePower, 
														CLONE_NODE(a.node, a.m_dValue),     
														shared_ptr<adNode>(new adConstantNode(v)) )); 
	    return tmp;
	}

	tmp.m_dValue = ::pow(a.m_dValue, v);
    real_t tmp2 = v * ::pow(a.m_dValue, v-1);
    tmp.m_dDeriv = tmp2 * a.m_dDeriv;
    return tmp;
}

const adouble pow(const adouble &a, const adouble &b) 
{
    adouble tmp;
	if(a.m_bGatherInfo || b.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(	ePower, 
														CLONE_NODE(a.node, a.m_dValue), 
														CLONE_NODE(b.node, b.m_dValue) )); 
	    return tmp;
	}
    tmp.m_dValue = ::pow(a.m_dValue, b.m_dValue);
    real_t tmp2 = b.m_dValue * ::pow(a.m_dValue, b.m_dValue-1);
    real_t tmp3 = ::log(a.m_dValue) * tmp.m_dValue;
    tmp.m_dDeriv = tmp2 * a.m_dDeriv + tmp3 * b.m_dDeriv;
    return tmp;
}

const adouble pow(real_t v, const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(	ePower, 
														shared_ptr<adNode>(new adConstantNode(v)),
														CLONE_NODE(a.node, a.m_dValue) )); 
	    return tmp;
	}
    tmp.m_dValue = ::pow(v, a.m_dValue);
    real_t tmp2 = tmp.m_dValue * ::log(v);
    
    tmp.m_dDeriv = tmp2 * a.m_dDeriv;
    return tmp;
}

const adouble log10(const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eLog, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue = ::log10(a.m_dValue);
    real_t tmp2 = ::log((real_t)10) * a.m_dValue;
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

const adouble exp(const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eExp, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue = ::exp(a.m_dValue);
    tmp.m_dDeriv = tmp.m_dValue * a.m_dDeriv;
    return tmp;
}

const adouble log(const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eLn, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue = ::log(a.m_dValue);
    if (a.m_dValue > 0 || a.m_dValue == 0 && a.m_dDeriv >= 0)  // ????????????????????
		tmp.m_dDeriv = a.m_dDeriv / a.m_dValue;
    else 
		tmp.m_dDeriv = makeNaN();
    return tmp;
}

const adouble sqrt(const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eSqrt, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
	tmp.m_dValue = ::sqrt(a.m_dValue);
	if(a.m_dValue > 0 || a.m_dValue == 0 && a.m_dDeriv >= 0)  // ????????????????????
		tmp.m_dDeriv = a.m_dDeriv / tmp.m_dValue / 2;
	else 
		tmp.m_dDeriv = makeNaN();
	return tmp;
}

const adouble abs(const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eAbs, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
	tmp.m_dValue = ::fabs(a.m_dValue);
	int as = 0;
	if(a.m_dValue > 0) 
		as = 1;
	if(a.m_dValue < 0) 
		as = -1;
	if(as != 0)        
	{
		tmp.m_dDeriv = a.m_dDeriv * as;
	}
	else
	{
		as = 0;
		if(a.m_dDeriv > 0) 
			as = 1;
		if(a.m_dDeriv < 0) 
			as = -1;
		tmp.m_dDeriv = a.m_dDeriv * as;
	}
	return tmp;
}

const adouble sin(const adouble &a) 
{
    adouble tmp;
    real_t tmp2;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eSin, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue = ::sin(a.m_dValue);
    tmp2         = ::cos(a.m_dValue);
    
    tmp.m_dDeriv = tmp2 * a.m_dDeriv;
    return tmp;
}

const adouble cos(const adouble &a) 
{
    adouble tmp;
    real_t tmp2;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eCos, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue = ::cos(a.m_dValue);
    tmp2         = -::sin(a.m_dValue);
    
    tmp.m_dDeriv = tmp2 * a.m_dDeriv;
    return tmp;
}

const adouble tan(const adouble& a) 
{
    adouble tmp;
    real_t tmp2;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eTan, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue = ::tan(a.m_dValue);
    tmp2         = ::cos(a.m_dValue);
    tmp2 *= tmp2;
    
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

const adouble asin(const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eArcSin, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue = ::asin(a.m_dValue);
    real_t tmp2  = ::sqrt(1 - a.m_dValue * a.m_dValue);
    
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

const adouble acos(const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eArcCos, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue =  ::acos(a.m_dValue);
    real_t tmp2  = -::sqrt(1-a.m_dValue*a.m_dValue);
    
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

const adouble atan(const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eArcTan, 
													   CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}
    tmp.m_dValue = ::atan(a.m_dValue);
    real_t tmp2  = 1 + a.m_dValue * a.m_dValue;
    tmp2 = 1 / tmp2;
    if (tmp2 != 0)
        tmp.m_dDeriv = a.m_dDeriv * tmp2;
    else
        tmp.m_dDeriv = 0.0;
    return tmp;
}

const adouble ceil(const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eCeil, 
									  			       CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}

	tmp.m_dValue = ::ceil(a.m_dValue);    
    tmp.m_dDeriv = 0.0;
    return tmp;
}

const adouble floor(const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adUnaryNode( eFloor, 
									  			       CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}

	tmp.m_dValue = ::floor(a.m_dValue);
    tmp.m_dDeriv = 0.0;
    return tmp;
}

const adouble max(const adouble &a, const adouble &b) 
{
    adouble tmp;
	if(a.getGatherInfo() || b.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMax, 
									  			       CLONE_NODE(a.node, a.m_dValue),
									  			       CLONE_NODE(b.node, b.m_dValue) ));
	    return tmp;
	}

	real_t tmp2 = a.m_dValue - b.m_dValue;
	if(tmp2 < 0) 
	{
		tmp.m_dValue = b.m_dValue;
		tmp.m_dDeriv = b.m_dDeriv;
	} 
	else 
	{
		tmp.m_dValue = a.m_dValue;
		if(tmp2 > 0) 
		{            
			tmp.m_dDeriv = a.m_dDeriv;
		} 
		else 
		{
			if(a.m_dDeriv < b.m_dDeriv) 
				tmp.m_dDeriv = b.m_dDeriv;
			else 
				tmp.m_dDeriv = a.m_dDeriv;
		}
	}
	return tmp;
}

const adouble max(real_t v, const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMax, 
									  			       shared_ptr<adNode>(new adConstantNode(v)),
									  			       CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}

	real_t tmp2 = v - a.m_dValue;
	if(tmp2 < 0)
	{
		tmp.m_dValue = a.m_dValue;
		tmp.m_dDeriv = a.m_dDeriv;
	} 
	else 
	{
		tmp.m_dValue = v;
		if(tmp2 > 0) 
		{
			tmp.m_dDeriv = 0.0;
		} 
		else 
		{
			if(a.m_dDeriv > 0)
				tmp.m_dDeriv = a.m_dDeriv;
			else 
				tmp.m_dDeriv = 0.0;
		}
	}
	return tmp;
}

const adouble max(const adouble &a, real_t v) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMax, 
									  			       CLONE_NODE(a.node, a.m_dValue),
									  			       shared_ptr<adNode>(new adConstantNode(v)) ));
	    return tmp;
	}

	real_t tmp2 = a.m_dValue - v;
	if(tmp2 < 0) 
	{
		tmp.m_dValue = v;
		tmp.m_dDeriv = 0.0;
	} 
	else 
	{
		tmp.m_dValue = a.m_dValue;
		if(tmp2 > 0)
		{
			tmp.m_dDeriv = a.m_dDeriv;
		} 
		else 
		{
			if(a.m_dDeriv > 0) 
				tmp.m_dDeriv = a.m_dDeriv;
			else 
				tmp.m_dDeriv = 0.0;
		}
	}
	return tmp;
}

const adouble min(const adouble &a, const adouble &b) 
{
    adouble tmp;
	if(a.getGatherInfo() || b.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMin, 
									  			       CLONE_NODE(a.node, a.m_dValue),
									  			       CLONE_NODE(b.node, b.m_dValue) ));
	    return tmp;
	}

	real_t tmp2 = a.m_dValue - b.m_dValue;
	if(tmp2 < 0) 
	{
		tmp.m_dValue = a.m_dValue;
		tmp.m_dDeriv = a.m_dDeriv;
	} 
	else 
	{
		tmp.m_dValue = b.m_dValue;
		if(tmp2 > 0) 
		{
			tmp.m_dDeriv = b.m_dDeriv;
		} 
		else 
		{
			if(a.m_dDeriv < b.m_dDeriv)
				tmp.m_dDeriv = a.m_dDeriv;
			else 
				tmp.m_dDeriv = b.m_dDeriv;
		}
	}
	return tmp;
}

const adouble min(real_t v, const adouble &a) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMin, 
									  			       shared_ptr<adNode>(new adConstantNode(v)),
									  			       CLONE_NODE(a.node, a.m_dValue) ));
	    return tmp;
	}

	real_t tmp2 = v - a.m_dValue;
	if(tmp2 < 0) 
	{
		tmp.m_dValue = v;
		tmp.m_dDeriv = 0.0;
	} 
	else 
	{
		tmp.m_dValue = a.m_dValue;
		if(tmp2 > 0) 
		{
			tmp.m_dDeriv = a.m_dDeriv;
		} 
		else 
		{
			if(a.m_dDeriv < 0) 
				tmp.m_dDeriv = a.m_dDeriv;
			else 
				tmp.m_dDeriv = 0.0;
		}
	}
	return tmp;
}

const adouble min(const adouble &a, real_t v) 
{
    adouble tmp;
	if(a.getGatherInfo())
	{
		tmp.m_bGatherInfo = true;
		tmp.node = shared_ptr<adNode>(new adBinaryNode(eMin, 
									  			       CLONE_NODE(a.node, a.m_dValue),
									  			       shared_ptr<adNode>(new adConstantNode(v)) ));
	    return tmp;
	}

	real_t tmp2 = a.m_dValue - v;
	if(tmp2 < 0) 
	{
		tmp.m_dValue = a.m_dValue;
		tmp.m_dDeriv = a.m_dDeriv;
	} 
	else 
	{
		tmp.m_dValue = v;
		if(tmp2 > 0) 
		{
			tmp.m_dDeriv = 0.0;
		} 
		else 
		{
			if(a.m_dDeriv < 0) 
				tmp.m_dDeriv = a.m_dDeriv;
			else 
				tmp.m_dDeriv = 0.0;
		}
	}
	return tmp;
}

void adouble::operator =(const real_t v) 
{
	//if(getGatherInfo())
	//{
		if(node)
			node.reset();
	//}
    m_dValue = v;
    m_dDeriv = 0.0;
}

void adouble::operator =(const adouble& a) 
{
	if(m_bGatherInfo || a.getGatherInfo())
	{
		m_bGatherInfo = true;
		node = shared_ptr<adNode>(  (a.node ? a.node->Clone() : new adConstantNode(a.getValue()))  );
	}
    m_dValue = a.m_dValue;
    m_dDeriv = a.m_dDeriv;
}

daeCondition adouble::operator != (const adouble &a) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eNotEQ, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator != (const real_t v) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eNotEQ, v);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition operator != (const real_t v, const adouble &a) 
{
 	condExpressionNode* expr = new condExpressionNode(v, eNotEQ, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator == (const adouble &a) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eEQ, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator == (const real_t v) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eEQ, v);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition operator == (const real_t v, const adouble &a) 
{
 	condExpressionNode* expr = new condExpressionNode(v, eEQ, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator <= (const adouble &a) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eLTEQ, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator <= (const real_t v) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eLTEQ, v);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition operator <= (const real_t v, const adouble &a) 
{
	condExpressionNode* expr = new condExpressionNode(v, eLTEQ, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator >= (const adouble &a) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eGTEQ, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator >= (const real_t v) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eGTEQ, v);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition operator >= (const real_t v, const adouble &a) 
{
	condExpressionNode* expr = new condExpressionNode(v, eGTEQ, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator > (const adouble &a) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eGT, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator > (const real_t v) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eGT, v);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition operator > (const real_t v, const adouble &a) 
{
	condExpressionNode* expr = new condExpressionNode(v, eGT, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator <  (const adouble &a) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eLT, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition adouble::operator <  (const real_t v) const 
{
	condExpressionNode* expr = new condExpressionNode(*this, eLT, v);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

daeCondition operator <  (const real_t v, const adouble &a)
{
	condExpressionNode* expr = new condExpressionNode(v, eLT, a);
	shared_ptr<condNode> node(expr);
	daeCondition cond(node);
    return cond;
}

}
}
