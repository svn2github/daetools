#ifndef DAE_SIMULATION_LOADER_C_H
#define DAE_SIMULATION_LOADER_C_H

#include "simulation_loader_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Loading function */
DAE_API void* LoadSimulation(const char*  strPythonFile, const char* strSimulationCallable, const char* strArguments);

/* Nota bene:
 * Initialize() functions not needed anymore since the python callable object has to instantiate and initialize a simulation.
 * However, they are still available, just in case. */
DAE_API bool Initialize(void* s,
                        const char* strDAESolver,
                        const char* strLASolver,
                        const char* strDataReporter,
                        const char* strDataReporterConnectionString,
                        const char* strLog,
                        bool bCalculateSensitivities);
DAE_API bool InitializeJSON(void* s, const char* strJSONRuntimeSettings);

/* Low-level simulation functions */
DAE_API bool SetRelativeTolerance(void* s, double relTolerance);
DAE_API bool SetTimeHorizon(void* s, double timeHorizon);
DAE_API bool SetReportingInterval(void* s, double reportingInterval);
DAE_API bool SolveInitial(void* s);
DAE_API bool Run(void* s);
DAE_API bool Reinitialize(void* s);
DAE_API bool ResetToInitialSystem(void* s);
DAE_API bool Finalize(void* s);
DAE_API bool FreeSimulation(void* s);

/* Data reporting */
DAE_API bool ReportData(void* s);

/* Integration functions */
DAE_API bool Integrate(void* s);
DAE_API bool IntegrateForTimeInterval(void* s, double timeInterval);
DAE_API bool IntegrateUntilTime(void* s, double time);

/* Info functions */
DAE_API unsigned int GetNumberOfParameters(void* s);
DAE_API unsigned int GetNumberOfInputs(void* s);
DAE_API unsigned int GetNumberOfOutputs(void* s);
DAE_API unsigned int GetNumberOfStateTransitionNetworks(void* s);

DAE_API bool GetParameterInfo(void* s, unsigned int index, char strName[512], unsigned int* numberOfPoints);
DAE_API bool GetInputInfo(void* s, unsigned int index, char strName[512], unsigned int* numberOfPoints);
DAE_API bool GetOutputInfo(void* s, unsigned int index, char strName[512], unsigned int* numberOfPoints);
DAE_API bool GetStateTransitionNetworkInfo(void* s, unsigned int index, char strName[512], unsigned int* numberOfStates);

/* Get/Set value functions */
DAE_API bool GetParameterValue(void* s, unsigned int index, double* value, unsigned int numberOfPoints);
DAE_API bool GetInputValue(void* s, unsigned int index, double* value, unsigned int numberOfPoints);
DAE_API bool GetOutputValue(void* s, unsigned int index, double* value, unsigned int numberOfPoints);
DAE_API bool GetActiveState(void* s, unsigned int stnIndex, char strActiveState[64]);

DAE_API bool SetParameterValue(void* s, unsigned int index, const double* value, unsigned int numberOfPoints);
DAE_API bool SetInputValue(void* s, unsigned int index, const double* value, unsigned int numberOfPoints);
DAE_API bool SetOutputValue(void* s, unsigned int index, const double* value, unsigned int numberOfPoints);
DAE_API bool SetActiveState(void* s, unsigned int stnIndex, char strActiveState[64]);

/* FMI interface */
DAE_API bool GetFMIValue(void* s, unsigned int fmi_reference, double* value);
DAE_API bool GetFMIActiveState(void* s, unsigned int fmi_reference, char* state);

DAE_API bool SetFMIValue(void* s, unsigned int fmi_reference, double value);
DAE_API bool SetFMIActiveState(void* s, unsigned int fmi_reference, const char* state);

#ifdef __cplusplus
}
#endif


#endif
