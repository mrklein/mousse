// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mixtureViscosityModels::plastic
// Description
//   Viscosity correction model for a generic power-law plastic.
// SourceFiles
//   plastic.cpp
#ifndef PLASTIC_HPP_
#define PLASTIC_HPP_
#include "mixture_viscosity_model.hpp"
#include "dimensioned_scalar.hpp"
#include "vol_fields.hpp"
namespace mousse
{
class incompressibleTwoPhaseInteractingMixture;
namespace mixtureViscosityModels
{
class plastic
:
  public mixtureViscosityModel
{
protected:
  // Protected data
    //- Dictionary
    dictionary plasticCoeffs_;
    //- Plastic viscosity coefficient
    dimensionedScalar plasticViscosityCoeff_;
    //- Plastic viscosity exponent
    dimensionedScalar plasticViscosityExponent_;
    //- Maximum viscosity
    dimensionedScalar muMax_;
    //- Plastic phase fraction
    const volScalarField& alpha_;
public:
  //- Runtime type information
  TYPE_NAME("plastic");
  // Constructors
    //- Construct from components
    plastic
    (
      const word& name,
      const dictionary& viscosityProperties,
      const volVectorField& U,
      const surfaceScalarField& phi,
      const word modelName=typeName
    );
  //- Destructor
  ~plastic()
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
