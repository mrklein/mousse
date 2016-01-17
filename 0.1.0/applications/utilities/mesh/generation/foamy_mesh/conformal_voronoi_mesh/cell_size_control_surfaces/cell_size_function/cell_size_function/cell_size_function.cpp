// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_size_function.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeNameAndDebug(cellSizeFunction, 0);
  defineRunTimeSelectionTable(cellSizeFunction, dictionary);
  scalar cellSizeFunction::snapToSurfaceTol_ = 1e-10;
}
// Constructors 
mousse::cellSizeFunction::cellSizeFunction
(
  const word& type,
  const dictionary& cellSizeFunctionDict,
  const searchableSurface& surface,
  const scalar& defaultCellSize,
  const labelList regionIndices
)
:
  dictionary(cellSizeFunctionDict),
  surface_(surface),
  surfaceCellSizeFunction_
  (
    surfaceCellSizeFunction::New
    (
      cellSizeFunctionDict,
      surface,
      defaultCellSize
    )
  ),
  coeffsDict_(subDict(type + "Coeffs")),
  defaultCellSize_(defaultCellSize),
  regionIndices_(regionIndices),
  sideMode_(),
  priority_(readLabel(cellSizeFunctionDict.lookup("priority", true)))
{
  word mode = cellSizeFunctionDict.lookup("mode", true);
  if (surface_.hasVolumeType())
  {
    if (mode == "inside")
    {
      sideMode_ = smInside;
    }
    else if (mode == "outside")
    {
      sideMode_ = smOutside;
    }
    else if (mode == "bothSides")
    {
      sideMode_ = rmBothsides;
    }
    else
    {
      FatalErrorIn("searchableSurfaceControl::searchableSurfaceControl")
        << "Unknown mode, expected: inside, outside or bothSides" << nl
        << exit(FatalError);
    }
  }
  else
  {
    if (mode != "bothSides")
    {
      WarningIn("searchableSurfaceControl::searchableSurfaceControl")
        << "surface does not support volumeType, defaulting mode to "
        << "bothSides."
        << endl;
    }
    sideMode_ = rmBothsides;
  }
  if (debug)
  {
    Info<< nl
      << "Cell size function for surface " << surface.name()
      << ", " << mode
      << ", priority = " << priority_
      << ", regions = " << regionIndices_
      << endl;
  }
}
// Selectors
mousse::autoPtr<mousse::cellSizeFunction> mousse::cellSizeFunction::New
(
  const dictionary& cellSizeFunctionDict,
  const searchableSurface& surface,
  const scalar& defaultCellSize,
  const labelList regionIndices
)
{
  word cellSizeFunctionTypeName
  (
    cellSizeFunctionDict.lookup("cellSizeFunction")
  );
  Info<< indent << "Selecting cellSizeFunction " << cellSizeFunctionTypeName
    << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(cellSizeFunctionTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "cellSizeFunction::New(dictionary&, "
      "const conformalVoronoiMesh&, const searchableSurface&)"
    )   << "Unknown cellSizeFunction type "
      << cellSizeFunctionTypeName
      << endl << endl
      << "Valid cellSizeFunction types are :" << endl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<cellSizeFunction>
  (
    cstrIter()
    (
      cellSizeFunctionDict,
      surface,
      defaultCellSize,
      regionIndices
    )
  );
}
// Destructor 
mousse::cellSizeFunction::~cellSizeFunction()
{}
