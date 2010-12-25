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
#include "../DataReporters/datareporters.h"
#include "../Simulation/dyn_simulation.h"
#include "../Simulation/optimization.h"
#include "../Solver/ida_solver.h"
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
		this->daeSimulation::SetModel(pModel);
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
    
public:
	boost::python::object model;	
};



class daeIPOPTWrapper : public daeIPOPT,
                        public boost::python::wrapper<daeIPOPT>
{
public:
    daeIPOPTWrapper(void)
    {
    }
	
	void SetOptionS(const string& strOptionName, const string& strValue)
	{
		daeIPOPT::SetOption(strOptionName, strValue);
	}
	    
	void SetOptionN(const string& strOptionName, real_t dValue)
	{
		daeIPOPT::SetOption(strOptionName, dValue);
	}
    
	void SetOptionI(const string& strOptionName, int iValue)
	{
		daeIPOPT::SetOption(strOptionName, iValue);
	}   
};


}

#endif
