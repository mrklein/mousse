#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_AVERAGE_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_AVERAGE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"


namespace mousse {
namespace fvc {

//- Area-weighted average a surfaceField creating a volField
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> average
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>&
);
//- Area-weighted average a surfaceField creating a volField
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> average
(
  const tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>&
);
//- Area-weighted average a surfaceField creating a volField
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> average
(
  const GeometricField<Type, fvPatchField, volMesh>&
);
//- Area-weighted average a surfaceField creating a volField
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> average
(
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&
);

}
}  // namespace mousse

#include "fvc_average.ipp"

#endif
