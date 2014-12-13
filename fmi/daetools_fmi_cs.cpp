#include "daetools_fmi_cs.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/detail/file_parser_error.hpp>

/***************************************************
  Types for Common Functions
****************************************************/
const char* fmi2GetTypesPlatform(void)
{
    return fmi2TypesPlatform;
}

const char* fmi2GetVersion(void)
{
    return fmi2Version;
}

fmi2Status fmi2SetDebugLogging(fmi2Component comp, fmi2Boolean, size_t, const fmi2String[])
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2OK;
}

/* Creation and destruction of FMU instances and setting debug status */
fmi2Component fmi2Instantiate(fmi2String                    instanceName,
                              fmi2Type                      fmuType,
                              fmi2String                    fmuGUID,
                              fmi2String                    fmuResourceLocation,
                              const fmi2CallbackFunctions*  functions,
                              fmi2Boolean                   visible,
                              fmi2Boolean                   loggingOn)
{
    if(fmuType != fmi2CoSimulation)
        return NULL;

    daeFMIComponent_t* c = NULL;
    std::string strPythonFile, strCallableObjectName, strArguments;

    try
    {
        c = new daeFMIComponent_t();

        c->instanceName         = instanceName;
        c->fmuGUID              = fmuGUID;
        c->fmuResourceLocation  = fmuResourceLocation;
        c->functions            = functions;
        c->visible              = visible;
        c->loggingOn            = loggingOn;

        boost::property_tree::ptree pt;
        std::string resources_dir = std::string(fmuResourceLocation) + "/";
        std::string	json_settings = resources_dir + "settings.json";
        std::string	json_ini_file = resources_dir + "init.json";

        boost::property_tree::json_parser::read_json(json_settings, pt);
        strPythonFile         = resources_dir + pt.get<std::string>("simulationFile");
        strCallableObjectName = pt.get<std::string>("callableObjectName");
        strArguments          = pt.get<std::string>("arguments");

        c->simulationLoader.LoadSimulation(strPythonFile, strCallableObjectName, strArguments);

        /* Nota bene:
         * Not needed anymore: the callable object should return an initialized simulation!!

        strDAESolver    = "Sundials IDAS";
        strDataReporter = "TCPIPDataReporter"; // BlackHoleDataReporter
        strLog          = "BaseLog";

        c->simulationLoader.Initialize(strDAESolver, strLASolver, strDataReporter, strDataReporterConnectionString, strLog, false, "");
        */

        return c;
    }
    catch(std::exception& e)
    {
        std::string error = "Exception thrown in daetools FMI:\n";
        error += std::string(e.what()) + "\n\n";
        error += std::string("FMU settings:\n");
        error += std::string("  fmuResourceLocation = ") + fmuResourceLocation+  + "\n";
        error += std::string("  PythonFile = ")          + strPythonFile.c_str() + "\n";
        error += std::string("  CallableObjectName = ")  + strCallableObjectName.c_str() + "\n";
        error += std::string("  Arguments = ")           + strArguments.c_str() + "\n";

        std::cout << error<< std::endl;
        if(c)
            c->logFatal(error);
    }

    return NULL;
}

void fmi2FreeInstance(fmi2Component comp)
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return;

    try
    {
        delete c;
    }
    catch(std::exception& e)
    {
        c->logError(e.what());
    }
}

/* Enter and exit initialization mode, terminate and reset */
fmi2Status fmi2SetupExperiment(fmi2Component comp,
                               fmi2Boolean   toleranceDefined,
                               fmi2Real      tolerance,
                               fmi2Real      startTime,
                               fmi2Boolean   stopTimeDefined,
                               fmi2Real      stopTime)
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    try
    {
        if(stopTimeDefined)
        {
            c->simulationLoader.SetTimeHorizon(stopTime);
            c->simulationLoader.SetReportingInterval(stopTime/2);
        }

        /* First solve initial with the inputs provided at the beggining.
         * Then, report the data in case the user set-up the model to use the daetools data-reporter.
         * Finally, show the SimulationExplorer if the "visible" flag is set to allow inspection and further changes.  */
        c->simulationLoader.SolveInitial();
        c->simulationLoader.ReportData();

        /* Achtung, Achtung!! A bug!!
         * Whenever I show SimulationExplorer from the SimulationLoader I get SEG. FAULT during pyFinalize()
         * The best is to restrain showing it at all!
         if(c->visible)
            c->simulationLoader.ShowSimulationExplorer();
        */
    }
    catch(std::exception& e)
    {
        c->logFatal(e.what());
        return fmi2Fatal;
    }

    return fmi2OK;
}

fmi2Status fmi2EnterInitializationMode(fmi2Component comp)
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2OK;
}

fmi2Status fmi2ExitInitializationMode(fmi2Component comp)
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2OK;
}

fmi2Status fmi2Terminate(fmi2Component comp)
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    try
    {
        c->simulationLoader.Finalize();
    }
    catch(std::exception& e)
    {
        c->logFatal(e.what());
        return fmi2Fatal;
    }

    return fmi2OK;
}

fmi2Status fmi2Reset(fmi2Component comp)
{
    // Not implemented
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    c->simulationLoader.ResetToInitialSystem();

    return fmi2OK;
}

/* Getting and setting variable values */
fmi2Status fmi2GetReal(fmi2Component comp, const fmi2ValueReference vr[], size_t nvr, fmi2Real value[])
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    try
    {
        for(size_t i = 0; i < nvr; i++)
        {
            unsigned int reference = static_cast<unsigned int>(vr[i]);
            value[i] = c->simulationLoader.GetFMIValue(reference);
        }
    }
    catch(std::exception& e)
    {
        c->logFatal(e.what());
        return fmi2Fatal;
    }

    return fmi2OK;
}

fmi2Status fmi2GetInteger(fmi2Component comp, const fmi2ValueReference vr[], size_t nvr, fmi2Integer value[])
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2Error;
}

fmi2Status fmi2GetBoolean(fmi2Component comp, const fmi2ValueReference vr[], size_t nvr, fmi2Boolean value[])
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2Error;
}

fmi2Status fmi2GetString(fmi2Component comp, const fmi2ValueReference vr[], size_t nvr, fmi2String value[])
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    try
    {
        for(size_t i = 0; i < nvr; i++)
        {
            unsigned int reference = static_cast<unsigned int>(vr[i]);

            /* Here, we just set an item of the value[] array (shallow copy, no memory copy).
             * If the simulator wants to use the returned strings - it needs to deep copy them. */
            value[i] = c->simulationLoader.GetFMIActiveState(reference).c_str();
        }
    }
    catch(std::exception& e)
    {
        return fmi2Fatal;
    }

    return fmi2OK;
}

fmi2Status fmi2SetReal(fmi2Component comp, const fmi2ValueReference vr[], size_t nvr, const fmi2Real value[])
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    try
    {
        for(size_t i = 0; i < nvr; i++)
        {
            unsigned int reference = static_cast<unsigned int>(vr[i]);
            c->simulationLoader.SetFMIValue(reference, value[i]);
        }
    }
    catch(std::exception& e)
    {
        c->logFatal(e.what());
        return fmi2Fatal;
    }

    return fmi2OK;
}

fmi2Status fmi2SetInteger(fmi2Component comp, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer value[])
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2Error;
}

fmi2Status fmi2SetBoolean(fmi2Component comp, const fmi2ValueReference vr[], size_t nvr, const fmi2Boolean value[])
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2Error;
}

fmi2Status fmi2SetString(fmi2Component comp, const fmi2ValueReference vr[], size_t nvr, const fmi2String value[])
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    try
    {
        for(size_t i = 0; i < nvr; i++)
        {
            unsigned int reference = static_cast<unsigned int>(vr[i]);
            c->simulationLoader.SetFMIActiveState(reference, std::string(value[i]));
        }
    }
    catch(std::exception& e)
    {
        c->logFatal(e.what());
        return fmi2Fatal;
    }

    return fmi2OK;
}

/***************************************************
  Types for Functions for FMI2 for Co-Simulation
****************************************************/
/* Simulating the slave */
fmi2Status fmi2DoStep(fmi2Component comp,
                      fmi2Real      currentCommunicationPoint,
                      fmi2Real      communicationStepSize,
                      fmi2Boolean   noSetFMUStatePriorToCurrentPoint)
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    try
    {
        double stepTimeHorizon = currentCommunicationPoint + communicationStepSize;

        /* Integrate until the specified step final time. */
        //if(c->debugMode)
        //    c->functions->logger("Integrating from %.6f to %.6f ...\n", stepStartTime, stepTimeHorizon);

        /* Set the time horizon. */
        c->simulationLoader.SetTimeHorizon(stepTimeHorizon);

        /* Integrate until specified time and report data. */
        c->simulationLoader.Reinitialize();
        c->simulationLoader.IntegrateUntilTime(stepTimeHorizon, false, true);
        c->simulationLoader.ReportData();
    }
    catch(std::exception& e)
    {
        c->logError(e.what());
        return fmi2Error;
    }

    return fmi2OK;
}
/*
fmi2Status fmi2CancelStep(fmi2Component comp)
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2OK;
}
*/
/* Inquire slave status */
/*
fmi2Status fmi2GetStatus(fmi2Component comp, const fmi2StatusKind kind, fmi2Status* )
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2OK;
}

fmi2Status fmi2GetRealStatus(fmi2Component comp, const fmi2StatusKind kind, fmi2Real* )
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2OK;
}

fmi2Status fmi2GetIntegerStatus(fmi2Component comp, const fmi2StatusKind kind, fmi2Integer*)
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2OK;
}

fmi2Status fmi2GetBooleanStatus(fmi2Component comp, const fmi2StatusKind kind, fmi2Boolean*)
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2OK;
}

fmi2Status fmi2GetStringStatus(fmi2Component comp, const fmi2StatusKind, fmi2String* )
{
    daeFMIComponent_t* c = (daeFMIComponent_t*)comp;
    if(c == NULL)
        return fmi2Fatal;

    return fmi2OK;
}
*/

/***************************************************
 *  daeFMIComponent_t
***************************************************/
daeFMIComponent_t::daeFMIComponent_t()
{
    debugMode = false;
}

daeFMIComponent_t::~daeFMIComponent_t()
{

}

void daeFMIComponent_t::logFatal(const std::string& error)
{
    if(functions && loggingOn && functions->logger)
        functions->logger(functions->componentEnvironment, instanceName.c_str(), fmi2Fatal, "logStatusFatal", error.c_str());
}

void daeFMIComponent_t::logError(const std::string& error)
{
    if(functions && loggingOn && functions->logger)
        functions->logger(functions->componentEnvironment, instanceName.c_str(), fmi2Error, "logStatusError", error.c_str());
}
