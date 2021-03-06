#ifndef DAE_SUPERLU_SOLVERS_H
#define DAE_SUPERLU_SOLVERS_H
#include "../Core/solver.h"
//#include "../IDAS_DAESolver/ida_la_solver_interface.h"

namespace dae
{
namespace solver
{
#ifdef daeSuperLU
daeLASolver_t* daeCreateSuperLUSolver(void);
#endif

#ifdef daeSuperLU_MT
daeLASolver_t* daeCreateSuperLU_MTSolver(void);
#endif
}
}

#endif
