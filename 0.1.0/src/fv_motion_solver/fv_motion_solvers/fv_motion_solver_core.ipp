// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_motion_solver_core.hpp"
#include "fixed_value_point_patch_fields.hpp"
#include "cell_motion_fv_patch_fields.hpp"


// Protected Member Functions 
template<class Type>
mousse::wordList mousse::fvMotionSolverCore::cellMotionBoundaryTypes
(
  const typename GeometricField<Type, pointPatchField, pointMesh>::
  GeometricBoundaryField& pmUbf
) const
{
  wordList cmUbf = pmUbf.types();
  // Remove global patches from the end of the list
  cmUbf.setSize(fvMesh_.boundary().size());
  FOR_ALL(cmUbf, patchi) {
    if (isA<fixedValuePointPatchField<Type> >(pmUbf[patchi])) {
      cmUbf[patchi] = cellMotionFvPatchField<Type>::typeName;
    }
    if (debug) {
      Pout << "Patch:" << fvMesh_.boundary()[patchi].patch().name()
        << " pointType:" << pmUbf.types()[patchi]
        << " cellType:" << cmUbf[patchi] << endl;
    }
  }
  return cmUbf;
}

