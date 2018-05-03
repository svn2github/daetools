#include "stdafx.h"
using namespace std;
#include "ida_solver.h"
#include "../Core/helpers.h"
#include "dae_solvers.h"
#include <idas/idas.h>
#include <idas/idas_impl.h>
#include <idas/idas_dense.h>
//#include <idas/idas_lapack.h>
#include <idas/idas_spgmr.h>

#define JACOBIAN(A) (A->cols)

namespace dae
{
namespace solver
{
class daeBlockOfEquations : public daeBlockOfEquations_t
{
public:
    daeBlockOfEquations(daeBlock_t* block)
    {
        m_pBlock = block;
    }

public:
    int CalcNonZeroElements()
    {
        int nnz;
        m_pBlock->CalcNonZeroElements(nnz);
        return nnz;
    }

    void FillSparseMatrix(daeSparseMatrix<real_t>* pmatrix)
    {
        m_pBlock->FillSparseMatrix(pmatrix);
    }

    void CalculateJacobian(real_t				time,
                           real_t				inverseTimeStep,
                           daeArray<real_t>&	arrValues,
                           daeArray<real_t>&	arrResiduals,
                           daeArray<real_t>&	arrTimeDerivatives,
                           daeMatrix<real_t>&	matJacobian)
    {
        m_pBlock->CalculateJacobian(time,
                                    arrValues,
                                    arrResiduals,
                                    arrTimeDerivatives,
                                    matJacobian,
                                    inverseTimeStep);
    }
public:
    daeBlock_t* m_pBlock;
};

int init_la(IDAMem ida_mem);
int setup_la(IDAMem ida_mem,
              N_Vector	vectorVariables,
              N_Vector	vectorTimeDerivatives,
              N_Vector	vectorResiduals,
              N_Vector	vectorTemp1,
              N_Vector	vectorTemp2,
              N_Vector	vectorTemp3);
int solve_la(IDAMem ida_mem,
              N_Vector	b,
              N_Vector	weight,
              N_Vector	vectorVariables,
              N_Vector	vectorTimeDerivatives,
              N_Vector	vectorResiduals);
int free_la(IDAMem ida_mem);

int residuals(realtype	time,
              N_Vector	vectorVariables,
              N_Vector	vectorTimeDerivatives,
              N_Vector	vectorResiduals,
              void*		pUserData);

int roots(realtype	time,
          N_Vector	vectorVariables,
          N_Vector	vectorTimeDerivatives,
          realtype*	gout,
          void*		pUserData);

int jacobian(long int    Neq,
             realtype	time,
             realtype	dInverseTimeStep,
             N_Vector	vectorVariables,
             N_Vector	vectorTimeDerivatives,
             N_Vector	vectorResiduals,
             DlsMat		dense_matrixJacobian,
             void*		pUserData,
             N_Vector	vectorTemp1,
             N_Vector	vectorTemp2,
             N_Vector	vectorTemp3);

int sens_residuals(int			Ns,
                   realtype		time,
                   N_Vector		vectorVariables,
                   N_Vector		vectorTimeDerivatives,
                   N_Vector		vectorResVal, /* WTF ?? */
                   N_Vector*	pvectorSVariables,
                   N_Vector*	pvectorSTimeDerivatives,
                   N_Vector*	pvectorResidualValues,
                   void*		pUserData,
                   N_Vector		vectorTemp1,
                   N_Vector		vectorTemp2,
                   N_Vector		vectorTemp3);

int setup_preconditioner(realtype	time,
                         N_Vector	vectorVariables,
                         N_Vector	vectorTimeDerivatives,
                         N_Vector	vectorResiduals,
                         realtype	dInverseTimeStep,
                         void*		pUserData,
                         N_Vector	vectorTemp1,
                         N_Vector	vectorTemp2,
                         N_Vector	vectorTemp3);

int solve_preconditioner(realtype	time,
                         N_Vector	vectorVariables,
                         N_Vector	vectorTimeDerivatives,
                         N_Vector	vectorResiduals,
                         N_Vector	vectorR,
                         N_Vector	vectorZ,
                         realtype	dInverseTimeStep,
                         realtype	delta,
                         void*		pUserData,
                         N_Vector	vectorTemp);

int jac_times_vector(realtype time,
                     N_Vector vectorVariables,
                     N_Vector vectorTimeDerivatives,
                     N_Vector vectorResiduals,
                     N_Vector vectorV,
                     N_Vector vectorJV,
                     realtype dInverseTimeStep,
                     void*    pUserData,
                     N_Vector tmp1,
                     N_Vector tmp2);

void error_function(int error_code,
                    const char *module,
                    const char *function,
                    char *msg,
                    void *pUserData);

daeIDASolver::daeIDASolver(void)
{
    m_pLog					         = NULL;
    m_pBlock				         = NULL;
    m_pIDA					         = NULL;
    m_eLASolver				         = eSundialsLU;
    m_pLASolver				         = NULL;
    m_dCurrentTime			         = 0;
    m_nNumberOfEquations	         = 0;
    m_timeStart				         = 0;
    m_dTargetTime			         = 0;
    m_bCalculateSensitivities        = false;
    m_bIsModelDynamic				 = false;
    m_eInitialConditionMode          = eAlgebraicValuesProvided;

    m_pIDASolverData.reset(new daeIDASolverData);

    daeConfig& cfg = daeConfig::GetConfig();
    m_dRelTolerance                             = cfg.GetFloat("daetools.IDAS.relativeTolerance",                         1e-5);
    m_dNextTimeAfterReinitialization            = cfg.GetFloat("daetools.IDAS.nextTimeAfterReinitialization",             1e-5);
    m_strSensitivityMethod                      = cfg.GetString("daetools.IDAS.sensitivityMethod",                        "Simultaneous");
    m_bErrorControl                             = cfg.GetBoolean("daetools.IDAS.SensErrCon",					          false);
    m_bPrintInfo                                = cfg.GetBoolean("daetools.IDAS.printInfo",                               false);
    m_bResetLAMatrixAfterDiscontinuity          = cfg.GetBoolean("daetools.core.resetLAMatrixAfterDiscontinuity",         true);
    m_iNumberOfSTNRebuildsDuringInitialization  = cfg.GetInteger("daetools.IDAS.numberOfSTNRebuildsDuringInitialization", 1000);

    m_dSensRelTolerance                         = cfg.GetFloat("daetools.IDAS.sensRelativeTolerance",                1e-5);
    m_dSensAbsTolerance                         = cfg.GetFloat("daetools.IDAS.sensAbsoluteTolerance",                1e-5);
}

daeIDASolver::~daeIDASolver(void)
{
    Finalize();
}

daeIDALASolver_t* daeIDASolver::GetLASolver() const
{
    return m_pLASolver;
}

void daeIDASolver::SetLASolver(daeLASolver_t* pLASolver)
{
    daeIDALASolver_t* la_solver = static_cast<daeIDALASolver_t*>(pLASolver);
    if(!la_solver)
        daeDeclareAndThrowException(exInvalidCall);

    m_eLASolver = eThirdParty;
    m_pLASolver = la_solver;
}

void daeIDASolver::SetLASolver(daeeIDALASolverType eLASolverType)
{
    if(eLASolverType == eThirdParty)
        daeDeclareAndThrowException(exInvalidCall);

    m_eLASolver = eLASolverType;
    m_pLASolver = NULL;
}

void daeIDASolver::SetTimeHorizon(real_t timeHorizon)
{
    if(!m_pIDA)
        daeDeclareAndThrowException(exInvalidPointer);
    if(timeHorizon <= 0)
        daeDeclareAndThrowException(exInvalidCall);

    IDASetStopTime(m_pIDA, timeHorizon);
}

std::string daeIDASolver::GetName(void) const
{
    return string("Sundials IDAS");
}

size_t daeIDASolver::GetNumberOfVariables(void) const
{
    return m_nNumberOfEquations;
}

void daeIDASolver::SetRelativeTolerance(real_t relTol)
{
    m_dRelTolerance = relTol;

    if(m_pIDA)
    {
        int retval = IDASVtolerances(m_pIDA, m_dRelTolerance, m_pIDASolverData->m_vectorAbsTolerances);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exMiscellanous);
            e << "Sundials IDAS solver cowardly refused to set the relative tolerance; " << CreateIDAErrorMessage(retval);
            throw e;
        }
    }
}

real_t daeIDASolver::GetRelativeTolerance(void) const
{
    return m_dRelTolerance;
}

void daeIDASolver::Initialize(daeBlock_t* pBlock,
                              daeLog_t* pLog,
                              daeSimulation_t* pSimulation,
                              daeeInitialConditionMode eMode,
                              bool bCalculateSensitivities,
                              const std::vector<size_t>& narrParametersIndexes)
{
    if(!pBlock)
        daeDeclareAndThrowException(exInvalidPointer);
    if(!pLog)
        daeDeclareAndThrowException(exInvalidPointer);
    if(!pSimulation)
        daeDeclareAndThrowException(exInvalidPointer);

    m_pLog					= pLog;
    m_pBlock				= pBlock;
    m_pSimulation			= pSimulation;
    m_eInitialConditionMode = eMode;

    m_nNumberOfEquations    = m_pBlock->GetNumberOfEquations();

    m_pBlockOfEquations.reset(new daeBlockOfEquations(m_pBlock));

// Create data IDA vectors etc
    CreateArrays();

// Setting initial conditions and initial values, and set rel. and abs. tolerances
// and determine if the model is dynamic or steady-state
    SetInitialOptions();

// Create IDA structure
    CreateIDA();

// Sensitivity related data
    m_bCalculateSensitivities = bCalculateSensitivities;
    m_narrParametersIndexes   = narrParametersIndexes;

    if(bCalculateSensitivities && narrParametersIndexes.empty())
    {
        daeDeclareException(exInvalidPointer);
        e << "In order to calculate sensitivities the list of parameters must be given.";
        throw e;
    }

    if(m_bCalculateSensitivities)
        SetupSensitivityCalculation();

// Create linear solver
    CreateLinearSolver();

// Rebuild the expression map and set the number of roots
    RefreshEquationSetAndRootFunctions();
}

void daeIDASolver::Finalize()
{
    if(m_pIDA)
        IDAFree(&m_pIDA);

    m_pLog      = NULL;
    m_pBlock    = NULL;
    m_pIDA      = NULL;
    m_pLASolver = NULL;
    m_pIDASolverData.reset();
    m_pBlockOfEquations.reset();
}

void daeIDASolver::CreateArrays(void)
{
    m_pIDASolverData->CreateSerialArrays(m_nNumberOfEquations);
}

void daeIDASolver::SetInitialOptions(void)
{
    daeRawDataArray<real_t> arrAbsoluteTolerances;
    daeRawDataArray<real_t> arrInitialConditionsTypes;
    daeRawDataArray<real_t> arrValueConstraints;
    realtype *pVariableValues, *pTimeDerivatives, *pInitialConditionsTypes, *pAbsoluteTolerances, *pValueConstraints;

    pVariableValues         = NV_DATA_S(m_pIDASolverData->m_vectorVariables);
    pTimeDerivatives        = NV_DATA_S(m_pIDASolverData->m_vectorTimeDerivatives);
    pInitialConditionsTypes = NV_DATA_S(m_pIDASolverData->m_vectorInitialConditionsTypes);
    pAbsoluteTolerances		= NV_DATA_S(m_pIDASolverData->m_vectorAbsTolerances);
    pValueConstraints		= NV_DATA_S(m_pIDASolverData->m_vectorValueConstraints);

    /*
       We have to fill the initial values of Variables and Time derivatives, set the
       Variable types (whether the variable is algebraic or differential) and set the
       Absolute tolerances from the model.
       To do so first we create/initialize arrays which will be used to copy the values
       by the function FillAbsoluteTolerancesInitialConditionsAndInitialGuesses().
    */
    m_arrValues.InitArray(m_nNumberOfEquations,				  pVariableValues);
    m_arrTimeDerivatives.InitArray(m_nNumberOfEquations,	  pTimeDerivatives);
    arrInitialConditionsTypes.InitArray(m_nNumberOfEquations, pInitialConditionsTypes);
    arrAbsoluteTolerances.InitArray(m_nNumberOfEquations,     pAbsoluteTolerances);
    arrValueConstraints.InitArray(m_nNumberOfEquations,       pValueConstraints);

    if(m_eInitialConditionMode == eQuasiSteadyState)
        memset(pTimeDerivatives, 0, m_nNumberOfEquations*sizeof(realtype));

    m_pBlock->FillAbsoluteTolerancesInitialConditionsAndInitialGuesses(m_arrValues,
                                                                       m_arrTimeDerivatives,
                                                                       arrInitialConditionsTypes,
                                                                       arrAbsoluteTolerances,
                                                                       arrValueConstraints);

// Determine if the model is dynamic or steady-state
    m_bIsModelDynamic = m_pBlock->IsModelDynamic();
    if(m_bPrintInfo)
        m_pLog->Message(string("The model is ") + string(m_bIsModelDynamic ? "dynamic" : "steady-state"), 0);

/*
   Now we have all the initial data copied to the DAESolver vectors so we can
   set the pointers mapping between the data in DAESolver and DataProxy
*/
    m_pBlock->CreateIndexMappings(pVariableValues, pTimeDerivatives);
    m_pBlock->SetBlockData(m_arrValues, m_arrTimeDerivatives);
}

void daeIDASolver::CreateIDA(void)
{
    int	retval;

    m_pIDA = IDACreate();
    if(!m_pIDA)
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver faint-heartedly failed to allocate a memory for itself";
        throw e;
    }

    retval = IDASetId(m_pIDA, m_pIDASolverData->m_vectorInitialConditionsTypes);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver faint-heartedly failed to set initial condition types; " << CreateIDAErrorMessage(retval);
        throw e;
    }

    retval = IDAInit(m_pIDA,
                     residuals,
                     m_timeStart,
                     m_pIDASolverData->m_vectorVariables,
                     m_pIDASolverData->m_vectorTimeDerivatives);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver faint-heartedly failed to allocate IDA structure; " << CreateIDAErrorMessage(retval);
        throw e;
    }

    retval = IDASVtolerances(m_pIDA, m_dRelTolerance, m_pIDASolverData->m_vectorAbsTolerances);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver cowardly refused to set tolerances; " << CreateIDAErrorMessage(retval);
        throw e;
    }

    retval = IDASetUserData(m_pIDA, this);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver cowardly refused to set residual data; " << CreateIDAErrorMessage(retval);
        throw e;
    }


    real_t* pValueConstraints = NV_DATA_S(m_pIDASolverData->m_vectorValueConstraints);
    bool constraintsSet = false;
    for(size_t i = 0; i < m_nNumberOfEquations; i++)
    {
        if(pValueConstraints[i] != 0.0) // zero means no constraint set
        {
            constraintsSet = true;
            break;
        }
    }
    // If all constraints are 0.0 - no constraints were set; therefore, do not call IDASetConstraints
    if(constraintsSet)
    {
        retval = IDASetConstraints(m_pIDA, m_pIDASolverData->m_vectorValueConstraints);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exMiscellanous);
            e << "Sundials IDAS solver cowardly refused to set value constraints; " << CreateIDAErrorMessage(retval);
            throw e;
        }
    }

    real_t fval;
    bool bval;
    daeConfig& cfg = daeConfig::GetConfig();

    IDASetMaxOrd(m_pIDA,         cfg.GetInteger("daetools.IDAS.MaxOrd",         5));
    IDASetMaxNumSteps(m_pIDA,    cfg.GetInteger("daetools.IDAS.MaxNumSteps",    500));

    fval = cfg.GetFloat("daetools.IDAS.InitStep", 0.0);
    if(fval > 0.0)
        IDASetInitStep(m_pIDA, fval);

    fval = cfg.GetFloat("daetools.IDAS.MaxStep", 0.0);
    if(fval > 0.0)
        IDASetMaxStep(m_pIDA, fval);

    IDASetMaxErrTestFails(m_pIDA,   cfg.GetInteger("daetools.IDAS.MaxErrTestFails", 10));
    IDASetMaxNonlinIters(m_pIDA,    cfg.GetInteger("daetools.IDAS.MaxNonlinIters",  4));
    IDASetMaxConvFails(m_pIDA,      cfg.GetInteger("daetools.IDAS.MaxConvFails",    10));
    IDASetNonlinConvCoef(m_pIDA,    cfg.GetFloat  ("daetools.IDAS.NonlinConvCoef",  0.33));
    IDASetSuppressAlg(m_pIDA,       cfg.GetBoolean("daetools.IDAS.SuppressAlg",     false));
    bval = cfg.GetBoolean("daetools.IDAS.NoInactiveRootWarn", false);
    if(bval)
        IDASetNoInactiveRootWarn(m_pIDA);

    // Set error function
    IDASetErrHandlerFn(m_pIDA, error_function, this);

// After a successful initialization we do not need some data; therefore free whatever is possible
// Clear vectors of variable ids since they are not needed anymore (their copies are kept in the IDA structure).
// Here it does not matter who owns the data in the vectors; IDA keeps track of it and wont free the data it does not own
    m_pIDASolverData->CleanUpSetupData();
}

void daeIDASolver::SetupSensitivityCalculation(void)
{
    int	retval;
    int iSensMethod;

    size_t Ns = m_narrParametersIndexes.size();

    if(!m_bCalculateSensitivities || Ns == 0)
    {
        daeDeclareException(exInvalidCall);
        e << "Sundials IDAS solver abjectly refused to enable sensitivity calculation: it has not been enabled";
        throw e;
    }

// Create matrixes S, SD and Sres
    m_pIDASolverData->CreateSensitivityArrays(Ns);

    if(m_strSensitivityMethod == "Simultaneous")
    {
        iSensMethod = IDA_SIMULTANEOUS;
    }
    else if(m_strSensitivityMethod == "Staggered")
    {
        iSensMethod = IDA_STAGGERED;
    }
    else
    {
        daeDeclareException(exInvalidCall);
        e << "IDAS solver error: invalid sensitivity method " << m_strSensitivityMethod;
        throw e;
    }

    retval = IDASensInit(m_pIDA, Ns, iSensMethod, sens_residuals, m_pIDASolverData->m_pvectorSVariables, m_pIDASolverData->m_pvectorSTimeDerivatives);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver abjectly refused to setup sensitivity calculation; " << CreateIDAErrorMessage(retval);
        throw e;
    }

    //retval = IDASensEEtolerances(m_pIDA);
    std::vector<real_t> arrAbsTols;
    arrAbsTols.resize(Ns, m_dSensAbsTolerance);
    retval = IDASensSStolerances(m_pIDA, m_dSensRelTolerance, &arrAbsTols[0]);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver abjectly refused to setup sensitivity error tolerances; " << CreateIDAErrorMessage(retval);
        throw e;
    }

    retval = IDASetSensErrCon(m_pIDA, m_bErrorControl);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver abjectly refused to setup sensitivity error control; " << CreateIDAErrorMessage(retval);
        throw e;
    }

    daeConfig& cfg = daeConfig::GetConfig();
    int nNonlinIters = cfg.GetInteger("daetools.IDAS.maxNonlinIters", 3);
    retval = IDASetSensMaxNonlinIters(m_pIDA, nNonlinIters);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver abjectly refused to setup sensitivity maximal number of nonlinear iterations; " << CreateIDAErrorMessage(retval);
        throw e;
    }
}

// Think about possibility to initialize matrix m_matSValues only once;
// Then the GetSensitivities call can simply return m_matSValues with no
// additional processing
daeMatrix<real_t>& daeIDASolver::GetSensitivities(void)
{
    int	retval;

    if(!m_bCalculateSensitivities)
        daeDeclareAndThrowException(exInvalidCall)

    // Do not use m_dCurrentTime since this function can be called after the system has been reset
    // and tretlast has not been reset to zero (CHECK WHY!!!).
    real_t currentTime;
    retval = IDAGetSens(m_pIDA, &currentTime, m_pIDASolverData->m_pvectorSVariables);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver spinelessly failed to return sensitivities; " << CreateIDAErrorMessage(retval);
        throw e;
    }

    if(!m_pIDASolverData->ppdSValues)
        daeDeclareAndThrowException(exInvalidPointer)
    if(!m_pIDASolverData->m_pvectorSVariables)
        daeDeclareAndThrowException(exInvalidPointer)

    size_t N  = m_pIDASolverData->m_N;
    size_t Ns = m_pIDASolverData->m_Ns;

    for(int i = 0; i < Ns; i++)
        m_pIDASolverData->ppdSValues[i] = NV_DATA_S(m_pIDASolverData->m_pvectorSVariables[i]);

    m_matSValues.InitMatrix(Ns, N, m_pIDASolverData->ppdSValues, eRowWise);
    return m_matSValues;
}

void daeIDASolver::CreateLinearSolver(void)
{
    int	retval;

    if(m_eLASolver == eSundialsLU)
    {
    // Sundials dense LU LA Solver
        retval = IDADense(m_pIDA, (long)m_nNumberOfEquations);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exRuntimeCheck);
            e << "Sundials IDAS solver ignobly refused to create Sundials dense linear solver; " << CreateIDAErrorMessage(retval);
            throw e;
        }

        retval = IDADlsSetDenseJacFn(m_pIDA, jacobian);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exRuntimeCheck);
            e << "Sundials IDAS solver ignobly refused to set Jacobian function for Sundials dense linear solver; " << CreateIDAErrorMessage(retval);
            throw e;
        }
    }
    else if(m_eLASolver == eSundialsLapack)
    {
    // Lapack dense LU LA Solver
//		retval = IDALapackDense(m_pIDA, (long)m_nNumberOfEquations);
//		if(!CheckFlag(retval))
//		{
//			daeDeclareException(exRuntimeCheck);
//			e << "Sundials IDAS solver ignobly refused to create Sundials dense linear solver; " << CreateIDAErrorMessage(retval);
//			throw e;
//		}

        retval = IDADlsSetDenseJacFn(m_pIDA, jacobian);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exRuntimeCheck);
            e << "Sundials IDAS solver ignobly refused to set Jacobian function for Sundials dense linear solver; " << CreateIDAErrorMessage(retval);
            throw e;
        }
    }
    else if(m_eLASolver == eSundialsGMRES)
    {
        daeDeclareAndThrowException(exNotImplemented);
/*
    // Sundials dense GMRES LA Solver
        retval = IDASpgmr(m_pIDA, 20);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exRuntimeCheck);
            e << "Sundials IDAS solver ignobly refused to create Sundials GMRES linear solver; " << CreateIDAErrorMessage(retval);
            throw e;
        }

//		retval = IDASpilsSetJacTimesVecFn(m_pIDA, jac_times_vector);
//		if(!CheckFlag(retval))
//		{
//			daeDeclareException(exRuntimeCheck);
//			e << "Sundials IDAS solver ignobly refused to set jacobian x vector function for Sundials GMRES linear solver; " << CreateIDAErrorMessage(retval);
//			throw e;
//		}

        retval = IDASpilsSetPreconditioner(m_pIDA, setup_preconditioner, solve_preconditioner);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exRuntimeCheck);
            e << "Sundials IDAS solver ignobly refused to set preconditioner functions for Sundials GMRES linear solver; " << CreateIDAErrorMessage(retval);
            throw e;
        }

//		m_pIDASolverData->CreatePreconditionerArrays(m_nNumberOfEquations);
*/
    }
    else if(m_eLASolver == eThirdParty)
    {
        if(m_pLASolver == NULL)
        {
            daeDeclareException(exRuntimeCheck);
            e << "Sundials IDAS solver ignobly refused to set the third party linear solver: the solver object is null";
            throw e;
        }

    // Third party LA Solver
        int nnz = 0;
        m_pBlock->CalcNonZeroElements(nnz);
        retval = m_pLASolver->Create(m_nNumberOfEquations, nnz, m_pBlockOfEquations.get());
        if(!CheckFlag(retval))
        {
            daeDeclareException(exRuntimeCheck);
            e << "The third party linear solver chickenly failed to initialize it self; " << CreateIDAErrorMessage(retval);
            throw e;
        }

        IDAMem ida_mem = (IDAMem)m_pIDA;

        ida_mem->ida_linit        = init_la;
        ida_mem->ida_lsetup       = setup_la;
        ida_mem->ida_lsolve       = solve_la;
        ida_mem->ida_lperf        = NULL;
        ida_mem->ida_lfree        = free_la;
        ida_mem->ida_lmem         = m_pLASolver;
        ida_mem->ida_setupNonNull = TRUE;
    }
    else
    {
        daeDeclareException(exRuntimeCheck);
        e << "Sundials IDAS solver abjectly refused to set an unspecified linear solver";
        throw e;
    }
}

void daeIDASolver::RefreshEquationSetAndRootFunctions(void)
{
    int	retval;
    size_t nNoRoots;

    if(!m_pBlock)
        daeDeclareAndThrowException(exInvalidPointer);

    m_pBlock->RebuildActiveEquationSetAndRootExpressions(m_bCalculateSensitivities);
    nNoRoots = m_pBlock->GetNumberOfRoots();

    retval = IDARootInit(m_pIDA, (int)nNoRoots, roots);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver timidly failed to initialize roots' functions; " << CreateIDAErrorMessage(retval);
        throw e;
    }
}

void daeIDASolver::SolveInitial(void)
{
    int retval, iCounter;

    if(!m_pLog || !m_pBlock || m_nNumberOfEquations == 0)
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver timidly refuses to solve initial: the solver has not been initialized";
        throw e;
    }

    daeConfig& cfg = daeConfig::GetConfig();

    IDASetNonlinConvCoefIC(m_pIDA,  cfg.GetFloat  ("daetools.IDAS.NonlinConvCoefIC", 0.0033));
    IDASetMaxNumStepsIC(m_pIDA,     cfg.GetInteger("daetools.IDAS.MaxNumStepsIC",    5));
    IDASetMaxNumJacsIC(m_pIDA,      cfg.GetInteger("daetools.IDAS.MaxNumJacsIC",     4));
    IDASetMaxNumItersIC(m_pIDA,     cfg.GetInteger("daetools.IDAS.MaxNumItersIC",    10));
    IDASetLineSearchOffIC(m_pIDA,   cfg.GetBoolean("daetools.IDAS.LineSearchOffIC",  false));

    for(iCounter = 0; iCounter < m_iNumberOfSTNRebuildsDuringInitialization; iCounter++)
    {
        if(m_eInitialConditionMode == eAlgebraicValuesProvided)
            retval = IDACalcIC(m_pIDA, IDA_YA_YDP_INIT, m_dNextTimeAfterReinitialization);
        else if(m_eInitialConditionMode == eQuasiSteadyState)
            retval = IDACalcIC(m_pIDA, IDA_Y_INIT, m_dNextTimeAfterReinitialization);
        else
            daeDeclareAndThrowException(exNotImplemented);

        if(!CheckFlag(retval))
        {
            // Report the data with the values at the failure (they are already in m_arrValues and m_arrTimeDerivatives)
            realtype* pdValues			 = NV_DATA_S(m_pIDASolverData->m_vectorVariables);
            realtype* pdTimeDerivatives	 = NV_DATA_S(m_pIDASolverData->m_vectorTimeDerivatives);
            realtype* srcValues          = m_arrValues.Data();
            realtype* srcTimeDerivatives = m_arrTimeDerivatives.Data();
            memcpy(pdValues,          srcValues,          m_nNumberOfEquations*sizeof(realtype));
            memcpy(pdTimeDerivatives, srcTimeDerivatives, m_nNumberOfEquations*sizeof(realtype));
            m_pSimulation->ReportData(0);

            daeDeclareException(exMiscellanous);
            e << "Sundials IDAS solver cowardly failed to calculate initial conditions at TIME = 0; " << CreateIDAErrorMessage(retval);
            e << string("\n") << m_strLastIDASError;
            throw e;
        }

        // Get the corrected IC and send them to the block
        retval = IDAGetConsistentIC(m_pIDA, m_pIDASolverData->m_vectorVariables, m_pIDASolverData->m_vectorTimeDerivatives);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exMiscellanous);
            e << "Could not get the corrected initial conditions from the Sundials IDAS solver at TIME = 0; " << CreateIDAErrorMessage(retval);
            throw e;
        }

        realtype* pdValues			= NV_DATA_S(m_pIDASolverData->m_vectorVariables);
        realtype* pdTimeDerivatives	= NV_DATA_S(m_pIDASolverData->m_vectorTimeDerivatives);

        m_arrValues.InitArray         (m_nNumberOfEquations, pdValues);
        m_arrTimeDerivatives.InitArray(m_nNumberOfEquations, pdTimeDerivatives);

        m_pBlock->SetBlockData(m_arrValues, m_arrTimeDerivatives);

        if(m_pBlock->CheckForDiscontinuities())
        {
            m_pBlock->ExecuteOnConditionActions();
            RefreshEquationSetAndRootFunctions();
            ResetIDASolver(true, m_dCurrentTime, false);

            // debug
            if(m_bPrintInfo)
            {
            }
        }
        else
        {
            break;
        }
    }

    if(iCounter >= m_iNumberOfSTNRebuildsDuringInitialization)
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver cowardly failed to calculate initial conditions at TIME = 0: Max number of STN rebuilds reached; " << CreateIDAErrorMessage(retval);
        throw e;
    }

// Get the corrected sensitivity IC
    if(m_bCalculateSensitivities)
    {
        retval = IDAGetSensConsistentIC(m_pIDA, m_pIDASolverData->m_pvectorSVariables, m_pIDASolverData->m_pvectorSTimeDerivatives);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exMiscellanous);
            e << "Could not get the corrected sensitivity initial conditions from the Sundials IDAS solver at TIME = "
              << m_dCurrentTime << "; " << CreateIDAErrorMessage(retval);
            throw e;
        }
    }

// Here we have a problem. If the model is steady-state then IDACalcIC does nothing!
// The system is not solved!! We have to do something?
// We can run the system for a small period of time and then return (here only for one step: IDA_ONE_STEP).
// But what if IDASolve fails?
    if(!m_bIsModelDynamic)
    {
        retval = IDASolve(m_pIDA,
                          m_dNextTimeAfterReinitialization,
                          &m_dCurrentTime,
                          m_pIDASolverData->m_vectorVariables,
                          m_pIDASolverData->m_vectorTimeDerivatives,
                          IDA_ONE_STEP);
        if(!CheckFlag(retval))
        {
            // Report the data with the values at the failure
            m_pSimulation->ReportData(0);

            daeDeclareException(exMiscellanous);
            e << "Sundials IDAS solver cowardly failed to initialize the system at TIME = 0; " << CreateIDAErrorMessage(retval);
            throw e;
        }
    }
}

void daeIDASolver::Reset(void)
{
    // Achtung, Achtung!!
    // Resets sensitivities, too!
    ResetIDASolver(true, 0, true);
}

void daeIDASolver::Reinitialize(bool bCopyDataFromBlock, bool bResetSensitivities)
{
    int retval, iCounter;

    if(m_bPrintInfo)
    {
        string strMessage = "Reinitializing at time: " + toString<real_t>(m_dCurrentTime);
        m_pLog->Message(strMessage, 0);
    }

    RefreshEquationSetAndRootFunctions();
    ResetIDASolver(bCopyDataFromBlock, m_dCurrentTime, bResetSensitivities);

    for(iCounter = 0; iCounter < m_iNumberOfSTNRebuildsDuringInitialization; iCounter++)
    {
        // Here we always use the IDA_YA_YDP_INIT flag (and discard InitialConditionMode).
        // The reason is that in this phase we may have been reinitialized the diff. variables
        // with the new values and using the eQuasySteadyState flag would be meaningless.
        retval = IDACalcIC(m_pIDA, IDA_YA_YDP_INIT, m_dCurrentTime + m_dNextTimeAfterReinitialization);
        if(!CheckFlag(retval))
        {
            // Report the data with the values at the failure (they are already in m_arrValues and m_arrTimeDerivatives)
            realtype* pdValues			= NV_DATA_S(m_pIDASolverData->m_vectorVariables);
            realtype* pdTimeDerivatives	= NV_DATA_S(m_pIDASolverData->m_vectorTimeDerivatives);
            realtype* srcValues          = m_arrValues.Data();
            realtype* srcTimeDerivatives = m_arrTimeDerivatives.Data();
            memcpy(pdValues,          srcValues,          m_nNumberOfEquations*sizeof(realtype));
            memcpy(pdTimeDerivatives, srcTimeDerivatives, m_nNumberOfEquations*sizeof(realtype));
            m_pSimulation->ReportData(m_dCurrentTime);

            daeDeclareException(exMiscellanous);
            e << "Sundials IDAS solver dastardly failed re-initialize the system at TIME = " << m_dCurrentTime << "; "
              << CreateIDAErrorMessage(retval);
            e << string("\n") << m_strLastIDASError;
            throw e;
        }

        // Get the corrected IC and send them to the block
        retval = IDAGetConsistentIC(m_pIDA, m_pIDASolverData->m_vectorVariables, m_pIDASolverData->m_vectorTimeDerivatives);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exMiscellanous);
            e << "Could not get the corrected initial conditions from the Sundials IDAS solver at TIME = "
              << m_dCurrentTime << "; " << CreateIDAErrorMessage(retval);
            throw e;
        }

        realtype* pdValues			= NV_DATA_S(m_pIDASolverData->m_vectorVariables);
        realtype* pdTimeDerivatives	= NV_DATA_S(m_pIDASolverData->m_vectorTimeDerivatives);

        m_arrValues.InitArray         (m_nNumberOfEquations, pdValues);
        m_arrTimeDerivatives.InitArray(m_nNumberOfEquations, pdTimeDerivatives);

        m_pBlock->SetBlockData(m_arrValues, m_arrTimeDerivatives);

        if(m_pBlock->CheckForDiscontinuities())
        {
            m_pBlock->ExecuteOnConditionActions();
            RefreshEquationSetAndRootFunctions();
            ResetIDASolver(true, m_dCurrentTime, false);

            // debug
            if(m_bPrintInfo)
            {
            }
        }
        else
        {
            break;
        }
    }

    if(iCounter >= m_iNumberOfSTNRebuildsDuringInitialization)
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver dastardly failed re-initialize the system at TIME = " << m_dCurrentTime << ": Max number of STN rebuilds reached; "
          << CreateIDAErrorMessage(retval);
        throw e;
    }

// Get the corrected sensitivity IC
    if(m_bCalculateSensitivities)
    {
        retval = IDAGetSensConsistentIC(m_pIDA, m_pIDASolverData->m_pvectorSVariables, m_pIDASolverData->m_pvectorSTimeDerivatives);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exMiscellanous);
            e << "Could not get the corrected sensitivity initial conditions from the Sundials IDAS solver at TIME = "
              << m_dCurrentTime << "; " << CreateIDAErrorMessage(retval);
            throw e;
        }
    }
}

void daeIDASolver::ResetIDASolver(bool bCopyDataFromBlock, real_t t0, bool bResetSensitivities)
{
    int retval, iSensMethod;

    if(!m_pLog || !m_pBlock || m_nNumberOfEquations == 0)
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver timidly refuses to reset: the solver has not been initialized";
        throw e;
    }

// Set the current time
    m_dCurrentTime = t0;
    m_pBlock->SetTime(t0);

/*
******************************************************************************
   This part is not needed now, since I directly access the solver's data;
   therefore there is no need to copy anything.
******************************************************************************
// Copy data from the block if requested
    if(bCopyDataFromBlock)
    {
        N = m_pBlock->GetNumberOfEquations();

        pdValues			= NV_DATA_S(m_pIDASolverData->m_vectorVariables);
        pdTimeDerivatives	= NV_DATA_S(m_pIDASolverData->m_vectorTimeDerivatives);

        m_arrValues.InitArray(N, pdValues);
        m_arrTimeDerivatives.InitArray(N, pdTimeDerivatives);

        m_pBlock->CopyDataToSolver(m_arrValues, m_arrTimeDerivatives);
    }
*/

// Call the LA solver Reinitialize, if needed.
// (the sparse matrix pattern has been changed after the discontinuity
    if(m_eLASolver == eThirdParty)
    {
        if(m_bResetLAMatrixAfterDiscontinuity)
        {
            int nnz = 0;
            m_pBlock->CalcNonZeroElements(nnz);
            m_pLASolver->Reinitialize(nnz);
        }
    }

// ReInit IDA
    retval = IDAReInit(m_pIDA,
                       m_dCurrentTime,
                       m_pIDASolverData->m_vectorVariables,
                       m_pIDASolverData->m_vectorTimeDerivatives);
    if(!CheckFlag(retval))
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver timidly refused to re-init at TIME = " << m_dCurrentTime << "; "
          << CreateIDAErrorMessage(retval);
        throw e;
    }

// ReInit sensitivities
    if(m_bCalculateSensitivities && bResetSensitivities)
    {
        size_t Ns = m_narrParametersIndexes.size();
        for(size_t i = 0; i < Ns; i++)
            N_VConst(0, m_pIDASolverData->m_pvectorSVariables[i]);
        for(size_t i = 0; i < Ns; i++)
            N_VConst(0, m_pIDASolverData->m_pvectorSTimeDerivatives[i]);

        if(m_strSensitivityMethod == "Simultaneous")
        {
            iSensMethod = IDA_SIMULTANEOUS;
        }
        else if(m_strSensitivityMethod == "Staggered")
        {
            iSensMethod = IDA_STAGGERED;
        }
        else
        {
            daeDeclareException(exInvalidCall);
            e << "IDAS solver error: invalid sensitivity method " << m_strSensitivityMethod;
            throw e;
        }

        retval = IDASensReInit(m_pIDA, iSensMethod, m_pIDASolverData->m_pvectorSVariables, m_pIDASolverData->m_pvectorSTimeDerivatives);
        if(!CheckFlag(retval))
        {
            daeDeclareException(exMiscellanous);
            e << "Sundials IDAS solver abjectly refused to re-init sensitivity calculation; " << CreateIDAErrorMessage(retval);
            throw e;
        }
    }
}

real_t daeIDASolver::Solve(real_t dTime, daeeStopCriterion eCriterion, bool bReportDataAroundDiscontinuities)
{
    int retval;
    daeeDiscontinuityType eDiscontinuityType;

    if(!m_pLog || !m_pBlock || m_nNumberOfEquations == 0)
    {
        daeDeclareException(exMiscellanous);
        e << "Sundials IDAS solver cravenly refuses to solve the system: the solver has not been initialized";
        throw e;
    }

    if(dTime <= m_dCurrentTime)
    {
        daeDeclareException(exInvalidCall);
        e << "Sundials IDAS solver cravenly refuses to solve the system: an invalid time horizon specified ["
          << dTime << "]; it must be higher than [" << m_dCurrentTime << "]";
        throw e;
    }
    m_dTargetTime = dTime;

    for(;;)
    {
        retval = IDASolve(m_pIDA, m_dTargetTime, &m_dCurrentTime, m_pIDASolverData->m_vectorVariables, m_pIDASolverData->m_vectorTimeDerivatives, IDA_NORMAL);
        if(retval == IDA_TOO_MUCH_WORK)
        {
            m_pLog->Message(string("Warning: IDAS solver error at TIME = ") + toString(m_dCurrentTime) + string(" [IDA_TOO_MUCH_WORK]"), 0);
            m_pLog->Message(string("  Try to increase daetools.IDAS.MaxNumSteps option in daetools.cfg"), 0);
            realtype tolsfac = 0;
            retval = IDAGetTolScaleFactor(m_pIDA, &tolsfac);
            m_pLog->Message(string("  Suggested factor by which the user’s tolerances should be scaled is ") + toString(tolsfac), 0);
            continue;
        }
        else if(!CheckFlag(retval))
        {
            // First report data at the last successful time and then raise an exception
            m_pSimulation->ReportData(m_dCurrentTime);

            daeDeclareException(exMiscellanous);
            e << "Sundials IDAS solver cowardly failed to solve the system at TIME = " << m_dCurrentTime
              << "; time horizon [" << m_dTargetTime << "]; " << CreateIDAErrorMessage(retval);
            e << string("\n") << m_strLastIDASError;
            throw e;
        }

        //int kcur;
        //realtype hcur;
        //IDAGetCurrentOrder(m_pIDA, &kcur);
        //IDAGetCurrentStep(m_pIDA, &hcur);
        //printf("    t = %.15f, k = %d, h = %.15f\n", m_dCurrentTime, kcur, hcur);

    // Now we have to copy the values *from* the solver *to* the block.
    // Achtung, Achtung: This is extremely important to do, for we must be able to re-set the
    //                   variables' values or initial conditions after any IntegrateXXX function,
    //                   and to evaluate conditional expressions in state transitions.
        realtype* pdValues			= NV_DATA_S(m_pIDASolverData->m_vectorVariables);
        realtype* pdTimeDerivatives	= NV_DATA_S(m_pIDASolverData->m_vectorTimeDerivatives);

        m_arrValues.InitArray         (m_nNumberOfEquations, pdValues);
        m_arrTimeDerivatives.InitArray(m_nNumberOfEquations, pdTimeDerivatives);

        m_pBlock->SetBlockData(m_arrValues, m_arrTimeDerivatives);

    // If a root has been found, check if some of the conditions is satisfied and do what is necessary
        if(retval == IDA_ROOT_RETURN)
        {
            if(m_pBlock->CheckForDiscontinuities())
            {
            // Data will be reported only if there is a discontinuity
                if(bReportDataAroundDiscontinuities)
                    m_pSimulation->ReportData(m_dCurrentTime);

                eDiscontinuityType = m_pBlock->ExecuteOnConditionActions();

                if(eDiscontinuityType == eModelDiscontinuity)
                {
                    Reinitialize(false, false);

                // The data will be reported again ONLY if there was a discontinuity
                    if(bReportDataAroundDiscontinuities)
                        m_pSimulation->ReportData(m_dCurrentTime);

                    if(eCriterion == eStopAtModelDiscontinuity)
                        return m_dCurrentTime;
                }
                else if(eDiscontinuityType == eModelDiscontinuityWithDataChange)
                {
                    Reinitialize(true, false);

                // The data will be reported again ONLY if there was a discontinuity
                    if(bReportDataAroundDiscontinuities)
                        m_pSimulation->ReportData(m_dCurrentTime);

                    if(eCriterion == eStopAtModelDiscontinuity)
                        return m_dCurrentTime;
                }
                else if(eDiscontinuityType == eGlobalDiscontinuity)
                {
                    daeDeclareAndThrowException(exNotImplemented);
                }
            }
            else
            {
            }
        }

        if(m_dCurrentTime >= m_dTargetTime)
            break;
    }

    return m_dCurrentTime;
}

daeBlock_t* daeIDASolver::GetBlock(void) const
{
    return m_pBlock;
}

daeLog_t* daeIDASolver::GetLog(void) const
{
    return m_pLog;
}

bool daeIDASolver::CheckFlag(int flag)
{
    if(flag < 0)
        return false;
    else
        return true;
}

void daeIDASolver::SetLastIDASError(const string& strLastError)
{
    m_strLastIDASError = strLastError;
}

std::vector<real_t> daeIDASolver::GetEstLocalErrors()
{
    std::vector<real_t> le;
    le.resize(m_nNumberOfEquations);
    N_Vector vectorLocalErrors = N_VMake_Serial(m_nNumberOfEquations, &le[0]);

    int retval = IDAGetEstLocalErrors(m_pIDA, vectorLocalErrors);
    if(!CheckFlag(retval))
        daeDeclareAndThrowException(exMiscellanous);

    return le;
}

std::vector<real_t> daeIDASolver::GetErrWeights()
{
    std::vector<real_t> ew;
    ew.resize(m_nNumberOfEquations);
    N_Vector vectorErrWeights = N_VMake_Serial(m_nNumberOfEquations, &ew[0]);

    int retval = IDAGetErrWeights(m_pIDA, vectorErrWeights);
    if(!CheckFlag(retval))
        daeDeclareAndThrowException(exMiscellanous);

    return ew;
}

std::map<std::string, real_t> daeIDASolver::GetIntegratorStats()
{
    std::map<std::string, real_t> stats;

    long int nsteps, nrevals, nlinsetups, netfails;
    int klast, kcur;
    realtype hinused, hlast, hcur, tcur;

    int retval = IDAGetIntegratorStats(m_pIDA, &nsteps, &nrevals, &nlinsetups,
                                               &netfails, &klast, &kcur, &hinused,
                                               &hlast, &hcur, &tcur);
    if(!CheckFlag(retval))
        daeDeclareAndThrowException(exMiscellanous);

    stats["NumSteps"]           = nsteps;
    stats["NumResEvals"]        = nrevals;
    stats["NumLinSolvSetups"]   = nlinsetups;
    stats["NumErrTestFails"]    = netfails;
    stats["LastOrder"]          = klast;
    stats["CurrentOrder"]       = kcur;
    stats["ActualInitStep"]     = hinused;
    stats["LastStep"]           = hlast;
    stats["CurrentStep"]        = hcur;
    stats["CurrentTime"]        = tcur;

    long int nniters, nncfails;
    retval = IDAGetNonlinSolvStats(m_pIDA, &nniters, &nncfails);
    if(!CheckFlag(retval))
        daeDeclareAndThrowException(exMiscellanous);

    stats["NumNonlinSolvIters"]     = nniters;
    stats["NumNonlinSolvConvFails"] = nncfails;

    return stats;
}

std::string daeIDASolver::CreateIDAErrorMessage(int flag)
{
    // Do no reinvent a wheel - use the function provided by IDA
    char* flagName = IDAGetReturnFlagName(flag);
    std::string strError = flagName;
    free(flagName);
    return strError;

/*
    switch(flag)
    {
        case IDA_MEM_NULL:
            strError = "IDA_MEM_NULL";
            break;
        case IDA_ILL_INPUT:
            strError = "IDA_ILL_INPUT";
            break;
        case IDA_NO_MALLOC:
            strError = "IDA_NO_MALLOC";
            break;
        case IDA_TOO_MUCH_WORK:
            strError = "IDA_TOO_MUCH_WORK";
            break;
        case IDA_TOO_MUCH_ACC:
            strError = "IDA_TOO_MUCH_ACC";
            break;
        case IDA_ERR_FAIL:
            strError = "IDA_ERR_FAIL";
            break;
        case IDA_CONV_FAIL:
            strError = "IDA_CONV_FAIL";
            break;
        case IDA_LINIT_FAIL:
            strError = "IDA_LINIT_FAIL";
            break;
        case IDA_LSETUP_FAIL:
            strError = "IDA_LSETUP_FAIL";
            break;
        case IDA_LSOLVE_FAIL:
            strError = "IDA_LSOLVE_FAIL";
            break;
        case IDA_RES_FAIL:
            strError = "IDA_RES_FAIL";
            break;
        case IDA_CONSTR_FAIL:
            strError = "IDA_CONSTR_FAIL";
            break;
        case IDA_REP_RES_ERR:
            strError = "IDA_REP_RES_ERR";
            break;
        case IDA_MEM_FAIL:
            strError = "IDA_MEM_FAIL";
            break;
        case IDA_FIRST_RES_FAIL:
            strError = "IDA_FIRST_RES_FAIL";
            break;
        case IDA_LINESEARCH_FAIL:
            strError = "IDA_LINESEARCH_FAIL";
            break;
        case IDA_NO_RECOVERY:
            strError = "IDA_NO_RECOVERY";
            break;
        case IDA_RTFUNC_FAIL:
            strError = "IDA_RTFUNC_FAIL";
            break;
        case IDA_BAD_EWT:
            strError = "IDA_BAD_EWT";
            break;
        case IDA_BAD_K:
            strError = "IDA_BAD_K";
            break;
        case IDA_BAD_T:
            strError = "IDA_BAD_T";
            break;
        case IDA_BAD_DKY:
            strError = "IDA_BAD_DKY";
            break;
        case IDA_NO_SENS:
            strError = "IDA_NO_SENS";
            break;
        case IDA_SRES_FAIL:
            strError = "IDA_SRES_FAIL";
            break;
        case IDA_REP_SRES_ERR:
            strError = "IDA_REP_SRES_ERR";
            break;
        case IDA_BAD_IS:
            strError = "IDA_BAD_IS";
            break;

        default:
            strError = "Unknown error";
    }
    return strError;
*/
}

void daeIDASolver::SaveMatrixAsXPM(const std::string& strFilename)
{
    if(!m_pLASolver)
        return;
    m_pLASolver->SaveAsXPM(strFilename);
}

daeeInitialConditionMode daeIDASolver::GetInitialConditionMode(void) const
{
    return m_eInitialConditionMode;
}

void daeIDASolver::SetInitialConditionMode(daeeInitialConditionMode eMode)
{
    m_eInitialConditionMode = eMode;
}

// This function is not used anymore!!
// IDAS will calculate sensitivities for both dynamic and steady-state models!
void daeIDASolver::CalculateGradients(void)
{
    realtype *pdValues, *pdTimeDerivatives;

    if(!m_bCalculateSensitivities || m_bIsModelDynamic)
        daeDeclareAndThrowException(exInvalidCall);
    if(!m_pIDASolverData)
        daeDeclareAndThrowException(exInvalidPointer);
    if(!m_pBlock)
        daeDeclareAndThrowException(exInvalidPointer);

    size_t N  = m_pIDASolverData->m_N;
    size_t Ns = m_pIDASolverData->m_Ns;
    if(N == 0 || Ns == 0)
        daeDeclareAndThrowException(exInvalidCall);

    pdValues			= NV_DATA_S(m_pIDASolverData->m_vectorVariables);
    pdTimeDerivatives	= NV_DATA_S(m_pIDASolverData->m_vectorTimeDerivatives);

    if(!m_pIDASolverData->ppdSValues)
        daeDeclareAndThrowException(exInvalidPointer);
    if(!m_pIDASolverData->m_pvectorSVariables)
        daeDeclareAndThrowException(exInvalidPointer);

    for(int i = 0; i < Ns; i++)
        m_pIDASolverData->ppdSValues[i] = NV_DATA_S(m_pIDASolverData->m_pvectorSVariables[i]);

    m_arrValues.InitArray(N, pdValues);
    m_arrTimeDerivatives.InitArray(N, pdTimeDerivatives);
    m_matSValues.InitMatrix(Ns, N, m_pIDASolverData->ppdSValues, eRowWise);

    m_pBlock->CalculateSensitivityParametersGradients(m_narrParametersIndexes,
                                                      m_arrValues,
                                                      m_arrTimeDerivatives,
                                                      m_matSValues);

    if(m_bPrintInfo)
    {
        cout << "CalculateGradients function:" << endl;
        cout << "Gradients matrix:" << endl;
        m_matSValues.Print();
    }
}

void daeIDASolver::OnCalculateResiduals()
{

}

void daeIDASolver::OnCalculateConditions()
{

}

void daeIDASolver::OnCalculateJacobian()
{

}

void daeIDASolver::OnCalculateSensitivityResiduals()
{

}



int init_la(IDAMem ida_mem)
{
    daeIDALASolver_t* pLASolver = (daeIDALASolver_t*)ida_mem->ida_lmem;
    if(!pLASolver)
        return IDA_MEM_NULL;

    int ret = pLASolver->Init();
    if(ret == 0)
        return IDA_SUCCESS;
    else
        return IDA_LINIT_FAIL;
}

int setup_la(IDAMem	    ida_mem,
             N_Vector	vectorVariables,
             N_Vector	vectorTimeDerivatives,
             N_Vector	vectorResiduals,
             N_Vector	vectorTemp1,
             N_Vector	vectorTemp2,
             N_Vector	vectorTemp3)
{
    daeIDALASolver_t* pLASolver = (daeIDALASolver_t*)ida_mem->ida_lmem;
    if(!pLASolver)
        return IDA_MEM_NULL;

    realtype *pdValues, *pdTimeDerivatives, *pdResiduals;

    real_t time            = ida_mem->ida_tn;
    real_t inverseTimeStep = ida_mem->ida_cj;

    pdValues			= NV_DATA_S(vectorVariables);
    pdTimeDerivatives	= NV_DATA_S(vectorTimeDerivatives);
    pdResiduals			= NV_DATA_S(vectorResiduals);

    int ret = pLASolver->Setup(time,
                               inverseTimeStep,
                               pdValues,
                               pdTimeDerivatives,
                               pdResiduals);

    if(ret == 0)
        return IDA_SUCCESS;
    else
        return IDA_LSETUP_FAIL;
}

int solve_la(IDAMem	  ida_mem,
             N_Vector vectorB,
             N_Vector vectorWeight,
             N_Vector vectorVariables,
             N_Vector vectorTimeDerivatives,
             N_Vector vectorResiduals)
{
    daeIDALASolver_t* pLASolver = (daeIDALASolver_t*)ida_mem->ida_lmem;
    if(!pLASolver)
        return IDA_MEM_NULL;

    realtype *pdValues, *pdTimeDerivatives, *pdResiduals, *pdWeight, *pdB;

    real_t time            = ida_mem->ida_tn;
    real_t inverseTimeStep = ida_mem->ida_cj;
    real_t cjratio         = ida_mem->ida_cjratio;

    pdWeight			= NV_DATA_S(vectorWeight);
    pdB      			= NV_DATA_S(vectorB);
    pdValues			= NV_DATA_S(vectorVariables);
    pdTimeDerivatives	= NV_DATA_S(vectorTimeDerivatives);
    pdResiduals			= NV_DATA_S(vectorResiduals);

    int ret = pLASolver->Solve(time,
                               inverseTimeStep,
                               cjratio,
                               pdB,
                               pdWeight,
                               pdValues,
                               pdTimeDerivatives,
                               pdResiduals);
    if(ret == 0)
        return IDA_SUCCESS;
    else
        return IDA_LSOLVE_FAIL;
}

int free_la(IDAMem ida_mem)
{
    daeIDALASolver_t* pLASolver = (daeIDALASolver_t*)ida_mem->ida_lmem;
    if(!pLASolver)
        return IDA_MEM_NULL;

    int ret = pLASolver->Free();

    // ACHTUNG, ACHTUNG!!
    // It is the responsibility of the user to delete LA solver pointer!!
    ida_mem->ida_lmem = NULL;

    if(ret == 0)
        return IDA_SUCCESS;
    else
        return IDA_MEM_FAIL;
}

int residuals(realtype	time,
              N_Vector	vectorVariables,
              N_Vector	vectorTimeDerivatives,
              N_Vector	vectorResiduals,
              void*		pUserData)
{
    realtype *pdValues, *pdTimeDerivatives, *pdResiduals;

    daeIDASolver* pSolver = (daeIDASolver*)pUserData;
    if(!pSolver || !pSolver->m_pIDASolverData)
        return -1;

    daeBlock_t* pBlock = pSolver->m_pBlock;
    if(!pBlock)
        return -1;

    size_t N = pBlock->GetNumberOfEquations();

    pdValues			= NV_DATA_S(vectorVariables);
    pdTimeDerivatives	= NV_DATA_S(vectorTimeDerivatives);
    pdResiduals			= NV_DATA_S(vectorResiduals);

    pSolver->m_arrValues.InitArray(N, pdValues);
    pSolver->m_arrTimeDerivatives.InitArray(N, pdTimeDerivatives);
    pSolver->m_arrResiduals.InitArray(N, pdResiduals);

    pBlock->CalculateResiduals(time,
                               pSolver->m_arrValues,
                               pSolver->m_arrResiduals,
                               pSolver->m_arrTimeDerivatives);

    pSolver->OnCalculateResiduals();


/*
    int currentOrder;
    realtype currentStep;
    IDAGetCurrentOrder(pSolver->m_pIDA, &currentOrder);
    IDAGetCurrentStep(pSolver->m_pIDA,  &currentStep);
    printf("  residuals at t = %.15f\n", time);
    printf("      currentOrder = %d\n", currentOrder);
    printf("      currentStep  = %.15f\n", currentStep);
    cout<< "      x    = [";
    for(int i = 0; i < 30; i++)
        cout << toStringFormatted(pSolver->m_arrValues.GetItem(i), -1, DBL_DIG, false) << ", ";
    cout << "]" << endl;
    cout<< "      dxdt = [";
    for(int i = 0; i < 30; i++)
        cout << toStringFormatted(pSolver->m_arrTimeDerivatives.GetItem(i), -1, DBL_DIG, false) << ", ";
    cout << "]" << endl;
    printf("      res  = [");
    for(size_t i = 0; i < 30; i++)
        cout << toStringFormatted(pSolver->m_arrResiduals.GetItem(i), -1, DBL_DIG, false) << ", ";
    printf("]\n");
*/


    if(pSolver->m_bPrintInfo)
    {
        cout << "---- Residuals function ----" << endl;
        cout << "Time: " << toStringFormatted(time, -1, DBL_DIG, false) << endl;
        cout << "Variables:" << endl;
        pSolver->m_arrValues.Print();
        cout << "Time derivatives:" << endl;
        pSolver->m_arrTimeDerivatives.Print();
        cout << "Residuals:" << endl;
        pSolver->m_arrResiduals.Print();
    }

    return 0;
}

int roots(realtype	time,
          N_Vector	vectorVariables,
          N_Vector	vectorTimeDerivatives,
          realtype*	gout,
          void*		pUserData)
{
    realtype *pdValues, *pdTimeDerivatives;

    daeIDASolver* pSolver = (daeIDASolver*)pUserData;
    if(!pSolver || !pSolver->m_pIDASolverData)
        return -1;

    daeBlock_t* pBlock = pSolver->m_pBlock;
    if(!pBlock)
        return -1;

    size_t Nroots = pBlock->GetNumberOfRoots();
    size_t N      = pBlock->GetNumberOfEquations();

    pdValues			= NV_DATA_S(vectorVariables);
    pdTimeDerivatives	= NV_DATA_S(vectorTimeDerivatives);

    pSolver->m_arrValues.InitArray(N, pdValues);
    pSolver->m_arrTimeDerivatives.InitArray(N, pdTimeDerivatives);
    pSolver->m_arrRoots.InitArray(Nroots, gout);

    pBlock->CalculateConditions(time,
                                pSolver->m_arrValues,
                                pSolver->m_arrTimeDerivatives,
                                pSolver->m_arrRoots);

    pSolver->OnCalculateConditions();

    if(pSolver->m_bPrintInfo)
    {
        cout << "---- Roots function ----" << endl;
        cout << "Time: " << toStringFormatted(time, -1, DBL_DIG, false) << endl;
        cout << "Variables:" << endl;
        pSolver->m_arrValues.Print();
        cout << "Time derivatives:" << endl;
        pSolver->m_arrTimeDerivatives.Print();
        cout << "Roots:" << endl;
        pSolver->m_arrRoots.Print();
    }

    return 0;
}

int jacobian(long int    Neq,
             realtype	time,
             realtype	dInverseTimeStep,
             N_Vector	vectorVariables,
             N_Vector	vectorTimeDerivatives,
             N_Vector	vectorResiduals,
             DlsMat		dense_matrixJacobian,
             void*		pUserData,
             N_Vector	vectorTemp1,
             N_Vector	vectorTemp2,
             N_Vector	vectorTemp3)
{
    realtype *pdValues, *pdTimeDerivatives, *pdResiduals, **ppdJacobian;

    daeIDASolver* pSolver = (daeIDASolver*)pUserData;
    if(!pSolver || !pSolver->m_pIDASolverData)
        return -1;

    daeBlock_t* pBlock = pSolver->m_pBlock;
    if(!pBlock)
        return -1;

    size_t N = pBlock->GetNumberOfEquations();

    pdValues			= NV_DATA_S(vectorVariables);
    pdTimeDerivatives	= NV_DATA_S(vectorTimeDerivatives);
    pdResiduals			= NV_DATA_S(vectorResiduals);
    ppdJacobian			= JACOBIAN(dense_matrixJacobian);

    pSolver->m_arrValues.InitArray(N, pdValues);
    pSolver->m_arrTimeDerivatives.InitArray(N, pdTimeDerivatives);
    pSolver->m_arrResiduals.InitArray(N, pdResiduals);
    pSolver->m_matJacobian.InitMatrix(N, N, ppdJacobian, eColumnWise);

    pBlock->CalculateJacobian(time,
                              pSolver->m_arrValues,
                              pSolver->m_arrResiduals,
                              pSolver->m_arrTimeDerivatives,
                              pSolver->m_matJacobian,
                              dInverseTimeStep);

    pSolver->OnCalculateJacobian();

    if(pSolver->m_bPrintInfo)
    {
        cout << "---- Jacobian function ----" << endl;
        cout << "Time: " << toStringFormatted(time, -1, DBL_DIG, false) << endl;
        cout << "1/dt: " << toStringFormatted(dInverseTimeStep, -1, DBL_DIG, false) << endl;
        cout << "Variables:" << endl;
        pSolver->m_arrValues.Print();
        cout << "Time derivatives:" << endl;
        pSolver->m_arrTimeDerivatives.Print();
        cout << "Jacobian matrix:" << endl;
        pSolver->m_matJacobian.Print();
    }

    return 0;
}

int sens_residuals(int		 Ns,
                   realtype	 time,
                   N_Vector	 vectorVariables,
                   N_Vector	 vectorTimeDerivatives,
                   N_Vector	 vectorResVal, /* WTF ?? */
                   N_Vector* pvectorSVariables,
                   N_Vector* pvectorSTimeDerivatives,
                   N_Vector* pvectorResidualValues,
                   void*	 pUserData,
                   N_Vector	 vectorTemp1,
                   N_Vector	 vectorTemp2,
                   N_Vector	 vectorTemp3)
{
    realtype *pdValues, *pdTimeDerivatives;

    daeIDASolver* pSolver = (daeIDASolver*)pUserData;
    if(!pSolver || !pSolver->m_pIDASolverData)
        return -1;

    daeBlock_t* pBlock = pSolver->m_pBlock;
    if(!pBlock)
        return -1;

    size_t N  = pBlock->GetNumberOfEquations();
    if(N == 0 || Ns == 0)
        return -1;

    pdValues			= NV_DATA_S(vectorVariables);
    pdTimeDerivatives	= NV_DATA_S(vectorTimeDerivatives);

    if(!pSolver->m_pIDASolverData->ppdSValues      ||
       !pSolver->m_pIDASolverData->ppdSDValues     ||
       !pSolver->m_pIDASolverData->ppdSensResiduals )
        return -1;

    for(int i = 0; i < Ns; i++)
    {
        pSolver->m_pIDASolverData->ppdSValues[i]       = NV_DATA_S(pvectorSVariables[i]);
        pSolver->m_pIDASolverData->ppdSDValues[i]      = NV_DATA_S(pvectorSTimeDerivatives[i]);
        pSolver->m_pIDASolverData->ppdSensResiduals[i] = NV_DATA_S(pvectorResidualValues[i]);
    }

    pSolver->m_arrValues.InitArray         (N, pdValues);
    pSolver->m_arrTimeDerivatives.InitArray(N, pdTimeDerivatives);

    pSolver->m_matSValues.InitMatrix         (Ns, N, pSolver->m_pIDASolverData->ppdSValues,       eRowWise);
    pSolver->m_matSTimeDerivatives.InitMatrix(Ns, N, pSolver->m_pIDASolverData->ppdSDValues,      eRowWise);
    pSolver->m_matSResiduals.InitMatrix      (Ns, N, pSolver->m_pIDASolverData->ppdSensResiduals, eRowWise);

    pBlock->CalculateSensitivityResiduals(time,
                                          pSolver->m_narrParametersIndexes,
                                          pSolver->m_arrValues,
                                          pSolver->m_arrTimeDerivatives,
                                          pSolver->m_matSValues,
                                          pSolver->m_matSTimeDerivatives,
                                          pSolver->m_matSResiduals);

    pSolver->OnCalculateSensitivityResiduals();

    if(pSolver->m_bPrintInfo)
    {
        cout << "---- Sensitivity residuals function ----" << endl;
        cout << "Time: " << toStringFormatted(time, -1, DBL_DIG, false) << endl;
        cout << "Values:" << endl;
        pSolver->m_arrValues.Print();
        cout << "TimeDerivatives:" << endl;
        pSolver->m_arrTimeDerivatives.Print();
        cout << "S values:" << endl;
        pSolver->m_matSValues.Print();
        cout << "SD values:" << endl;
        pSolver->m_matSTimeDerivatives.Print();
        cout << "S residuals:" << endl;
        pSolver->m_matSResiduals.Print();
    }

    return 0;
}

int setup_preconditioner(realtype	time,
                         N_Vector	vectorVariables,
                         N_Vector	vectorTimeDerivatives,
                         N_Vector	vectorResiduals,
                         realtype	dInverseTimeStep,
                         void*		pUserData,
                         N_Vector	vectorTemp1,
                         N_Vector	vectorTemp2,
                         N_Vector	vectorTemp3)
{
/*
    realtype *pdValues, *pdTimeDerivatives, *pdResiduals, **ppdJacobian;

    daeIDASolver* pSolver = (daeIDASolver*)pUserData;
    if(!pSolver || !pSolver->m_pIDASolverData)
        return -1;
    if(!pSolver->m_pIDASolverData->m_matKrylov || !pSolver->m_pIDASolverData->m_vectorPivot)
        return -1;

    daeBlock_t* pBlock = pSolver->m_pBlock;
    if(!pBlock)
        return -1;

    size_t Neq = pBlock->GetNumberOfEquations();

    pdValues			= NV_DATA_S(vectorVariables);
    pdTimeDerivatives	= NV_DATA_S(vectorTimeDerivatives);
    pdResiduals			= NV_DATA_S(vectorResiduals);
    ppdJacobian			= JACOBIAN(pSolver->m_pIDASolverData->m_matKrylov);

    pSolver->m_arrValues.InitArray(Neq, pdValues);
    pSolver->m_arrTimeDerivatives.InitArray(Neq, pdTimeDerivatives);
    pSolver->m_arrResiduals.InitArray(Neq, pdResiduals);
    pSolver->m_matJacobian.InitMatrix(Neq, Neq, ppdJacobian, eColumnWise);

    SetToZero(pSolver->m_pIDASolverData->m_matKrylov);

    pBlock->CalculateJacobian(time,
                              pSolver->m_arrValues,
                              pSolver->m_arrResiduals,
                              pSolver->m_arrTimeDerivatives,
                              pSolver->m_matJacobian,
                              dInverseTimeStep);
    pSolver->m_pIDASolverData->SetMaxElements();
    //pSolver->m_matJacobian.Print();

    daeArray<real_t> arr;
    arr.InitArray(Neq, pSolver->m_pIDASolverData->m_vectorInvMaxElements);
    std::cout << "setup_preconditioner" << std::endl;
    arr.Print();
*/
    return 0;
}

int solve_preconditioner(realtype	time,
                         N_Vector	vectorVariables,
                         N_Vector	vectorTimeDerivatives,
                         N_Vector	vectorResiduals,
                         N_Vector	vectorR,
                         N_Vector	vectorZ,
                         realtype	dInverseTimeStep,
                         realtype	delta,
                         void*		pUserData,
                         N_Vector	vectorTemp)
{
//	realtype *pdR, *pdZ;

//	daeIDASolver* pSolver = (daeIDASolver*)pUserData;
//	if(!pSolver || !pSolver->m_pIDASolverData)
//		return -1;

//	daeBlock_t* pBlock = pSolver->m_pBlock;
//	if(!pBlock)
//		return -1;

//	size_t Neq = pBlock->GetNumberOfEquations();

//	pdR			= NV_DATA_S(vectorR);
//	pdZ			= NV_DATA_S(vectorZ);

//	daeArray<real_t> r, z;
//	r.InitArray(Neq, pdR);

//	for(size_t i = 0; i < Neq; i++)
//	{
//		int k = pSolver->m_pIDASolverData->matJacobian.BTF[i];
//		double val = pSolver->m_pIDASolverData->matJacobian.GetItem(k, i);
//		std::cout << "val = " << val << std::endl;
//		pdZ[i] = pdR[i] / val;
//	}
//	std::cout << "z" << std::endl;
//	z.InitArray(Neq, pdZ);
//	z.Print();

/*
    realtype *pdR, *pdZ;

    daeIDASolver* pSolver = (daeIDASolver*)pUserData;
    if(!pSolver || !pSolver->m_pIDASolverData)
        return -1;
    if(!pSolver->m_pIDASolverData->m_matKrylov || !pSolver->m_pIDASolverData->m_vectorPivot)
        return -1;

    daeBlock_t* pBlock = pSolver->m_pBlock;
    if(!pBlock)
        return -1;

    size_t Neq = pBlock->GetNumberOfEquations();

    pdR			= NV_DATA_S(vectorR);
    pdZ			= NV_DATA_S(vectorZ);

    daeArray<real_t> r, z;
    r.InitArray(Neq, pdR);
    std::cout << "r" << std::endl;
    r.Print();

    for(size_t i = 0; i < Neq; i++)
        pdZ[i] = pdR[i] * pSolver->m_pIDASolverData->m_vectorInvMaxElements[i];
    std::cout << "z" << std::endl;
    z.InitArray(Neq, pdZ);
    z.Print();
*/
    return 0;
}

//extern "C" void dgemv_(char*, int*, int*, double*, double*, int*, double*, int*, double*, double*, int*);

int jac_times_vector(realtype time,
                     N_Vector vectorVariables,
                     N_Vector vectorTimeDerivatives,
                     N_Vector vectorResiduals,
                     N_Vector vectorV,
                     N_Vector vectorJV,
                     realtype dInverseTimeStep,
                     void*    pUserData,
                     N_Vector tmp1,
                     N_Vector tmp2)
{
/*
    realtype *pV, *pJV;
    realtype *pdValues, *pdTimeDerivatives, *pdResiduals, **ppdJacobian;

    daeIDASolver* pSolver = (daeIDASolver*)pUserData;
    if(!pSolver || !pSolver->m_pIDASolverData)
        return -1;
    if(!pSolver->m_pIDASolverData->m_matKrylov || !pSolver->m_pIDASolverData->m_vectorPivot)
        return -1;

    daeBlock_t* pBlock = pSolver->m_pBlock;
    if(!pBlock)
        return -1;

    size_t Neq = pBlock->GetNumberOfEquations();

    pV	= NV_DATA_S(vectorV);
    pJV	= NV_DATA_S(vectorJV);

    pdValues			= NV_DATA_S(vectorVariables);
    pdTimeDerivatives	= NV_DATA_S(vectorTimeDerivatives);
    pdResiduals			= NV_DATA_S(vectorResiduals);
    ppdJacobian			= JACOBIAN(pSolver->m_pIDASolverData->m_matKrylov);

    pSolver->m_arrValues.InitArray(Neq, pdValues);
    pSolver->m_arrTimeDerivatives.InitArray(Neq, pdTimeDerivatives);
    pSolver->m_arrResiduals.InitArray(Neq, pdResiduals);
    pSolver->m_matJacobian.InitMatrix(Neq, Neq, ppdJacobian, eColumnWise);

    SetToZero(pSolver->m_pIDASolverData->m_matKrylov);

    pBlock->CalculateJacobian(time,
                              pSolver->m_arrValues,
                              pSolver->m_arrResiduals,
                              pSolver->m_arrTimeDerivatives,
                              pSolver->m_matJacobian,
                              dInverseTimeStep);
    //pSolver->m_matJacobian.Print();

    daeArray<real_t> arr;
    arr.InitArray(Neq, pV);
    std::cout << "V vector:" << std::endl;
    arr.Print();

    char op = 'N';
    double alpha = 1;
    double beta = 0;
    int n = Neq;
    int lda = n;
    int incx = 1;
    int incy = 1;
    //dgemv_(&op, &n, &n, &alpha, pSolver->m_pIDASolverData->m_matKrylov->data, &lda, pV, &incx, &beta, pJV, &incy);

    arr.InitArray(Neq, pJV);
    std::cout << "JV vector:" << std::endl;
    arr.Print();
*/
    return 0;
}

void error_function(int error_code,
                    const char *module,
                    const char *function,
                    char *msg,
                    void *pUserData)
{
    daeIDASolver* pSolver = (daeIDASolver*)pUserData;
    if(!pSolver)
        return;

    string strErrorCode = pSolver->CreateIDAErrorMessage(error_code);
    string msgFormat    = "IDAS solver error in module '%s' in function '%s': %s [%s]";
    string strError     = (boost::format(msgFormat) % module % function % msg % strErrorCode).str();
    pSolver->SetLastIDASError(strError);
}

}
}

