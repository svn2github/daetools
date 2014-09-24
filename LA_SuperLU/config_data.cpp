#include "stdafx.h"
#include "superlu_la_solver.h"
#include "../config.h"

#include "../config.h"
void get_from_config(bool& bUseUserSuppliedWorkSpace, real_t& dWorkspaceMemoryIncrement, real_t& dWorkspaceSizeMultiplier, std::string& strReuse)
{
    daeConfig& cfg = daeConfig::GetConfig();
    bUseUserSuppliedWorkSpace = cfg.Get<bool>  ("daetools.superlu.useUserSuppliedWorkSpace",    false);
    dWorkspaceMemoryIncrement = cfg.Get<double>("daetools.superlu.workspaceMemoryIncrement",    1.5);
    dWorkspaceSizeMultiplier  = cfg.Get<double>("daetools.superlu.workspaceSizeMultiplier",     2.0);
    strReuse                  = cfg.Get<std::string>("daetools.superlu.factorizationMethod",    std::string("SamePattern"));
}

