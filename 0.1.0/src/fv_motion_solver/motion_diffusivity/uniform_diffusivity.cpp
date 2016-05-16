// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_diffusivity.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(uniformDiffusivity, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  motionDiffusivity,
  uniformDiffusivity,
  Istream
);

}


// Constructors 
mousse::uniformDiffusivity::uniformDiffusivity
(
  const fvMesh& mesh,
  Istream&
)
:
  motionDiffusivity{mesh},
  faceDiffusivity_
  {
    IOobject
    {
      "faceDiffusivity",
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh,
    {"1.0", dimless, 1.0}
  }
{}


// Destructor 
mousse::uniformDiffusivity::~uniformDiffusivity()
{}

