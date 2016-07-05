#ifndef TRANSPORT_MODELS_INCOMPRESSIBLE_VISCOSITY_MODELS_BIRD_CARREAU_HPP_
#define TRANSPORT_MODELS_INCOMPRESSIBLE_VISCOSITY_MODELS_BIRD_CARREAU_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::viscosityModels::BirdCarreau
// Description
//   An incompressible Bird-Carreau non-Newtonian viscosity model.
//   The Bird-Carreau-Yasuda form is also supported if the optional "a"
//   coefficient is specified.  "a" defaults to 2 for the Bird-Carreau model.

#include "viscosity_model.hpp"
#include "dimensioned_scalar.hpp"
#include "vol_fields.hpp"


namespace mousse {
namespace viscosityModels {

class BirdCarreau
:
  public viscosityModel
{
  // Private data
    dictionary coeffs_;
    dimensionedScalar nu0_;
    dimensionedScalar nuInf_;
    dimensionedScalar k_;
    dimensionedScalar n_;
    dimensionedScalar a_;
    volScalarField nu_;
  // Private Member Functions
    //- Calculate and return the laminar viscosity
    tmp<volScalarField> calcNu() const;
public:
  //- Runtime type information
  TYPE_NAME("BirdCarreau");
  // Constructors
    //- Construct from components
    BirdCarreau
    (
      const word& name,
      const dictionary& viscosityProperties,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  ~BirdCarreau()
  {}
  // Member Functions
    //- Return the laminar viscosity
    tmp<volScalarField> nu() const
    {
      return nu_;
    }
    //- Return the laminar viscosity for patch
    tmp<scalarField> nu(const label patchi) const
    {
      return nu_.boundaryField()[patchi];
    }
    //- Correct the laminar viscosity
    void correct()
    {
      nu_ = calcNu();
    }
    //- Read transportProperties dictionary
    bool read(const dictionary& viscosityProperties);
};

}  // namespace viscosityModels
}  // namespace mousse

#endif

