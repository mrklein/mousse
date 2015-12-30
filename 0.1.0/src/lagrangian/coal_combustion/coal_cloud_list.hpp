// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef coal_cloud_list_hpp_
#define coal_cloud_list_hpp_
#include "coal_cloud.hpp"
#include "vol_fields_fwd.hpp"
#include "fv_matrices_fwd.hpp"
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
#include "coal_cloud_list_i.hpp"
#endif
