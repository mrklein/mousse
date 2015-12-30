// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_to_cell.hpp"
#include "poly_mesh.hpp"
#include "point_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(pointToCell, 0);
  addToRunTimeSelectionTable(topoSetSource, pointToCell, word);
  addToRunTimeSelectionTable(topoSetSource, pointToCell, istream);
  template<>
  const char* mousse::NamedEnum
  <
    mousse::pointToCell::pointAction,
    2
  >::names[] =
  {
    "any",
    "edge"
  };
}
mousse::topoSetSource::addToUsageTable mousse::pointToCell::usage_
(
  pointToCell::typeName,
  "\n    Usage: pointToCell <pointSet> any|edge\n\n"
  "    Select all cells with any point ('any') or any edge ('edge')"
  " in the pointSet\n\n"
);
const mousse::NamedEnum<mousse::pointToCell::pointAction, 2>
  mousse::pointToCell::pointActionNames_;
// Private Member Functions 
void mousse::pointToCell::combine(topoSet& set, const bool add) const
{
  // Load the set
  pointSet loadedSet(mesh_, setName_);
  // Handle any selection
  if (option_ == ANY)
  {
    forAllConstIter(pointSet, loadedSet, iter)
    {
      const label pointI = iter.key();
      const labelList& pCells = mesh_.pointCells()[pointI];
      forAll(pCells, pCellI)
      {
        addOrDelete(set, pCells[pCellI], add);
      }
    }
  }
  else if (option_ == EDGE)
  {
    const faceList& faces = mesh_.faces();
    forAll(faces, faceI)
    {
      const face& f = faces[faceI];
      forAll(f, fp)
      {
        if (loadedSet.found(f[fp]) && loadedSet.found(f.nextLabel(fp)))
        {
          addOrDelete(set, mesh_.faceOwner()[faceI], add);
          if (mesh_.isInternalFace(faceI))
          {
            addOrDelete(set, mesh_.faceNeighbour()[faceI], add);
          }
        }
      }
    }
  }
}
// Constructors 
// Construct from components
mousse::pointToCell::pointToCell
(
  const polyMesh& mesh,
  const word& setName,
  const pointAction option
)
:
  topoSetSource(mesh),
  setName_(setName),
  option_(option)
{}
// Construct from dictionary
mousse::pointToCell::pointToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  setName_(dict.lookup("set")),
  option_(pointActionNames_.read(dict.lookup("option")))
{}
// Construct from Istream
mousse::pointToCell::pointToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  setName_(checkIs(is)),
  option_(pointActionNames_.read(checkIs(is)))
{}
// Destructor 
mousse::pointToCell::~pointToCell()
{}
// Member Functions 
void mousse::pointToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding cells according to pointSet " << setName_
      << " ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing cells according to pointSet " << setName_
      << " ..." << endl;
    combine(set, false);
  }
}
