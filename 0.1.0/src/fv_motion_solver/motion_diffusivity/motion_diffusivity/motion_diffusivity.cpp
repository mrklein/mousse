// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "motion_diffusivity.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(motionDiffusivity, 0);
  defineRunTimeSelectionTable(motionDiffusivity, Istream);
}
// Constructors 
mousse::motionDiffusivity::motionDiffusivity(const fvMesh& mesh)
:
  mesh_(mesh)
{}
// Selectors
mousse::autoPtr<mousse::motionDiffusivity> mousse::motionDiffusivity::New
(
  const fvMesh& mesh,
  Istream& mdData
)
{
  const word motionType(mdData);
  Info<< "Selecting motion diffusion: " << motionType << endl;
  IstreamConstructorTable::iterator cstrIter =
    IstreamConstructorTablePtr_->find(motionType);
  if (cstrIter == IstreamConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "motionDiffusivity::New(const fvMesh&, "
      "const Istream& dict)"
    )   << "Unknown diffusion type "
      << motionType << nl << nl
      << "Valid diffusion types are :" << endl
      << IstreamConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<motionDiffusivity>(cstrIter()(mesh, mdData));
}
// Destructor 
mousse::motionDiffusivity::~motionDiffusivity()
{}
