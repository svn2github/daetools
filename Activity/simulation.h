#ifndef DYN_SIMULATION_H
#define DYN_SIMULATION_H

#include "activity_class_factory.h"
#include "../Core/coreimpl.h"
#include "../config.h"
#include "../Core/optimization.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "../IDAS_DAESolver/dae_array_matrix.h"

#if defined(DAE_MPI)
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/utility.hpp>
#endif

namespace dae
{
namespace activity
{
/*********************************************************************************************
    daeSimulation
**********************************************************************************************/
class DAE_ACTIVITY_API daeSimulation : public daeSimulation_t
{
public:
    daeSimulation(void);
    virtual ~daeSimulation(void);

public:
    virtual daeModel_t*			GetModel(void) const;
    virtual void				SetModel(daeModel_t* pModel);
    virtual daeDataReporter_t*	GetDataReporter(void) const;
    virtual daeLog_t*			GetLog(void) const;
    virtual void				Run(void);
    virtual void				Finalize(void);
    virtual void				Reset(void);
    virtual void				ReRun(void);
    virtual void				RegisterData(const std::string& strIteration);
    virtual void				ReportData(real_t dCurrentTime);
    virtual void				StoreInitializationValues(const std::string& strFileName) const;
    virtual void				LoadInitializationValues(const std::string& strFileName) const;
    virtual void                SetJSONRuntimeSettings(const std::string& strJSONRuntimeSettings);
    virtual std::string         GetJSONRuntimeSettings() const;
    virtual bool                GetCalculateSensitivities() const;
    virtual void                SetCalculateSensitivities(bool bCalculateSensitivities);

    virtual real_t				GetCurrentTime_() const;
    virtual real_t				GetNextReportingTime(void) const;
    virtual void				SetTimeHorizon(real_t dTimeHorizon);
    virtual real_t				GetTimeHorizon(void) const;
    virtual void				SetReportingInterval(real_t dReportingInterval);
    virtual real_t				GetReportingInterval(void) const;
    virtual void				GetReportingTimes(std::vector<real_t>& darrReportingTimes) const;
    virtual void				SetReportingTimes(const std::vector<real_t>& darrReportingTimes);
    virtual void				Resume(void);
    virtual void				Pause(void);

    virtual daeeActivityAction	GetActivityAction(void) const;
    virtual daeeSimulationMode	GetSimulationMode(void) const;
    virtual void				SetSimulationMode(daeeSimulationMode eMode);

    virtual void				Initialize(daeDAESolver_t* pDAESolver,
                                           daeDataReporter_t* pDataReporter,
                                           daeLog_t* pLog,
                                           bool bCalculateSensitivities = false,
                                           const std::string& strJSONRuntimeSettings = "");
    virtual void				CleanUpSetupData(void);

    virtual void				Reinitialize(void);
    virtual void				SolveInitial(void);
    virtual daeDAESolver_t*		GetDAESolver(void) const;
    virtual real_t				Integrate(daeeStopCriterion eStopCriterion, bool bReportDataAroundDiscontinuities = true);
    virtual real_t				IntegrateForTimeInterval(real_t time_interval, daeeStopCriterion eStopCriterion, bool bReportDataAroundDiscontinuities = true);
    virtual real_t				IntegrateUntilTime(real_t time, daeeStopCriterion eStopCriterion, bool bReportDataAroundDiscontinuities = true);
    virtual void				SetUpParametersAndDomains(void);
    virtual void				SetUpVariables(void);
    virtual void				SetUpOptimization(void);
    virtual void				SetUpParameterEstimation(void);
    virtual void				SetUpSensitivityAnalysis(void);
    virtual void                DoDataPartitioning(daeEquationsIndexes& equationsIndexes, std::map<size_t,size_t>& mapVariableIndexes);
    virtual void				DoPostProcessing(void);

    virtual void GetOptimizationConstraints(std::vector<daeOptimizationConstraint_t*>& ptrarrConstraints) const;
    virtual void GetOptimizationVariables(std::vector<daeOptimizationVariable_t*>& ptrarrOptVariables) const;
    virtual void GetMeasuredVariables(std::vector<daeMeasuredVariable_t*>& ptrarrMeasuredVariables) const;
    virtual void GetObjectiveFunctions(std::vector<daeObjectiveFunction_t*>& ptrarrObjectiveFunctions) const;
    virtual daeObjectiveFunction* GetObjectiveFunction(void) const;

    daeeInitialConditionMode	GetInitialConditionMode(void) const;
    void						SetInitialConditionMode(daeeInitialConditionMode eMode);

    daeOptimizationConstraint* CreateInequalityConstraint(string strDescription = "");// <= 0
    daeOptimizationConstraint* CreateEqualityConstraint(string strDescription = "");  // == 0

    daeOptimizationVariable* SetContinuousOptimizationVariable(daeVariable& variable, real_t LB, real_t UB, real_t defaultValue);
    daeOptimizationVariable* SetContinuousOptimizationVariable(daeVariable& variable, quantity qLB, quantity qUB, quantity qdefaultValue);
    daeOptimizationVariable* SetIntegerOptimizationVariable(daeVariable& variable, int LB, int UB, int defaultValue);
    daeOptimizationVariable* SetBinaryOptimizationVariable(daeVariable& variable, bool defaultValue);

    daeOptimizationVariable* SetContinuousOptimizationVariable(adouble a, real_t LB, real_t UB, real_t defaultValue);
    daeOptimizationVariable* SetContinuousOptimizationVariable(adouble a, quantity qLB, quantity qUB, quantity qdefaultValue);
    daeOptimizationVariable* SetIntegerOptimizationVariable(adouble a, int LB, int UB, int defaultValue);
    daeOptimizationVariable* SetBinaryOptimizationVariable(adouble a, bool defaultValue);

    daeMeasuredVariable*		SetMeasuredVariable(daeVariable& variable);
    daeVariableWrapper*			SetInputVariable(daeVariable& variable);
    daeOptimizationVariable*	SetModelParameter(daeVariable& variable, real_t LB, real_t UB, real_t defaultValue);

    daeMeasuredVariable*		SetMeasuredVariable(adouble a);
    daeVariableWrapper*			SetInputVariable(adouble a);
    daeOptimizationVariable*	SetModelParameter(adouble a, real_t LB, real_t UB, real_t defaultValue);

    daeCondition* GetLastSatisfiedCondition(void) const;

    size_t GetNumberOfObjectiveFunctions(void) const;
    void   SetNumberOfObjectiveFunctions(size_t n);

    std::vector<daeEquationExecutionInfo*> GetEquationExecutionInfos(void) const;
    size_t	GetNumberOfEquations(void) const;
    size_t	GetTotalNumberOfVariables(void) const; // including assigned

    void	Register(daeModel* pModel);
    void	Register(daePort* pPort);
    void	Register(daeVariable* pVariable);
    void	Register(daeParameter* pParameter);
    void	Register(daeDomain* pDomain);

    void	CollectVariables(daeModel* pModel);
    void	CollectVariables(daePort* pPort);
    void	CollectVariables(daeVariable* pVariable);
    void	CollectVariables(daeParameter* pParameter);

    //void	Report(daeModel* pModel, real_t time);
    //void	Report(daePort* pPort, real_t time);
    void	Report(daeVariable* pVariable, real_t time);
    void	Report(daeParameter* pParameter, real_t time);

    bool GetIsInitialized(void) const;
    bool GetIsSolveInitial(void) const;

    bool GetReportTimeDerivatives(void) const;
    void SetReportTimeDerivatives(bool bReportTimeDerivatives);

    bool GetReportSensitivities(void) const;
    void SetReportSensitivities(bool bReportSensitivities);

    std::string GetSensitivityDataDirectory(void) const;
    void        SetSensitivityDataDirectory(const std::string strSensitivityDataDirectory);

protected:
//	void	SetInitialConditionsToZero(void);
    void	CheckSystem(void) const;
    void	SetupSolver(void);

    boost::shared_ptr<daeObjectiveFunction> AddObjectiveFunction(void);

    void	EnterConditionalIntegrationMode(void);
    real_t	IntegrateUntilConditionSatisfied(daeCondition rCondition, daeeStopCriterion eStopCriterion);

    void    SetUpParametersAndDomains_RuntimeSettings();
    void    SetUpVariables_RuntimeSettings();

protected:
    std::string					m_strIteration;
    real_t						m_dCurrentTime;
    real_t						m_dTimeHorizon;
    real_t						m_dReportingInterval;
    std::vector<real_t>			m_darrReportingTimes;
    daeLog_t*					m_pLog;
    daeModel*					m_pModel;
    daeDataReporter_t*			m_pDataReporter;
    daeDAESolver_t*				m_pDAESolver;
    daeBlock_t*                 m_ptrBlock;
    daeeActivityAction			m_eActivityAction;
    daeeSimulationMode			m_eSimulationMode;
    double						m_ProblemCreationStart;
    double						m_ProblemCreationEnd;
    double						m_InitializationStart;
    double						m_InitializationEnd;
    double						m_IntegrationStart;
    double						m_IntegrationEnd;
    bool						m_bConditionalIntegrationMode;
    bool						m_bIsInitialized;
    bool						m_bIsSolveInitial;
    std::string                 m_strJSONRuntimeSettings;
    boost::property_tree::ptree m_ptreeRuntimeSettings;

    bool                        m_bReportTimeDerivatives;
    bool                        m_bReportSensitivities;
    daeDenseMatrix*             m_pCurrentSensitivityMatrix;
    std::vector<daeVariable*>	m_ptrarrReportVariables;
    std::vector<daeParameter*>	m_ptrarrReportParameters;

// Optimization related data
    bool														m_bCalculateSensitivities;
    std::string                                                 m_strSensitivityDataDirectory;
    int                                                         m_iNoSensitivityFiles;

    size_t														m_nNumberOfObjectiveFunctions;
    std::vector< boost::shared_ptr<daeObjectiveFunction> >		m_arrObjectiveFunctions;
    std::vector< boost::shared_ptr<daeOptimizationConstraint> >	m_arrConstraints;
    std::vector< boost::shared_ptr<daeOptimizationVariable> >	m_arrOptimizationVariables;

// Parameter estimation related data
    std::vector< boost::shared_ptr<daeVariableWrapper> >		m_arrInputVariables;
    std::vector< boost::shared_ptr<daeMeasuredVariable> >		m_arrMeasuredVariables;
};

/*********************************************************************************************
    daeOptimization
**********************************************************************************************/
class DAE_ACTIVITY_API daeOptimization :  public daeOptimization_t
{
public:
    daeOptimization(void);
    virtual ~daeOptimization(void);

public:
    virtual void Initialize(daeSimulation_t*   pSimulation,
                            daeNLPSolver_t*    pNLPSolver,
                            daeDAESolver_t*    pDAESolver,
                            daeDataReporter_t* pDataReporter,
                            daeLog_t*          pLog,
                            const std::string& initializationFile = std::string(""));
    virtual void Run(void);
    virtual void Finalize(void);

    virtual void StartIterationRun(int iteration);
    virtual void EndIterationRun(int iteration);

protected:
    daeSimulation_t*	m_pSimulation;
    daeLog_t*			m_pLog;
    daeNLPSolver_t*		m_pNLPSolver;
    daeDataReporter_t*	m_pDataReporter;
    daeDAESolver_t*		m_pDAESolver;
    std::string         m_InitializationFile;
    bool				m_bIsInitialized;
    double				m_Initialization;
    double				m_Optimization;
};


}
}

#endif
