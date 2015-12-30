// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bad_quality_to_cell.hpp"
#include "poly_mesh.hpp"
#include "motion_smoother.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "face_set.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(badQualityToCell, 0);
addToRunTimeSelectionTable(topoSetSource, badQualityToCell, word);
addToRunTimeSelectionTable(topoSetSource, badQualityToCell, istream);
}
mousse::topoSetSource::addToUsageTable mousse::badQualityToCell::usage_
(
  badQualityToCell::typeName,
  "\n    Usage: badQualityToCell mesh-quality-dictionary\n\n"
  "    Select all cells that do not satisfy the selection criterion\n\n"
);
// Private Member Functions 
void mousse::badQualityToCell::combine(topoSet& set, const bool add) const
{
  faceSet faces(mesh_, "meshQualityFaces", mesh_.nFaces()/100+1);
  motionSmoother::checkMesh(false, mesh_, dict_, faces);
  faces.sync(mesh_);
  forAllConstIter(faceSet, faces, iter)
  {
    label faceI = iter.key();
    addOrDelete(set, mesh_.faceOwner()[faceI], add);
    if (mesh_.isInternalFace(faceI))
    {
      addOrDelete(set, mesh_.faceNeighbour()[faceI], add);
    }
  }
}
// Constructors 
// Construct from dictionary
mousse::badQualityToCell::badQualityToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  dict_(dict)
{}
// Construct from Istream
mousse::badQualityToCell::badQualityToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  dict_(is)
{}
// Destructor 
mousse::badQualityToCell::~badQualityToCell()
{}
// Member Functions 
void mousse::badQualityToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding bad-quality cells" << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing bad-quality cells" << endl;
    combine(set, false);
  }
}
