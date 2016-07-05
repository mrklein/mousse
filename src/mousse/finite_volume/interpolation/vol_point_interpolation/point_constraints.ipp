// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_constraints.hpp"
#include "point_fields.hpp"
#include "value_point_patch_fields.hpp"


namespace mousse {

template<class Type, class CombineOp>
void pointConstraints::syncUntransformedData
(
  const polyMesh& mesh,
  List<Type>& pointData,
  const CombineOp& cop
)
{
  // Transfer onto coupled patch
  const globalMeshData& gmd = mesh.globalData();
  const indirectPrimitivePatch& cpp = gmd.coupledPatch();
  const labelList& meshPoints = cpp.meshPoints();
  const mapDistribute& slavesMap = gmd.globalCoPointSlavesMap();
  const labelListList& slaves = gmd.globalCoPointSlaves();
  List<Type> elems{slavesMap.constructSize()};
  FOR_ALL(meshPoints, i) {
    elems[i] = pointData[meshPoints[i]];
  }
  // Pull slave data onto master. No need to update transformed slots.
  slavesMap.distribute(elems, false);
  // Combine master data with slave data
  FOR_ALL(slaves, i) {
    Type& elem = elems[i];
    const labelList& slavePoints = slaves[i];
    // Combine master with untransformed slave data
    FOR_ALL(slavePoints, j) {
      cop(elem, elems[slavePoints[j]]);
    }
    // Copy result back to slave slots
    FOR_ALL(slavePoints, j) {
      elems[slavePoints[j]] = elem;
    }
  }
  // Push slave-slot data back to slaves
  slavesMap.reverseDistribute(elems.size(), elems, false);
  // Extract back onto mesh
  FOR_ALL(meshPoints, i) {
    pointData[meshPoints[i]] = elems[i];
  }
}


template<class Type>
void pointConstraints::setPatchFields
(
  GeometricField<Type, pointPatchField, pointMesh>& pf
)
{
  FOR_ALL(pf.boundaryField(), patchI) {
    pointPatchField<Type>& ppf = pf.boundaryField()[patchI];
    if (isA<valuePointPatchField<Type> >(ppf)) {
      refCast<valuePointPatchField<Type> >(ppf) =
        ppf.patchInternalField();
    }
  }
}


template<class Type>
void pointConstraints::constrainCorners
(
  GeometricField<Type, pointPatchField, pointMesh>& pf
) const
{
  FOR_ALL(patchPatchPointConstraintPoints_, pointi) {
    pf[patchPatchPointConstraintPoints_[pointi]] = transform
    (
      patchPatchPointConstraintTensors_[pointi],
      pf[patchPatchPointConstraintPoints_[pointi]]
    );
  }
}


template<class Type>
void pointConstraints::constrain
(
  GeometricField<Type, pointPatchField, pointMesh>& pf,
  const bool overrideFixedValue
) const
{
  // Override constrained pointPatchField types with the constraint value.
  // This relies on only constrained pointPatchField implementing the evaluate
  // function
  pf.correctBoundaryConditions();
  // Sync any dangling points
  syncUntransformedData(mesh()(), pf.internalField(), maxMagSqrEqOp<Type>());
  // Apply multiple constraints on edge/corner points
  constrainCorners(pf);
  if (overrideFixedValue) {
    setPatchFields(pf);
  }
}

}  // namespace mousse
