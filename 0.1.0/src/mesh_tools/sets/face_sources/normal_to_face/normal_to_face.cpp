// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "normal_to_face.hpp"
#include "poly_mesh.hpp"
#include "face_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(normalToFace, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, normalToFace, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, normalToFace, istream);
}

mousse::topoSetSource::addToUsageTable mousse::normalToFace::usage_
(
  normalToFace::typeName,
  "\n    Usage: normalToFace (nx ny nz) <tol>\n\n"
  "    Select faces with normal aligned to unit vector (nx ny nz)\n"
  "    to within tol\n"
);

// Private Member Functions 
void mousse::normalToFace::setNormal()
{
  normal_ /= mag(normal_) + VSMALL;
  Info<< "    normalToFace : Normalized vector to " << normal_ << endl;
  if (tol_ < -1 || tol_ > 1)
  {
    FATAL_ERROR_IN
    (
      "normalToFace::normalToFace(const polyMesh&, const vector&"
      ", const scalar)"
    )   << "tolerance not within range -1..1 : " << tol_
      << exit(FatalError);
  }
}
// Constructors 
// Construct from components
mousse::normalToFace::normalToFace
(
  const polyMesh& mesh,
  const vector& normal,
  const scalar tol
)
:
  topoSetSource(mesh),
  normal_(normal),
  tol_(tol)
{
  setNormal();
}
// Construct from dictionary
mousse::normalToFace::normalToFace(const polyMesh& mesh, const dictionary& dict)
:
  topoSetSource(mesh),
  normal_(dict.lookup("normal")),
  tol_(readScalar(dict.lookup("cos")))
{
  setNormal();
}
// Construct from Istream
mousse::normalToFace::normalToFace(const polyMesh& mesh, Istream& is)
:
  topoSetSource(mesh),
  normal_(checkIs(is)),
  tol_(readScalar(checkIs(is)))
{
  setNormal();
}
// Destructor 
mousse::normalToFace::~normalToFace()
{}
// Member Functions 
void mousse::normalToFace::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding faces according to normal being aligned with "
      << normal_ << " (to within " << tol_ << ") ..." << endl;
    FOR_ALL(mesh_.faceAreas(), faceI)
    {
      vector n = mesh_.faceAreas()[faceI];
      n /= mag(n) + VSMALL;
      if (mag(1 - (n & normal_)) < tol_)
      {
        set.insert(faceI);
      }
    }
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing faces according to normal being aligned with "
      << normal_ << " (to within " << tol_ << ") ..." << endl;
    DynamicList<label> toBeRemoved(set.size()/10);
    FOR_ALL_CONST_ITER(topoSet, set, iter)
    {
      const label faceI = iter.key();
      vector n = mesh_.faceAreas()[faceI];
      n /= mag(n) + VSMALL;
      if (mag(1 - (n & normal_)) < tol_)
      {
        toBeRemoved.append(faceI);
      }
    }
    FOR_ALL(toBeRemoved, i)
    {
      set.erase(toBeRemoved[i]);
    }
  }
}
