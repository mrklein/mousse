#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_BLENDED_INTERFACIAL_MODEL_BLENDING_METHODS_HYPERBOLIC_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_BLENDED_INTERFACIAL_MODEL_BLENDING_METHODS_HYPERBOLIC_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::hyperbolic
// Description
// SourceFiles
//   hyperbolic.cpp
#include "blending_method.hpp"
namespace mousse
{
namespace blendingMethods
{
class hyperbolic
:
  public blendingMethod
{
  // Private data
    //- Minimum fraction of phases which can be considered continuous
    HashTable<dimensionedScalar, word, word::hash> minContinuousAlpha_;
    //- Width of the transition
    const dimensionedScalar transitionAlphaScale_;
public:
  //- Runtime type information
  TYPE_NAME("hyperbolic");
  // Constructors
    //- Construct from a dictionary and a list of phase names
    hyperbolic
    (
      const dictionary& dict,
      const wordList& phaseNames
    );
  //- Destructor
  ~hyperbolic();
  // Member Functions
    //- Factor for first phase
    virtual tmp<volScalarField> f1
    (
      const phaseModel& phase1,
      const phaseModel& phase2
    ) const;
    //- Factor for second phase
    virtual tmp<volScalarField> f2
    (
      const phaseModel& phase1,
      const phaseModel& phase2
    ) const;
};
}  // namespace blendingMethods
}  // namespace mousse
#endif
