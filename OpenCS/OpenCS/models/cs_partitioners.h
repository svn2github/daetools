/***********************************************************************************
                 OpenCS Project: www.daetools.com
                 Copyright (C) Dragan Nikolic
************************************************************************************
OpenCS is free software; you can redistribute it and/or modify it under the terms of
the GNU Lesser General Public License version 3 as published by the Free Software
Foundation. OpenCS is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License along with
the OpenCS software; if not, see <http://www.gnu.org/licenses/>.
***********************************************************************************/
#ifndef CS_GRAPH_PARTITIONERS_H
#define CS_GRAPH_PARTITIONERS_H

#include <stdint.h>
#include <set>
#include <vector>
#include <string>
#include "../cs_model.h"

namespace cs
{
class csGraphPartitioner_t
{
public:
    virtual ~csGraphPartitioner_t(){}

    virtual std::string GetName() = 0;

/* Arguments:
 *   [in]  Npe:                                   Number of partitions to create
 *   [in]  Nvertices:                             Number of graph vertices
 *   [in]  Nconstraints:                          Number of balancing constraints
 *   [in]  rowIndices, colIndices:                The system's incidence matrix (in CRS format)
 *   [in]  vertexWeights[Nconstraints,Nvertices]: 2D array with weights for every vertex
 *   [out] partitions[Npe,]:                      2D array of Npe sets with equation indexes
 */
    virtual int Partition(int32_t                               Npe,           /* [in] */
                          int32_t                               Nvertices,     /* [in] */
                          int32_t                               Nconstraints,  /* [in] */
                          std::vector<uint32_t>&                rowIndices,    /* [in] */
                          std::vector<uint32_t>&                colIndices,    /* [in] */
                          std::vector< std::vector<int32_t> >&  vertexWeights, /* [in] */
                          std::vector< std::set<int32_t> >&     partitions     /* [out]*/) = 0;
};

class OPENCS_MODELS_API csGraphPartitioner_Simple: public csGraphPartitioner_t
{
public:
    csGraphPartitioner_Simple();
    virtual ~csGraphPartitioner_Simple();

    virtual std::string GetName();
    virtual int Partition(int32_t                               Npe,
                          int32_t                               Nvertices,
                          int32_t                               Nconstraints,
                          std::vector<uint32_t>&                rowIndices,
                          std::vector<uint32_t>&                colIndices,
                          std::vector< std::vector<int32_t> >&  vertexWeights,
                          std::vector< std::set<int32_t> >&     partitions);
};

enum MetisRoutine
{
    PartGraphKway,     /* Multilevel k-way partitioning */
    PartGraphRecursive /* Multilevel recursive bisectioning */
};

class OPENCS_MODELS_API csGraphPartitioner_Metis: public csGraphPartitioner_t
{
public:
    csGraphPartitioner_Metis(MetisRoutine routine);
    virtual ~csGraphPartitioner_Metis();

    virtual std::string GetName();
    virtual int Partition(int32_t                               Npe,
                          int32_t                               Nvertices,
                          int32_t                               Nconstraints,
                          std::vector<uint32_t>&                rowIndices,
                          std::vector<uint32_t>&                colIndices,
                          std::vector< std::vector<int32_t> >&  vertexWeights,
                          std::vector< std::set<int32_t> >&     partitions);

    std::vector<int32_t> GetOptions() const;
    void SetOptions(const std::vector<int32_t>& options);

protected:
    MetisRoutine         metisRoutine;
    std::vector<int32_t> metisOptions;
};

}

#endif
