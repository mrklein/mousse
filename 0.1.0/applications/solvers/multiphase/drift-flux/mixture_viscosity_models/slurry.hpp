// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mixtureViscosityModels::slurry
// Description
//   Thomas' viscosity correction for slurry.
//   References:
//   \verbatim
//     "Transport characteristics of suspension:
//      VIII. A note on the viscosity of Newtonian suspensions
//      of uniform spherical particles".
//      D.G. Thomas,
//      J. Colloid Sci. 20 (3), 1965, p267.
//   \endverbatim
// SourceFiles
//   slurry.cpp
#ifndef SLURRY_HPP_
#define SLURRY_HPP_
#include "mixture_viscosity_model.hpp"
#include "dimensioned_scalar.hpp"
#include "vol_fields.hpp"
namespace mousse
{
class incompressibleTwoPhaseInteractingMixture;
namespace mixtureViscosityModels
{
class slurry
:
  public mixtureViscosityModel
{
protected:
  // Protected data
    //- Slurry phase fraction
    const volScalarField& alpha_;
public:
  //- Runtime type information
  TYPE_NAME("slurry");
  // Constructors
    //- Construct from components
    slurry
    (
      const word& name,
      const dictionary& viscosityProperties,
      const volVectorField& U,
      const surfaceScalarField& phi,
      const word modelName=typeName
    );
  //- Destructor
  ~slurry()
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
