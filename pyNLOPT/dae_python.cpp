#include "stdafx.h"
#include "python_wraps.h"
#define PY_ARRAY_UNIQUE_SYMBOL dae_extension
#include "docstrings.h"
#include <noprefix.h>
using namespace boost::python;

BOOST_PYTHON_MODULE(pyNLOPT)
{
    import_array();
    boost::python::numeric::array::set_module_and_type("numpy", "ndarray");
    
    docstring_options doc_options(true, true, false);

	class_<daepython::daeNLPSolverWrapper, boost::noncopyable>("daeNLPSolver_t", no_init)
        .def("Initialize",               pure_virtual(&daeNLPSolver_t::Initialize))
        .def("Solve",                    pure_virtual(&daeNLPSolver_t::Solve))
		;
	
    class_<daepython::daeNLOPTWrapper, bases<daeNLPSolver_t>, boost::noncopyable>("daeNLOPT", DOCSTR_daeNLOPT, no_init)
        .add_property("Name",		&daeNLOPTSolver::GetName, DOCSTR_daeNLOPT_Name)
        .add_property("xtol_rel",	&daeNLOPTSolver::get_xtol_rel,
                                    &daeNLOPTSolver::set_xtol_rel, DOCSTR_daeNLOPT_xtol_rel)
        .add_property("xtol_abs",	&daeNLOPTSolver::get_xtol_abs,
                                    &daeNLOPTSolver::set_xtol_abs, DOCSTR_daeNLOPT_xtol_abs)
        .add_property("ftol_rel",	&daeNLOPTSolver::get_ftol_rel,
                                    &daeNLOPTSolver::set_ftol_rel, DOCSTR_daeNLOPT_ftol_rel)
        .add_property("ftol_abs",	&daeNLOPTSolver::get_ftol_abs,
                                    &daeNLOPTSolver::set_ftol_abs, DOCSTR_daeNLOPT_ftol_abs)
        .def("Initialize",			&daeNLOPTSolver::Initialize, ( boost::python::arg("self"),
                                                                   boost::python::arg("simulation"),
                                                                   boost::python::arg("daeSolver"),
                                                                   boost::python::arg("dataReporter"),
                                                                   boost::python::arg("log")
                                                                 ), DOCSTR_daeNLOPT_Initialize)
        .def("Solve",				&daeNLOPTSolver::Solve, ( boost::python::arg("self") ), DOCSTR_daeNLOPT_Solve)
        .def("PrintOptions",		&daeNLOPTSolver::PrintOptions, ( boost::python::arg("self") ), DOCSTR_daeNLOPT_PrintOptions)
        ; 

    def("daeCreateNLOPTSolver", daeCreateNLOPTSolver, return_value_policy<manage_new_object>(), ( boost::python::arg("algorithmName") ), DOCSTR_daeCreateNLOPTSolver);
}

