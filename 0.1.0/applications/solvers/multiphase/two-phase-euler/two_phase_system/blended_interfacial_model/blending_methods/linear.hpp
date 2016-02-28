// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linear
// Description
// SourceFiles
//   linear.cpp
#ifndef LINEAR_HPP_
#define LINEAR_HPP_
#include "blending_method.hpp"
namespace mousse {
namespace blendingMethods {
class linear
:
  public blendingMethod
{
  // Private data
    //- Maximum fraction of phases which can be considered fully dispersed
    HashTable<dimensionedScalar, word, word::hash> maxFullyDispersedAlpha_;
    //- Maximum fraction of phases which can be considered partly dispersed
    HashTable<dimensionedScalar, word, word::hash> maxPartlyDispersedAlpha_;
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
