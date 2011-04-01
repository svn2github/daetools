#include "stdafx.h"
#define PY_ARRAY_UNIQUE_SYMBOL dae_extension
#include <boost/python.hpp>
#include "cusp_la_solver.h" 
using namespace boost::python;
using namespace dae::solver;

BOOST_PYTHON_MODULE(pyCUSP)
{
/**************************************************************
	LA Solver
***************************************************************/
	class_<daeIDALASolver_t, boost::noncopyable>("daeIDALASolver_t", no_init)
		.def("Create",		pure_virtual(&daeIDALASolver_t::Create))
		.def("Reinitialize",pure_virtual(&daeIDALASolver_t::Reinitialize))
		.def("SaveAsXPM",	pure_virtual(&daeIDALASolver_t::SaveAsXPM))
		;

	class_<daeCUSPSolver, bases<daeIDALASolver_t>, boost::noncopyable>("daeCUSPSolver")
		.def("Create",		&daeCUSPSolver::Create)
		.def("Reinitialize",&daeCUSPSolver::Reinitialize)
		.def("SaveAsXPM",	&daeCUSPSolver::SaveAsXPM)
		;

	def("daeCreateCUSPSolver", daeCreateCUSPSolver, return_value_policy<reference_existing_object>());

}
