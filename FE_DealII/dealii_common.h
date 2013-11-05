#ifndef DAE_DEALII_COMMON_H
#define DAE_DEALII_COMMON_H

#include <deal.II/base/point.h>
#include <deal.II/base/tensor.h>
#include <deal.II/base/function.h>
#include <deal.II/lac/vector.h>
#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/sparsity_pattern.h>
#include "../Core/definitions.h"
#include <typeinfo>

namespace dae
{
namespace fe_solver
{
using namespace dealii;
/*********************************************************
 * deal.II related classes and typedefs
 *********************************************************/
typedef Function<1> Function_1D;
typedef Function<2> Function_2D;
typedef Function<3> Function_3D;

// Tensors of rank=1 are used for a gradient of scalar functions
typedef Tensor<1, 1, double> Tensor_1_1D;
typedef Tensor<1, 2, double> Tensor_1_2D;
typedef Tensor<1, 3, double> Tensor_1_3D;

// Points are in fact tensors with rank=1 just their coordinates mean length
// and have some additional functions
typedef Point<1, double> Point_1D;
typedef Point<2, double> Point_2D;
typedef Point<3, double> Point_3D;

///******************************************************************
//    dealiiFunction
//*******************************************************************/
//template <int dim>
//class dealiiFunction : public Function<dim>
//{
//public:
//    dealiiFunction(const unsigned int n_components = 1) : Function<dim>(n_components)
//    {
//    }
//};

/*********************************************************
 * daeFEMatrix
 * A wrapper around deal.II SparseMatrix<double>
 *********************************************************/
template<typename REAL = double>
class daeFEMatrix : public daeMatrix<REAL>
{
public:
    daeFEMatrix(const SparseMatrix<REAL>& matrix) : deal_ii_matrix(matrix)
    {
    }

    virtual ~daeFEMatrix(void)
    {
    }

public:
    virtual REAL GetItem(size_t row, size_t col) const
    {
        return deal_ii_matrix(row, col);
    }

    virtual void SetItem(size_t row, size_t col, REAL value)
    {
        // ACHTUNG, ACHTUNG!! Setting a new value is NOT permitted!
        daeDeclareAndThrowException(exInvalidCall);
    }

    virtual size_t GetNrows(void) const
    {
        return deal_ii_matrix.n();
    }

    virtual size_t GetNcols(void) const
    {
        return deal_ii_matrix.m();
    }

protected:
    const SparseMatrix<REAL>& deal_ii_matrix;
};

/*********************************************************
 * daeFEArray
 * A wrapper around deal.II Vector<REAL>
 *********************************************************/
template<typename REAL = double>
class daeFEArray : public daeArray<REAL>
{
public:
    daeFEArray(const Vector<REAL>& vect) : deal_ii_vector(vect)
    {
    }

    virtual ~daeFEArray(void)
    {
    }

public:
    REAL operator [](size_t i) const
    {
        return deal_ii_vector[i];
    }

    REAL GetItem(size_t i) const
    {
        return deal_ii_vector[i];
    }

    void SetItem(size_t i, REAL value)
    {
        // ACHTUNG, ACHTUNG!! Setting a new value is NOT permitted!
        daeDeclareAndThrowException(exInvalidCall);
    }

    size_t GetSize(void) const
    {
        return deal_ii_vector.size();
    }

protected:
    const Vector<REAL>& deal_ii_vector;
};

/*********************************************************
 * dealiiSparsityPatternIterator
 * A wrapper around dealSparsityPattern::iterator
 *********************************************************/
class dealiiSparsityPatternIterator : public daeSparseMatrixRowIterator
{
public:
    dealiiSparsityPatternIterator(const SparsityPattern::iterator& start, const SparsityPattern::iterator& end):
        m_iterator(start),
        m_end(end)
    {
    }

    void first()
    {
    }

    void next()
    {
        ++m_iterator;
    }

    bool isDone()
    {
        return m_iterator == m_end;
    }

    unsigned int currentItem()
    {
        return m_iterator->column();
    }

public:
    SparsityPattern::iterator m_iterator;
    SparsityPattern::iterator m_end;
};


/*********************************************************
 * feExpression
 *********************************************************/
enum efeNumberType
{
    eFEScalar = 0,
    eFETensor1,
    eFETensor2,
    eFEPoint,
    eFEInvalid
};

enum efeFunctionCall
{
    eFunctionValue,
    eFunctionGradient
};

const int fe_q = -1000;
const int fe_i = -1001;
const int fe_j = -1002;


template<int dim>
class feNode;

template<int dim>
class feExpression
{
public:
    typedef typename boost::shared_ptr< feNode<dim> > feNodePtr;

    feExpression()
    {
    }

    feExpression(feNodePtr node) : m_node(node)
    {
    }

    std::string ToString() const
    {
        return m_node->ToString();
    }

public:
    feNodePtr m_node;
};


template<int dim>
class feRuntimeNumber
{
public:
    feRuntimeNumber()
    {
        m_eType = eFEInvalid;
    }

    feRuntimeNumber(double v)
    {
        m_eType = eFEScalar;
        m_value = v;
    }

    feRuntimeNumber(const Tensor<1, dim, double>& t)
    {
        m_eType   = eFETensor1;
        m_tensor1 = t;
    }

    feRuntimeNumber(const Tensor<2, dim, double>& t)
    {
        m_eType   = eFETensor2;
        m_tensor2 = t;
    }

    feRuntimeNumber(const Point<dim, double>& t)
    {
        m_eType = eFEPoint;
        m_point = t;
    }

    std::string ToString() const
    {
        if(m_eType == eFEScalar)
        {
            return (boost::format("%f") % m_value).str();
        }
        else if(m_eType == eFETensor1)
        {
            if(dim == 1)
                return (boost::format("(%f)") % m_tensor1[0]).str();
            else if(dim == 2)
                return (boost::format("(%f, %f)") % m_tensor1[0] % m_tensor1[1]).str();
            else if(dim == 3)
                return (boost::format("(%f, %f, %f)") % m_tensor1[0] % m_tensor1[1] % m_tensor1[2]).str();
        }
        else if(m_eType == eFETensor2)
        {
            return "";
        }
        else if(m_eType == eFEPoint)
        {
            if(dim == 1)
                return (boost::format("(%f)") % m_point[0]).str();
            else if(dim == 2)
                return (boost::format("(%f, %f)") % m_point[0] % m_point[1]).str();
            else if(dim == 3)
                return (boost::format("(%f, %f, %f)") % m_point[0] % m_point[1] % m_point[2]).str();
        }
        else
            throw std::runtime_error(std::string("Invalid runtime number type"));
    }

public:
    efeNumberType          m_eType;
    Point<dim, double>     m_point;
    Tensor<1, dim, double> m_tensor1;
    Tensor<2, dim, double> m_tensor2;
    double                 m_value;
};

template<int dim>
feRuntimeNumber<dim> operator -(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = -fe.m_value;
    }
    else if(fe.m_eType == eFETensor1)
    {
        tmp.m_eType = eFETensor1;
        tmp.m_tensor1 = -fe.m_tensor1;
    }
    else if(fe.m_eType == eFETensor2)
    {
        tmp.m_eType = eFETensor2;
        tmp.m_tensor2 = -fe.m_tensor2;
    }
    else if(fe.m_eType == eFEPoint)
    {
        tmp.m_eType = eFEPoint;
        tmp.m_point = -fe.m_point;
    }
    else
        throw std::runtime_error(std::string("Invalid operation - ") + typeid(fe).name());
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> operator +(const feRuntimeNumber<dim>& l, const feRuntimeNumber<dim>& r)
{
    feRuntimeNumber<dim> tmp;
    if(l.m_eType == eFEScalar && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = l.m_value + r.m_value;
    }
    else if(l.m_eType == eFETensor1 && r.m_eType == eFETensor1)
    {
        tmp.m_eType = eFETensor1;
        tmp.m_tensor1 = l.m_tensor1 + r.m_tensor1;
    }
    else if(l.m_eType == eFETensor2 && r.m_eType == eFETensor2)
    {
        tmp.m_eType = eFETensor2;
        tmp.m_tensor2 = l.m_tensor2 + r.m_tensor2;
    }
    else if(l.m_eType == eFEPoint && r.m_eType == eFEPoint)
    {
        tmp.m_eType = eFEPoint;
        tmp.m_tensor1 = l.m_point + r.m_point;
    }
    else
        throw std::runtime_error(std::string("Invalid operation ") + typeid(l).name() + " + " + typeid(r).name());
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> operator -(const feRuntimeNumber<dim>& l, const feRuntimeNumber<dim>& r)
{
    feRuntimeNumber<dim> tmp;
    if(l.m_eType == eFEScalar && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = l.m_value - r.m_value;
    }
    else if(l.m_eType == eFETensor1 && r.m_eType == eFETensor1)
    {
        tmp.m_eType = eFETensor1;
        tmp.m_tensor1 = l.m_tensor1 - r.m_tensor1;
    }
    else if(l.m_eType == eFETensor2 && r.m_eType == eFETensor2)
    {
        tmp.m_eType = eFETensor2;
        tmp.m_tensor2 = l.m_tensor2 - r.m_tensor2;
    }
    else if(l.m_eType == eFEPoint && r.m_eType == eFEPoint)
    {
        tmp.m_eType = eFEPoint;
        tmp.m_tensor1 = l.m_point - r.m_point;
    }
    else
        throw std::runtime_error(std::string("Invalid operation ") + typeid(l).name() + " - " + typeid(r).name());
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> operator *(const feRuntimeNumber<dim>& l, const feRuntimeNumber<dim>& r)
{
    feRuntimeNumber<dim> tmp;
    if(l.m_eType == eFEScalar && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = l.m_value * r.m_value;
    }
    else if(l.m_eType == eFETensor1 && r.m_eType == eFETensor1)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = l.m_tensor1 * r.m_tensor1;
    }
    else if(l.m_eType == eFETensor2 && r.m_eType == eFETensor2)
    {
        tmp.m_eType = eFETensor2;
        tmp.m_tensor2 = l.m_tensor2 * r.m_tensor2;
    }
    else if(l.m_eType == eFEPoint && r.m_eType == eFEPoint)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = l.m_point * r.m_point;
    }
    else if(l.m_eType == eFEScalar && r.m_eType == eFETensor1)
    {
        tmp.m_eType = eFETensor1;
        tmp.m_tensor1 = l.m_value * r.m_tensor1;
    }
    else if(l.m_eType == eFETensor1 && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFETensor1;
        tmp.m_tensor1 = l.m_tensor1 * r.m_value;
    }
    else if(l.m_eType == eFEScalar && r.m_eType == eFETensor2)
    {
        tmp.m_eType = eFETensor2;
        tmp.m_tensor2 = l.m_value * r.m_tensor2;
    }
    else if(l.m_eType == eFETensor2 && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFETensor2;
        tmp.m_tensor2 = l.m_tensor2 * r.m_value;
    }
    else if(l.m_eType == eFEScalar && r.m_eType == eFEPoint)
    {
        tmp.m_eType = eFEPoint;
        tmp.m_point = l.m_value * r.m_point;
    }
    else if(l.m_eType == eFEPoint && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEPoint;
        tmp.m_point = l.m_point * r.m_value;
    }
    else
        throw std::runtime_error(std::string("Invalid operation ") + typeid(l).name() + " * " + typeid(r).name());
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> operator /(const feRuntimeNumber<dim>& l, const feRuntimeNumber<dim>& r)
{
    feRuntimeNumber<dim> tmp;
    if(l.m_eType == eFEScalar && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = l.m_value + r.m_value;
    }
    else if(l.m_eType == eFETensor1 && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFETensor1;
        tmp.m_tensor1 = l.m_tensor1 / r.m_value;
    }
    else if(l.m_eType == eFETensor2 && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFETensor2;
        tmp.m_tensor2 = l.m_tensor2 / r.m_value;
    }
    else if(l.m_eType == eFEPoint && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEPoint;
        tmp.m_point = l.m_point / r.m_value;
    }
    else
        throw std::runtime_error(std::string("Invalid operation ") + typeid(l).name() + " / " + typeid(r).name());
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> operator ^(const feRuntimeNumber<dim>& l, const feRuntimeNumber<dim>& r)
{
    feRuntimeNumber<dim> tmp;
    if(l.m_eType == eFEScalar && r.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = pow(l.m_value, r.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation ") + typeid(l).name() + " ** " + typeid(r).name());
    return tmp;
}


template<int dim>
feRuntimeNumber<dim> sin_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = sin(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation sin(") + typeid(fe).name() + ")");
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> cos_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = cos(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation cos(") + typeid(fe).name() + ")");
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> tan_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = tan(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation tan(") + typeid(fe).name() + ")");
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> asin_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = asin(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation asin(") + typeid(fe).name() + ")");
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> acos_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = acos(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation acos(") + typeid(fe).name() + ")");
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> atan_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = atan(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation atan(") + typeid(fe).name() + ")");
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> sqrt_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = sqrt(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation sqrt(") + typeid(fe).name() + ")");
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> log_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = log(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation log(") + typeid(fe).name() + ")");
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> log10_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = log10(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation log10(") + typeid(fe).name() + ")");
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> exp_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = exp(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation exp(") + typeid(fe).name() + ")");
    return tmp;
}

template<int dim>
feRuntimeNumber<dim> abs_(const feRuntimeNumber<dim>& fe)
{
    feRuntimeNumber<dim> tmp;
    if(fe.m_eType == eFEScalar)
    {
        tmp.m_eType = eFEScalar;
        tmp.m_value = abs(fe.m_value);
    }
    else
        throw std::runtime_error(std::string("Invalid operation abs(") + typeid(fe).name() + ")");
    return tmp;
}


template<int dim>
class feCellContext
{
public:
    ~feCellContext(){}

public:
    virtual double shape_value(const unsigned int i,
                               const unsigned int j) const = 0;
    virtual double shape_value_component (const unsigned int i,
                                          const unsigned int j,
                                          const unsigned int component) const = 0;
    virtual const Tensor<1,dim>& shape_grad (const unsigned int i,
                                             const unsigned int j) const = 0;
    virtual Tensor<1,dim> shape_grad_component (const unsigned int i,
                                                const unsigned int j,
                                                const unsigned int component) const = 0;
    virtual const Tensor<2,dim>& shape_hessian (const unsigned int i,
                                                const unsigned int j) const = 0;
    virtual Tensor<2,dim> shape_hessian_component (const unsigned int i,
                                                   const unsigned int j,
                                                   const unsigned int component) const = 0;
    virtual const Point<dim>& quadrature_point (const unsigned int q) const = 0;
    virtual double JxW (const unsigned int q) const = 0;
    virtual const Point<dim>& normal_vector (const unsigned int q) const = 0;

    virtual const Function<dim>& function (const std::string& name) const = 0;

    virtual unsigned int q() const = 0;
    virtual unsigned int i() const = 0;
    virtual unsigned int j() const = 0;
};

template<int dim>
class feNode
{
public:
    virtual ~feNode(void){}

public:
    virtual feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const = 0;
    virtual std::string          ToString() const                                       = 0;
};

template<int dim>
class feNode_constant : public feNode<dim>
{
public:
    feNode_constant(double value)
    {
        m_value = value;
    }

public:
    feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const
    {
        return feRuntimeNumber<dim>(m_value);
    }

    std::string ToString() const
    {
        if(m_value < 0)
            return (boost::format("(%f)") % m_value).str();
        else
            return (boost::format("%f") % m_value).str();
    }

public:
    double m_value;
};

template<int dim>
unsigned int getIndex(int index, const feCellContext<dim>* pCellContext)
{
    if(index == fe_i)
        return pCellContext->i();
    else if(index == fe_j)
        return pCellContext->j();
    else if(index == fe_q)
        return pCellContext->q();
    else
        return index;
}

inline std::string getIndex(int index)
{
    if(index == fe_i)
        return "i";
    else if(index == fe_j)
        return "j";
    else if(index == fe_q)
        return "q";
    else
        return (boost::format("%d") % index).str();
}

template<int dim>
class feNode_phi : public feNode<dim>
{
public:
    feNode_phi(int i, int q, unsigned int component = 0)
    {
        m_i = i;
        m_q = q;
        m_component = component;
    }

public:
    feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const
    {
        unsigned int i = getIndex<dim>(m_i, pCellContext);
        unsigned int q = getIndex<dim>(m_q, pCellContext);
        return feRuntimeNumber<dim>( pCellContext->shape_value(i, q) );
    }

    std::string ToString() const
    {
        return (boost::format("phi(%d, %d)") % getIndex(m_i) % getIndex(m_q)).str();
    }

public:
    int            m_i;
    int            m_q;
    unsigned int   m_component;
};

template<int dim>
class feNode_dphi : public feNode<dim>
{
public:
    feNode_dphi(int i, int q, unsigned int component = 0)
    {
        m_i = i;
        m_q = q;
        m_component = component;
    }

public:
    feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const
    {
        unsigned int i = getIndex<dim>(m_i, pCellContext);
        unsigned int q = getIndex<dim>(m_q, pCellContext);
        return feRuntimeNumber<dim>( pCellContext->shape_grad(i, q) );
    }

    std::string ToString() const
    {
        return (boost::format("dphi(%d, %d)") % getIndex(m_i) % getIndex(m_q)).str();
    }

public:
    int            m_i;
    int            m_q;
    unsigned int   m_component;
};

template<int dim>
class feNode_d2phi : public feNode<dim>
{
public:
    feNode_d2phi(int i, int q, unsigned int component = 0)
    {
        m_i = i;
        m_q = q;
        m_component = component;
    }

public:
    feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const
    {
        unsigned int i = getIndex<dim>(m_i, pCellContext);
        unsigned int q = getIndex<dim>(m_q, pCellContext);
        return feRuntimeNumber<dim>( pCellContext->shape_hessian(i, q) );
    }

    std::string ToString() const
    {
        return (boost::format("d2phi(%d, %d)") % getIndex(m_i) % getIndex(m_q)).str();
    }

public:
    int            m_i;
    int            m_q;
    unsigned int   m_component;
};

template<int dim>
class feNode_JxW : public feNode<dim>
{
public:
    feNode_JxW(int q)
    {
        m_q = q;
    }

public:
    feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const
    {
        unsigned int q = getIndex<dim>(m_q, pCellContext);
        return feRuntimeNumber<dim>( pCellContext->JxW(q) );
    }

    std::string ToString() const
    {
        return (boost::format("JxW(%d)") % getIndex(m_q)).str();
    }

public:
    int m_q;
};

template<int dim>
class feNode_xyz : public feNode<dim>
{
public:
    feNode_xyz(int q)
    {
        m_q = q;
    }

public:
    feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const
    {
        unsigned int q = getIndex<dim>(m_q, pCellContext);
        return feRuntimeNumber<dim>( pCellContext->quadrature_point(q) );
    }

    std::string ToString() const
    {
        return (boost::format("xyz(%d)") % getIndex(m_q)).str();
    }

public:
    int m_q;
};


template<int dim>
class feNode_normal : public feNode<dim>
{
public:
    feNode_normal(int q)
    {
        m_q = q;
    }

public:
    feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const
    {
        unsigned int q = getIndex<dim>(m_q, pCellContext);
        return feRuntimeNumber<dim>( pCellContext->normal_vector(q) );
    }

    std::string ToString() const
    {
        return (boost::format("normal(%d)") % getIndex(m_q)).str();
    }

public:
    int m_q;
};

template<int dim>
class feNode_function : public feNode<dim>
{
public:
    typedef typename boost::shared_ptr< feNode<dim> > feNodePtr;

    feNode_function(const std::string& name, efeFunctionCall call, feNodePtr xyz_node, unsigned int component = -1)
    {
        if(!dynamic_cast<feNode_xyz<dim>*>(xyz_node.get()))
            throw std::runtime_error(std::string("An argument to the Function must be a point"));

        m_xyz_node = xyz_node;
        m_name = name;
        m_call = call;
        m_component = component;
    }

public:
    feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const
    {
        feRuntimeNumber<dim> node = m_xyz_node->Evaluate(pCellContext);

        if(node.m_eType != eFEPoint)
            throw std::runtime_error(std::string("An argument to the Function must be a point"));

        if(m_call == eFunctionValue)
        {
            if(m_component == -1)
                return feRuntimeNumber<dim>( pCellContext->function(m_name).value(node.m_point) );
            else
                return feRuntimeNumber<dim>( pCellContext->function(m_name).value(node.m_point, m_component) );

        }
        else if(m_call == eFunctionGradient)
        {
            if(m_component == -1)
                return feRuntimeNumber<dim>( pCellContext->function(m_name).gradient(node.m_point) );
            else
                return feRuntimeNumber<dim>( pCellContext->function(m_name).gradient(node.m_point, m_component) );
        }
        else
        {
            throw std::runtime_error(std::string("Invalid Function call type"));
        }
    }

    std::string ToString() const
    {
        if(m_call == eFunctionValue)
        {
            if(m_component == -1)
                return (boost::format("fvalue('%s'', %s)") % m_name % m_xyz_node->ToString()).str();
            else
                return (boost::format("fvalue(%s, %s, %d)") % m_name % m_xyz_node->ToString() % m_component).str();
        }
        else if(m_call == eFunctionGradient)
        {
            if(m_component == -1)
                return (boost::format("fgrad('%s'', %s)") % m_name % m_xyz_node->ToString()).str();
            else
                return (boost::format("fgrad(%s, %s, %d)") % m_name % m_xyz_node->ToString() % m_component).str();
        }
        else
            throw std::runtime_error(std::string("Invalid Function call type"));
    }

public:
    feNodePtr       m_xyz_node;
    std::string     m_name;
    efeFunctionCall m_call;
    unsigned int    m_component;
};

enum efeUnaryFunction
{
    eSign,
    eSqrt,
    eExp,
    eLog,
    eLog10,
    eAbs,
    eSin,
    eCos,
    eTan,
    eASin,
    eACos,
    eATan
};

enum efeBinaryFunction
{
    ePlus,
    eMinus,
    eMulti,
    eDivide,
    ePower
};

template<int dim>
class feNode_unary : public feNode<dim>
{
public:
    typedef typename boost::shared_ptr< feNode<dim> > feNodePtr;

    feNode_unary(efeUnaryFunction function, feNodePtr node)
    {
        m_function = function;
        m_node     = node;
    }

    feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const
    {
        feRuntimeNumber<dim> node = m_node->Evaluate(pCellContext);

        if(m_function == eSign)
            return -node;
        else if(m_function == eSqrt)
            return sqrt_(node);
        else if(m_function == eExp)
            return exp_(node);
        else if(m_function == eLog)
            return log_(node);
        else if(m_function == eLog10)
            return log10_(node);
        else if(m_function == eAbs)
            return abs_(node);
        else if(m_function == eSin)
            return sin_(node);
        else if(m_function == eCos)
            return cos_(node);
        else if(m_function == eTan)
            return tan_(node);
        else if(m_function == eASin)
            return asin_(node);
        else if(m_function == eACos)
            return acos_(node);
        else if(m_function == eATan)
            return atan_(node);
        else
            throw std::runtime_error(std::string("Invalid unary function"));
    }

    std::string ToString() const
    {
        if(m_function == eSign)
            return (boost::format("-(%s)") % m_node->ToString()).str();
        else if(m_function == eSqrt)
            return (boost::format("sqrt(%s)") % m_node->ToString()).str();
        else if(m_function == eExp)
            return (boost::format("exp(%s)") % m_node->ToString()).str();
        else if(m_function == eLog)
            return (boost::format("log(%s)") % m_node->ToString()).str();
        else if(m_function == eLog10)
            return (boost::format("log10(%s)") % m_node->ToString()).str();
        else if(m_function == eAbs)
            return (boost::format("abs(%s)") % m_node->ToString()).str();
        else if(m_function == eSin)
            return (boost::format("sin(%s)") % m_node->ToString()).str();
        else if(m_function == eCos)
            return (boost::format("cos(%s)") % m_node->ToString()).str();
        else if(m_function == eTan)
            return (boost::format("tan(%s)") % m_node->ToString()).str();
        else if(m_function == eASin)
            return (boost::format("asin(%s)") % m_node->ToString()).str();
        else if(m_function == eACos)
            return (boost::format("acos(%s)") % m_node->ToString()).str();
        else if(m_function == eATan)
            return (boost::format("atan(%s)") % m_node->ToString()).str();
        else
            throw std::runtime_error(std::string("Invalid unary function"));
    }

public:
    efeUnaryFunction m_function;
    feNodePtr        m_node;
};

template<int dim>
class feNode_binary : public feNode<dim>
{
public:
    typedef typename boost::shared_ptr< feNode<dim> > feNodePtr;

    feNode_binary(efeBinaryFunction function, feNodePtr left, feNodePtr right)
    {
        m_function = function;
        m_left     = left;
        m_right    = right;
    }

    feRuntimeNumber<dim> Evaluate(const feCellContext<dim>* pCellContext) const
    {
        feRuntimeNumber<dim> left  = m_left->Evaluate(pCellContext);
        feRuntimeNumber<dim> right = m_right->Evaluate(pCellContext);

        if(m_function == ePlus)
            return left + right;
        else if(m_function == eMinus)
            return left - right;
        else if(m_function == eMulti)
            return left * right;
        else if(m_function == eDivide)
            return left / right;
        else if(m_function == ePower)
            return left ^ right;

        throw std::runtime_error(std::string("Invalid binary function"));
    }

    std::string ToString() const
    {
        if(m_function == ePlus)
            return (boost::format("(%s + %s)") % m_left->ToString() % m_right->ToString()).str();
        else if(m_function == eMinus)
            return (boost::format("(%s - %s)") % m_left->ToString() % m_right->ToString()).str();
        else if(m_function == eMulti)
            return (boost::format("(%s * %s)") % m_left->ToString() % m_right->ToString()).str();
        else if(m_function == eDivide)
            return (boost::format("(%s / %s)") % m_left->ToString() % m_right->ToString()).str();
        else if(m_function == ePower)
            return (boost::format("(%s ** %s)") % m_left->ToString() % m_right->ToString()).str();

        throw std::runtime_error(std::string("Invalid unary function"));
    }

public:
    efeBinaryFunction m_function;
    feNodePtr         m_left;
    feNodePtr         m_right;
};

template<int dim>
feExpression<dim> operator +(const feExpression<dim>& l, const feExpression<dim>& r)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(ePlus, l.m_node, r.m_node) ));
}

template<int dim>
feExpression<dim> operator -(const feExpression<dim>& l, const feExpression<dim>& r)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(eMinus, l.m_node, r.m_node) ));
}

template<int dim>
feExpression<dim> operator *(const feExpression<dim>& l, const feExpression<dim>& r)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(eMulti, l.m_node, r.m_node) ));
}

template<int dim>
feExpression<dim> operator /(const feExpression<dim>& l, const feExpression<dim>& r)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(eDivide, l.m_node, r.m_node) ));
}


template<int dim>
feExpression<dim> operator +(const feExpression<dim>& l, double r)
{
    typename feExpression<dim>::feNodePtr rnode( new feNode_constant<dim>(r) );
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(ePlus, l.m_node, rnode) ));
}

template<int dim>
feExpression<dim> operator -(const feExpression<dim>& l, double r)
{
    typename feExpression<dim>::feNodePtr rnode( new feNode_constant<dim>(r) );
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(eMinus, l.m_node, rnode) ));
}

template<int dim>
feExpression<dim> operator *(const feExpression<dim>& l, double r)
{
    typename feExpression<dim>::feNodePtr rnode( new feNode_constant<dim>(r) );
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(eMulti, l.m_node, rnode) ));
}

template<int dim>
feExpression<dim> operator /(const feExpression<dim>& l, double r)
{
    typename feExpression<dim>::feNodePtr rnode( new feNode_constant<dim>(r) );
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(eDivide, l.m_node, rnode) ));
}

template<int dim>
feExpression<dim> operator ^(const feExpression<dim>& l, double r)
{
    typename feExpression<dim>::feNodePtr rnode( new feNode_constant<dim>(r) );
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(ePower, l.m_node, rnode) ));
}

template<int dim>
feExpression<dim> pow(const feExpression<dim>& l, double r)
{
    typename feExpression<dim>::feNodePtr rnode( new feNode_constant<dim>(r) );
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(ePower, l.m_node, rnode) ));
}


template<int dim>
feExpression<dim> operator +(double l, const feExpression<dim>& r)
{
    typename feExpression<dim>::feNodePtr lnode( new feNode_constant<dim>(l) );
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(ePlus, lnode, r.m_node) ));
}

template<int dim>
feExpression<dim> operator -(double l, const feExpression<dim>& r)
{
    typename feExpression<dim>::feNodePtr lnode( new feNode_constant<dim>(l) );
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(eMinus, lnode, r.m_node) ));
}

template<int dim>
feExpression<dim> operator *(double l, const feExpression<dim>& r)
{
    typename feExpression<dim>::feNodePtr lnode( new feNode_constant<dim>(l) );
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(eMulti, lnode, r.m_node) ));
}

template<int dim>
feExpression<dim> operator /(double l, const feExpression<dim>& r)
{
    typename feExpression<dim>::feNodePtr lnode( new feNode_constant<dim>(l) );
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_binary<dim>(eDivide, lnode, r.m_node) ));
}


template<int dim>
feExpression<dim> operator -(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eSign, fe.m_node) ));
}

template<int dim>
feExpression<dim> sqrt(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eSqrt, fe.m_node) ));
}

template<int dim>
feExpression<dim> exp(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eExp, fe.m_node) ));
}

template<int dim>
feExpression<dim> log(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eLog, fe.m_node) ));
}

template<int dim>
feExpression<dim> log10(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eLog10, fe.m_node) ));
}

template<int dim>
feExpression<dim> abs(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eAbs, fe.m_node) ));
}

template<int dim>
feExpression<dim> sin(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eSin, fe.m_node) ));
}

template<int dim>
feExpression<dim> cos(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eCos, fe.m_node) ));
}

template<int dim>
feExpression<dim> tan(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eTan, fe.m_node) ));
}

template<int dim>
feExpression<dim> asin(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eASin, fe.m_node) ));
}

template<int dim>
feExpression<dim> acos(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eACos, fe.m_node) ));
}

template<int dim>
feExpression<dim> atan(const feExpression<dim>& fe)
{
    return feExpression<dim>(typename feExpression<dim>::feNodePtr( new feNode_unary<dim>(eATan, fe.m_node) ));
}







template<int dim>
feExpression<dim> constant(double value)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_constant<dim>(value) ) );
}

template<int dim>
feExpression<dim> phi(int i, int q)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_phi<dim>(i, q) ) );
}

template<int dim>
feExpression<dim> dphi(int i, int q)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_dphi<dim>(i, q) ) );
}

template<int dim>
feExpression<dim> d2phi(int i, int q)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_d2phi<dim>(i, q) ) );
}

template<int dim>
feExpression<dim> xyz(int q)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_xyz<dim>(q) ) );
}

template<int dim>
feExpression<dim> JxW(int q)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_JxW<dim>(q) ) );
}

template<int dim>
feExpression<dim> normal(int q)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_normal<dim>(q) ) );
}

template<int dim>
feExpression<dim> function_value(const std::string name, const feExpression<dim>& xyz)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_function<dim>(name, eFunctionValue, xyz.m_node, -1) ) );
}

template<int dim>
feExpression<dim> function_value2(const std::string name, const feExpression<dim>& xyz, unsigned int component)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_function<dim>(name, eFunctionValue, xyz.m_node, component) ) );
}

template<int dim>
feExpression<dim> function_gradient(const std::string name, const feExpression<dim>& xyz)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_function<dim>(name, eFunctionGradient, xyz.m_node, -1) ) );
}

template<int dim>
feExpression<dim> function_gradient2(const std::string name, const feExpression<dim>& xyz, unsigned int component)
{
    return feExpression<dim>( typename feExpression<dim>::feNodePtr( new feNode_function<dim>(name, eFunctionGradient, xyz.m_node, component) ) );
}





template<int dim>
class feCellContext_dummy : public feCellContext<dim>
{
public:
    feCellContext_dummy()
    {
    }

public:
    virtual double shape_value(const unsigned int i,
                               const unsigned int j) const
    {
        return 1.5;
    }

    virtual double shape_value_component (const unsigned int i,
                                          const unsigned int j,
                                          const unsigned int component) const
    {
        return 2.5;
    }

    virtual const Tensor<1,dim>& shape_grad (const unsigned int i,
                                             const unsigned int j) const
    {
        static Tensor<1,dim> tensor;
        return tensor;
    }

    virtual Tensor<1,dim> shape_grad_component (const unsigned int i,
                                                const unsigned int j,
                                                const unsigned int component) const
    {
        static Tensor<1,dim> tensor;
        return tensor;
    }

    virtual const Tensor<2,dim>& shape_hessian (const unsigned int i,
                                                const unsigned int j) const
    {
        static Tensor<2,dim> tensor;
        return tensor;
    }

    virtual Tensor<2,dim> shape_hessian_component (const unsigned int i,
                                                   const unsigned int j,
                                                   const unsigned int component) const
    {
        static Tensor<2,dim> tensor;
        return tensor;
    }

    virtual const Point<dim>& quadrature_point (const unsigned int q) const
    {
        static Point<dim> point;
        return point;
    }

    virtual double JxW (const unsigned int q) const
    {
        return 123.15;
    }

    virtual const Point<dim>& normal_vector (const unsigned int q) const
    {
        static Point<dim> point;
        return point;
    }

    virtual const Function<dim>& function (const std::string& name) const
    {
        static ConstantFunction<dim> fn(153.2);
        return fn;
    }

    virtual unsigned int q() const
    {
        return 1;
    }

    virtual unsigned int i() const
    {
        return 2;
    }

    virtual unsigned int j() const
    {
        return 3;
    }
};

template<int dim>
feCellContext<dim>* getDummyCellContext()
{
    return new feCellContext_dummy<dim>();
}

template<int dim>
feRuntimeNumber<dim> Evaluate(const feExpression<dim>& number, feCellContext<dim>* pContext)
{
    return number.m_node->Evaluate(pContext);
}

}
}

#endif
