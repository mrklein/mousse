// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noBlending
// Description
// SourceFiles
//   no_blending.cpp
#ifndef NO_BLENDING_HPP_
#define NO_BLENDING_HPP_
#include "blending_method.hpp"
namespace mousse {
namespace blendingMethods {
class noBlending
:
  public blendingMethod
{
  // Private data
    //- Name of the continuous phase
    const word continuousPhase_;
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from a dictionary and two phases
    noBlending
    (
      const dictionary& dict,
      const wordList& phaseNames
    );
  //- Destructor
  ~noBlending();
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
