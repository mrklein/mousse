// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "global_mesh_data.hpp"
#include "poly_mesh.hpp"
#include "map_distribute.hpp"

// Member Functions 
template<class Type, class CombineOp, class TransformOp>
void mousse::globalMeshData::syncData
(
  List<Type>& elems,
  const labelListList& slaves,
  const labelListList& transformedSlaves,
  const mapDistribute& slavesMap,
  const globalIndexAndTransform& transforms,
  const CombineOp& cop,
  const TransformOp& top
)
{
  // Pull slave data onto master
  slavesMap.distribute(transforms, elems, top);
  // Combine master data with slave data
  FOR_ALL(slaves, i)
  {
    Type& elem = elems[i];
    const labelList& slavePoints = slaves[i];
    label nTransformSlavePoints =
    (
      transformedSlaves.size() == 0
     ? 0
     : transformedSlaves[i].size()
    );
    if (slavePoints.size()+nTransformSlavePoints > 0)
    {
      // Combine master with untransformed slave data
      FOR_ALL(slavePoints, j)
      {
        cop(elem, elems[slavePoints[j]]);
      }
      // Combine master with transformed slave data
      if (nTransformSlavePoints)
      {
        const labelList& transformSlavePoints = transformedSlaves[i];
        FOR_ALL(transformSlavePoints, j)
        {
          cop(elem, elems[transformSlavePoints[j]]);
        }
      }
      // Copy result back to slave slots
      FOR_ALL(slavePoints, j)
      {
        elems[slavePoints[j]] = elem;
      }
      if (nTransformSlavePoints)
      {
        const labelList& transformSlavePoints = transformedSlaves[i];
        FOR_ALL(transformSlavePoints, j)
        {
          elems[transformSlavePoints[j]] = elem;
        }
      }
    }
  }
  // Push slave-slot data back to slaves
  slavesMap.reverseDistribute
  (
    transforms,
    elems.size(),
    elems,
    top
  );
}


template<class Type, class CombineOp>
void mousse::globalMeshData::syncData
(
  List<Type>& elems,
  const labelListList& slaves,
  const labelListList& transformedSlaves,
  const mapDistribute& slavesMap,
  const CombineOp& cop
)
{
  // Pull slave data onto master
  slavesMap.distribute(elems);
  // Combine master data with slave data
  FOR_ALL(slaves, i)
  {
    Type& elem = elems[i];
    const labelList& slavePoints = slaves[i];
    label nTransformSlavePoints =
    (
      transformedSlaves.size() == 0
     ? 0
     : transformedSlaves[i].size()
    );
    if (slavePoints.size()+nTransformSlavePoints > 0)
    {
      // Combine master with untransformed slave data
      FOR_ALL(slavePoints, j)
      {
        cop(elem, elems[slavePoints[j]]);
      }
      // Combine master with transformed slave data
      if (nTransformSlavePoints)
      {
        const labelList& transformSlavePoints = transformedSlaves[i];
        FOR_ALL(transformSlavePoints, j)
        {
          cop(elem, elems[transformSlavePoints[j]]);
        }
      }
      // Copy result back to slave slots
      FOR_ALL(slavePoints, j)
      {
        elems[slavePoints[j]] = elem;
      }
      if (nTransformSlavePoints)
      {
        const labelList& transformSlavePoints = transformedSlaves[i];
        FOR_ALL(transformSlavePoints, j)
        {
          elems[transformSlavePoints[j]] = elem;
        }
      }
    }
  }
  // Push slave-slot data back to slaves
  slavesMap.reverseDistribute(elems.size(), elems);
}


template<class Type, class CombineOp, class TransformOp>
void mousse::globalMeshData::syncPointData
(
  List<Type>& pointData,
  const CombineOp& cop,
  const TransformOp& top
) const
{
  if (pointData.size() != mesh_.nPoints())
  {
    FATAL_ERROR_IN("globalMeshData::syncPointData(..)")
      << "Number of elements in data:" << pointData.size()
      << " differs from number of points in mesh:" << mesh_.nPoints()
      << abort(FatalError);
  }
  // Transfer onto coupled patch
  const indirectPrimitivePatch& cpp = coupledPatch();
  List<Type> cppFld(UIndirectList<Type>(pointData, cpp.meshPoints()));
  syncData
  (
    cppFld,
    globalPointSlaves(),
    globalPointTransformedSlaves(),
    globalPointSlavesMap(),
    globalTransforms(),
    cop,
    top
  );
  // Extract back onto mesh
  FOR_ALL(cpp.meshPoints(), i)
  {
    pointData[cpp.meshPoints()[i]] = cppFld[i];
  }
}
