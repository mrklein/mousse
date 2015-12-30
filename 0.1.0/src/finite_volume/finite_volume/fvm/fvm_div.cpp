// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvm_div.hpp"
#include "fv_mesh.hpp"
#include "fv_matrix.hpp"
#include "convection_scheme.hpp"
namespace mousse
{
namespace fvm
{
template<class Type>
tmp<fvMatrix<Type> >
div
(
  const surfaceScalarField& flux,
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  const word& name
)
{
  return fv::convectionScheme<Type>::New
  (
    vf.mesh(),
    flux,
    vf.mesh().divScheme(name)
  )().fvmDiv(flux, vf);
}
template<class Type>
tmp<fvMatrix<Type> >
div
(
  const tmp<surfaceScalarField>& tflux,
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  const word& name
)
{
  tmp<fvMatrix<Type> > Div(fvm::div(tflux(), vf, name));
  tflux.clear();
  return Div;
}
template<class Type>
tmp<fvMatrix<Type> >
div
(
  const surfaceScalarField& flux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fvm::div(flux, vf, "div("+flux.name()+','+vf.name()+')');
}
template<class Type>
tmp<fvMatrix<Type> >
div
(
  const tmp<surfaceScalarField>& tflux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type> > Div(fvm::div(tflux(), vf));
  tflux.clear();
  return Div;
}
}  // namespace fvm
}  // namespace mousse
