// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ordered_phase_pair.hpp"
#include "aspect_ratio_model.hpp"
// Constructors 
mousse::orderedPhasePair::orderedPhasePair
(
  const phaseModel& dispersed,
  const phaseModel& continuous
)
:
  phasePair
  (
    dispersed,
    continuous,
    true
  )
{}
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
  word namec(second());
  namec[0] = toupper(namec[0]);
  return first() + "In" + namec;
}
mousse::tmp<mousse::volScalarField> mousse::orderedPhasePair::E() const
{
  return
    phase1().mesh().lookupObject<aspectRatioModel>
    (
      IOobject::groupName
      (
        aspectRatioModel::typeName,
        orderedPhasePair::name()
      )
    ).E();
}
