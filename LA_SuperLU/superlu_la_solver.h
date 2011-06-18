#ifndef MKL_PARDISO_LA_SOLVER_H
#define MKL_PARDISO_LA_SOLVER_H

#include "../IDAS_DAESolver/ida_la_solver_interface.h"
#include "../IDAS_DAESolver/solver_class_factory.h"
#include "../IDAS_DAESolver/dae_array_matrix.h"
#include <idas/idas.h>
#include <idas/idas_impl.h>
#include <nvector/nvector_serial.h>
#include <sundials/sundials_types.h>
#include <sundials/sundials_math.h>

#ifdef daeSuperLU_MT
#include <pdsp_defs.h>
#endif

#ifdef daeSuperLU_CUDA
#include <cuda.h>
#include <cuda_runtime_api.h>
#include "superlu_mt_gpu.h"
#endif

#ifdef daeSuperLU
#include <slu_ddefs.h>
#endif

				
namespace dae
{
namespace solver
{
daeIDALASolver_t* daeCreateSuperLUSolver(void);


class DAE_SOLVER_API daeSuperLUSolver : public dae::solver::daeIDALASolver_t
{
	typedef daeCSRMatrix<real_t, int> daeSuperLUMatrix;
public:
	daeSuperLUSolver();
	~daeSuperLUSolver();
	
public:
	int Create(void* ida, size_t n, daeDAESolver_t* pDAESolver);
	int Reinitialize(void* ida);
	int SaveAsXPM(const std::string& strFileName);
	int SaveAsMatrixMarketFile(const std::string& strFileName, const std::string& strMatrixName, const std::string& strMatrixDescription);
	string GetName(void) const;

	int Init(void* ida);
	int Setup(void* ida,
			  N_Vector	vectorVariables, 
			  N_Vector	vectorTimeDerivatives, 
			  N_Vector	vectorResiduals,
			  N_Vector	vectorTemp1, 
			  N_Vector	vectorTemp2, 
			  N_Vector	vectorTemp3);
	int Solve(void* ida,
			  N_Vector	b, 
			  N_Vector	weight, 
			  N_Vector	vectorVariables,
			  N_Vector	vectorTimeDerivatives, 
			  N_Vector	vectorResiduals);
	int Free(void* ida);

#ifdef daeSuperLU_MT
	superlumt_options_t& GetOptions(void);
#endif

#ifdef daeSuperLU_CUDA
	void SetMatrixValues_A(void);
	void Factorize(void);
	void Solve(double** b);
	void AllocateMatrix_A(void);
	void AllocateMatrix_B(void);
	void FreeMatrixStore_A(void);
	void FreeMatrixStore_B(void);
	void FreeFactorizationData(void);
	
#endif

#ifdef daeSuperLU
	superlu_options_t& GetOptions(void);
#endif
	
protected:
	void InitializeSuperLU(size_t nnz);
	void FreeMemory(void);
	void PrintStats(void);
	
public:
	daeBlock_t*			m_pBlock;
	int					m_nNoEquations;  
	real_t*				m_vecB;
	real_t*				m_vecX;
	daeDAESolver_t*		m_pDAESolver;
	size_t				m_nJacobianEvaluations;
	
	daeDenseArray		m_arrValues;
	daeDenseArray		m_arrTimeDerivatives;
	daeDenseArray		m_arrResiduals;
	daeSuperLUMatrix	m_matJacobian;
	bool				m_bFactorizationDone;

#ifdef daeSuperLU_MT
	SuperMatrix			m_matA;
	SuperMatrix			m_matB;
	SuperMatrix			m_matX;
	SuperMatrix			m_matL;	
	SuperMatrix			m_matU;	
	SuperMatrix			m_matAC;

    superlumt_options_t	m_Options;
    Gstat_t				m_Stats;
    int*				m_perm_c;
    int*				m_perm_r;
	int					m_lwork;
	void*				m_work;
#endif
	
#ifdef daeSuperLU_CUDA
    superlu_mt_gpu_solver	m_superlu_mt_gpuSolver;
#endif
	
#ifdef daeSuperLU
	SuperMatrix			m_matA;
	SuperMatrix			m_matB;
	SuperMatrix			m_matX;
	SuperMatrix			m_matL;	
	SuperMatrix			m_matU;	

    superlu_options_t	m_Options;
    mem_usage_t			m_memUsage;
    SuperLUStat_t		m_Stats;
    int*				m_etree;
    real_t*				m_R;
    real_t*				m_C;
	char				m_equed;
    real_t				m_ferr;
    real_t				m_berr;
	SuperMatrix			m_matAC;
	
	int					m_lwork;
	void*				m_work;
    int*				m_perm_c;
    int*				m_perm_r;
#endif
	
	double m_solve, m_factorize;
};

}
}

#endif
