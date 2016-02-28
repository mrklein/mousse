// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::orderedPhasePair
// Description
// SourceFiles
//   ordered_phase_pair.cpp
#ifndef ordered_phase_pair_hpp_
#define ordered_phase_pair_hpp_
#include "phase_pair.hpp"
namespace mousse
{
class aspectRatioModel;
class orderedPhasePair
:
  public phasePair
{
  // Private data
    //- Aspect ratio model
    autoPtr<aspectRatioModel> aspectRatio_;
  // Private member functions
    //- Set the aspect ratio model, if there is one
    void setAspectRatioModel(const dictTable aspectRatioTable);
public:
  // Constructors
    //- Construct from two phases and gravity
    orderedPhasePair
    (
      const phaseModel& dispersed,
      const phaseModel& continuous
    );
  //- Destructor
  virtual ~orderedPhasePair();
  // Member Functions
    //- Dispersed phase
    virtual const phaseModel& dispersed() const;
    //- Continuous phase
    virtual const phaseModel& continuous() const;
    //- Pair name
    virtual word name() const;
    //- Aspect ratio
    virtual tmp<volScalarField> E() const;
};
}  // namespace mousse
#endif
