// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_to_point.hpp"
#include "poly_mesh.hpp"
#include "cell_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(cellToPoint, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, cellToPoint, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, cellToPoint, istream);

template<>
const char* mousse::NamedEnum
<
  mousse::cellToPoint::cellAction,
  1
>::names[] =
{
  "all"
};
}

mousse::topoSetSource::addToUsageTable mousse::cellToPoint::usage_
(
  cellToPoint::typeName,
  "\n    Usage: cellToPoint <cellSet> all\n\n"
  "    Select all points of cells in the cellSet\n\n"
);

const mousse::NamedEnum<mousse::cellToPoint::cellAction, 1>
  mousse::cellToPoint::cellActionNames_;
// Private Member Functions 
void mousse::cellToPoint::combine(topoSet& set, const bool add) const
{
  // Load the set
  cellSet loadedSet(mesh_, setName_);
  // Add all point from cells in loadedSet
  FOR_ALL_CONST_ITER(cellSet, loadedSet, iter)
  {
    const label cellI = iter.key();
    const labelList& cFaces = mesh_.cells()[cellI];
    FOR_ALL(cFaces, cFaceI)
    {
      const face& f = mesh_.faces()[cFaces[cFaceI]];
      FOR_ALL(f, fp)
      {
        addOrDelete(set, f[fp], add);
      }
    }
  }
}
// Constructors 
// Construct from components
mousse::cellToPoint::cellToPoint
(
  const polyMesh& mesh,
  const word& setName,
  const cellAction option
)
:
  topoSetSource(mesh),
  setName_(setName),
  option_(option)
{}
// Construct from dictionary
mousse::cellToPoint::cellToPoint
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("set")),
  option_(cellActionNames_.read(dict.lookup("option")))
{}
// Construct from Istream
mousse::cellToPoint::cellToPoint
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is)),
  option_(cellActionNames_.read(checkIs(is)))
{}
// Destructor 
mousse::cellToPoint::~cellToPoint()
{}
// Member Functions 
void mousse::cellToPoint::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding from " << setName_ << " ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing from " << setName_ << " ..." << endl;
    combine(set, false);
  }
}
