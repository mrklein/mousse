// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ordered_phase_pair.hpp"
#include "aspect_ratio_model.hpp"
// Constructors 
mousse::orderedPhasePair::orderedPhasePair
(
  const phaseModel& dispersed,
  const phaseModel& continuous,
  const dimensionedVector& g,
  const scalarTable& sigmaTable,
  const dictTable& aspectRatioTable
)
:
  phasePair{dispersed, continuous, g, sigmaTable, true}
{
  if (aspectRatioTable.found(*this))
  {
    aspectRatio_.set
    (
      aspectRatioModel::New
      (
        aspectRatioTable[*this],
        *this
      ).ptr()
    );
  }
}
// Destructor 
mousse::orderedPhasePair::~orderedPhasePair()
{}
// Member Functions 
const mousse::phaseModel& mousse::orderedPhasePair::dispersed() const
{
  return phase1();
}
const mousse::phaseModel& mousse::orderedPhasePair::continuous() const
{
  return phase2();
}
mousse::word mousse::orderedPhasePair::name() const
{
  word namec{continuous().name()};
  namec[0] = toupper(namec[0]);
  return dispersed().name() + "In" + namec;
}
mousse::tmp<mousse::volScalarField> mousse::orderedPhasePair::E() const
{
  if (!aspectRatio_.valid())
  {
    FATAL_ERROR_IN("mousse::orderedPhasePair::E() const")
      << "Aspect ratio model not specified for " << *this << "."
      << exit(FatalError);
  }
  return aspectRatio_->E();
}
