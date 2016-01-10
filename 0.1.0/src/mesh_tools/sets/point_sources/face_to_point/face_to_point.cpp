// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_to_point.hpp"
#include "poly_mesh.hpp"
#include "face_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(faceToPoint, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, faceToPoint, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, faceToPoint, istream);

template<>
const char* mousse::NamedEnum
<
  mousse::faceToPoint::faceAction,
  1
>::names[] =
{
  "all"
};
}

mousse::topoSetSource::addToUsageTable mousse::faceToPoint::usage_
(
  faceToPoint::typeName,
  "\n    Usage: faceToPoint <faceSet> all\n\n"
  "    Select all points of faces in the faceSet\n\n"
);

const mousse::NamedEnum<mousse::faceToPoint::faceAction, 1>
  mousse::faceToPoint::faceActionNames_;
// Private Member Functions 
void mousse::faceToPoint::combine(topoSet& set, const bool add) const
{
  // Load the set
  faceSet loadedSet(mesh_, setName_);
  // Add all points from faces in loadedSet
  FOR_ALL_CONST_ITER(faceSet, loadedSet, iter)
  {
    const face& f = mesh_.faces()[iter.key()];
    FOR_ALL(f, fp)
    {
      addOrDelete(set, f[fp], add);
    }
  }
}
// Constructors 
// Construct from components
mousse::faceToPoint::faceToPoint
(
  const polyMesh& mesh,
  const word& setName,
  const faceAction option
)
:
  topoSetSource(mesh),
  setName_(setName),
  option_(option)
{}
// Construct from dictionary
mousse::faceToPoint::faceToPoint
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("set")),
  option_(faceActionNames_.read(dict.lookup("option")))
{}
// Construct from Istream
mousse::faceToPoint::faceToPoint
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is)),
  option_(faceActionNames_.read(checkIs(is)))
{}
// Destructor 
mousse::faceToPoint::~faceToPoint()
{}
// Member Functions 
void mousse::faceToPoint::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding points from face in faceSet " << setName_
      << " ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing points from face in faceSet " << setName_
      << " ..." << endl;
    combine(set, false);
  }
}
