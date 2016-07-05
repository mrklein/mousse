// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kineticTheoryModels::frictionalStressModels::Schaeffer
// Description
// SourceFiles
//   schaeffer_frictional_stress.cpp
#ifndef SCHAEFFER_HPP_
#define SCHAEFFER_HPP_
#include "frictional_stress_model.hpp"
namespace mousse {
namespace kineticTheoryModels {
namespace frictionalStressModels {
class Schaeffer
:
  public frictionalStressModel
{
  // Private data
    dictionary coeffDict_;
    //- Angle of internal friction
    dimensionedScalar phi_;
public:
    //- Runtime type information
    TYPE_NAME("Schaeffer");
  // Constructors
    //- Construct from components
    Schaeffer(const dictionary& dict);
  //- Destructor
  virtual ~Schaeffer();
  // Member functions
    virtual tmp<volScalarField> frictionalPressure
    (
      const volScalarField& alpha1,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax
    ) const;
    virtual tmp<volScalarField> frictionalPressurePrime
    (
      const volScalarField& alpha1,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax
    ) const;
    virtual tmp<volScalarField> nu
    (
      const volScalarField& alpha1,
      const dimensionedScalar& alphaMinFriction,
      const dimensionedScalar& alphaMax,
      const volScalarField& pf,
      const volSymmTensorField& D
    ) const;
    virtual bool read();
};
}  // namespace frictionalStressModels
}  // namespace kineticTheoryModels
}  // namespace mousse
#endif
