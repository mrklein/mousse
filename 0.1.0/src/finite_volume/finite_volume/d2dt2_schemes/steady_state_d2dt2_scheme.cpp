// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "steady_state_d2dt2_scheme.hpp"
#include "fvc_div.hpp"
#include "fv_matrices.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
steadyStateD2dt2Scheme<Type>::fvcD2dt2
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return tmp<GeometricField<Type, fvPatchField, volMesh> >
  (
    new GeometricField<Type, fvPatchField, volMesh>
    (
      IOobject
      (
        "d2dt2("+vf.name()+')',
        mesh().time().timeName(),
        mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh(),
      dimensioned<Type>
      (
        "0",
        vf.dimensions()/dimTime/dimTime,
        pTraits<Type>::zero
      )
    )
  );
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
steadyStateD2dt2Scheme<Type>::fvcD2dt2
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return tmp<GeometricField<Type, fvPatchField, volMesh> >
  (
    new GeometricField<Type, fvPatchField, volMesh>
    (
      IOobject
      (
        "d2dt2("+rho.name()+','+vf.name()+')',
        mesh().time().timeName(),
        mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh(),
      dimensioned<Type>
      (
        "0",
        rho.dimensions()*vf.dimensions()/dimTime/dimTime,
        pTraits<Type>::zero
      )
    )
  );
}
template<class Type>
tmp<fvMatrix<Type> >
steadyStateD2dt2Scheme<Type>::fvmD2dt2
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type> > tfvm
  (
    new fvMatrix<Type>
    (
      vf,
      vf.dimensions()*dimVol/dimTime/dimTime
    )
  );
  return tfvm;
}
template<class Type>
tmp<fvMatrix<Type> >
steadyStateD2dt2Scheme<Type>::fvmD2dt2
(
  const dimensionedScalar& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type> > tfvm
  (
    new fvMatrix<Type>
    (
      vf,
      rho.dimensions()*vf.dimensions()*dimVol/dimTime/dimTime
    )
  );
  return tfvm;
}
template<class Type>
tmp<fvMatrix<Type> >
steadyStateD2dt2Scheme<Type>::fvmD2dt2
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type> > tfvm
  (
    new fvMatrix<Type>
    (
      vf,
      rho.dimensions()*vf.dimensions()*dimVol/dimTime/dimTime
    )
  );
  return tfvm;
}
}  // namespace fv
}  // namespace mousse
