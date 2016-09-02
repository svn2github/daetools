#ifndef DEAL_II_DATA_REPORTER_H
#define DEAL_II_DATA_REPORTER_H

#include "../Core/coreimpl.h"
#include "../Core/datareporting.h"
#define BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <boost/algorithm/string.hpp>

namespace dae
{
namespace fe_solver
{
using namespace dealii;
using namespace dae::datareporting;

typedef boost::function<void (unsigned int, double*, unsigned int)>  fnUpdateBlock;
typedef boost::function<void (const std::string&)>                   fnWriteSolution;

/*********************************************************************
   dealIIDataReporter
*********************************************************************/
class dealIIDataReporter : public daeDataReporter_t
{
public:
    dealIIDataReporter(const fnUpdateBlock& update_block_callback,
                       const fnWriteSolution& write_solution_callback,
                       const std::map<std::string, size_t>& mapVariables)
        : m_update_block_callback(update_block_callback),
          m_write_solution_callback(write_solution_callback),
          m_outputCounter(0),
          m_mapVariables(mapVariables)
    {
    }

    virtual ~dealIIDataReporter(void)
    {
    }

public:
    std::string GetName() const
    {
        return "DealIIDataReporter";
    }

    std::string GetConnectString() const
    {
        return m_strOutputDirectory;
    }

    std::string GetProcessName() const
    {
        return m_strProcessName;
    }

    void SetName(const std::string& strName)
    {
    }

    void SetConnectString(const std::string& strConnectString)
    {
        m_strOutputDirectory = strConnectString;
    }

    void SetProcessName(const std::string& strProcessName)
    {
        m_strProcessName = strProcessName;
    }

    bool Connect(const string& strConnectString, const string& strProcessName)
    {
        m_strOutputDirectory = strConnectString;
        m_strProcessName     = strProcessName;

        // Check if output directory exists
        // If not create the whole hierarchy, up to the top directory
        boost::filesystem::path folder(m_strOutputDirectory);
        if(!boost::filesystem::exists(folder))
        {
            if(!boost::filesystem::create_directories(folder))
            {
                daeDeclareException(exInvalidCall);
                e << "Invalid output directory name (" << m_strOutputDirectory
                  << ") specified as a 'connection string representing an output directory for daeDealIIDataReporter";
                throw e;
            }
        }

        return true;
    }

    bool Disconnect(void)
    {
        return true;
    }

    bool IsConnected(void)
    {
        return true;
    }

    bool StartRegistration(void)
    {
        return true;
    }

    bool RegisterDomain(const daeDataReporterDomain* pDomain)
    {
        return true;
    }

    bool RegisterVariable(const daeDataReporterVariable* pVariable)
    {
    /*
        std::vector<std::string> names;
        boost::algorithm::split(names, pVariable->m_strName, boost::algorithm::is_any_of("."));
        std::string strVariableName = names[names.size()-1];

        boost::filesystem::path vtkPath(m_strOutputDirectory);
        boost::filesystem::path variableFolder(strVariableName);
        vtkPath /= variableFolder;

        if(!boost::filesystem::exists(vtkPath.c_str()))
        {
            if(!boost::filesystem::create_directories(vtkPath.c_str()))
            {
                daeDeclareException(exInvalidCall);
                e << "Cannot create an output directory for the variable: " << strVariableName;
                throw e;
            }
        }
    */
        return true;
    }

    bool EndRegistration(void)
    {
        return true;
    }

    bool StartNewResultSet(real_t dTime)
    {
        // Before starting the new set output the data
        dealiiSaveData();

        m_dCurrentTime = dTime;
        m_outputCounter++;
        return true;
    }

    bool EndOfData(void)
    {
        // Before ending the process output the last data
        dealiiSaveData();

        return true;
    }

    bool SendVariable(const daeDataReporterVariableValue* pVariableValue)
    {
        std::vector<std::string> names;
        boost::algorithm::split(names, pVariableValue->m_strName, boost::algorithm::is_any_of("."));
        std::string strVariableName = names[names.size()-1];

        // Do not report variables that do not belong to this finite element object
        if(m_mapVariables.find(strVariableName) == m_mapVariables.end())
            return true;

        unsigned int block_index = m_mapVariables[strVariableName];

        // Call function in the finite element object to perform some processing and to save values in a file
        m_update_block_callback(block_index, pVariableValue->m_pValues, pVariableValue->m_nNumberOfPoints);

        return true;
    }

    void dealiiSaveData(void)
    {
        // Build the new file name
        boost::filesystem::path vtkFilename((boost::format("solution-%05d(t=%f).vtk") % m_outputCounter % m_dCurrentTime).str());
        boost::filesystem::path vtkPath(m_strOutputDirectory);
        vtkPath /= vtkFilename;

        m_write_solution_callback(vtkPath.string().c_str());
    }

public:
    real_t                          m_dCurrentTime;
    int                             m_outputCounter;
    fnWriteSolution                 m_write_solution_callback;
    fnUpdateBlock                   m_update_block_callback;
    std::string                     m_strOutputDirectory;
    std::string                     m_strProcessName;
    std::map<std::string, size_t>   m_mapVariables;
};


}
}

#endif
