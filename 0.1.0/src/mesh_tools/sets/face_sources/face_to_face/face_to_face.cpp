// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_to_face.hpp"
#include "poly_mesh.hpp"
#include "face_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(faceToFace, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, faceToFace, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, faceToFace, istream);
}
mousse::topoSetSource::addToUsageTable mousse::faceToFace::usage_
(
  faceToFace::typeName,
  "\n    Usage: faceToFace <faceSet>\n\n"
  "    Select all faces in the faceSet\n\n"
);
// Constructors 
// Construct from components
mousse::faceToFace::faceToFace
(
  const polyMesh& mesh,
  const word& setName
)
:
  topoSetSource(mesh),
  setName_(setName)
{}
// Construct from dictionary
mousse::faceToFace::faceToFace
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("set"))
{}
// Construct from Istream
mousse::faceToFace::faceToFace
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is))
{}
// Destructor 
mousse::faceToFace::~faceToFace()
{}
// Member Functions 
void mousse::faceToFace::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding all faces from faceSet " << setName_ << " ..."
      << endl;
    // Load the set
    faceSet loadedSet(mesh_, setName_);
    set.addSet(loadedSet);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing all faces from faceSet " << setName_ << " ..."
      << endl;
    // Load the set
    faceSet loadedSet(mesh_, setName_);
    set.deleteSet(loadedSet);
  }
}
