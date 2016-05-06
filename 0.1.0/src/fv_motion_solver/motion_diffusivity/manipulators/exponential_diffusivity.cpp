// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "exponential_diffusivity.hpp"
#include "surface_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(exponentialDiffusivity, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  motionDiffusivity,
  exponentialDiffusivity,
  Istream
);

}


// Constructors 
mousse::exponentialDiffusivity::exponentialDiffusivity
(
  const fvMesh& mesh,
  Istream& mdData
)
:
  motionDiffusivity{mesh},
  alpha_{readScalar(mdData)},
  basicDiffusivityPtr_{motionDiffusivity::New(mesh, mdData)}
{}


// Destructor 
mousse::exponentialDiffusivity::~exponentialDiffusivity()
{}


// Member Functions 
mousse::tmp<mousse::surfaceScalarField>
mousse::exponentialDiffusivity::operator()() const
{
  return exp(-alpha_/basicDiffusivityPtr_->operator()());
}


void mousse::exponentialDiffusivity::correct()
{
  basicDiffusivityPtr_->correct();
}

