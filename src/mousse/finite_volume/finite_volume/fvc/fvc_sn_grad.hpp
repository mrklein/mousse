#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_SN_GRAD_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_SN_GRAD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"


namespace mousse {
namespace fvc {

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> snGrad
(
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word& name
);

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> snGrad
(
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
  const word& name
);

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> snGrad
(
  const GeometricField<Type, fvPatchField, volMesh>&
);

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> snGrad
(
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&
);

}
}  // namespace mousse

#include "fvc_sn_grad.ipp"

#endif
