#ifndef DAE_NLPSOLVER_H
#define DAE_NLPSOLVER_H

#include "stdafx.h"
#include "nlpsolver_class_factory.h"
#include "../Core/optimization.h"
#include <stdio.h>
#include <time.h>
#include <iomanip>
#include <fstream>
#include "../nlp_common.h"
#include "base_solvers.h"

#ifdef daeIPOPT
#include <IpTNLP.hpp>
#include <IpIpoptApplication.hpp>
using namespace Ipopt;
#endif

#ifdef daeBONMIN
#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif

#include "BonTMINLP.hpp"
#include "CoinTime.hpp"
#include "CoinError.hpp"

#include "BonOsiTMINLPInterface.hpp"
#include "BonIpoptSolver.hpp"
#include "BonCbc.hpp"
#include "BonBonminSetup.hpp"

#include "BonOACutGenerator2.hpp"
#include "BonEcpCuts.hpp"
#include "BonOaNlpOptim.hpp"
using namespace Ipopt;
using namespace Bonmin;
#endif

using namespace dae::core;
using namespace dae::solver;
using namespace dae::activity;

namespace dae
{
namespace nlpsolver
{
/******************************************************************
	daeNLP
*******************************************************************/
#ifdef daeIPOPT
class DAE_NLPSOLVER_API daeMINLP : public TNLP,
                                   public daeNLPCommon
#endif
#ifdef daeBONMIN
class DAE_NLPSOLVER_API daeMINLP : public TMINLP,
                                   public daeNLPCommon
#endif
{
public:
	daeMINLP(daeOptimization_t* pOptimization,
             daeSimulation_t*   pSimulation, 
		     daeDAESolver_t*    pDAESolver, 
		     daeDataReporter_t* pDataReporter, 
		     daeLog_t*          pLog);
	
	virtual ~daeMINLP(void);

public:
// TMINLP interface
#ifdef daeBONMIN
	virtual bool get_variables_types(Index n, 
									 VariableType* var_types);
	
	virtual bool get_variables_linearity(Index n, 
										 Ipopt::TNLP::LinearityType* var_types);
	
	virtual bool get_constraints_linearity(Index m, 
										   Ipopt::TNLP::LinearityType* const_types);
	
	virtual const TMINLP::SosInfo* sosConstraints() const;
	
	virtual const TMINLP::BranchingInfo* branchingInfo() const;
    
	virtual bool eval_gi(Index n, 
						 const Number* x, 
						 bool new_x, 
						 Index i, 
						 Number& gi);
	
    virtual bool eval_grad_gi(Index n, 
							  const Number* x, 
							  bool new_x,
							  Index i, 
							  Index& nele_grad_gi, 
							  Index* jCol,
							  Number* values);
#endif
	
// TNLP interface
	virtual bool get_nlp_info(Index& n, 
							  Index& m, 
							  Index& nnz_jac_g,
							  Index& nnz_h_lag, 
							  TNLP::IndexStyleEnum& index_style);
  
	virtual bool get_bounds_info(Index n, 
								 Number* x_l, 
								 Number* x_u,
								 Index m, 
								 Number* g_l, 
								 Number* g_u);
  
	virtual bool get_starting_point(Index n, 
									bool init_x, 
									Number* x,
									bool init_z, 
									Number* z_L, 
									Number* z_U,
									Index m, 
									bool init_lambda,
									Number* lambda);
  
	virtual bool eval_f(Index n, 
						const Number* x, 
						bool new_x, 
						Number& obj_value);
  
	virtual bool eval_grad_f(Index n, 
							 const Number* x, 
							 bool new_x, 
							 Number* grad_f);
  
	virtual bool eval_g(Index n, 
						const Number* x, 
						bool new_x, 
						Index m, 
						Number* g);
  
	virtual bool eval_jac_g(Index n, 
							const Number* x,
							bool new_x,
							Index m, 
							Index nele_jac, 
							Index* iRow, 
							Index *jCol,
							Number* values);
  
	virtual bool eval_h(Index n, 
						const Number* x, 
						bool new_x,
						Number obj_factor, 
						Index m, 
						const Number* lambda,
						bool new_lambda, 
						Index nele_hess, 
						Index* iRow,
						Index* jCol, 
						Number* values);
	
#ifdef daeIPOPT
	virtual bool intermediate_callback( AlgorithmMode mode,
										Index iter, 
										Number obj_value,
										Number inf_pr, 
										Number inf_du,
										Number mu, 
										Number d_norm,
										Number regularization_size,
										Number alpha_du, 
										Number alpha_pr,
										Index ls_trials,
										const IpoptData* ip_data,
										IpoptCalculatedQuantities* ip_cq);
	
	virtual void finalize_solution(SolverReturn status,
								   Index n, 
								   const Number* x, 
								   const Number* z_L,
								   const Number* z_U,
								   Index m, 
								   const Number* g, 
								   const Number* lambda,
								   Number obj_value,
								   const IpoptData* ip_data,
								   IpoptCalculatedQuantities* ip_cq);
#endif
#ifdef daeBONMIN
    virtual void finalize_solution(TMINLP::SolverReturn status,
                                   Index n, 
								   const Number* x, 
								   Number obj_value);
#endif
	
public:
	real_t* m_pdTempStorage;
};

/******************************************************************
	daeBONMINSolver
*******************************************************************/
class DAE_NLPSOLVER_API daeBONMINSolver : public daeNLPSolver_t
{
public:
	daeBONMINSolver(void);
	virtual ~daeBONMINSolver(void);

public:
	virtual void Initialize(daeOptimization_t* pOptimization,
                            daeSimulation_t*   pSimulation, 
							daeDAESolver_t*    pDAESolver, 
							daeDataReporter_t* pDataReporter, 
							daeLog_t*          pLog);
	virtual void Solve(void);
	virtual std::string GetName(void) const;
	
	void SetOption(const string& strOptionName, const string& strValue);
	void SetOption(const string& strOptionName, real_t dValue);
	void SetOption(const string& strOptionName, int iValue);
	
    void ClearOptions(void);
    void PrintOptions(void);
    void PrintUserOptions(void);
	void LoadOptionsFile(const string& strOptionsFile);
    void SetOpenBLASNoThreads(int n);

protected:
    daeOptimization_t*  m_pOptimization;
	daeSimulation_t*	m_pSimulation;
	daeDAESolver_t*		m_pDAESolver;
	daeLog_t*			m_pLog;
	daeDataReporter_t*	m_pDataReporter;
	
#ifdef daeBONMIN
	SmartPtr<TMINLP>	m_MINLP;	
	BonminSetup			m_Bonmin;
#endif	
#ifdef daeIPOPT
	SmartPtr<TNLP>				m_NLP;	
	SmartPtr<IpoptApplication>	m_Application;
#endif
	
};

}
}
#endif
