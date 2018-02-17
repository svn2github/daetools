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
#include "adouble.h"
#include <math.h>

real_t _makeNaN_()
{
#ifdef NAN
    return NAN;
#else
    return 0.0/0.0;
#endif
}

adouble _adouble_(real_t value, real_t derivative) 
{
    adouble a;
    a.m_dValue = value;
    a.m_dDeriv = derivative;
    return a;
}

real_t _getValue_(const adouble* a)
{
    return a->m_dValue;
}

void _setValue_(adouble* a, const real_t v)
{
    a->m_dValue = v;
}

real_t _getDerivative_(const adouble* a)
{
    return a->m_dDeriv;
}

void _setDerivative_(adouble* a, real_t v)
{
    a->m_dDeriv = v;
}      

adouble _sign_(adouble a)
{
    adouble tmp;
    
    tmp.m_dValue = -a.m_dValue;
    tmp.m_dDeriv = -a.m_dDeriv;
    return tmp;
}

adouble _plus_(const adouble a, const adouble b)
{
    adouble tmp;
    
    tmp.m_dValue = a.m_dValue + b.m_dValue;
    tmp.m_dDeriv = a.m_dDeriv + b.m_dDeriv;
    return tmp;
}

adouble _minus_(const adouble a, const adouble b)
{
    adouble tmp;
    
    tmp.m_dValue = a.m_dValue - b.m_dValue;
    tmp.m_dDeriv = a.m_dDeriv - b.m_dDeriv;
    return tmp;
}

adouble _multi_(const adouble a, const adouble b)
{
    adouble tmp;
    
    tmp.m_dValue = a.m_dValue * b.m_dValue;
    tmp.m_dDeriv = a.m_dDeriv * b.m_dValue + a.m_dValue * b.m_dDeriv;
    return tmp;
}

adouble _divide_(const adouble a, const adouble b)
{
    adouble tmp;
    
    tmp.m_dValue = a.m_dValue / b.m_dValue;
    tmp.m_dDeriv = (a.m_dDeriv * b.m_dValue - a.m_dValue * b.m_dDeriv) / (b.m_dValue * b.m_dValue);
    return tmp;
}

adouble _pow_(const adouble a, const adouble b)
{
    adouble tmp;
    
    if(b.m_dDeriv == 0)
    {
        /* To avoid logarithm of a negative number assume we have pow(adouble, const). */
        tmp.m_dValue = pow(a.m_dValue, b.m_dValue);
        real_t tmp2 = b.m_dValue * pow(a.m_dValue, b.m_dValue-1);
        tmp.m_dDeriv = tmp2 * a.m_dDeriv;
    }
    else if(a.m_dValue <= 0)
    {
        /* Power function called for a negative base. */
        tmp.m_dValue = _makeNaN_();
        tmp.m_dDeriv = _makeNaN_();
    }
    else
    {
        tmp.m_dValue = pow(a.m_dValue, b.m_dValue);
        real_t tmp2 = b.m_dValue * pow(a.m_dValue, b.m_dValue-1);
        real_t tmp3 = log(a.m_dValue) * tmp.m_dValue;
        tmp.m_dDeriv = tmp2 * a.m_dDeriv + tmp3 * b.m_dDeriv;
    }

    return tmp;
}

adouble _log10_(const adouble a)
{
    adouble tmp;
    
    if(a.m_dValue <= 0)
    {
        /* log10(number) = NaN if the number is <= 0 */
        tmp.m_dValue = _makeNaN_();
        tmp.m_dDeriv = _makeNaN_();
    }
    else
    {
        tmp.m_dValue = log10(a.m_dValue);
        real_t tmp2 = log((real_t)10) * a.m_dValue;
        tmp.m_dDeriv = a.m_dDeriv / tmp2;
    }
    return tmp;
}

adouble _exp_(const adouble a)
{
    adouble tmp;
    
    tmp.m_dValue = exp(a.m_dValue);
    tmp.m_dDeriv = tmp.m_dValue * a.m_dDeriv;
    return tmp;
}

adouble _log_(const adouble a)
{
    adouble tmp;
    
    if(a.m_dValue <= 0)
    {
        /* log(number) = NaN if the number is <= 0 */
        tmp.m_dValue = _makeNaN_();
        tmp.m_dDeriv = _makeNaN_();
    }
    else
    {
        tmp.m_dValue = log(a.m_dValue);
        tmp.m_dDeriv = a.m_dDeriv / a.m_dValue;
    }
    return tmp;
}

adouble _sqrt_(const adouble a)
{
    adouble tmp;
    
    /* ACHTUNG, ACHTUNG!!! */
    /* sqrt(number) = NaN if the number is < 0 */
    if(a.m_dValue > 0)
    {
        tmp.m_dValue = sqrt(a.m_dValue);
        tmp.m_dDeriv = a.m_dDeriv / tmp.m_dValue / 2;
    }
    else if(a.m_dValue == 0)
    {
        tmp.m_dValue = 0; /* sqrt(0) = 0 */
        tmp.m_dDeriv = 0; /* number/0 = 0 (Is it??) */
    }
    else
    {
        tmp.m_dValue = _makeNaN_();
        tmp.m_dDeriv = _makeNaN_();
    }
    return tmp;
}

adouble _abs_(const adouble a)
{
    adouble tmp;
    
    tmp.m_dValue = fabs(a.m_dValue);
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

adouble _sin_(const adouble a)
{
    adouble tmp;
    real_t tmp2;
    
    tmp.m_dValue = sin(a.m_dValue);
    tmp2         = cos(a.m_dValue);
    
    tmp.m_dDeriv = tmp2 * a.m_dDeriv;
    return tmp;
}

adouble _cos_(const adouble a)
{
    adouble tmp;
    real_t tmp2;
    
    tmp.m_dValue = cos(a.m_dValue);
    tmp2         = -sin(a.m_dValue);
    
    tmp.m_dDeriv = tmp2 * a.m_dDeriv;
    return tmp;
}

adouble _tan_(const adouble a)
{
    adouble tmp;
    real_t tmp2;
    
    tmp.m_dValue = tan(a.m_dValue);
    tmp2         = cos(a.m_dValue);
    tmp2 *= tmp2;
    
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

adouble _asin_(const adouble a)
{
    adouble tmp;
    
    tmp.m_dValue = asin(a.m_dValue);
    real_t tmp2  = sqrt(1 - a.m_dValue * a.m_dValue);
    
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

adouble _acos_(const adouble a)
{
    adouble tmp;
    
    tmp.m_dValue =  acos(a.m_dValue);
    real_t tmp2  = -sqrt(1 - a.m_dValue*a.m_dValue);
    
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

adouble _atan_(const adouble a)
{
    adouble tmp;
    
    tmp.m_dValue = atan(a.m_dValue);
    real_t tmp2  = 1 + a.m_dValue * a.m_dValue;
    tmp2 = 1 / tmp2;
    if (tmp2 != 0)
        tmp.m_dDeriv = a.m_dDeriv * tmp2;
    else
        tmp.m_dDeriv = 0.0;
    return tmp;
}

adouble _sinh_(const adouble a)
{
    adouble tmp;
    const adouble c1  = {1.0, 0.0};
    const adouble c05 = {0.5, 0.0};

    if(a.m_dValue < 0.0)
    {
        tmp = _exp_(a);
        tmp1 = _divide_(c1, tmp);
        tmp2 = _minus_(tmp, tmp1);
        return _multi_(c05, tmp2);
        /*return 0.5*(tmp - 1.0/tmp);*/
    }
    else
    {
        tmp = _exp_(_sign_(a));
        tmp1 = _divide_(c1, tmp);
        tmp2 = _minus_(tmp1, tmp);
        return _multi_(c05, tmp2);
        /*return 0.5*(1.0/tmp - tmp);*/
    }
    return tmp;
}

adouble _cosh_(const adouble a)
{
    adouble tmp;
    const adouble c1  = {1.0, 0.0};
    const adouble c05 = {0.5, 0.0};

    if(a.m_dValue < 0.0)
        tmp = _exp_(a);
    else
        tmp = _exp_(_sign_(a));
    tmp1 = _divide_(c1, tmp);
    tmp2 = _plus_(tmp, tmp1);
    return _multi_(c05, tmp2);
    /*return 0.5*(tmp + 1.0/tmp);*/
}

adouble _tanh_(const adouble a)
{
    adouble tmp, tmp1, tmp2;
    const adouble c1 = {1.0, 0.0};
    const adouble c2 = {2.0, 0.0};
    if(a.m_dValue < 0.0)
    {
        tmp = _exp_(_multi_(c2, a));
        /*tmp = exp(2.0*a);*/
        tmp1 = _minus_(tmp, c1);
        tmp2 = _plus_(tmp, c1);
        /*return (tmp - 1.0)/(tmp + 1.0);*/
    }
    else
    {
        tmp = _exp_(_multi_(_sign_(c2), a));
        /*tmp = exp(-2.0*a);*/
        tmp1 = _minus_(c1, tmp);
        tmp2 = _plus_(tmp, c1);
        /*return (1.0 - tmp)/(tmp + 1.0);*/
    }
    return _divide_(tmp1, tmp2);
}

adouble _asinh_(const adouble a)
{
    adouble tmp;
    tmp.m_dValue = asinh(a.m_dValue);
    tmp.m_dDeriv = a.m_dDeriv / sqrt(a.m_dValue*a.m_dValue + 1);
    return tmp;
}

adouble _acosh_(const adouble a)
{
    adouble tmp;
    tmp.m_dValue = acosh(a.m_dValue);
    tmp.m_dDeriv = a.m_dDeriv / sqrt(a.m_dValue*a.m_dValue - 1);
    return tmp;
}

adouble _atanh_(const adouble a)
{
    adouble tmp;
    tmp.m_dValue = atanh(a.m_dValue);
    tmp.m_dDeriv = a.m_dDeriv / (1 - a.m_dValue*a.m_dValue);
    return tmp;
}

adouble _erf_(const adouble a)
{
    adouble tmp;
    tmp.m_dValue = erf(a.m_dValue);
    double pi = cos(-1);
    tmp.m_dDeriv = a.m_dDeriv * (2.0 / sqrt(pi)) * exp(-a.m_dValue*a.m_dValue);
    return tmp;
}

adouble _atan2_(const adouble a, const adouble b)
{
    adouble tmp;
    tmp.m_dValue = atan2(a.m_dValue, b.m_dValue);
    double tmp2 = a.m_dValue*a.m_dValue;
    double tmp3 = b.m_dValue*b.m_dValue;
    double tmp4 = tmp3 / (tmp2 + tmp3);
    if(tmp4 != 0)
        tmp.m_dDeriv = (a.m_dDeriv*b.m_dValue - a.m_dValue*b.m_dDeriv) / tmp3*tmp4;
    else
        tmp.m_dDeriv = 0.0;
    return tmp;
}

/* ceil is non-differentiable: should I remove it? */
adouble _ceil_(const adouble a)
{
    adouble tmp;
    
    tmp.m_dValue = ceil(a.m_dValue);    
    tmp.m_dDeriv = 0.0;
    return tmp;
}

/* floor is non-differentiable: should I remove it? */
adouble _floor_(const adouble a)
{
    adouble tmp;
    
    tmp.m_dValue = floor(a.m_dValue);
    tmp.m_dDeriv = 0.0;
    return tmp;
}

adouble _max_(const adouble a, const adouble b)
{
    adouble tmp;
    
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

adouble _min_(const adouble a, const adouble b)
{
    adouble tmp;
    
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

bool _neq_(const adouble a, const adouble b)
{
    return (a.m_dValue != b.m_dValue);
}

bool _eq_(const adouble a, const adouble b)
{
    return (a.m_dValue == b.m_dValue);
}

bool _lteq_(const adouble a, const adouble b)
{
    return (a.m_dValue <= b.m_dValue);
}

bool _gteq_(const adouble a, const adouble b)
{
    return (a.m_dValue >= b.m_dValue);
}

bool _gt_(const adouble a, const adouble b)
{
    return (a.m_dValue > b.m_dValue);
}

bool _lt_(const adouble a, const adouble b)
{
    return (a.m_dValue < b.m_dValue);
}
