// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "file_diffusivity.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(fileDiffusivity, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  motionDiffusivity,
  fileDiffusivity,
  Istream
);

}


// Constructors 
mousse::fileDiffusivity::fileDiffusivity
(
  const fvMesh& mesh,
  Istream& mdData
)
:
  motionDiffusivity{mesh},
  faceDiffusivity_
  {
    IOobject
    {
      word(mdData),
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    },
    mesh
  }
{}


// Destructor 
mousse::fileDiffusivity::~fileDiffusivity()
{}

