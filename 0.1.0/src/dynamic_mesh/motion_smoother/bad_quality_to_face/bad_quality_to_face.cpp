// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bad_quality_to_face.hpp"
#include "poly_mesh.hpp"
#include "motion_smoother.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "face_set.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(badQualityToFace, 0);
addToRunTimeSelectionTable(topoSetSource, badQualityToFace, word);
addToRunTimeSelectionTable(topoSetSource, badQualityToFace, istream);
}
mousse::topoSetSource::addToUsageTable mousse::badQualityToFace::usage_
(
  badQualityToFace::typeName,
  "\n    Usage: badQualityToFace mesh-quality-dictionary\n\n"
  "    Select all faces that do not satisfy the selection criterion\n\n"
);
// Private Member Functions 
void mousse::badQualityToFace::combine(topoSet& set, const bool add) const
{
  faceSet faces(mesh_, "meshQualityFaces", mesh_.nFaces()/100+1);
  motionSmoother::checkMesh(false, mesh_, dict_, faces);
  faces.sync(mesh_);
  forAllConstIter(faceSet, faces, iter)
  {
    label faceI = iter.key();
    addOrDelete(set, faceI, add);
  }
}
// Constructors 
// Construct from dictionary
mousse::badQualityToFace::badQualityToFace
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  dict_(dict)
{}
// Construct from Istream
mousse::badQualityToFace::badQualityToFace
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  dict_(is)
{}
// Destructor 
mousse::badQualityToFace::~badQualityToFace()
{}
// Member Functions 
void mousse::badQualityToFace::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding bad-quality faces" << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing bad-quality faces" << endl;
    combine(set, false);
  }
}
