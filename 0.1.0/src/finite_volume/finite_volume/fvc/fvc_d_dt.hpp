#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_D_DT_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_D_DT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"


namespace mousse {
namespace fvc {

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > DDt
(
  const surfaceScalarField&,
  const GeometricField<Type, fvPatchField, volMesh>&
);

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > DDt
(
  const tmp<surfaceScalarField>&,
  const GeometricField<Type, fvPatchField, volMesh>&
);

}
}  // namespace mousse

#include "fvc_d_dt.ipp"

#endif
