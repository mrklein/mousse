// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "box_to_face.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(boxToFace, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, boxToFace, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, boxToFace, istream);
}

mousse::topoSetSource::addToUsageTable mousse::boxToFace::usage_
(
  boxToFace::typeName,
  "\n    Usage: boxToFace ((minx miny minz) (maxx maxy maxz))\n\n"
  "    Select all face with faceCentre within bounding box\n\n"
);

// Private Member Functions 
void mousse::boxToFace::combine(topoSet& set, const bool add) const
{
  const pointField& ctrs = mesh_.faceCentres();
  FOR_ALL(ctrs, faceI)
  {
    FOR_ALL(bbs_, i)
    {
      if (bbs_[i].contains(ctrs[faceI]))
      {
        addOrDelete(set, faceI, add);
        break;
      }
    }
  }
}
// Constructors 
// Construct from components
mousse::boxToFace::boxToFace
(
  const polyMesh& mesh,
  const treeBoundBoxList& bbs
)
:
  topoSetSource(mesh),
  bbs_(bbs)
{}
// Construct from dictionary
mousse::boxToFace::boxToFace
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  bbs_
  (
    dict.found("box")
   ? treeBoundBoxList(1, treeBoundBox(dict.lookup("box")))
   : dict.lookup("boxes")
  )
{}
// Construct from Istream
mousse::boxToFace::boxToFace
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  bbs_(1, treeBoundBox(checkIs(is)))
{}
// Destructor 
mousse::boxToFace::~boxToFace()
{}
// Member Functions 
void mousse::boxToFace::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding faces with centre within boxes " << bbs_ << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing faces with centre within boxes " << bbs_ << endl;
    combine(set, false);
  }
}
