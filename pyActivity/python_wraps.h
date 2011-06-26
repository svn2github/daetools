#ifndef DAE_PYTHON_WRAPS_H
#define DAE_PYTHON_WRAPS_H

#if defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64)
#pragma warning(disable: 4250)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#endif

#include <string>
#include <boost/python.hpp>
#include <boost/python/numeric.hpp>
#include <boost/python/slice.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include "../dae_develop.h"
#include "../DataReporting/datareporters.h"
#include "../Activity/simulation.h"
#include "../IDAS_DAESolver/ida_solver.h"
#include "../Core/base_logging.h"
#include "../Core/tcpiplog.h"

namespace daepython
{
class daeSimulationWrapper : public daeSimulation_t,
							 public boost::python::wrapper<daeSimulation_t>
{
public:
	daeModel_t* GetModel(void) const
	{
		return this->get_override("GetModel")();
	}

	void SetModel(daeModel_t* pModel)
	{
		this->get_override("SetModel")(pModel);
	}

	daeDataReporter_t* GetDataReporter(void) const
	{
		return this->get_override("GetDataReporter")();
	}

	daeLog_t* GetLog(void) const
	{
		return this->get_override("GetLog")();
	}

	void Run(void)
	{
		this->get_override("Run")();
	}

	void ReportData(void) const
	{
		this->get_override("ReportData")();
	}

	void SetTimeHorizon(real_t dTimeHorizon)
	{
		this->get_override("SetTimeHorizon")(dTimeHorizon);
	}
	
	real_t GetTimeHorizon(void) const
	{
		return this->get_override("GetTimeHorizon")();
	}
	
	void SetReportingInterval(real_t dReportingInterval)
	{
		this->get_override("SetReportingInterval")(dReportingInterval);
	}
	
	real_t GetReportingInterval(void) const
	{
		return this->get_override("GetReportingInterval")();
	}
	
	void Pause(void)
	{
		this->get_override("Pause")();
	}

	void Resume(void)
	{
		this->get_override("Resume")();
	}

	void Stop(void)
	{
		this->get_override("Stop")();
	}

	void Initialize(daeDAESolver_t* pDAESolver, daeDataReporter_t* pDataReporter, daeLog_t* pLog)
	{
		this->get_override("Initialize")(pDAESolver, pDataReporter, pLog);
	}
	
	void Reinitialize(void)
	{
		this->get_override("Reinitialize")();
	}

	void SolveInitial(void)
	{
		this->get_override("SolveInitial")();
	}
	
	daeDAESolver_t* GetDAESolver(void) const
	{
		return this->get_override("GetDAESolver")();
	}
	
	void SetUpParametersAndDomains(void)
	{
		this->get_override("SetUpParametersAndDomains")();
	}

	void SetUpVariables(void)
	{
		this->get_override("SetUpVariables")();
	}
	
	real_t Integrate(daeeStopCriterion eStopCriterion)
	{
		return this->get_override("Integrate")(eStopCriterion);
	}
	
	real_t IntegrateForTimeInterval(real_t time_interval)
	{
		return this->get_override("IntegrateForTimeInterval")(time_interval);
	}
	
	real_t IntegrateUntilTime(real_t time, daeeStopCriterion eStopCriterion)
	{
		return this->get_override("IntegrateUntilTime")(time, eStopCriterion);
	}
	
};
	

class daeDefaultSimulationWrapper : public daeSimulation,
	                                public boost::python::wrapper<daeSimulation>
{
public:
	daeDefaultSimulationWrapper(void)
	{
	}

	boost::python::object GetModel_(void) const
	{
		return model;
	}

	void SetModel_(boost::python::object Model)
	{
		model = Model;
		daeModel* pModel = boost::python::extract<daeModel*>(Model);
		if(!pModel)
			daeDeclareAndThrowException(exInvalidPointer)
		this->m_pModel = pModel;
	}

	void SetUpParametersAndDomains(void)
	{
        if(boost::python::override f = this->get_override("SetUpParametersAndDomains"))
			f();
		else
			this->daeSimulation::SetUpParametersAndDomains();
	}
	void def_SetUpParametersAndDomains(void)
	{
		this->daeSimulation::SetUpParametersAndDomains();
	}

	void SetUpVariables(void)
	{
        if(boost::python::override f = this->get_override("SetUpVariables"))
            f();
		else
			this->daeSimulation::SetUpVariables();
	}
	void def_SetUpVariables(void)
	{
		this->daeSimulation::SetUpVariables();
	}

	void Run(void)
	{
        if(boost::python::override f = this->get_override("Run"))
			f();
 		else
	       return daeSimulation::Run();
	}
	void def_Run(void)
	{
        this->daeSimulation::Run();
	}

    void SetUpOptimization(void)
    {
        if(boost::python::override f = this->get_override("SetUpOptimization"))
            f();
        else
            this->daeSimulation::SetUpOptimization();
    }
    void def_SetUpOptimization(void)
    {
        this->daeSimulation::SetUpOptimization();
    }
	
	daeOptimizationVariable* SetContinuousOptimizationVariable1(daeVariable& variable, real_t LB, real_t UB, real_t defaultValue)
	{
        return this->daeSimulation::SetContinuousOptimizationVariable(variable, LB, UB, defaultValue);
	}
	
	daeOptimizationVariable* SetBinaryOptimizationVariable1(daeVariable& variable, bool defaultValue)
	{
        return this->daeSimulation::SetBinaryOptimizationVariable(variable, defaultValue);
	}
	
	daeOptimizationVariable* SetIntegerOptimizationVariable1(daeVariable& variable, int LB, int UB, int defaultValue)
	{
        return this->daeSimulation::SetIntegerOptimizationVariable(variable, LB, UB, defaultValue);
	}
	
	daeOptimizationVariable* SetContinuousOptimizationVariable2(adouble a, real_t LB, real_t UB, real_t defaultValue)
	{
        return this->daeSimulation::SetContinuousOptimizationVariable(a, LB, UB, defaultValue);
	}
	
	daeOptimizationVariable* SetBinaryOptimizationVariable2(adouble a, bool defaultValue)
	{
        return this->daeSimulation::SetBinaryOptimizationVariable(a, defaultValue);
	}
	
	daeOptimizationVariable* SetIntegerOptimizationVariable2(adouble a, int LB, int UB, int defaultValue)
	{
        return this->daeSimulation::SetIntegerOptimizationVariable(a, LB, UB, defaultValue);
	}
	
	daeObjectiveFunction* GetObjectiveFunction(void) const
	{
		return m_pObjectiveFunction.get();
	}

	boost::python::list GetOptimizationVariables(void)
	{
		daeOptimizationVariable* obj;
		boost::python::list l;
	
		for(size_t i = 0; i < m_arrOptimizationVariables.size(); i++)
		{
			obj = m_arrOptimizationVariables[i].get();
			l.append(obj);
		}
		return l;
	}
 
	boost::python::list GetConstraints(void)
	{
		daeOptimizationConstraint* obj;
		boost::python::list l;
	
		for(size_t i = 0; i < m_arrConstraints.size(); i++)
		{
			obj = m_arrConstraints[i].get();
			l.append(obj);
		}
		return l;
	}
	
public:
	boost::python::object model;	
};



class daeOptimizationWrapper : public daeOptimization_t,
							   public boost::python::wrapper<daeOptimization_t>
{
public:
	void Initialize(daeSimulation_t*   pSimulation,
			        daeNLPSolver_t*    pNLPSolver, 
					daeDAESolver_t*    pDAESolver, 
					daeDataReporter_t* pDataReporter, 
					daeLog_t*          pLog)
	{
		this->get_override("Initialize")(pSimulation, pNLPSolver, pDAESolver, pDataReporter, pLog);
	}
	
	void Run(void)
	{
		this->get_override("Run")();
	}
	
	void Finalize(void)
	{
		this->get_override("Run")();
	}
};


}

#endif
