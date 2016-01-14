// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "quadratic_diffusivity.hpp"
#include "surface_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(quadraticDiffusivity, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    motionDiffusivity,
    quadraticDiffusivity,
    Istream
  );
}
// Constructors 
mousse::quadraticDiffusivity::quadraticDiffusivity
(
  const fvMesh& mesh,
  Istream& mdData
)
:
  motionDiffusivity(mesh),
  basicDiffusivityPtr_(motionDiffusivity::New(mesh, mdData))
{}
// Destructor 
mousse::quadraticDiffusivity::~quadraticDiffusivity()
{}
// Member Functions 
mousse::tmp<mousse::surfaceScalarField>
mousse::quadraticDiffusivity::operator()() const
{
  return sqr(basicDiffusivityPtr_->operator()());
}
void mousse::quadraticDiffusivity::correct()
{
  basicDiffusivityPtr_->correct();
}
