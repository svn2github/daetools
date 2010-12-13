#include "stdafx.h"
#define PY_ARRAY_UNIQUE_SYMBOL dae_extension
#include <boost/python.hpp>
#include "trilinos_amesos_la_solver.h"
using namespace boost::python;
using namespace dae::solver;

boost::python::list pydaeTrilinosAmesosSupportedSolvers(void);

boost::python::list pydaeTrilinosAmesosSupportedSolvers(void)
{
	std::vector<string> strarrSolvers = daeTrilinosAmesosSupportedSolvers();
	boost::python::list l;
	
	for(size_t i = 0; i < strarrSolvers.size(); i++)
		l.append(strarrSolvers[i]);
	return l;
}

BOOST_PYTHON_MODULE(pyTrilinosAmesos)
{
/**************************************************************
	TrilinosAmesos LA Solver
***************************************************************/
	class_<daeIDALASolver_t, boost::noncopyable>("daeIDALASolver_t", no_init)
		.def("Create",		pure_virtual(&daeIDALASolver_t::Create))
		.def("Reinitialize",pure_virtual(&daeIDALASolver_t::Reinitialize))
		.def("SaveAsPBM",	pure_virtual(&daeIDALASolver_t::SaveAsPBM))
		.def("SaveAsXPM",	pure_virtual(&daeIDALASolver_t::SaveAsXPM))
		;

	def("daeCreateTrilinosAmesosSolver",      daeCreateTrilinosAmesosSolver,  return_value_policy<reference_existing_object>());
	def("daeTrilinosAmesosSupportedSolvers",  pydaeTrilinosAmesosSupportedSolvers);
}
