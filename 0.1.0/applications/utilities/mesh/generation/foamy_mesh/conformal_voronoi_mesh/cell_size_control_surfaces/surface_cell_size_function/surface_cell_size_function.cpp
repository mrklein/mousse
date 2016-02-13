// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_cell_size_function.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(surfaceCellSizeFunction, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(surfaceCellSizeFunction, dictionary);
}
// Constructors 
mousse::surfaceCellSizeFunction::surfaceCellSizeFunction
(
  const word& type,
  const dictionary& surfaceCellSizeFunctionDict,
  const searchableSurface& surface,
  const scalar& defaultCellSize
)
:
  dictionary(surfaceCellSizeFunctionDict),
  surface_(surface),
  coeffsDict_(subDict(type + "Coeffs")),
  defaultCellSize_(defaultCellSize),
  refinementFactor_
  (
    lookupOrDefault<scalar>("refinementFactor", 1.0)
  )
{}
// Selectors
mousse::autoPtr<mousse::surfaceCellSizeFunction> mousse::surfaceCellSizeFunction::New
(
  const dictionary& surfaceCellSizeFunctionDict,
  const searchableSurface& surface,
  const scalar& defaultCellSize
)
{
  word surfaceCellSizeFunctionTypeName
  (
    surfaceCellSizeFunctionDict.lookup("surfaceCellSizeFunction")
  );
  Info<< indent << "Selecting surfaceCellSizeFunction "
    << surfaceCellSizeFunctionTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(surfaceCellSizeFunctionTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "surfaceCellSizeFunction::New(dictionary&, "
      "const conformalVoronoiMesh&, const searchableSurface&)"
    )   << "Unknown surfaceCellSizeFunction type "
      << surfaceCellSizeFunctionTypeName
      << endl << endl
      << "Valid surfaceCellSizeFunction types are :" << endl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<surfaceCellSizeFunction>
  (
    cstrIter()(surfaceCellSizeFunctionDict, surface, defaultCellSize)
  );
}
// Destructor 
mousse::surfaceCellSizeFunction::~surfaceCellSizeFunction()
{}
