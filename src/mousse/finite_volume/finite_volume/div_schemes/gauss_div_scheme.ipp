// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gauss_div_scheme.hpp"
#include "fvc_surface_integrate.hpp"
#include "fv_matrices.hpp"


namespace mousse {
namespace fv {

template<class Type>
tmp
<
  GeometricField
  <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
>
gaussDivScheme<Type>::fvcDiv
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp
  <
    GeometricField
    <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
  > tDiv
  (
    fvc::surfaceIntegrate
    (
      this->mesh_.Sf() & this->tinterpScheme_().interpolate(vf)
    )
  );
  tDiv().rename("div(" + vf.name() + ')');
  return tDiv;
}

}  // namespace fv
}  // namespace mousse
