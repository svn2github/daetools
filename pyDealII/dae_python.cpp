#include "stdafx.h"
#include "python_wraps.h"
#include "docstrings.h"
//#define PY_ARRAY_UNIQUE_SYMBOL dae_extension
//#include <noprefix.h>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/return_value_policy.hpp>
using namespace boost::python;

BOOST_PYTHON_MODULE(pyDealII)
{
	//import_array();
	//boost::python::numeric::array::set_module_and_type("numpy", "ndarray");
    
    docstring_options doc_options(true, true, false);

    class_< Tensor<1,1,double>, boost::noncopyable>("Tensor_1_1D")
        .add_property("dimension",                  &daepython::Tensor_1_1D_dimension)
        .add_property("rank",                       &daepython::Tensor_1_1D_rank)
        .add_property("n_independent_components",   &daepython::Tensor_1_1D_n_independent_components)

        .def(self == self)
        .def(self != self)
        .def(self += self)
        .def(self -= self)
        .def(self *= double())
        .def(self /= double())
        .def(- self)
        .def(self + self)
        .def(self - self)
        .def(self * self)

        .def("__getitem__",         &daepython::Tensor_1_1D_getitem)
        .def("__setitem__",         &daepython::Tensor_1_1D_setitem)
        .def("__str__",             &daepython::Tensor_1_1D_str)
        .def("__repr__",            &daepython::Tensor_1_1D_repr)

        .def("norm",                &Tensor_1_1D::norm)
        .def("norm_square",         &Tensor_1_1D::norm_square)
        .def("clear",               &Tensor_1_1D::clear)
        .def("memory_consumption",  &Tensor_1_1D::memory_consumption)
    ;

    class_< Tensor<1,2,double>, boost::noncopyable>("Tensor_1_2D")
        .add_property("dimension",                  &daepython::Tensor_1_2D_dimension)
        .add_property("rank",                       &daepython::Tensor_1_2D_rank)
        .add_property("n_independent_components",   &daepython::Tensor_1_2D_n_independent_components)

        .def(self == self)
        .def(self != self)
        .def(self += self)
        .def(self -= self)
        .def(self *= double())
        .def(self /= double())
        .def(- self)
        .def(self + self)
        .def(self - self)
        .def(self * self)

        .def("__getitem__",         &daepython::Tensor_1_2D_getitem)
        .def("__setitem__",         &daepython::Tensor_1_2D_setitem)
        .def("__str__",             &daepython::Tensor_1_2D_str)
        .def("__repr__",            &daepython::Tensor_1_2D_repr)

        .def("norm",                &Tensor_1_2D::norm)
        .def("norm_square",         &Tensor_1_2D::norm_square)
        .def("clear",               &Tensor_1_2D::clear)
        .def("memory_consumption",  &Tensor_1_2D::memory_consumption)
    ;

    class_< Tensor<1,3,double>, boost::noncopyable>("Tensor_1_3D")
        .add_property("dimension",                  &daepython::Tensor_1_3D_dimension)
        .add_property("rank",                       &daepython::Tensor_1_3D_rank)
        .add_property("n_independent_components",   &daepython::Tensor_1_3D_n_independent_components)

        .def(self == self)
        .def(self != self)
        .def(self += self)
        .def(self -= self)
        .def(self *= double())
        .def(self /= double())
        .def(- self)
        .def(self + self)
        .def(self - self)
        .def(self * self)

        .def("__getitem__",         &daepython::Tensor_1_3D_getitem)
        .def("__setitem__",         &daepython::Tensor_1_3D_setitem)
        .def("__str__",             &daepython::Tensor_1_3D_str)
        .def("__repr__",            &daepython::Tensor_1_3D_repr)

        .def("norm",                &Tensor_1_3D::norm)
        .def("norm_square",         &Tensor_1_3D::norm_square)
        .def("clear",               &Tensor_1_3D::clear)
        .def("memory_consumption",  &Tensor_1_3D::memory_consumption)
    ;


    class_< Tensor<2,1,double>, boost::noncopyable>("Tensor_2_1D")
        .add_property("dimension",                  &daepython::Tensor_2_1D_dimension)
        .add_property("rank",                       &daepython::Tensor_2_1D_rank)
        .add_property("n_independent_components",   &daepython::Tensor_2_1D_n_independent_components)

        .def(self == self)
        .def(self != self)
        .def(self += self)
        .def(self -= self)
        .def(self *= double())
        .def(self /= double())
        .def(- self)
        .def(self + self)
        .def(self - self)
        .def(self * self)

        .def("__getitem__",         &daepython::Tensor_2_1D_getitem)
        .def("__setitem__",         &daepython::Tensor_2_1D_setitem)
        .def("__str__",             &daepython::Tensor_2_1D_str)
        .def("__repr__",            &daepython::Tensor_2_1D_repr)

        .def("norm",                &Tensor_2_1D::norm)
        .def("norm_square",         &Tensor_2_1D::norm_square)
        .def("clear",               &Tensor_2_1D::clear)
        .def("memory_consumption",  &Tensor_2_1D::memory_consumption)
    ;

    class_< Tensor<2,2,double>, boost::noncopyable>("Tensor_2_2D")
        .add_property("dimension",                  &daepython::Tensor_2_2D_dimension)
        .add_property("rank",                       &daepython::Tensor_2_2D_rank)
        .add_property("n_independent_components",   &daepython::Tensor_2_2D_n_independent_components)

        .def(self == self)
        .def(self != self)
        .def(self += self)
        .def(self -= self)
        .def(self *= double())
        .def(self /= double())
        .def(- self)
        .def(self + self)
        .def(self - self)
        .def(self * self)

        .def("__getitem__",         &daepython::Tensor_2_2D_getitem)
        .def("__setitem__",         &daepython::Tensor_2_2D_setitem)
        .def("__str__",             &daepython::Tensor_2_2D_str)
        .def("__repr__",            &daepython::Tensor_2_2D_repr)

        .def("norm",                &Tensor_2_2D::norm)
        .def("norm_square",         &Tensor_2_2D::norm_square)
        .def("clear",               &Tensor_2_2D::clear)
        .def("memory_consumption",  &Tensor_2_2D::memory_consumption)
    ;

    class_< Tensor<2,3,double>, boost::noncopyable>("Tensor_2_3D")
        .add_property("dimension",                  &daepython::Tensor_2_3D_dimension)
        .add_property("rank",                       &daepython::Tensor_2_3D_rank)
        .add_property("n_independent_components",   &daepython::Tensor_2_3D_n_independent_components)

        .def(self == self)
        .def(self != self)
        .def(self += self)
        .def(self -= self)
        .def(self *= double())
        .def(self /= double())
        .def(- self)
        .def(self + self)
        .def(self - self)
        .def(self * self)

        .def("__getitem__",         &daepython::Tensor_2_3D_getitem)
        .def("__setitem__",         &daepython::Tensor_2_3D_setitem)
        .def("__str__",             &daepython::Tensor_2_3D_str)
        .def("__repr__",            &daepython::Tensor_2_3D_repr)

        .def("norm",                &Tensor_2_3D::norm)
        .def("norm_square",         &Tensor_2_3D::norm_square)
        .def("clear",               &Tensor_2_3D::clear)
        .def("memory_consumption",  &Tensor_2_3D::memory_consumption)
    ;




    class_<Point<1,double>, bases<Tensor_1_1D> >("Point_1D")
        .def(init<double>())

        .def(self == self)
        .def(self != self)
        .def(self += self)
        .def(self -= self)
        .def(self *= double())
        .def(self /= double())
        .def(- self)
        .def(self + self)
        .def(self - self)
        .def(self * self)
        .def(self * double())
        .def(double() * self)
        .def(self / double())

        .def("__repr__",   &daepython::Point_1D_repr)

        .def("distance",   &Point_1D::distance)
        .def("square",     &Point_1D::square)

        .add_property("x", &daepython::Point_1D_x)
    ;

    class_<Point<2,double>, bases<Tensor_1_2D> >("Point_2D")
        .def(init<double, double>())

        .def(self == self)
        .def(self != self)
        .def(self += self)
        .def(self -= self)
        .def(self *= double())
        .def(self /= double())
        .def(- self)
        .def(self + self)
        .def(self - self)
        .def(self * self)
        .def(self * double())
        .def(double() * self)
        .def(self / double())

        .def("__repr__",   &daepython::Point_2D_repr)

        .def("distance",   &Point_2D::distance)
        .def("square",     &Point_2D::square)

        .add_property("x", &daepython::Point_2D_x)
        .add_property("y", &daepython::Point_2D_y)
    ;

    class_<Point<3,double>, bases<Tensor_1_3D> >("Point_3D")
        .def(init<double, double, double>())

        .def(self == self)
        .def(self != self)
        .def(self += self)
        .def(self -= self)
        .def(self *= double())
        .def(self /= double())
        .def(- self)
        .def(self + self)
        .def(self - self)
        .def(self * self)
        .def(self * double())
        .def(double() * self)
        .def(self / double())

        .def("__repr__",   &daepython::Point_3D_repr)

        .def("distance",   &Point_3D::distance)
        .def("square",     &Point_3D::square)

        .add_property("x", &daepython::Point_3D_x)
        .add_property("y", &daepython::Point_3D_y)
        .add_property("z", &daepython::Point_3D_z)
    ;

    class_<daepython::Function_wrapper<1>, boost::noncopyable>("Function_1D", no_init)
        .def(init<unsigned int>((arg("self"), arg("n_components") = 1)))

        .def_readonly("dimension",	   &Function<1>::dimension)
        .def_readonly("n_components",  &Function<1>::n_components)

        .def("value",           pure_virtual(&daepython::Function_wrapper_1D::value),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_value",    pure_virtual(&daepython::Function_wrapper_1D::vector_value),
                                ( arg("self"), arg("point") ) )

        .def("gradient",        pure_virtual(&daepython::Function_wrapper_1D::gradient),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_gradient",	pure_virtual(&daepython::Function_wrapper_1D::vector_gradient),
                                ( arg("self"), arg("point") ) )
    ;

    class_<daepython::Function_wrapper<2>, boost::noncopyable>("Function_2D", no_init)
        .def(init<unsigned int>((arg("self"), arg("n_components") = 1)))

        .def_readonly("dimension",	   &Function<2>::dimension)
        .def_readonly("n_components",  &Function<2>::n_components)

        .def("value",           pure_virtual(&daepython::Function_wrapper_2D::value),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_value",    pure_virtual(&daepython::Function_wrapper_2D::vector_value),
                                ( arg("self"), arg("point") ) )

        .def("gradient",        pure_virtual(&daepython::Function_wrapper_2D::gradient),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_gradient",	pure_virtual(&daepython::Function_wrapper_2D::vector_gradient),
                                ( arg("self"), arg("point") ) )
    ;

    class_<daepython::Function_wrapper<3>, boost::noncopyable>("Function_3D", no_init)
        .def(init<unsigned int>((arg("self"), arg("n_components") = 1)))

        .def_readonly("dimension",	   &Function<3>::dimension)
        .def_readonly("n_components",  &Function<3>::n_components)

        .def("value",           pure_virtual(&daepython::Function_wrapper_3D::value),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_value",    pure_virtual(&daepython::Function_wrapper_3D::vector_value),
                                ( arg("self"), arg("point") ) )

        .def("gradient",        pure_virtual(&daepython::Function_wrapper_3D::gradient),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_gradient",	pure_virtual(&daepython::Function_wrapper_3D::vector_gradient),
                                ( arg("self"), arg("point") ) )
    ;

    class_<daepython::adoubleFunction_wrapper<1>, boost::noncopyable>("adoubleFunction_1D", no_init)
        .def(init<unsigned int>((arg("self"), arg("n_components") = 1)))

        .def_readonly("dimension",	   &Function<1,adouble>::dimension)
        .def_readonly("n_components",  &Function<1,adouble>::n_components)

        .def("value",           pure_virtual(&daepython::adoubleFunction_wrapper_1D::value),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_value",    pure_virtual(&daepython::adoubleFunction_wrapper_1D::vector_value),
                                ( arg("self"), arg("point") ) )

        .def("gradient",        pure_virtual(&daepython::adoubleFunction_wrapper_1D::gradient),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_gradient",	pure_virtual(&daepython::adoubleFunction_wrapper_1D::vector_gradient),
                                ( arg("self"), arg("point") ) )
    ;

    class_<daepython::adoubleFunction_wrapper<2>, boost::noncopyable>("adoubleFunction_2D", no_init)
        .def(init<unsigned int>((arg("self"), arg("n_components") = 1)))

        .def_readonly("dimension",	   &Function<2,adouble>::dimension)
        .def_readonly("n_components",  &Function<2,adouble>::n_components)

        .def("value",           pure_virtual(&daepython::adoubleFunction_wrapper_2D::value),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_value",    pure_virtual(&daepython::adoubleFunction_wrapper_2D::vector_value),
                                ( arg("self"), arg("point") ) )

        .def("gradient",        pure_virtual(&daepython::adoubleFunction_wrapper_2D::gradient),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_gradient",	pure_virtual(&daepython::adoubleFunction_wrapper_2D::vector_gradient),
                                ( arg("self"), arg("point") ) )
    ;

    class_<daepython::adoubleFunction_wrapper<3>, boost::noncopyable>("adoubleFunction_3D", no_init)
        .def(init<unsigned int>((arg("self"), arg("n_components") = 1)))

        .def_readonly("dimension",	   &Function<3,adouble>::dimension)
        .def_readonly("n_components",  &Function<3,adouble>::n_components)

        .def("value",           pure_virtual(&daepython::adoubleFunction_wrapper_3D::value),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_value",    pure_virtual(&daepython::adoubleFunction_wrapper_3D::vector_value),
                                ( arg("self"), arg("point") ) )

        .def("gradient",        pure_virtual(&daepython::adoubleFunction_wrapper_3D::gradient),
                                ( arg("self"), arg("point"), arg("component") = 0 ) )
        .def("vector_gradient",	pure_virtual(&daepython::adoubleFunction_wrapper_3D::vector_gradient),
                                ( arg("self"), arg("point") ) )
    ;

    class_<ConstantFunction<1>, bases< Function<1> >, boost::noncopyable>("ConstantFunction_1D", no_init)
        .def(init<const double, optional<const unsigned int> >((arg("self"), arg("value"), arg("n_components") = 1)))
        .def("__init__", make_constructor(&daepython::ConstantFunction_init<1>, default_call_policies(), ( arg("values") ) ))
    ;
    class_<ConstantFunction<2>, bases< Function<2> >, boost::noncopyable>("ConstantFunction_2D", no_init)
        .def(init<const double, optional<const unsigned int> >((arg("self"), arg("value"), arg("n_components") = 1)))
        .def("__init__", make_constructor(&daepython::ConstantFunction_init<2>, default_call_policies(), ( arg("values") ) ))
    ;
    class_<ConstantFunction<3>, bases< Function<3> >, boost::noncopyable>("ConstantFunction_3D", no_init)
        .def(init<const double, optional<const unsigned int> >((arg("self"), arg("value"), arg("n_components") = 1)))
        .def("__init__", make_constructor(&daepython::ConstantFunction_init<3>, default_call_policies(), ( arg("values") ) ))
    ;

    class_<ConstantFunction<1,adouble>, bases< Function<1,adouble> >, boost::noncopyable>("adoubleConstantFunction_1D", no_init)
        .def(init<const adouble, optional<const unsigned int> >((arg("self"), arg("value"), arg("n_components") = 1)))
        .def("__init__", make_constructor(&daepython::adoubleConstantFunction_init<1>, default_call_policies(), ( arg("values") ) ))
    ;
    class_<ConstantFunction<2,adouble>, bases< Function<2,adouble> >, boost::noncopyable>("adoubleConstantFunction_2D", no_init)
        .def(init<const adouble, optional<const unsigned int> >((arg("self"), arg("value"), arg("n_components") = 1)))
        .def("__init__", make_constructor(&daepython::adoubleConstantFunction_init<2>, default_call_policies(), ( arg("values") ) ))
    ;
    class_<ConstantFunction<3,adouble>, bases< Function<3,adouble> >, boost::noncopyable>("adoubleConstantFunction_3D", no_init)
        .def(init<const adouble, optional<const unsigned int> >((arg("self"), arg("value"), arg("n_components") = 1)))
        .def("__init__", make_constructor(&daepython::adoubleConstantFunction_init<3>, default_call_policies(), ( arg("values") ) ))
    ;

    class_<ZeroFunction<1>, bases< Function<1> >, boost::noncopyable>("ZeroFunction_1D", no_init)
        .def(init< optional<const unsigned int> >((arg("self"), arg("n_components") = 1)))
    ;
    class_<ZeroFunction<2>, bases< Function<2> >, boost::noncopyable>("ZeroFunction_2D", no_init)
        .def(init< optional<const unsigned int> >((arg("self"), arg("n_components") = 1)))
    ;
    class_<ZeroFunction<3>, bases< Function<3> >, boost::noncopyable>("ZeroFunction_3D", no_init)
        .def(init< optional<const unsigned int> >((arg("self"), arg("n_components") = 1)))
    ;

    class_<ComponentSelectFunction<1>, bases< Function<1> >, boost::noncopyable>("ComponentSelectFunction_1D", no_init)
        .def(init<const unsigned int, double, const unsigned int>((arg("self"), arg("selected"), arg("value"), arg("n_components"))))
        .def(init<const unsigned int, const unsigned int>((arg("self"), arg("value"), arg("n_components"))))
    ;

    class_<ComponentSelectFunction<2>, bases< Function<2> >, boost::noncopyable>("ComponentSelectFunction_2D", no_init)
        .def(init<const unsigned int, double, const unsigned int>((arg("self"), arg("selected"), arg("value"), arg("n_components"))))
        .def(init<const unsigned int, const unsigned int>((arg("self"), arg("value"), arg("n_components"))))
    ;

    class_<ComponentSelectFunction<3>, bases< Function<3> >, boost::noncopyable>("ComponentSelectFunction_3D", no_init)
        .def(init<const unsigned int, double, const unsigned int>((arg("self"), arg("selected"), arg("value"), arg("n_components"))))
        .def(init<const unsigned int, const unsigned int>((arg("self"), arg("value"), arg("n_components"))))
    ;


    class_< std::vector< Point<1,double> > >("vector_Point_1D")
        .def(vector_indexing_suite< std::vector< Point<1,double> > >())
    ;
    class_< std::vector< Point<2,double> > >("vector_Point_2D")
        .def(vector_indexing_suite< std::vector< Point<2,double> > >())
    ;
    class_< std::vector< Point<3,double> > >("vector_Point_3D")
        .def(vector_indexing_suite< std::vector< Point<3,double> > >())
    ;

    class_<Vector<double>, boost::noncopyable>("Vector", no_init)
        .def("__call__",    &daepython::Vector_getitem)
        .def("__getitem__", &daepython::Vector_getitem)
        .def("__setitem__", &daepython::Vector_set)
        .def("add",         &daepython::Vector_add)
    ;
    class_<FullMatrix<double>, boost::noncopyable>("FullMatrix", no_init)
        .def("__call__",    &daepython::FullMatrix_getitem)
        .def("set",         &daepython::FullMatrix_set)
        .def("add",         &daepython::FullMatrix_add)
    ;
    class_<SparseMatrix<double>, boost::noncopyable>("SparseMatrix", no_init)
        .add_property("n",                  &SparseMatrix<double>::n)
        .add_property("m",                  &SparseMatrix<double>::m)
        .add_property("n_nonzero_elements", &SparseMatrix<double>::n_nonzero_elements)
        .def("__call__",                    &daepython::SparseMatrix_getitem)
        .def("el",                          &daepython::SparseMatrix_el)
        .def("set",                         &daepython::SparseMatrix_set)
        .def("add",                         &daepython::SparseMatrix_add)
    ;

    class_<Quadrature<0>, boost::noncopyable>("Quadrature_0D", no_init)
    ;
    class_<Quadrature<1>, boost::noncopyable>("Quadrature_1D", no_init)
    ;
    class_<Quadrature<2>, boost::noncopyable>("Quadrature_2D", no_init)
    ;
    class_<Quadrature<3>, boost::noncopyable>("Quadrature_3D", no_init)
    ;

    class_<QGauss<1>, bases< Quadrature<1> >, boost::noncopyable>("QGauss_1D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;
    class_<QGauss<2>, bases< Quadrature<2> >, boost::noncopyable>("QGauss_2D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;
    class_<QGauss<3>, bases< Quadrature<3> >, boost::noncopyable>("QGauss_3D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;

    class_<QGaussLobatto<1>, bases< Quadrature<1> >, boost::noncopyable>("QGaussLobatto_1D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;
    class_<QGaussLobatto<2>, bases< Quadrature<2> >, boost::noncopyable>("QGaussLobatto_2D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;
    class_<QGaussLobatto<3>, bases< Quadrature<3> >, boost::noncopyable>("QGaussLobatto_3D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;

    class_<QMidpoint<1>, bases< Quadrature<1> >, boost::noncopyable>("QMidpoint_1D", no_init)
        .def(init<>((arg("self"))))
    ;
    class_<QMidpoint<2>, bases< Quadrature<2> >, boost::noncopyable>("QMidpoint_2D", no_init)
        .def(init<>((arg("self"))))
    ;
    class_<QMidpoint<3>, bases< Quadrature<3> >, boost::noncopyable>("QMidpoint_3D", no_init)
        .def(init<>((arg("self"))))
    ;

    class_<QSimpson<1>, bases< Quadrature<1> >, boost::noncopyable>("QSimpson_1D", no_init)
        .def(init<>((arg("self"))))
    ;
    class_<QSimpson<2>, bases< Quadrature<2> >, boost::noncopyable>("QSimpson_2D", no_init)
        .def(init<>((arg("self"))))
    ;
    class_<QSimpson<3>, bases< Quadrature<3> >, boost::noncopyable>("QSimpson_3D", no_init)
        .def(init<>((arg("self"))))
    ;

    class_<QTrapez<1>, bases< Quadrature<1> >, boost::noncopyable>("QTrapez_1D", no_init)
        .def(init<>((arg("self"))))
    ;
    class_<QTrapez<2>, bases< Quadrature<2> >, boost::noncopyable>("QTrapez_2D", no_init)
        .def(init<>((arg("self"))))
    ;
    class_<QTrapez<3>, bases< Quadrature<3> >, boost::noncopyable>("QTrapez_3D", no_init)
        .def(init<>((arg("self"))))
    ;

    class_<QMilne<1>, bases< Quadrature<1> >, boost::noncopyable>("QMilne_1D", no_init)
        .def(init<>((arg("self"))))
    ;
    class_<QMilne<2>, bases< Quadrature<2> >, boost::noncopyable>("QMilne_2D", no_init)
        .def(init<>((arg("self"))))
    ;
    class_<QMilne<3>, bases< Quadrature<3> >, boost::noncopyable>("QMilne_3D", no_init)
        .def(init<>((arg("self"))))
    ;

    class_<QWeddle<1>, bases< Quadrature<1> >, boost::noncopyable>("QWeddle_1D", no_init)
        .def(init<>((arg("self"))))
    ;
    class_<QWeddle<2>, bases< Quadrature<2> >, boost::noncopyable>("QWeddle_2D", no_init)
        .def(init<>((arg("self"))))
    ;
    class_<QWeddle<3>, bases< Quadrature<3> >, boost::noncopyable>("QWeddle_3D", no_init)
        .def(init<>((arg("self"))))
    ;

    class_<QGaussLog<1>, bases< Quadrature<1> >, boost::noncopyable>("QGaussLog_1D", no_init)
        .def(init<const unsigned int, optional<const bool> >((arg("self"), arg("n_quadrature_points"), arg("revert") = false)))
    ;

    class_<QGaussLogR<1>, bases< Quadrature<1> >, boost::noncopyable>("QGaussLogR_1D", no_init)
        .def(init<const unsigned int, optional<const Point<1>, const double, const bool> >((arg("self"), arg("n_quadrature_points"), arg("x0") = Point<2>(), arg("alpha") = 1.0, arg("factor_out_singular_weight") = false)))
    ;

    class_<QGaussOneOverR<2>, bases< Quadrature<2> >, boost::noncopyable>("QGaussOneOverR_2D", no_init)
        .def(init<const unsigned int, const Point<2>, optional<const bool> >((arg("self"), arg("n_quadrature_points"), arg("singularity"), arg("factor_out_singular_weight") = false)))
    ;

    class_<QGaussChebyshev<1>, bases< Quadrature<1> >, boost::noncopyable>("QGaussChebyshev_1D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;
    class_<QGaussChebyshev<2>, bases< Quadrature<2> >, boost::noncopyable>("QGaussChebyshev_2D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;
    class_<QGaussChebyshev<3>, bases< Quadrature<3> >, boost::noncopyable>("QGaussChebyshev_3D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;

    class_<QGaussLobattoChebyshev<1>, bases< Quadrature<1> >, boost::noncopyable>("QGaussLobattoChebyshev_1D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;
    class_<QGaussLobattoChebyshev<2>, bases< Quadrature<2> >, boost::noncopyable>("QGaussLobattoChebyshev_2D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;
    class_<QGaussLobattoChebyshev<3>, bases< Quadrature<3> >, boost::noncopyable>("QGaussLobattoChebyshev_3D", no_init)
        .def(init<const unsigned int>((arg("self"), arg("n_quadrature_points"))))
    ;


    class_< feRuntimeNumber<1> >("feRuntimeNumber_1D", no_init)
        .def("__str__",   &feRuntimeNumber<1>::ToString)
    ;
    class_< feRuntimeNumber<2> >("feRuntimeNumber_2D", no_init)
        .def("__str__",   &feRuntimeNumber<2>::ToString)
    ;
    class_< feRuntimeNumber<3> >("feRuntimeNumber_3D", no_init)
        .def("__str__",   &feRuntimeNumber<3>::ToString)
    ;


    class_< feExpression<1> >("feExpression_1D", no_init)
        .def(self + self)
        .def(self - self)
        .def(self * self)
        .def(self / self)

        .def(self + double())
        .def(self - double())
        .def(self * double())
        .def(self / double())
        .def(pow(self, double()))

        .def(double() + self)
        .def(double() - self)
        .def(double() * self)
        .def(double() / self)

        .def("__str__",   &feExpression<1>::ToString)

        .def("exp",   &fe_solver::exp<1>).staticmethod("exp")
        .def("log",   &fe_solver::log<1>).staticmethod("log")
        .def("log10", &fe_solver::log10<1>).staticmethod("log10")
        .def("sqrt",  &fe_solver::sqrt<1>).staticmethod("sqrt")
        .def("sin",   &fe_solver::sin<1>).staticmethod("sin")
        .def("cos",   &fe_solver::cos<1>).staticmethod("cos")
        .def("tan",   &fe_solver::tan<1>).staticmethod("tan")
        .def("asin",  &fe_solver::asin<1>).staticmethod("asin")
        .def("acos",  &fe_solver::acos<1>).staticmethod("acos")
        .def("atan",  &fe_solver::atan<1>).staticmethod("atan")
        .def("abs",   &fe_solver::abs<1>).staticmethod("abs")
    ;

    class_< feExpression<2> >("feExpression_2D", no_init)
        .def(self + self)
        .def(self - self)
        .def(self * self)
        .def(self / self)

        .def(self + double())
        .def(self - double())
        .def(self * double())
        .def(self / double())
        .def(pow(self, double()))

        .def(double() + self)
        .def(double() - self)
        .def(double() * self)
        .def(double() / self)

        .def("__str__",   &feExpression<2>::ToString)

        .def("exp",   &fe_solver::exp<2>).staticmethod("exp")
        .def("log",   &fe_solver::log<2>).staticmethod("log")
        .def("log10", &fe_solver::log10<2>).staticmethod("log10")
        .def("sqrt",  &fe_solver::sqrt<2>).staticmethod("sqrt")
        .def("sin",   &fe_solver::sin<2>).staticmethod("sin")
        .def("cos",   &fe_solver::cos<2>).staticmethod("cos")
        .def("tan",   &fe_solver::tan<2>).staticmethod("tan")
        .def("asin",  &fe_solver::asin<2>).staticmethod("asin")
        .def("acos",  &fe_solver::acos<2>).staticmethod("acos")
        .def("atan",  &fe_solver::atan<2>).staticmethod("atan")
        .def("abs",   &fe_solver::abs<2>).staticmethod("abs")
    ;

    class_< feExpression<3> >("feExpression_3D", no_init)
        .def(self + self)
        .def(self - self)
        .def(self * self)
        .def(self / self)

        .def(self + double())
        .def(self - double())
        .def(self * double())
        .def(self / double())
        .def(pow(self, double()))

        .def(double() + self)
        .def(double() - self)
        .def(double() * self)
        .def(double() / self)

        .def("__str__",  &feExpression<3>::ToString)

        .def("exp",   &fe_solver::exp<3>).staticmethod("exp")
        .def("log",   &fe_solver::log<3>).staticmethod("log")
        .def("log10", &fe_solver::log10<3>).staticmethod("log10")
        .def("sqrt",  &fe_solver::sqrt<3>).staticmethod("sqrt")
        .def("sin",   &fe_solver::sin<3>).staticmethod("sin")
        .def("cos",   &fe_solver::cos<3>).staticmethod("cos")
        .def("tan",   &fe_solver::tan<3>).staticmethod("tan")
        .def("asin",  &fe_solver::asin<3>).staticmethod("asin")
        .def("acos",  &fe_solver::acos<3>).staticmethod("acos")
        .def("atan",  &fe_solver::atan<3>).staticmethod("atan")
        .def("abs",   &fe_solver::abs<3>).staticmethod("abs")
    ;

    boost::python::scope().attr("fe_q") = fe_q;
    boost::python::scope().attr("fe_i") = fe_i;
    boost::python::scope().attr("fe_j") = fe_j;

    def("constant_1D", &constant<1>, ( arg("value") ));
    def("constant_2D", &constant<2>, ( arg("value") ));
    def("constant_3D", &constant<3>, ( arg("value") ));

    def("phi_1D", &phi<1>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("phi_2D", &phi<2>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("phi_3D", &phi<3>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));

    def("dphi_1D", &dphi<1>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("dphi_2D", &dphi<2>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("dphi_3D", &dphi<3>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));

    def("d2phi_1D", &d2phi<1>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("d2phi_2D", &d2phi<2>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("d2phi_3D", &d2phi<3>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));

    def("JxW_1D", &JxW<1>, ( arg("quadraturePoint") ));
    def("JxW_2D", &JxW<2>, ( arg("quadraturePoint") ));
    def("JxW_3D", &JxW<3>, ( arg("quadraturePoint") ));

    def("xyz_1D", &xyz<1>, ( arg("quadraturePoint") ));
    def("xyz_2D", &xyz<2>, ( arg("quadraturePoint") ));
    def("xyz_3D", &xyz<3>, ( arg("quadraturePoint") ));

    def("normal_1D", &normal<1>, ( arg("quadraturePoint") ));
    def("normal_2D", &normal<2>, ( arg("quadraturePoint") ));
    def("normal_3D", &normal<3>, ( arg("quadraturePoint") ));

    def("function_value_1D", &function_value<1>, ( arg("functionName"), arg("point") ));
    def("function_value_2D", &function_value<2>, ( arg("functionName"), arg("point") ));
    def("function_value_3D", &function_value<3>, ( arg("functionName"), arg("point") ));

    def("function_value_1D", &function_value2<1>, ( arg("functionName"), arg("point"), arg("component") ));
    def("function_value_2D", &function_value2<2>, ( arg("functionName"), arg("point"), arg("component") ));
    def("function_value_3D", &function_value2<3>, ( arg("functionName"), arg("point"), arg("component") ));

    def("function_adouble_value_1D", &function_adouble_value<1>, ( arg("functionName"), arg("point") ));
    def("function_adouble_value_2D", &function_adouble_value<2>, ( arg("functionName"), arg("point") ));
    def("function_adouble_value_3D", &function_adouble_value<3>, ( arg("functionName"), arg("point") ));

    def("function_adouble_value_1D", &function_adouble_value2<1>, ( arg("functionName"), arg("point"), arg("component") ));
    def("function_adouble_value_2D", &function_adouble_value2<2>, ( arg("functionName"), arg("point"), arg("component") ));
    def("function_adouble_value_3D", &function_adouble_value2<3>, ( arg("functionName"), arg("point"), arg("component") ));

    def("function_gradient_1D", &function_gradient<1>, ( arg("functionName"), arg("point") ));
    def("function_gradient_2D", &function_gradient<2>, ( arg("functionName"), arg("point") ));
    def("function_gradient_3D", &function_gradient<3>, ( arg("functionName"), arg("point") ));

    def("function_gradient_1D", &function_gradient2<1>, ( arg("functionName"), arg("point"), arg("component") ));
    def("function_gradient_2D", &function_gradient2<2>, ( arg("functionName"), arg("point"), arg("component") ));
    def("function_gradient_3D", &function_gradient2<3>, ( arg("functionName"), arg("point"), arg("component") ));

    def("phi_vec_1D", &phi_vec<1>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("phi_vec_2D", &phi_vec<2>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("phi_vec_3D", &phi_vec<3>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));

    def("dphi_vec_1D", &dphi_vec<1>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("dphi_vec_2D", &dphi_vec<2>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("dphi_vec_3D", &dphi_vec<3>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));

    def("d2phi_vec_1D", &d2phi_vec<1>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("d2phi_vec_2D", &d2phi_vec<2>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("d2phi_vec_3D", &d2phi_vec<3>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));

    def("div_phi_1D", &div_phi<1>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("div_phi_2D", &div_phi<2>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("div_phi_3D", &div_phi<3>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));

    /* CURL
    def("curl_2D", &curl<2>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    def("curl_3D", &curl<3>, ( arg("variableName"), arg("shapeFunction"), arg("quadraturePoint") ));
    */

    class_< std::map< unsigned int, feExpression<1> > >("map_Uint_Expression_1D")
        .def(map_indexing_suite< std::map< unsigned int, feExpression<1> > >())
    ;
    class_< std::map< unsigned int, feExpression<2> > >("map_Uint_Expression_2D")
        .def(map_indexing_suite< std::map< unsigned int, feExpression<2> > >())
    ;
    class_< std::map< unsigned int, feExpression<3> > >("map_Uint_Expression_3D")
        .def(map_indexing_suite< std::map< unsigned int, feExpression<3> > >())
    ;
    
    /* Scalar finite elements:
        FE_Q, FE_Bernstein

       Vector finite elements:
        FE_ABF
        FE_BDM, FE_DGBDM
        FE_Nedelec, FE_DGNedelec
        FE_RaviartThomas, FE_DGRaviartThomas

       Discontinuous Galerkin finite elements
        scalar: FE_DGP, FE_DGQ
        scalar, different shape functions: FE_DGPMonomial, FE_DGPNonparametric, FE_DGQArbitraryNodes
        vector-valued: FE_DGBDM, FE_DGNedelec, FE_DGRaviartThomas
    */
    class_<FiniteElement<1>, boost::noncopyable>("FiniteElement_1D", no_init)
    ;
    class_<FiniteElement<2>, boost::noncopyable>("FiniteElement_2D", no_init)
    ;
    class_<FiniteElement<3>, boost::noncopyable>("FiniteElement_3D", no_init)
    ;

    // Scalar finite elements
    class_<FE_Q<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_Q<1> >, boost::noncopyable>("FE_Q_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_Q<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_Q<2> >, boost::noncopyable>("FE_Q_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_Q<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_Q<3> >, boost::noncopyable>("FE_Q_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;

    class_<FE_Bernstein<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_Bernstein<1> >, boost::noncopyable>("FE_Bernstein_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_Bernstein<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_Bernstein<2> >, boost::noncopyable>("FE_Bernstein_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_Bernstein<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_Bernstein<3> >, boost::noncopyable>("FE_Bernstein_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;

    // Vector finite elements
    class_<FE_RaviartThomas<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_RaviartThomas<1> >, boost::noncopyable>("FE_RaviartThomas_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_RaviartThomas<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_RaviartThomas<2> >, boost::noncopyable>("FE_RaviartThomas_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_RaviartThomas<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_RaviartThomas<3> >, boost::noncopyable>("FE_RaviartThomas_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;

    class_<FE_DGRaviartThomas<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_DGRaviartThomas<1> >, boost::noncopyable>("FE_DGRaviartThomas_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_DGRaviartThomas<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_DGRaviartThomas<2> >, boost::noncopyable>("FE_DGRaviartThomas_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_DGRaviartThomas<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_DGRaviartThomas<3> >, boost::noncopyable>("FE_DGRaviartThomas_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;

    class_<FE_Nedelec<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_Nedelec<1> >, boost::noncopyable>("FE_Nedelec_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_Nedelec<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_Nedelec<2> >, boost::noncopyable>("FE_Nedelec_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_Nedelec<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_Nedelec<3> >, boost::noncopyable>("FE_Nedelec_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;

    class_<FE_DGNedelec<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_DGNedelec<1> >, boost::noncopyable>("FE_DGNedelec_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_DGNedelec<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_DGNedelec<2> >, boost::noncopyable>("FE_DGNedelec_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_DGNedelec<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_DGNedelec<3> >, boost::noncopyable>("FE_DGNedelec_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;

    class_<FE_BDM<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_BDM<1> >, boost::noncopyable>("FE_BDM_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_BDM<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_BDM<2> >, boost::noncopyable>("FE_BDM_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_BDM<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_BDM<3> >, boost::noncopyable>("FE_BDM_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;

    class_<FE_DGBDM<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_DGBDM<1> >, boost::noncopyable>("FE_DGBDM_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_DGBDM<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_DGBDM<2> >, boost::noncopyable>("FE_DGBDM_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_DGBDM<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_DGBDM<3> >, boost::noncopyable>("FE_DGBDM_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;

    class_<FE_ABF<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_ABF<1> >, boost::noncopyable>("FE_ABF_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_ABF<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_ABF<2> >, boost::noncopyable>("FE_ABF_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_ABF<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_ABF<3> >, boost::noncopyable>("FE_ABF_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;


    class_<FE_DGQ<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_DGQ<1> >, boost::noncopyable>("FE_DGQ_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_DGQ<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_DGQ<2> >, boost::noncopyable>("FE_DGQ_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_DGQ<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_DGQ<3> >, boost::noncopyable>("FE_DGQ_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;

    class_<FE_DGP<1>, bases< FiniteElement<1> >, boost::shared_ptr< FE_DGP<1> >, boost::noncopyable>("FE_DGP_1D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_DGP<2>, bases< FiniteElement<2> >, boost::shared_ptr< FE_DGP<2> >, boost::noncopyable>("FE_DGP_2D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;
    class_<FE_DGP<3>, bases< FiniteElement<3> >, boost::shared_ptr< FE_DGP<3> >, boost::noncopyable>("FE_DGP_3D", no_init)
        .def(init<const unsigned int>((arg("polynomialOrder"))))
    ;


    class_<dealiiFiniteElementDOF<1>, boost::noncopyable>("dealiiFiniteElementDOF_1D", no_init)
        .def(init<const std::string&,
                  const std::string&,
                  boost::shared_ptr< FiniteElement<1> >,
                  unsigned int>((arg("name"),
                                 arg("description"),
                                 arg("fe"),
                                 arg("multiplicity")
                                )))
        .def_readonly("Name",          &dealiiFiniteElementDOF<1>::m_strName)
        .def_readonly("Description",   &dealiiFiniteElementDOF<1>::m_strDescription)
        .def_readonly("Multiplicity",  &dealiiFiniteElementDOF<1>::m_nMultiplicity)
    ;
    class_<dealiiFiniteElementDOF<2>, boost::noncopyable>("dealiiFiniteElementDOF_2D", no_init)
        .def(init<const std::string&,
                  const std::string&,
                  boost::shared_ptr< FiniteElement<2> >,
                  unsigned int>((arg("name"),
                                 arg("description"),
                                 arg("fe"),
                                 arg("multiplicity")
                                )))
        .def_readonly("Name",          &dealiiFiniteElementDOF<2>::m_strName)
        .def_readonly("Description",   &dealiiFiniteElementDOF<2>::m_strDescription)
        .def_readonly("Multiplicity",  &dealiiFiniteElementDOF<2>::m_nMultiplicity)
    ;
    class_<dealiiFiniteElementDOF<3>, boost::noncopyable>("dealiiFiniteElementDOF_3D", no_init)
        .def(init<const std::string&,
                  const std::string&,
                  boost::shared_ptr< FiniteElement<3> >,
                  unsigned int>((arg("name"),
                                 arg("description"),
                                 arg("fe"),
                                 arg("multiplicity")
                                )))
        .def_readonly("Name",          &dealiiFiniteElementDOF<3>::m_strName)
        .def_readonly("Description",   &dealiiFiniteElementDOF<3>::m_strDescription)
        .def_readonly("Multiplicity",  &dealiiFiniteElementDOF<3>::m_nMultiplicity)
    ;

    class_<daepython::dealiiFiniteElementWeakFormWrapper<1>, boost::noncopyable>("dealiiFiniteElementWeakForm_1D", no_init)
        .def(init<const feExpression<1>&,
                  const feExpression<1>&,
                  const feExpression<1>&,
                  boost::python::dict,
                  boost::python::dict,
                  boost::python::dict,
                  boost::python::dict>(( arg("Aij"),
                                         arg("Mij"),
                                         arg("Fi"),
                                         arg("faceAij")              = boost::python::dict(),
                                         arg("faceFi")               = boost::python::dict(),
                                         arg("functions")            = boost::python::dict(),
                                         arg("functionsDirichletBC") = boost::python::dict()
                                      )))

        .def_readonly("Aij",                   &dealiiFiniteElementWeakForm<1>::m_Aij)
        .def_readonly("Mij",                   &dealiiFiniteElementWeakForm<1>::m_Mij)
        .def_readonly("Fi",                    &dealiiFiniteElementWeakForm<1>::m_Fi)
        .def_readonly("faceAij",               &dealiiFiniteElementWeakForm<1>::m_faceAij)
        .def_readonly("faceFi",                &dealiiFiniteElementWeakForm<1>::m_faceFi)
        .def_readonly("Functions",             &dealiiFiniteElementWeakForm<1>::m_functions)
        .def_readonly("FunctionsDirichletBC",  &dealiiFiniteElementWeakForm<1>::m_functionsDirichletBC)
    ;

    class_<daepython::dealiiFiniteElementWeakFormWrapper<2>, boost::noncopyable>("dealiiFiniteElementWeakForm_2D", no_init)
        .def(init<const feExpression<2>&,
                  const feExpression<2>&,
                  const feExpression<2>&,
                  boost::python::dict,
                  boost::python::dict,
                  boost::python::dict,
                  boost::python::dict>(( arg("Aij"),
                                         arg("Mij"),
                                         arg("Fi"),
                                         arg("faceAij")              = boost::python::dict(),
                                         arg("faceFi")               = boost::python::dict(),
                                         arg("functions")            = boost::python::dict(),
                                         arg("functionsDirichletBC") = boost::python::dict()
                                      )))

        .def_readonly("Aij",                   &dealiiFiniteElementWeakForm<2>::m_Aij)
        .def_readonly("Mij",                   &dealiiFiniteElementWeakForm<2>::m_Mij)
        .def_readonly("Fi",                    &dealiiFiniteElementWeakForm<2>::m_Fi)
        .def_readonly("faceAij",               &dealiiFiniteElementWeakForm<2>::m_faceAij)
        .def_readonly("faceFi",                &dealiiFiniteElementWeakForm<2>::m_faceFi)
        .def_readonly("Functions",             &dealiiFiniteElementWeakForm<2>::m_functions)
        .def_readonly("FunctionsDirichletBC",  &dealiiFiniteElementWeakForm<2>::m_functionsDirichletBC)
    ;

    class_<daepython::dealiiFiniteElementWeakFormWrapper<3>, boost::noncopyable>("dealiiFiniteElementWeakForm_3D", no_init)
        .def(init<const feExpression<3>&,
                  const feExpression<3>&,
                  const feExpression<3>&,
                  boost::python::dict,
                  boost::python::dict,
                  boost::python::dict,
                  boost::python::dict>(( arg("Aij"),
                                         arg("Mij"),
                                         arg("Fi"),
                                         arg("faceAij")              = boost::python::dict(),
                                         arg("faceFi")               = boost::python::dict(),
                                         arg("functions")            = boost::python::dict(),
                                         arg("functionsDirichletBC") = boost::python::dict()
                                      )))

        .def_readonly("Aij",                   &dealiiFiniteElementWeakForm<3>::m_Aij)
        .def_readonly("Mij",                   &dealiiFiniteElementWeakForm<3>::m_Mij)
        .def_readonly("Fi",                    &dealiiFiniteElementWeakForm<3>::m_Fi)
        .def_readonly("faceAij",               &dealiiFiniteElementWeakForm<3>::m_faceAij)
        .def_readonly("faceFi",                &dealiiFiniteElementWeakForm<3>::m_faceFi)
        .def_readonly("Functions",             &dealiiFiniteElementWeakForm<3>::m_functions)
        .def_readonly("FunctionsDirichletBC",  &dealiiFiniteElementWeakForm<3>::m_functionsDirichletBC)
    ;

    class_<dealIIDataReporter, bases<daeDataReporter_t>, boost::noncopyable>("dealIIDataReporter", no_init)
        .def("Connect",				&dealIIDataReporter::Connect)
        .def("Disconnect",			&dealIIDataReporter::Disconnect)
        .def("IsConnected",			&dealIIDataReporter::IsConnected)
        .def("StartRegistration",	&dealIIDataReporter::StartRegistration)
        .def("RegisterDomain",		&dealIIDataReporter::RegisterDomain)
        .def("RegisterVariable",	&dealIIDataReporter::RegisterVariable)
        .def("EndRegistration",		&dealIIDataReporter::EndRegistration)
        .def("StartNewResultSet",	&dealIIDataReporter::StartNewResultSet)
        .def("EndOfData",	    	&dealIIDataReporter::EndOfData)
        .def("SendVariable",	  	&dealIIDataReporter::SendVariable)
    ;

    class_<daepython::dealiiFiniteElementSystemWrapper<1>, bases<daeFiniteElementObject>, boost::noncopyable>("dealiiFiniteElementSystem_1D", no_init)
        .def(init<std::string,
                  const Quadrature<1>&,
                  const Quadrature<0>&,
                  boost::python::list,
                  boost::python::object>((arg("meshFilename"),
                                          arg("quadrature"),
                                          arg("faceQuadrature"),
                                          arg("dofs"),
                                          arg("weakForm")
                                        )))

        .def("AssembleSystem",      &daepython::dealiiFiniteElementSystemWrapper<1>::AssembleSystem,
                                    &daepython::dealiiFiniteElementSystemWrapper<1>::def_AssembleSystem, ( arg("self") ))
        .def("ReAssembleSystem",    &daepython::dealiiFiniteElementSystemWrapper<1>::ReAssembleSystem,
                                    &daepython::dealiiFiniteElementSystemWrapper<1>::def_ReAssembleSystem, ( arg("self") ))
        .def("NeedsReAssembling",   &daepython::dealiiFiniteElementSystemWrapper<1>::NeedsReAssembling,
                                    &daepython::dealiiFiniteElementSystemWrapper<1>::def_NeedsReAssembling, ( arg("self") ))
        .def("CreateDataReporter",  &dealiiFiniteElementSystem<1>::CreateDataReporter, ( arg("self") ), return_value_policy<manage_new_object>())
        .def("RowIndices",          &daepython::dealiiFiniteElementSystemWrapper<1>::GetRowIndices, ( arg("self"), arg("row") ))
        .def("GetDOFtoBoundaryMap", &dealiiFiniteElementSystem<1>::GetDOFtoBoundaryMap, ( arg("self") ))
    ;


    class_<daepython::dealiiFiniteElementSystemWrapper<2>, bases<daeFiniteElementObject>, boost::noncopyable>("dealiiFiniteElementSystem_2D", no_init)
        .def(init<std::string,
                  const Quadrature<2>&,
                  const Quadrature<1>&,
                  boost::python::list,
                  boost::python::object>((arg("meshFilename"),
                                          arg("quadrature"),
                                          arg("faceQuadrature"),
                                          arg("dofs"),
                                          arg("weakForm")
                                        )))

        .def("AssembleSystem",      &daepython::dealiiFiniteElementSystemWrapper<2>::AssembleSystem,
                                    &daepython::dealiiFiniteElementSystemWrapper<2>::def_AssembleSystem, ( arg("self") ))
        .def("ReAssembleSystem",    &daepython::dealiiFiniteElementSystemWrapper<2>::ReAssembleSystem,
                                    &daepython::dealiiFiniteElementSystemWrapper<2>::def_ReAssembleSystem, ( arg("self") ))
        .def("NeedsReAssembling",   &daepython::dealiiFiniteElementSystemWrapper<2>::NeedsReAssembling,
                                    &daepython::dealiiFiniteElementSystemWrapper<2>::def_NeedsReAssembling, ( arg("self") ))
        .def("CreateDataReporter",  &dealiiFiniteElementSystem<2>::CreateDataReporter, ( arg("self") ), return_value_policy<manage_new_object>())
        .def("RowIndices",          &daepython::dealiiFiniteElementSystemWrapper<2>::GetRowIndices, ( arg("self"), arg("row") ))
        .def("GetDOFtoBoundaryMap", &dealiiFiniteElementSystem<2>::GetDOFtoBoundaryMap, ( arg("self") ))
    ;

    class_<daepython::dealiiFiniteElementSystemWrapper<3>, bases<daeFiniteElementObject>, boost::noncopyable>("dealiiFiniteElementSystem_3D", no_init)
        .def(init<std::string,
                  const Quadrature<3>&,
                  const Quadrature<2>&,
                  boost::python::list,
                  boost::python::object>((arg("meshFilename"),
                                          arg("quadrature"),
                                          arg("faceQuadrature"),
                                          arg("dofs"),
                                          arg("weakForm")
                                        )))

        .def("AssembleSystem",      &daepython::dealiiFiniteElementSystemWrapper<3>::AssembleSystem,
                                    &daepython::dealiiFiniteElementSystemWrapper<3>::def_AssembleSystem, ( arg("self") ))
        .def("ReAssembleSystem",    &daepython::dealiiFiniteElementSystemWrapper<3>::ReAssembleSystem,
                                    &daepython::dealiiFiniteElementSystemWrapper<3>::def_ReAssembleSystem, ( arg("self") ))
        .def("NeedsReAssembling",   &daepython::dealiiFiniteElementSystemWrapper<3>::NeedsReAssembling,
                                    &daepython::dealiiFiniteElementSystemWrapper<3>::def_NeedsReAssembling, ( arg("self") ))
        .def("CreateDataReporter",  &dealiiFiniteElementSystem<3>::CreateDataReporter, ( arg("self") ), return_value_policy<manage_new_object>())
        .def("RowIndices",          &daepython::dealiiFiniteElementSystemWrapper<3>::GetRowIndices, ( arg("self"), arg("row") ))
        .def("GetDOFtoBoundaryMap", &dealiiFiniteElementSystem<3>::GetDOFtoBoundaryMap, ( arg("self") ))
    ;
}
