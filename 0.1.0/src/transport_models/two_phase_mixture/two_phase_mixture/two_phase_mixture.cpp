// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "two_phase_mixture.hpp"
// Constructors 
mousse::twoPhaseMixture::twoPhaseMixture
(
  const fvMesh& mesh,
  const dictionary& dict
)
:
  phase1Name_(wordList(dict.lookup("phases"))[0]),
  phase2Name_(wordList(dict.lookup("phases"))[1]),
  alpha1_
  (
    IOobject
    (
      IOobject::groupName("alpha", phase1Name_),
      mesh.time().timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    ),
    mesh
  ),
  alpha2_
  (
    IOobject
    (
      IOobject::groupName("alpha", phase2Name_),
      mesh.time().timeName(),
      mesh
    ),
    1.0 - alpha1_
  )
{}
