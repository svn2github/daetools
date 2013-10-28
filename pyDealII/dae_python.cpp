#include "stdafx.h"
#include "python_wraps.h"
#define PY_ARRAY_UNIQUE_SYMBOL dae_extension
#include "docstrings.h"
#include <noprefix.h>
using namespace boost::python;

BOOST_PYTHON_MODULE(pyDealII)
{
	import_array(); 
	boost::python::numeric::array::set_module_and_type("numpy", "ndarray");  
    
    docstring_options doc_options(true, true, false);

    class_<diffusion::daeDiffusion_1D, bases<daeModel>, boost::noncopyable>("daeDiffusion_1D", no_init)
        .def(init<string, optional<daeModel*, string> >(( arg("self"),
                                                          arg("name"),
                                                          arg("parentModel") = NULL,
                                                          arg("description") = ""
                                                       )))
        .def("Initialize", &daepython::daeDiffusion_Initialize<1>,
                           ( arg("self"), arg("meshFilename"), arg("polynomialOrder"), arg("diffusivity"), arg("velocity"), arg("generation"), arg("dirichletBC"), arg("neumannBC") ) )
        ;

    class_<diffusion::daeDiffusion_2D, bases<daeModel>, boost::noncopyable>("daeDiffusion_2D", no_init)
        .def(init<string, optional<daeModel*, string> >(( arg("self"),
                                                          arg("name"),
                                                          arg("parentModel") = NULL,
                                                          arg("description") = ""
                                                       )))
        .def("Initialize", &daepython::daeDiffusion_Initialize<2>,
                           ( arg("self"), arg("meshFilename"), arg("polynomialOrder"), arg("diffusivity"), arg("velocity"), arg("generation"), arg("dirichletBC"), arg("neumannBC") ) )
        ;
    
    class_<diffusion::daeDiffusion_3D, bases<daeModel>, boost::noncopyable>("daeDiffusion_3D", no_init)
        .def(init<string, optional<daeModel*, string> >(( arg("self"),
                                                          arg("name"),
                                                          arg("parentModel") = NULL,
                                                          arg("description") = ""
                                                       )))
        .def("Initialize", &daepython::daeDiffusion_Initialize<3>,
                           ( arg("self"), arg("meshFilename"), arg("polynomialOrder"), arg("diffusivity"), arg("velocity"), arg("generation"), arg("dirichletBC"), arg("neumannBC") ) )
        ;
}
