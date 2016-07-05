#ifndef SOLVERS_MULTIPHASE_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_PHASE_PAIR_ORDERED_PHASE_PAIR_HPP_
#define SOLVERS_MULTIPHASE_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_PHASE_PAIR_ORDERED_PHASE_PAIR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::orderedPhasePair
// Description
// SourceFiles
//   ordered_phase_pair.cpp
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
public:
  // Constructors
    //- Construct from two phases, gravity, surface tension and aspect
    //  ratio tables
    orderedPhasePair
    (
      const phaseModel& dispersed,
      const phaseModel& continuous,
      const dimensionedVector& g,
      const scalarTable& sigmaTable,
      const dictTable& aspectRatioTable
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
