// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform_value.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(uniformValue, 0);
  addToRunTimeSelectionTable
  (
    surfaceCellSizeFunction,
    uniformValue,
    dictionary
  );
}
// Constructors 
mousse::uniformValue::uniformValue
(
  const dictionary& cellSizeFunctionDict,
  const searchableSurface& surface,
  const scalar& defaultCellSize
)
:
  surfaceCellSizeFunction
  (
    typeName,
    cellSizeFunctionDict,
    surface,
    defaultCellSize
  ),
  surfaceCellSize_
  (
    readScalar(coeffsDict().lookup("surfaceCellSizeCoeff"))*defaultCellSize
  )
{}
// Member Functions 
mousse::scalar mousse::uniformValue::interpolate
(
  const point& pt,
  const label index
) const
{
  return surfaceCellSize_;
}
