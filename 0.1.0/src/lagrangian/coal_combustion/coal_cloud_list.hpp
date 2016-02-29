#ifndef LAGRANGIAN_COAL_COMBUSTION_COAL_CLOUD_LIST_HPP_
#define LAGRANGIAN_COAL_COMBUSTION_COAL_CLOUD_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coal_cloud.hpp"
#include "fv_matrices.hpp"
#include "vol_fields.hpp"
#include "dimensioned_field.hpp"
namespace mousse
{
class coalCloudList
:
  public PtrList<coalCloud>
{
private:
  //- Reference to the mesh
  const fvMesh& mesh_;
public:
  // Constructor
    coalCloudList
    (
      const volScalarField& rho,
      const volVectorField& U,
      const dimensionedVector& g,
      const SLGThermo& slgThermo
    );
  // Member Functions
    // Evolution
      //- Evolve the cloud collection
      void evolve();
    // Source terms
      //- Return const reference to momentum source
      inline tmp<DimensionedField<vector, volMesh> > UTrans() const;
      //- Return tmp momentum source term
      inline tmp<fvVectorMatrix> SU(volVectorField& U) const;
      //- Sensible enthalpy transfer [J/kg]
      inline tmp<DimensionedField<scalar, volMesh> > hsTrans() const;
      //- Return sensible enthalpy source term [J/kg/m3/s]
      inline tmp<fvScalarMatrix> Sh(volScalarField& hs) const;
      //- Return mass source term for specie i - specie eqn
      inline tmp<fvScalarMatrix> SYi
      (
        const label i,
        volScalarField& Yi
      ) const;
      //- Return total mass transfer [kg/m3]
      inline tmp<DimensionedField<scalar, volMesh> > rhoTrans() const;
      //- Return tmp total mass source for carrier phase
      //  - fully explicit
      inline tmp<DimensionedField<scalar, volMesh> > Srho() const;
      //- Return tmp total mass source for carrier phase specie i
      //  - fully explicit
      inline tmp<DimensionedField<scalar, volMesh> > Srho
      (
        const label i
      ) const;
      //- Return total mass source term [kg/m3/s]
      inline tmp<fvScalarMatrix> Srho(volScalarField& rho) const;
};
}  // namespace mousse
mousse::tmp<mousse::DimensionedField<mousse::vector, mousse::volMesh> >
mousse::coalCloudList::UTrans() const
{
  tmp<DimensionedField<vector, volMesh> > tfld
  {
    new DimensionedField<vector, volMesh>
    {
      // IOobject
      {
        "UTransEff",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh_,
      // dimensionedVector("zero", dimMass*dimVelocity, vector::zero)
      {"zero", dimMass*dimVelocity, vector::zero}
    }
  };
  DimensionedField<vector, volMesh>& fld = tfld();
  FOR_ALL(*this, i)
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
  tmp<fvVectorMatrix> tfvm{new fvVectorMatrix{U, dimForce}};
  fvVectorMatrix& fvm = tfvm();
  FOR_ALL(*this, i)
  {
    fvm += operator[](i).SU(U);
  }
  return tfvm;
}
mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::coalCloudList::hsTrans() const
{
  tmp<DimensionedField<scalar, volMesh> > tfld
  {
    new DimensionedField<scalar, volMesh>
    {
      // IOobject
      {
        "hsTransEff",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh_,
      // dimensionedScalar("zero", dimEnergy, 0.0)
      {"zero", dimEnergy, 0.0}
    }
  };
  DimensionedField<scalar, volMesh>& fld = tfld();
  FOR_ALL(*this, i)
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
  tmp<fvScalarMatrix> tfvm{new fvScalarMatrix{hs, dimEnergy/dimTime}};
  fvScalarMatrix& fvm = tfvm();
  FOR_ALL(*this, i)
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
  tmp<fvScalarMatrix> tfvm{new fvScalarMatrix{Yi, dimMass/dimTime}};
  fvScalarMatrix& fvm = tfvm();
  FOR_ALL(*this, i)
  {
    fvm += operator[](i).SYi(ii, Yi);
  }
  return tfvm;
}
mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::coalCloudList::rhoTrans() const
{
  tmp<DimensionedField<scalar, volMesh> > tfld
  {
    new DimensionedField<scalar, volMesh>
    {
      // IOobject
      {
        "rhoTransEff",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh_,
      // dimensionedScalar("zero", dimMass, 0.0)
      {"zero", dimMass, 0.0}
    }
  };
  DimensionedField<scalar, volMesh>& fld = tfld();
  FOR_ALL(*this, i)
  {
    FOR_ALL(operator[](i).rhoTrans(), j)
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
  {
    new DimensionedField<scalar, volMesh>
    {
      // IOobject
      {
        "rhoTransEff",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh_,
      // dimensionedScalar("zero", dimDensity/dimTime, 0.0)
      {"zero", dimDensity/dimTime, 0.0}
    }
  };
  DimensionedField<scalar, volMesh>& fld = tfld();
  FOR_ALL(*this, i)
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
  {
    new DimensionedField<scalar, volMesh>
    {
      // IOobject
      {
        "rhoTransEff",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh_,
      // dimensionedScalar("zero", dimDensity/dimTime, 0.0)
      {"zero", dimDensity/dimTime, 0.0}
    }
  };
  DimensionedField<scalar, volMesh>& fld = tfld();
  FOR_ALL(*this, j)
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
  tmp<fvScalarMatrix> tfvm{new fvScalarMatrix{rho, dimMass/dimTime}};
  fvScalarMatrix& fvm = tfvm();
  FOR_ALL(*this, i)
  {
    fvm += operator[](i).Srho(rho);
  }
  return tfvm;
}
#endif
