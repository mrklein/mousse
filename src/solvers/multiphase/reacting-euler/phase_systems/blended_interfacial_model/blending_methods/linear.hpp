#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_BLENDED_INTERFACIAL_MODEL_BLENDING_METHODS_LINEAR_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_PHASE_SYSTEMS_BLENDED_INTERFACIAL_MODEL_BLENDING_METHODS_LINEAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linear
// Description
// SourceFiles
//   linear.cpp
#include "blending_method.hpp"
namespace mousse
{
namespace blendingMethods
{
class linear
:
  public blendingMethod
{
  // Private data
    //- Minimum fraction of phases which can be considered fully continuous
    HashTable<dimensionedScalar, word, word::hash> minFullyContinuousAlpha_;
    //- Minimum fraction of phases which can be considered partly continuous
    HashTable<dimensionedScalar, word, word::hash> minPartlyContinuousAlpha_;
public:
  //- Runtime type information
  TYPE_NAME("linear");
  // Constructors
    //- Construct from a dictionary and two phases
    linear
    (
      const dictionary& dict,
      const wordList& phaseNames
    );
  //- Destructor
  ~linear();
  // Member Functions
    //- Factor for primary phase
    virtual tmp<volScalarField> f1
    (
      const phaseModel& phase1,
      const phaseModel& phase2
    ) const;
    //- Factor for secondary phase
    virtual tmp<volScalarField> f2
    (
      const phaseModel& phase1,
      const phaseModel& phase2
    ) const;
};
}  // namespace blendingMethods
}  // namespace mousse
#endif
