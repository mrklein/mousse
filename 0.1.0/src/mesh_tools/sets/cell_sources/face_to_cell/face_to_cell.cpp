// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_to_cell.hpp"
#include "poly_mesh.hpp"
#include "face_set.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(faceToCell, 0);
  addToRunTimeSelectionTable(topoSetSource, faceToCell, word);
  addToRunTimeSelectionTable(topoSetSource, faceToCell, istream);
  template<>
  const char* mousse::NamedEnum
  <
    mousse::faceToCell::faceAction,
    4
  >::names[] =
  {
    "neighbour",
    "owner",
    "any",
    "all"
  };
}
mousse::topoSetSource::addToUsageTable mousse::faceToCell::usage_
(
  faceToCell::typeName,
  "\n    Usage: faceToCell <faceSet> neighbour|owner|any|all\n\n"
  "    Select cells that are the owner|neighbour|any"
  " of the faces in the faceSet or where all faces are in the faceSet\n\n"
);
const mousse::NamedEnum<mousse::faceToCell::faceAction, 4>
  mousse::faceToCell::faceActionNames_;
// Private Member Functions 
void mousse::faceToCell::combine(topoSet& set, const bool add) const
{
  // Load the set
  faceSet loadedSet(mesh_, setName_);
  // Handle owner/neighbour/any selection
  forAllConstIter(faceSet, loadedSet, iter)
  {
    const label faceI = iter.key();
    if ((option_ == OWNER) || (option_ == ANY))
    {
      const label cellI = mesh_.faceOwner()[faceI];
      addOrDelete(set, cellI, add);
    }
    if (mesh_.isInternalFace(faceI))
    {
      if ((option_ == NEIGHBOUR) || (option_ == ANY))
      {
        const label cellI = mesh_.faceNeighbour()[faceI];
        addOrDelete(set, cellI, add);
      }
    }
  }
  // Handle all selection.
  if (option_ == ALL)
  {
    // Count number of selected faces per cell.
    Map<label> facesPerCell(loadedSet.size());
    forAllConstIter(faceSet, loadedSet, iter)
    {
      const label faceI = iter.key();
      const label own = mesh_.faceOwner()[faceI];
      Map<label>::iterator fndOwn = facesPerCell.find(own);
      if (fndOwn == facesPerCell.end())
      {
        facesPerCell.insert(own, 1);
      }
      else
      {
        fndOwn()++;
      }
      if (mesh_.isInternalFace(faceI))
      {
        label nei = mesh_.faceNeighbour()[faceI];
        Map<label>::iterator fndNei = facesPerCell.find(nei);
        if (fndNei == facesPerCell.end())
        {
          facesPerCell.insert(nei, 1);
        }
        else
        {
          fndNei()++;
        }
      }
    }
    // Include cells that are referenced as many times as they have faces
    // -> all faces in set.
    forAllConstIter(Map<label>, facesPerCell, iter)
    {
      const label cellI = iter.key();
      if (iter() == mesh_.cells()[cellI].size())
      {
        addOrDelete(set, cellI, add);
      }
    }
  }
}
// Constructors 
// Construct from components
mousse::faceToCell::faceToCell
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
mousse::faceToCell::faceToCell
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
mousse::faceToCell::faceToCell
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
mousse::faceToCell::~faceToCell()
{}
// Member Functions 
void mousse::faceToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding cells according to faceSet " << setName_
      << " ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing cells according to faceSet " << setName_
      << " ..." << endl;
    combine(set, false);
  }
}
