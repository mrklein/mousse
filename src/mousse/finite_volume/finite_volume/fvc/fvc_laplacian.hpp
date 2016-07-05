#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_LAPLACIAN_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_LAPLACIAN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "dimensioned_types.hpp"


namespace mousse {
namespace fvc {

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
  const word&
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const GeometricField<Type, fvPatchField, volMesh>&
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const dimensioned<GType>&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const dimensioned<GType>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
  const word&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const dimensioned<GType>&,
  const GeometricField<Type, fvPatchField, volMesh>&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const dimensioned<GType>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const GeometricField<GType, fvPatchField, volMesh>&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const tmp<GeometricField<GType, fvPatchField, volMesh>>&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const GeometricField<GType, fvPatchField, volMesh>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
  const word&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const tmp<GeometricField<GType, fvPatchField, volMesh>>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
  const word&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const GeometricField<GType, fvPatchField, volMesh>&,
  const GeometricField<Type, fvPatchField, volMesh>&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const tmp<GeometricField<GType, fvPatchField, volMesh>>&,
  const GeometricField<Type, fvPatchField, volMesh>&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const GeometricField<GType, fvPatchField, volMesh>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const tmp<GeometricField<GType, fvPatchField, volMesh>>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const GeometricField<GType, fvsPatchField, surfaceMesh>&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const tmp<GeometricField<GType, fvsPatchField, surfaceMesh>>&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const GeometricField<GType, fvsPatchField, surfaceMesh>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
  const word&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const tmp<GeometricField<GType, fvsPatchField, surfaceMesh>>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
  const word&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const GeometricField<GType, fvsPatchField, surfaceMesh>&,
  const GeometricField<Type, fvPatchField, volMesh>&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const tmp<GeometricField<GType, fvsPatchField, surfaceMesh>>&,
  const GeometricField<Type, fvPatchField, volMesh>&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const GeometricField<GType, fvsPatchField, surfaceMesh>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&
);
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh>> laplacian
(
  const tmp<GeometricField<GType, fvsPatchField, surfaceMesh>>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh>>&
);

}
}  // namespace mousse

#include "fvc_laplacian.ipp"

#endif
