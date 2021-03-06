#ifndef FINITE_VOLUME_FINITE_VOLUME_FVM_FVM_LAPLACIAN_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVM_FVM_LAPLACIAN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "fv_matrix.hpp"
#include "zero.hpp"
#include "one.hpp"


namespace mousse {
namespace fvm {

template<class Type>
tmp<fvMatrix<Type> > laplacian
(
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);

template<class Type>
tmp<fvMatrix<Type> > laplacian
(
  const GeometricField<Type, fvPatchField, volMesh>&
);

template<class Type>
tmp<fvMatrix<Type> > laplacian
(
  const zero&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);

template<class Type>
tmp<fvMatrix<Type> > laplacian
(
  const zero&,
  const GeometricField<Type, fvPatchField, volMesh>&
);

template<class Type>
tmp<fvMatrix<Type> > laplacian
(
  const one&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);

template<class Type>
tmp<fvMatrix<Type> > laplacian
(
  const one&,
  const GeometricField<Type, fvPatchField, volMesh>&
);

template<class Type, class GType>
tmp<fvMatrix<Type> > laplacian
(
  const dimensioned<GType>&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);

template<class Type, class GType>
tmp<fvMatrix<Type> > laplacian
(
  const dimensioned<GType>&,
  const GeometricField<Type, fvPatchField, volMesh>&
);

template<class Type, class GType>
tmp<fvMatrix<Type> > laplacian
(
  const GeometricField<GType, fvPatchField, volMesh>&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);

template<class Type, class GType>
tmp<fvMatrix<Type> > laplacian
(
  const GeometricField<GType, fvPatchField, volMesh>&,
  const GeometricField<Type, fvPatchField, volMesh>&
);

template<class Type, class GType>
tmp<fvMatrix<Type> > laplacian
(
  const tmp<GeometricField<GType, fvPatchField, volMesh> >&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);

template<class Type, class GType>
tmp<fvMatrix<Type> > laplacian
(
  const tmp<GeometricField<GType, fvPatchField, volMesh> >&,
  const GeometricField<Type, fvPatchField, volMesh>&
);

template<class Type, class GType>
tmp<fvMatrix<Type> > laplacian
(
  const GeometricField<GType, fvsPatchField, surfaceMesh>&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);

template<class Type, class GType>
tmp<fvMatrix<Type> > laplacian
(
  const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word&
);

template<class Type, class GType>
tmp<fvMatrix<Type> > laplacian
(
  const GeometricField<GType, fvsPatchField, surfaceMesh>&,
  const GeometricField<Type, fvPatchField, volMesh>&
);

template<class Type, class GType>
tmp<fvMatrix<Type> > laplacian
(
  const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >&,
  const GeometricField<Type, fvPatchField, volMesh>&
);

}
}  // namespace mousse

#include "fvm_laplacian.ipp"

#endif
