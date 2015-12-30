// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "engine_piston.hpp"
#include "engine_time.hpp"
#include "poly_mesh.hpp"
#include "interpolate_xy.hpp"
// Constructors 
// Construct from components
mousse::enginePiston::enginePiston
(
  const polyMesh& mesh,
  const word& pistonPatchName,
  const autoPtr<coordinateSystem>& pistonCS,
  const scalar minLayer,
  const scalar maxLayer
)
:
  mesh_(mesh),
  engineDB_(refCast<const engineTime>(mesh.time())),
  patchID_(pistonPatchName, mesh.boundaryMesh()),
  csPtr_(pistonCS),
  minLayer_(minLayer),
  maxLayer_(maxLayer)
{}
// Construct from dictionary
mousse::enginePiston::enginePiston
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  mesh_(mesh),
  engineDB_(refCast<const engineTime>(mesh_.time())),
  patchID_(dict.lookup("patch"), mesh.boundaryMesh()),
  csPtr_
  (
    coordinateSystem::New
    (
      mesh_,
      dict.subDict("coordinateSystem")
    )
  ),
  minLayer_(readScalar(dict.lookup("minLayer"))),
  maxLayer_(readScalar(dict.lookup("maxLayer")))
{}
// Destructor 
// Member Functions 
void mousse::enginePiston::writeDict(Ostream& os) const
{
  os  << nl << token::BEGIN_BLOCK
    << "patch " << patchID_.name() << token::END_STATEMENT << nl
    << "minLayer " << minLayer_ << token::END_STATEMENT << nl
    << "maxLayer " << maxLayer_ << token::END_STATEMENT << nl
    << token::END_BLOCK << endl;
}
