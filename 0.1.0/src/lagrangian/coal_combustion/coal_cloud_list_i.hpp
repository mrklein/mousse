// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_matrices.hpp"
#include "vol_fields.hpp"
#include "dimensioned_field.hpp"
mousse::tmp<mousse::DimensionedField<mousse::vector, mousse::volMesh> >
mousse::coalCloudList::UTrans() const
{
  tmp<DimensionedField<vector, volMesh> > tfld
  (
    new DimensionedField<vector, volMesh>
    (
      IOobject
      (
        "UTransEff",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh_,
      dimensionedVector("zero", dimMass*dimVelocity, vector::zero)
    )
  );
  DimensionedField<vector, volMesh>& fld = tfld();
  forAll(*this, i)
  {
    fld += operator[](i).UTrans();
  }
  return tfld;
}
mousse::tmp<mousse::fvVectorMatrix> mousse::coalCloudList::SU
(
  volVectorField& U
) const
{
  tmp<fvVectorMatrix> tfvm(new fvVectorMatrix(U, dimForce));
  fvVectorMatrix& fvm = tfvm();
  forAll(*this, i)
  {
    fvm += operator[](i).SU(U);
  }
  return tfvm;
}
mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::coalCloudList::hsTrans() const
{
  tmp<DimensionedField<scalar, volMesh> > tfld
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "hsTransEff",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh_,
      dimensionedScalar("zero", dimEnergy, 0.0)
    )
  );
  DimensionedField<scalar, volMesh>& fld = tfld();
  forAll(*this, i)
  {
    fld += operator[](i).hsTrans();
  }
  return tfld;
}
mousse::tmp<mousse::fvScalarMatrix> mousse::coalCloudList::Sh
(
  volScalarField& hs
) const
{
  tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(hs, dimEnergy/dimTime));
  fvScalarMatrix& fvm = tfvm();
  forAll(*this, i)
  {
    fvm += operator[](i).Sh(hs);
  }
  return tfvm;
}
mousse::tmp<mousse::fvScalarMatrix> mousse::coalCloudList::SYi
(
  const label ii,
  volScalarField& Yi
) const
{
  tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(Yi, dimMass/dimTime));
  fvScalarMatrix& fvm = tfvm();
  forAll(*this, i)
  {
    fvm += operator[](i).SYi(ii, Yi);
  }
  return tfvm;
}
mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::coalCloudList::rhoTrans() const
{
  tmp<DimensionedField<scalar, volMesh> > tfld
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "rhoTransEff",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh_,
      dimensionedScalar("zero", dimMass, 0.0)
    )
  );
  DimensionedField<scalar, volMesh>& fld = tfld();
  forAll(*this, i)
  {
    forAll(operator[](i).rhoTrans(), j)
    {
      fld += operator[](i).rhoTrans()[j];
    }
  }
  return tfld;
}
mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::coalCloudList::Srho() const
{
  tmp<DimensionedField<scalar, volMesh> > tfld
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "rhoTransEff",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh_,
      dimensionedScalar("zero", dimDensity/dimTime, 0.0)
    )
  );
  DimensionedField<scalar, volMesh>& fld = tfld();
  forAll(*this, i)
  {
    fld += operator[](i).Srho();
  }
  return tfld;
}
mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::coalCloudList::Srho
(
  const label i
) const
{
  tmp<DimensionedField<scalar, volMesh> > tfld
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        "rhoTransEff",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh_,
      dimensionedScalar("zero", dimDensity/dimTime, 0.0)
    )
  );
  DimensionedField<scalar, volMesh>& fld = tfld();
  forAll(*this, j)
  {
    fld += operator[](j).Srho(i);
  }
  return tfld;
}
mousse::tmp<mousse::fvScalarMatrix> mousse::coalCloudList::Srho
(
  volScalarField& rho
) const
{
  tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(rho, dimMass/dimTime));
  fvScalarMatrix& fvm = tfvm();
  forAll(*this, i)
  {
    fvm += operator[](i).Srho(rho);
  }
  return tfvm;
}
