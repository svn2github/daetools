#include "stdafx.h"
#include "python_wraps.h"
#define PY_ARRAY_UNIQUE_SYMBOL dae_extension
#include "docstrings.h"
#include <noprefix.h>
using namespace boost::python;

BOOST_PYTHON_MODULE(pyUnits)
{
	import_array(); 
	boost::python::numeric::array::set_module_and_type("numpy", "ndarray"); 
    
    docstring_options doc_options(true, true, false);
  
/**************************************************************
	base_unit
***************************************************************/
    class_<base_unit>("base_unit", DOCSTR_base_unit)
        .def("__init__",	make_constructor(daepython::__init__base_unit))

        .def_readonly("L", &base_unit::L, DOCSTR_base_unit_L)
        .def_readonly("M", &base_unit::M, DOCSTR_base_unit_M)
        .def_readonly("T", &base_unit::T, DOCSTR_base_unit_T)
        .def_readonly("C", &base_unit::C, DOCSTR_base_unit_C)
        .def_readonly("I", &base_unit::I, DOCSTR_base_unit_I)
        .def_readonly("O", &base_unit::O, DOCSTR_base_unit_O)
        .def_readonly("N", &base_unit::N, DOCSTR_base_unit_N)
        .def_readonly("multiplier", &base_unit::multiplier, DOCSTR_base_unit_multiplier)

        .def("__str__",		&daepython::base_unit__str__)
		.def("__repr__",	&daepython::base_unit__repr__) 
		
		.def(self * self)
		.def(self / self)
		
		.def(self == self)
		.def(self != self)

		.def(self * double())
		.def(self / double())
		.def(pow(self, double()))

		.def(double() * self)
		.def(double() / self)
		;
	
/**************************************************************
	unit
***************************************************************/
    class_<unit>("unit", DOCSTR_unit)
        .def("__init__",	make_constructor(daepython::__init__unit), DOCSTR_unit_init)

        .def("__str__",		&daepython::unit__str__)
		.def("__repr__",	&daepython::unit__repr__)

        .add_property("baseUnit",	    &unit::getBaseUnit,              DOCSTR_unit_baseUnit)
        .add_property("unitDictionary",	&daepython::unit_get_units_dict, DOCSTR_unit_unitDictionary)
        
		.def(self * self)
		.def(self / self)
		
		.def(self == self)
		.def(self != self)

		.def(self * double())
		.def(self / double())
		.def(pow(self, double()))

		.def(double() * self)
		.def(double() / self)
		;

    class_<quantity>("quantity", DOCSTR_quantity)
        .def(init<double, const unit&>(( arg("self"), arg("value"), arg("unit") ), DOCSTR_quantity_init))
		
        .add_property("valueInSIUnits",	&quantity::getValueInSIUnits,                       DOCSTR_quantity_valueInSIUnits)
        .add_property("value",			&quantity::getValue, &daepython::quantity_setValue, DOCSTR_quantity_value)
        .add_property("units",			&quantity::getUnits, &quantity::setUnits,           DOCSTR_quantity_units)
        
		.def("__str__",		&daepython::quantity__str__)
		.def("__repr__",	&daepython::quantity__repr__)
        .def("scaleTo",		&daepython::quantity_scaleTo, ( arg("self"), arg("referrer") ), DOCSTR_quantity_scaleTo)
        
		.def(- self)
		.def(+ self)

		.def(self + self)
		.def(self - self)
		.def(self * self)
		.def(self / self)
		.def(pow(self, self))
		.def(self == self)
		.def(self != self)
		.def(self <  self)
		.def(self <= self)
		.def(self >  self)
		.def(self >= self)

		.def(self * unit())
		.def(self / unit())

		.def(self + double())
		.def(self - double())
		.def(self * double())
		.def(self / double())
		.def(pow(self, double()))
		.def(self == double())
		.def(self != double())
		.def(self <  double())
		.def(self <= double())
		.def(self >  double())
		.def(self >= double())

		.def(double() + self)
		.def(double() - self)
		.def(double() * self)
		.def(double() / self)
		.def(pow(double(), self))
		.def(double() == self)
		.def(double() != self)
		.def(double() <  self)
		.def(double() <= self)
		.def(double() >  self)
		.def(double() >= self)
		;
	
	std::map<std::string, base_unit>& bu = unit::get_base_units();
	for(std::map<std::string, base_unit>::iterator iter = bu.begin(); iter != bu.end(); iter++)
		scope().attr((*iter).first.c_str()) = unit((*iter).first, 1.0);
}

