// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "shape_to_cell.hpp"
#include "poly_mesh.hpp"
#include "unit_conversion.hpp"
#include "hex_matcher.hpp"
#include "cell_features.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(shapeToCell, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, shapeToCell, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, shapeToCell, istream);
}
mousse::topoSetSource::addToUsageTable mousse::shapeToCell::usage_
(
  shapeToCell::typeName,
  "\n    Usage: shapeToCell tet|pyr|prism|hex|tetWedge|wedge|splitHex\n\n"
  "    Select all cells of given cellShape.\n"
  "    (splitHex hardcoded with internal angle < 10 degrees)\n"
);
// Angle for polys to be considered splitHexes.
mousse::scalar mousse::shapeToCell::featureCos = mousse::cos(degToRad(10.0));
// Private Member Functions 
void mousse::shapeToCell::combine(topoSet& set, const bool add) const
{
  if (type_ == "splitHex")
  {
    for (label cellI = 0; cellI < mesh_.nCells(); cellI++)
    {
      cellFeatures superCell(mesh_, featureCos, cellI);
      if (hexMatcher().isA(superCell.faces()))
      {
        addOrDelete(set, cellI, add);
      }
    }
  }
  else
  {
    const cellModel& wantedModel = *(cellModeller::lookup(type_));
    const cellShapeList& cellShapes = mesh_.cellShapes();
    FOR_ALL(cellShapes, cellI)
    {
      if (cellShapes[cellI].model() == wantedModel)
      {
        addOrDelete(set, cellI, add);
      }
    }
  }
}
// Constructors 
// Construct from components
mousse::shapeToCell::shapeToCell
(
  const polyMesh& mesh,
  const word& type
)
:
  topoSetSource(mesh),
  type_(type)
{
  if (!cellModeller::lookup(type_) && (type_ != "splitHex"))
  {
    FATAL_ERROR_IN
    (
      "shapeToCell::shapeToCell(const polyMesh&, const word&)"
    )   << "Illegal cell type " << type_ << exit(FatalError);
  }
}
// Construct from dictionary
mousse::shapeToCell::shapeToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  type_(dict.lookup("type"))
{
  if (!cellModeller::lookup(type_) && (type_ != "splitHex"))
  {
    FATAL_ERROR_IN
    (
      "shapeToCell::shapeToCell(const polyMesh&, const dictionary&)"
    )   << "Illegal cell type " << type_ << exit(FatalError);
  }
}
// Construct from Istream
mousse::shapeToCell::shapeToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  type_(checkIs(is))
{
  if (!cellModeller::lookup(type_) && (type_ != "splitHex"))
  {
    FATAL_ERROR_IN
    (
      "shapeToCell::shapeToCell(const polyMesh&, Istream&)"
    )   << "Illegal cell type " << type_ << exit(FatalError);
  }
}
// Destructor 
mousse::shapeToCell::~shapeToCell()
{}
// Member Functions 
void mousse::shapeToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding all cells of type " << type_ << " ..." << endl;
    combine(set, true);
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing all cells of type " << type_ << " ..." << endl;
    combine(set, false);
  }
}
