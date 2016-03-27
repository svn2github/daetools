/***********************************************************************************
*                 DAE Tools Project: www.daetools.com
*                 Copyright (C) Dragan Nikolic, 2016
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

adouble operator -(const adouble& a)
{
    adouble tmp;
    
    tmp.m_dValue = -a.m_dValue;
    tmp.m_dDeriv = -a.m_dDeriv;
    return tmp;
}

adouble operator +(const adouble& a, const adouble& b)
{
    adouble tmp;
    
    tmp.m_dValue = a.m_dValue + b.m_dValue;
    tmp.m_dDeriv = a.m_dDeriv + b.m_dDeriv;
    return tmp;
}

adouble operator -(const adouble& a, const adouble& b)
{
    adouble tmp;
    
    tmp.m_dValue = a.m_dValue - b.m_dValue;
    tmp.m_dDeriv = a.m_dDeriv - b.m_dDeriv;
    return tmp;
}

adouble operator *(const adouble& a, const adouble& b)
{
    adouble tmp;
    
    tmp.m_dValue = a.m_dValue * b.m_dValue;
    tmp.m_dDeriv = a.m_dDeriv * b.m_dValue + a.m_dValue * b.m_dDeriv;
    return tmp;
}

adouble operator /(const adouble& a, const adouble& b)
{
    adouble tmp;
    
    tmp.m_dValue = a.m_dValue / b.m_dValue;
    tmp.m_dDeriv = (a.m_dDeriv * b.m_dValue - a.m_dValue * b.m_dDeriv) / (b.m_dValue * b.m_dValue);
    return tmp;
}

adouble operator ^(const adouble& a, const adouble& b)
{
    return pow_(a, b);
}

adouble pow_(const adouble& a, const adouble& b)
{
    adouble tmp;
    
    tmp.m_dValue = pow(a.m_dValue, b.m_dValue);
    real_t tmp2 = b.m_dValue * pow(a.m_dValue, b.m_dValue-1);
    real_t tmp3 = log(a.m_dValue) * tmp.m_dValue;
    tmp.m_dDeriv = tmp2 * a.m_dDeriv + tmp3 * b.m_dDeriv;
    return tmp;
}

adouble log10_(const adouble& a)
{
    adouble tmp;
    
    tmp.m_dValue = log10(a.m_dValue);
    real_t tmp2 = log((real_t)10) * a.m_dValue;
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

adouble exp_(const adouble& a)
{
    adouble tmp;
    
    tmp.m_dValue = exp(a.m_dValue);
    tmp.m_dDeriv = tmp.m_dValue * a.m_dDeriv;
    return tmp;
}

adouble log_(const adouble& a)
{
    adouble tmp;
    
    tmp.m_dValue = log(a.m_dValue);
    tmp.m_dDeriv = a.m_dDeriv / a.m_dValue;
    return tmp;
}

adouble sqrt_(const adouble& a)
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

adouble abs_(const adouble& a)
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

adouble sin_(const adouble& a)
{
    adouble tmp;
    real_t tmp2;
    
    tmp.m_dValue = sin(a.m_dValue);
    tmp2         = cos(a.m_dValue);
    
    tmp.m_dDeriv = tmp2 * a.m_dDeriv;
    return tmp;
}

adouble cos_(const adouble& a)
{
    adouble tmp;
    real_t tmp2;
    
    tmp.m_dValue = cos(a.m_dValue);
    tmp2         = -sin(a.m_dValue);
    
    tmp.m_dDeriv = tmp2 * a.m_dDeriv;
    return tmp;
}

adouble tan_(const adouble& a)
{
    adouble tmp;
    real_t tmp2;
    
    tmp.m_dValue = tan(a.m_dValue);
    tmp2         = cos(a.m_dValue);
    tmp2 *= tmp2;
    
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

adouble asin_(const adouble& a)
{
    adouble tmp;
    
    tmp.m_dValue = asin(a.m_dValue);
    real_t tmp2  = sqrt(1 - a.m_dValue * a.m_dValue);
    
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

adouble acos_(const adouble& a)
{
    adouble tmp;
    
    tmp.m_dValue =  acos(a.m_dValue);
    real_t tmp2  = -sqrt(1 - a.m_dValue*a.m_dValue);
    
    tmp.m_dDeriv = a.m_dDeriv / tmp2;
    return tmp;
}

adouble atan_(const adouble& a)
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

adouble sinh_(const adouble& a)
{
    adouble tmp;
    tmp.m_dValue = sinh(a.m_dValue);
    tmp.m_dDeriv = a.m_dDeriv / sqrt(a.m_dValue*a.m_dValue + 1);
    return tmp;
}

adouble cosh_(const adouble& a)
{
    adouble tmp;
    tmp.m_dValue = cosh(a.m_dValue);
    tmp.m_dDeriv = a.m_dDeriv / sqrt(a.m_dValue*a.m_dValue - 1);
    return tmp;
}

adouble tanh_(const adouble& a)
{
    adouble tmp;
    tmp.m_dValue = tanh(a.m_dValue);
    tmp.m_dDeriv = a.m_dDeriv / (1 - a.m_dValue*a.m_dValue);
    return tmp;
}

adouble asinh_(const adouble& a)
{
    adouble tmp;
    tmp.m_dValue = asinh(a.m_dValue);
    tmp.m_dDeriv = a.m_dDeriv / sqrt(a.m_dValue*a.m_dValue + 1);
    return tmp;
}

adouble acosh_(const adouble& a)
{
    adouble tmp;
    tmp.m_dValue = acosh(a.m_dValue);
    tmp.m_dDeriv = a.m_dDeriv / sqrt(a.m_dValue*a.m_dValue - 1);
    return tmp;
}

adouble atanh_(const adouble& a)
{
    adouble tmp;
    tmp.m_dValue = atanh(a.m_dValue);
    tmp.m_dDeriv = a.m_dDeriv / (1 - a.m_dValue*a.m_dValue);
    return tmp;
}

adouble erf_(const adouble& a)
{
    adouble tmp;
    tmp.m_dValue = erf(a.m_dValue);
    double pi = cos(-1);
    tmp.m_dDeriv = a.m_dDeriv * (2.0 / sqrt(pi)) * exp(-a.m_dValue*a.m_dValue);
    return tmp;
}

adouble atan2_(const adouble& a, const adouble& b)
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
adouble ceil_(const adouble& a)
{
    adouble tmp;
    
    tmp.m_dValue = ceil(a.m_dValue);    
    tmp.m_dDeriv = 0.0;
    return tmp;
}

/* floor is non-differentiable: should I remove it? */
adouble floor_(const adouble& a)
{
    adouble tmp;
    
    tmp.m_dValue = floor(a.m_dValue);
    tmp.m_dDeriv = 0.0;
    return tmp;
}

adouble max_(const adouble& a, const adouble& b)
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

adouble min_(const adouble& a, const adouble& b)
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

bool operator !=(const adouble& a, const adouble& b)
{
    return (a.m_dValue != b.m_dValue);
}

bool operator ==(const adouble& a, const adouble& b)
{
    return (a.m_dValue == b.m_dValue);
}

bool operator <=(const adouble& a, const adouble& b)
{
    return (a.m_dValue <= b.m_dValue);
}

bool operator >=(const adouble& a, const adouble& b)
{
    return (a.m_dValue >= b.m_dValue);
}

bool operator >(const adouble& a, const adouble& b)
{
    return (a.m_dValue > b.m_dValue);
}

bool operator <(const adouble& a, const adouble& b)
{
    return (a.m_dValue < b.m_dValue);
}

std::ostream& operator<<(std::ostream& os, const adouble& a)
{
    os << "adouble(" << a.m_dValue << ", " << a.m_dDeriv << ")";
    return os;
}
