#ifndef BINGHAM_PLASTIC_HPP_
#define BINGHAM_PLASTIC_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mixtureViscosityModels::BinghamPlastic
// Description
//   Viscosity correction model for Bingham plastics.
//   The strain-rate used is defined as sqrt(2.0)*mag(symm(grad(U)))

#include "plastic.hpp"


namespace mousse {
namespace mixtureViscosityModels {

class BinghamPlastic
:
  public plastic
{
protected:
  // Protected data
    //- Yield stress coefficient
    dimensionedScalar yieldStressCoeff_;
    //- Yield stress exponent
    dimensionedScalar yieldStressExponent_;
    //- Yield stress offset
    dimensionedScalar yieldStressOffset_;
    //- Velocity
    const volVectorField& U_;
public:
  //- Runtime type information
  TYPE_NAME("BinghamPlastic");
  // Constructors
    //- Construct from components
    BinghamPlastic
    (
      const word& name,
      const dictionary& viscosityProperties,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  ~BinghamPlastic()
  {}
  // Member Functions
    //- Return the mixture viscosity
    //  given the viscosity of the continuous phase
    tmp<volScalarField> mu(const volScalarField& muc) const;
    //- Read transportProperties dictionary
    bool read(const dictionary& viscosityProperties);
};

}  // namespace mixtureViscosityModels
}  // namespace mousse

#endif

