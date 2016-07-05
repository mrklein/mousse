#ifndef TRANSPORT_MODELS_INCOMPRESSIBLE_VISCOSITY_MODELS_NEWTONIAN_HPP_
#define TRANSPORT_MODELS_INCOMPRESSIBLE_VISCOSITY_MODELS_NEWTONIAN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::viscosityModels::Newtonian
// Description
//   An incompressible Newtonian viscosity model.

#include "viscosity_model.hpp"
#include "dimensioned_scalar.hpp"
#include "vol_fields.hpp"


namespace mousse {
namespace viscosityModels {

class Newtonian
:
  public viscosityModel
{
  // Private data
    dimensionedScalar nu0_;
    volScalarField nu_;
public:
  //- Runtime type information
  TYPE_NAME("Newtonian");
  // Constructors
    //- Construct from components
    Newtonian
    (
      const word& name,
      const dictionary& viscosityProperties,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  ~Newtonian()
  {}
  // Member Functions
    //- Return the laminar viscosity
    tmp<volScalarField> nu() const { return nu_; }
    //- Return the laminar viscosity for patch
    tmp<scalarField> nu(const label patchi) const
    {
      return nu_.boundaryField()[patchi];
    }
    //- Correct the laminar viscosity (not appropriate, viscosity constant)
    void correct()
    {}
    //- Read transportProperties dictionary
    bool read(const dictionary& viscosityProperties);
};

}  // namespace viscosityModels
}  // namespace mousse

#endif

