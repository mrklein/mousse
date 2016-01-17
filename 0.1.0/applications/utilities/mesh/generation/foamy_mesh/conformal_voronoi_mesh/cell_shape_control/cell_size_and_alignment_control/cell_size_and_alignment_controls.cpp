// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_size_and_alignment_controls.hpp"
#include "searchable_surface_control.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(cellSizeAndAlignmentControls, 0);
}
// Private Member Functions 
bool mousse::cellSizeAndAlignmentControls::evalCellSizeFunctions
(
  const point& pt,
  scalar& minSize,
  label& maxPriority
) const
{
  bool anyFunctionFound = false;
  // Regions requesting with the same priority take the smallest
  if (controlFunctions_.size())
  {
    // Maintain priority of current hit. Initialise so it always goes
    // through at least once.
    label previousPriority = labelMin;
    forAll(controlFunctions_, i)
    {
      const cellSizeAndAlignmentControl& cSF = controlFunctions_[i];
      if (isA<searchableSurfaceControl>(cSF))
      {
        const searchableSurfaceControl& sSC =
          refCast<const searchableSurfaceControl>(cSF);
        anyFunctionFound = sSC.cellSize(pt, minSize, previousPriority);
        if (previousPriority > maxPriority)
        {
          maxPriority = previousPriority;
        }
      }
    }
  }
  return anyFunctionFound;
}
// Constructors 
mousse::cellSizeAndAlignmentControls::cellSizeAndAlignmentControls
(
  const Time& runTime,
  const dictionary& shapeControlDict,
  const conformationSurfaces& geometryToConformTo,
  const scalar& defaultCellSize
)
:
  shapeControlDict_(shapeControlDict),
  geometryToConformTo_(geometryToConformTo),
  controlFunctions_(shapeControlDict_.size()),
  defaultCellSize_(defaultCellSize)
{
  label functionI = 0;
  forAllConstIter(dictionary, shapeControlDict_, iter)
  {
    word shapeControlEntryName = iter().keyword();
    const dictionary& controlFunctionDict
    (
      shapeControlDict_.subDict(shapeControlEntryName)
    );
    Info<< nl << "Shape Control : " << shapeControlEntryName << endl;
    Info<< incrIndent;
    controlFunctions_.set
    (
      functionI,
      cellSizeAndAlignmentControl::New
      (
        runTime,
        shapeControlEntryName,
        controlFunctionDict,
        geometryToConformTo_,
        defaultCellSize_
      )
    );
    Info<< decrIndent;
    functionI++;
  }
  // Sort controlFunctions_ by maxPriority
  SortableList<label> functionPriorities(functionI);
  forAll(controlFunctions_, funcI)
  {
    functionPriorities[funcI] = controlFunctions_[funcI].maxPriority();
  }
  functionPriorities.reverseSort();
  labelList invertedFunctionPriorities =
    invert(functionPriorities.size(), functionPriorities.indices());
  controlFunctions_.reorder(invertedFunctionPriorities);
}
// Destructor 
mousse::cellSizeAndAlignmentControls::~cellSizeAndAlignmentControls()
{}
// Member Functions 
mousse::scalar mousse::cellSizeAndAlignmentControls::cellSize
(
  const point& pt
) const
{
  scalar size = defaultCellSize_;
  label maxPriority = -1;
  evalCellSizeFunctions(pt, size, maxPriority);
  return size;
}
mousse::scalar mousse::cellSizeAndAlignmentControls::cellSize
(
  const point& pt,
  label& maxPriority
) const
{
  scalar size = defaultCellSize_;
  maxPriority = -1;
  evalCellSizeFunctions(pt, size, maxPriority);
  return size;
}
