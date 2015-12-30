// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "directional_diffusivity.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(directionalDiffusivity, 0);
  addToRunTimeSelectionTable
  (
    motionDiffusivity,
    directionalDiffusivity,
    Istream
  );
}
// Constructors 
mousse::directionalDiffusivity::directionalDiffusivity
(
  const fvMesh& mesh,
  Istream& mdData
)
:
  uniformDiffusivity(mesh, mdData),
  diffusivityVector_(mdData)
{
  correct();
}
// Destructor 
mousse::directionalDiffusivity::~directionalDiffusivity()
{}
// Member Functions 
void mousse::directionalDiffusivity::correct()
{
  const surfaceVectorField n(mesh().Sf()/mesh().magSf());
  faceDiffusivity_ == (n & cmptMultiply(diffusivityVector_, n));
}
