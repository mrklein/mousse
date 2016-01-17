// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "boundary_to_face.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(boundaryToFace, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, boundaryToFace, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, boundaryToFace, istream);
}

mousse::topoSetSource::addToUsageTable mousse::boundaryToFace::usage_
(
  boundaryToFace::typeName,
  "\n    Usage: boundaryToFace\n\n"
  "    Select all boundary faces\n\n"
);

// Private Member Functions 
void mousse::boundaryToFace::combine(topoSet& set, const bool add) const
{
  for
  (
    label faceI = mesh().nInternalFaces();
    faceI < mesh().nFaces();
    faceI++
  )
  {
    addOrDelete(set, faceI, add);
  }
}
// Constructors 
// Construct from components
mousse::boundaryToFace::boundaryToFace(const polyMesh& mesh)
:
  topoSetSource{mesh}
{}
// Construct from dictionary
mousse::boundaryToFace::boundaryToFace(const polyMesh& mesh, const dictionary&)
:
  topoSetSource{mesh}
{}
// Construct from Istream
mousse::boundaryToFace::boundaryToFace
(
  const polyMesh& mesh,
  Istream&
)
:
  topoSetSource{mesh}
{}
// Destructor 
mousse::boundaryToFace::~boundaryToFace()
{}
// Member Functions 
void mousse::boundaryToFace::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding all boundary faces ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing all boundary faces ..." << endl;
    combine(set, false);
  }
}
