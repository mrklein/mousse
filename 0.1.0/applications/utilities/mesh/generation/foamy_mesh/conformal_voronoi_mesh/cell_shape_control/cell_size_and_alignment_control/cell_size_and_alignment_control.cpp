// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_size_and_alignment_control.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(cellSizeAndAlignmentControl, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(cellSizeAndAlignmentControl, dictionary);
}
// Private Member Functions 
// Constructors 
mousse::cellSizeAndAlignmentControl::cellSizeAndAlignmentControl
(
  const Time& runTime,
  const word& name,
  const dictionary& controlFunctionDict,
  const conformationSurfaces& /*geometryToConformTo*/,
  const scalar& defaultCellSize
)
:
  runTime_(runTime),
  defaultCellSize_(defaultCellSize),
  forceInitialPointInsertion_
  (
    controlFunctionDict.lookupOrDefault<Switch>
    (
      "forceInitialPointInsertion",
      "off"
    )
  ),
  name_(name)
{}
// Selectors
mousse::autoPtr<mousse::cellSizeAndAlignmentControl>
mousse::cellSizeAndAlignmentControl::New
(
  const Time& runTime,
  const word& name,
  const dictionary& controlFunctionDict,
  const conformationSurfaces& geometryToConformTo,
  const scalar& defaultCellSize
)
{
  word cellSizeAndAlignmentControlTypeName
  (
    controlFunctionDict.lookup("type")
  );
  Info<< indent << "Selecting cellSizeAndAlignmentControl "
    << cellSizeAndAlignmentControlTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find
    (
      cellSizeAndAlignmentControlTypeName
    );
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "cellSizeAndAlignmentControl::New()"
    )   << "Unknown cellSizeAndAlignmentControl type "
      << cellSizeAndAlignmentControlTypeName
      << endl << endl
      << "Valid cellSizeAndAlignmentControl types are :" << endl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<cellSizeAndAlignmentControl>
  (
    cstrIter()
    (
      runTime,
      name,
      controlFunctionDict,
      geometryToConformTo,
      defaultCellSize
    )
  );
}
// Destructor 
mousse::cellSizeAndAlignmentControl::~cellSizeAndAlignmentControl()
{}
// Member Functions
