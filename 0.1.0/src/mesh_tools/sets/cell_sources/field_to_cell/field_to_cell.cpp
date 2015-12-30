// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_to_cell.hpp"
#include "poly_mesh.hpp"
#include "cell_set.hpp"
#include "time.hpp"
#include "ifstream.hpp"
#include "field_dictionary.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(fieldToCell, 0);
addToRunTimeSelectionTable(topoSetSource, fieldToCell, word);
addToRunTimeSelectionTable(topoSetSource, fieldToCell, istream);
}
mousse::topoSetSource::addToUsageTable mousse::fieldToCell::usage_
(
  fieldToCell::typeName,
  "\n    Usage: fieldToCell field min max\n\n"
  "    Select all cells with field value >= min and <= max\n\n"
);
// Private Member Functions 
void mousse::fieldToCell::applyToSet
(
  const topoSetSource::setAction action,
  const scalarField& field,
  topoSet& set
) const
{
  Info<< "    Field min:" << min(field)
    << " max:" << max(field) << endl;
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
  {
    Info<< "    Adding all cells with value of field " << fieldName_
      << " within range " << min_ << ".." << max_ << endl;
    forAll(field, cellI)
    {
      if (field[cellI] >= min_ && field[cellI] <= max_)
      {
        set.insert(cellI);
      }
    }
  }
  else if (action == topoSetSource::DELETE)
  {
    Info<< "    Removing all cells with value of field " << fieldName_
      << " within range " << min_ << ".." << max_ << endl;
    forAll(field, cellI)
    {
      if (field[cellI] >= min_ && field[cellI] <= max_)
      {
        set.erase(cellI);
      }
    }
  }
}
// Constructors 
// Construct from components
mousse::fieldToCell::fieldToCell
(
  const polyMesh& mesh,
  const word& fieldName,
  const scalar min,
  const scalar max
)
:
  topoSetSource(mesh),
  fieldName_(fieldName),
  min_(min),
  max_(max)
{}
// Construct from dictionary
mousse::fieldToCell::fieldToCell
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource(mesh),
  fieldName_(dict.lookup("fieldName")),
  min_(readScalar(dict.lookup("min"))),
  max_(readScalar(dict.lookup("max")))
{}
// Construct from Istream
mousse::fieldToCell::fieldToCell
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource(mesh),
  fieldName_(checkIs(is)),
  min_(readScalar(checkIs(is))),
  max_(readScalar(checkIs(is)))
{}
// Destructor 
mousse::fieldToCell::~fieldToCell()
{}
// Member Functions 
void mousse::fieldToCell::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
//    // Construct temporary fvMesh from polyMesh
//    fvMesh fMesh
//    (
//        mesh(), // IOobject
//        mesh().points(),
//        mesh().faces(),
//        mesh().cells()
//    );
//
//    const polyBoundaryMesh& patches = mesh().boundaryMesh();
//
//    List<polyPatch*> newPatches(patches.size());
//    forAll(patches, patchI)
//    {
//        const polyPatch& pp = patches[patchI];
//
//        newPatches[patchI] =
//            patches[patchI].clone
//            (
//                fMesh.boundaryMesh(),
//                patchI,
//                pp.size(),
//                pp.start()
//            ).ptr();
//    }
//    fMesh.addFvPatches(newPatches);
  // Try to load field
  IOobject fieldObject
  (
    fieldName_,
    mesh().time().timeName(),
    mesh(),
    IOobject::MUST_READ,
    IOobject::AUTO_WRITE,
    false
  );
  if (!fieldObject.headerOk())
  {
    WarningIn
    (
      "fieldToCell::applyToSet(const topoSetSource::setAction"
      ", topoSet& set)"
    )   << "Cannot read field " << fieldName_
      << " from time " << mesh().time().timeName() << endl;
  }
  else if (fieldObject.headerClassName() == "volScalarField")
  {
    IFstream str(fieldObject.filePath());
    // Read dictionary
    fieldDictionary fieldDict(fieldObject, fieldObject.headerClassName());
    scalarField internalVals("internalField", fieldDict, mesh().nCells());
    applyToSet(action, internalVals, set);
  }
  else if (fieldObject.headerClassName() == "volVectorField")
  {
    IFstream str(fieldObject.filePath());
    // Read dictionary
    fieldDictionary fieldDict(fieldObject, fieldObject.headerClassName());
    vectorField internalVals("internalField", fieldDict, mesh().nCells());
    applyToSet(action, mag(internalVals), set);
  }
  else
  {
    WarningIn
    (
      "fieldToCell::applyToSet(const topoSetSource::setAction"
      ", topoSet& set)"
    )   << "Cannot handle fields of type " << fieldObject.headerClassName()
      << endl;
  }
}
