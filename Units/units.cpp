#include "units.h"
#include <algorithm>

namespace units
{
/**************************************************************
		Some handy functions
**************************************************************/
inline std::string to_string(const std::string& name, double exponent)
{
	if(exponent == 0)
		return std::string();
	if(exponent == 1)
		return name;
	if((int)exponent == exponent)
		return (boost::format("%1%^%2%") % name % (int)exponent).str();
	else	
		return (boost::format("%1%^%2%") % name % exponent).str();
}

inline void to_string_and_add(const std::string& name, double exponent, std::vector<std::string>& arrUnits)
{
	std::string res = to_string(name, exponent);
	if(!res.empty())
		arrUnits.push_back(res);
}
	
/**************************************************************
		base_unit
**************************************************************/
base_unit::base_unit(void)
{
	L          = 0; // length, m
	M          = 0; // mass, kg
	T          = 0; // time, s
	C          = 0; // luminous intensity, cd
	I          = 0; // el. current, A
	O          = 0; // temperature, K
	N          = 0; // amount of a substance, mol
	multiplier = 1.0;	
}

base_unit::base_unit(double multi, double _L, double _M, double _T, 
		             double _C, double _I, double _O, double _N)
{
	L          = _L; // length, m
	M          = _M; // mass, kg
	T          = _T; // time, s
	C          = _C; // luminous intensity, cd
	I          = _I; // el. current, A
	O          = _O; // temperature, K
	N          = _N; // amount of a substance, mol
	multiplier = multi;	
}

bool base_unit::operator==(const base_unit& other) const
{
	return (multiplier == other.multiplier) && areDimensionsEqual(other);
}

bool base_unit::operator!=(const base_unit& other) const
{
	return !(*this == other);
}

bool base_unit::areDimensionsEqual(const base_unit& other) const
{
	return ((M == other.M) && (L == other.L) && (T == other.T) && (C == other.C) &&
			(I == other.I) && (O == other.O) && (N == other.N));
}

base_unit base_unit::operator*(const base_unit& other) const
{
	base_unit tmp;
	tmp.L = L + other.L;
	tmp.M = M + other.M;
	tmp.T = T + other.T;
	tmp.C = C + other.C;
	tmp.I = I + other.I;
	tmp.O = O + other.O;
	tmp.N = N + other.N;
	tmp.multiplier = multiplier * other.multiplier;
	return tmp;
}

base_unit base_unit::operator*(double multi) const
{
	base_unit tmp = *this;
	tmp.multiplier = multiplier * multi;
	return tmp;
}

base_unit base_unit::operator/(const base_unit& other) const
{
	base_unit tmp;
	tmp.L = L - other.L;
	tmp.M = M - other.M;
	tmp.T = T - other.T;
	tmp.C = C - other.C;
	tmp.I = I - other.I;
	tmp.O = O - other.O;
	tmp.N = N - other.N;
	tmp.multiplier = multiplier / other.multiplier;
	return tmp;
}

base_unit base_unit::operator/(double multi) const
{
	base_unit tmp = *this;
	tmp.multiplier = multiplier / multi;
	return tmp;
}

base_unit base_unit::operator^(double exponent) const
{
	base_unit tmp;
	tmp.L = L * exponent;
	tmp.M = M * exponent;
	tmp.T = T * exponent;
	tmp.C = C * exponent;
	tmp.I = I * exponent;
	tmp.O = O * exponent;
	tmp.N = N * exponent;
	tmp.multiplier = ::pow(multiplier, exponent);
	return tmp;		
}

base_unit base_unit::operator^(const base_unit& other) const
{
	throw units_error((boost::format("Invalid operation: %1% ^ %2%") % *this % other).str());
	return base_unit();
}

base_unit base_unit::operator+(const base_unit& other) const
{
	throw units_error((boost::format("Invalid operation: %1% + %2%") % *this % other).str());
	return base_unit();	
}

base_unit base_unit::operator+(double value) const
{
	throw units_error((boost::format("Invalid operation: %1% + %2%") % *this % value).str());
	return base_unit();	
}

base_unit base_unit::operator+(void) const
{
	throw units_error((boost::format("Invalid operation: +%1%") % *this).str());
	return base_unit();	
}

base_unit base_unit::operator-(const base_unit& other) const
{
	throw units_error((boost::format("Invalid operation: %1% - %2%") % *this % other).str());
	return base_unit();	
}

base_unit base_unit::operator-(double value) const
{
	throw units_error((boost::format("Invalid operation: %1% - %2%") % *this % value).str());
	return base_unit();	
}

base_unit base_unit::operator-(void) const
{
	throw units_error((boost::format("Invalid operation: -%1%") % *this).str());
	return base_unit();	
}

std::string base_unit::toString(bool bUnitsOnly) const
{
	std::vector<std::string> arrUnits;
	
	to_string_and_add("m",   L, arrUnits);
	to_string_and_add("kg",  M, arrUnits);
	to_string_and_add("s",   T, arrUnits);
	to_string_and_add("cd",  C, arrUnits);
	to_string_and_add("A",   I, arrUnits);
	to_string_and_add("K",   O, arrUnits);
	to_string_and_add("mol", N, arrUnits);
	
	std::string _units = boost::algorithm::join(arrUnits, __string_unit_delimiter__);
	if(bUnitsOnly)
		return (boost::format("%s") % _units).str();
	else
		return (boost::format("%.10f [%s]") % multiplier % _units).str();
}

std::ostream& operator<<(std::ostream& out, const base_unit& u)
{
	out << u.toString();
	return out;
}
	
base_unit operator*(double value, const base_unit& self)
{
	return self * value;
}

base_unit operator/(double value, const base_unit& self)
{
	return value * (self ^ (-1.0));
}

base_unit operator+(double value, const base_unit& self)
{
	throw units_error((boost::format("Invalid operation: %1% + %2%") % value % self).str());
	return base_unit();	
}

base_unit operator-(double value, const base_unit& self)
{
	throw units_error((boost::format("Invalid operation: %1% - %2%") % value % self).str());
	return base_unit();	
}

base_unit pow(const base_unit& self, double exponent)
{
	return self ^ exponent;
}

base_unit pow(const base_unit& self, const base_unit& exponent)
{
	return self ^ exponent;
}

/**************************************************************
		unit
**************************************************************/
unit::unit(std::string u1, double exp1,
	 std::string u2, double exp2,
	 std::string u3, double exp3,
	 std::string u4, double exp4,
	 std::string u5, double exp5,
	 std::string u6, double exp6,
	 std::string u7, double exp7,
	 std::string u8, double exp8)
{
	addUnit(u1, exp1);
	addUnit(u2, exp2);
	addUnit(u3, exp3);
	addUnit(u4, exp4);
	addUnit(u5, exp5);
	addUnit(u6, exp6);
	addUnit(u7, exp7);
	addUnit(u8, exp8);
}

unit::unit(const std::map<std::string, double>& mapUnits)
{
	setUnits(mapUnits);
}

void unit::addUnit(std::string name, double exp)
{
	if(name.empty())
		return;
	std::map<std::string, base_unit>& __base_units__ = get_base_units();
	std::map<std::string, base_unit>::const_iterator iter = __base_units__.find(name);
	if(iter == __base_units__.end())
		throw units_error((boost::format("Cannot find the base_unit: %1%") % name).str());
	units[name] = exp;
}

void unit::setUnits(const std::map<std::string, double>& mapUnits)
{
	units.clear();
	for(std::map<std::string, double>::const_iterator iter = mapUnits.begin(); iter != mapUnits.end(); iter++)
		addUnit(iter->first, iter->second);
}

base_unit unit::getBaseUnit(void) const
{
	double exp;
	std::string name;
	base_unit tmp(1.0, 0, 0, 0, 0, 0, 0, 0);
	std::map<std::string, base_unit>& __base_units__ = get_base_units();
	
	for(std::map<std::string, double>::const_iterator iter = units.begin(); iter != units.end(); iter++)
	{
		name = (*iter).first;
		exp  = (*iter).second;
		tmp = tmp * (__base_units__[name] ^ exp);
	}
	return tmp;
}

bool unit::operator==(const unit& other) const
{
	return (getBaseUnit() == other.getBaseUnit());
}

bool unit::operator!=(const unit& other) const
{
	return !(*this == other);
}

unit unit::operator*(const unit& other) const
{
	unit tmp(*this);
	for(std::map<std::string, double>::const_iterator iter = other.units.begin(); iter != other.units.end(); iter++)
	{
		std::string name = (*iter).first;
		double      exp  = (*iter).second;
		
		if(tmp.units.find(name) == tmp.units.end())
			tmp.units[name] = exp;
		else
			tmp.units[name] = tmp.units[name] + exp;
	}
	return tmp;
}

unit unit::operator/(const unit& other) const
{
	unit tmp(*this);
	for(std::map<std::string, double>::const_iterator iter = other.units.begin(); iter != other.units.end(); iter++)
	{
		std::string name = (*iter).first;
		double      exp  = (*iter).second;
		
		if(tmp.units.find(name) == tmp.units.end())
			tmp.units[name] = -exp;
		else
			tmp.units[name] = tmp.units[name] - exp;
	}
	return tmp;
}

unit unit::operator^(double exponent) const
{
	unit tmp;
	for(std::map<std::string, double>::const_iterator iter = units.begin(); iter != units.end(); iter++)
	{
		std::string name = (*iter).first;
		double      exp  = (*iter).second;
		tmp.units[name]  = exp * exponent;
	}
	return tmp;
}

unit unit::operator^(const unit& other) const
{
	throw units_error((boost::format("Invalid operation: %1% ^ %2%") % *this % other).str());
	return unit();	
}

unit unit::operator+(const unit& other) const
{
	throw units_error((boost::format("Invalid operation: %1% + %2%") % *this % other).str());
	return unit();	
}

unit unit::operator+(double value) const
{
	throw units_error((boost::format("Invalid operation: %1% + %2%") % *this % value).str());
	return unit();	
}

unit unit::operator+(void) const
{
	throw units_error((boost::format("Invalid operation: +%1%") % *this).str());
	return unit();	
}

unit unit::operator-(const unit& other) const
{
	throw units_error((boost::format("Invalid operation: %1% - %2%") % *this % other).str());
	return unit();	
}

unit unit::operator-(double value) const
{
	throw units_error((boost::format("Invalid operation: %1% - %2%") % *this % value).str());
	return unit();	
}

unit unit::operator-(void) const
{
	throw units_error((boost::format("Invalid operation: -%1%") % *this).str());
	return unit();	
}

//std::string unit::toString(void) const
//{
//	std::vector<std::string> arrUnits;
//	for(std::map<std::string, double>::const_iterator iter = units.begin(); iter != units.end(); iter++)
//	{
//		std::string name = (*iter).first;
//		double      exp  = (*iter).second;
//		to_string_and_add(name, exp, arrUnits);
//	}
//	return boost::algorithm::join(arrUnits, __string_unit_delimiter__);
//}
std::string unit::toString(void) const
{
	std::string strPositive, strNegative;
	std::vector<std::string> arrPositive, arrNegative;
	
	for(std::map<std::string, double>::const_iterator iter = units.begin(); iter != units.end(); iter++)
	{
		std::string name = (*iter).first;
		double      exp  = (*iter).second;
		if(exp > 0)
			to_string_and_add(name, exp, arrPositive);
		else
			to_string_and_add(name, ::abs(exp), arrNegative);
	}
	
	strPositive = boost::algorithm::join(arrPositive, __string_unit_delimiter__);
	strNegative = boost::algorithm::join(arrNegative, __string_unit_delimiter__);
	
	if(arrNegative.size() == 0)
		return strPositive;
	else if(arrNegative.size() == 1)
		return strPositive + "/" + strNegative;
	else
		return strPositive + "/(" + strNegative + ")"; 
}

std::ostream& operator<<(std::ostream& out, const unit& u)
{
	out << u.toString();
	return out;
}

inline void fill_base_units(std::map<std::string, base_unit>& __base_units__)
{
	__base_units__["kg"] = base_units_pool::kg;
	__base_units__["m"] = base_units_pool::m;
	__base_units__["s"] = base_units_pool::s;
	__base_units__["cd"] = base_units_pool::cd;
	__base_units__["A"] = base_units_pool::A;
	__base_units__["K"] = base_units_pool::K;
	__base_units__["mol"] = base_units_pool::mol;
	
	__base_units__["ms"] = base_units_pool::ms;
	__base_units__["us"] = base_units_pool::us;
	__base_units__["min"] = base_units_pool::min;
	__base_units__["hour"] = base_units_pool::hour;
	__base_units__["day"] = base_units_pool::day;
	__base_units__["Hz"] = base_units_pool::Hz;
	__base_units__["kHz"] = base_units_pool::kHz;
	__base_units__["MHz"] = base_units_pool::MHz;
	
	__base_units__["km"] = base_units_pool::km;
	__base_units__["dm"] = base_units_pool::dm;
	__base_units__["cm"] = base_units_pool::cm;
	__base_units__["mm"] = base_units_pool::mm;
	__base_units__["um"] = base_units_pool::um;
	__base_units__["nm"] = base_units_pool::nm;
	
	__base_units__["l"] = base_units_pool::lit;
	__base_units__["dl"] = base_units_pool::dl;
	
	__base_units__["N"] = base_units_pool::N;
	__base_units__["J"] = base_units_pool::J;
	__base_units__["kJ"] = base_units_pool::kJ;
	__base_units__["W"] = base_units_pool::W;
	__base_units__["kW"] = base_units_pool::kW;
	
	__base_units__["C"] = base_units_pool::C;
	__base_units__["F"] = base_units_pool::F;
	__base_units__["Ohm"] = base_units_pool::Ohm;
	__base_units__["V"] = base_units_pool::V;
	__base_units__["mV"] = base_units_pool::mV;
	__base_units__["mA"] = base_units_pool::mA;
	
	__base_units__["Pa"] = base_units_pool::Pa;
	__base_units__["kPa"] = base_units_pool::kPa;
	__base_units__["MPa"] = base_units_pool::MPa;
	
	__base_units__["P"] = base_units_pool::P;
	__base_units__["St"] = base_units_pool::St;
	
	//for(std::map<std::string, base_unit>::iterator iter = __base_units__.begin(); iter != __base_units__.end(); iter++)
	//	std::cout << (*iter).first << " = " << (*iter).second.toString() << std::endl;
}

std::map<std::string, base_unit>& unit::get_base_units(void)
{
	static std::map<std::string, base_unit> __base_units__;
	fill_base_units(__base_units__);
	return __base_units__;
}

quantity unit::operator*(double multi) const
{
    return quantity(multi, *this);
}

quantity operator*(double multi, const unit& self)
{
    return quantity(multi, self);
}

quantity unit::operator/(double multi) const
{
    return quantity(1.0 / multi, *this);
}

quantity operator/(double multi, const unit& self)
{
    return quantity(multi, self ^ (-1));
}

unit operator+(double value, const unit& self)
{
	throw units_error((boost::format("Invalid operation: %1% + %2%") % value % self).str());
	return unit();	
}

unit operator-(double value, const unit& self)
{
	throw units_error((boost::format("Invalid operation: %1% - %2%") % value % self).str());
	return unit();	
}

unit pow(const unit& self, const unit& other)
{
	return self ^ other;
}

unit pow(const unit& self, double exponent)
{
	return self ^ exponent;
}


}
