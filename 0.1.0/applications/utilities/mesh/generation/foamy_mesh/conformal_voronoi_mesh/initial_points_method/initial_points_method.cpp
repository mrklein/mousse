// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "initial_points_method.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(initialPointsMethod, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(initialPointsMethod, dictionary);
// Constructors 
initialPointsMethod::initialPointsMethod
(
  const word& type,
  const dictionary& initialPointsDict,
  const Time& runTime,
  Random& rndGen,
  const conformationSurfaces& geometryToConformTo,
  const cellShapeControl& cellShapeControls,
  const autoPtr<backgroundMeshDecomposition>& decomposition
)
:
  dictionary(initialPointsDict),
  runTime_(runTime),
  rndGen_(rndGen),
  geometryToConformTo_(geometryToConformTo),
  cellShapeControls_(cellShapeControls),
  decomposition_(decomposition),
  detailsDict_(subDict(type + "Coeffs")),
  minimumSurfaceDistanceCoeffSqr_
  (
    sqr
    (
      readScalar
      (
        initialPointsDict.lookup("minimumSurfaceDistanceCoeff")
      )
    )
  ),
  fixInitialPoints_(Switch(initialPointsDict.lookup("fixInitialPoints")))
{}
// Selectors
autoPtr<initialPointsMethod> initialPointsMethod::New
(
  const dictionary& initialPointsDict,
  const Time& runTime,
  Random& rndGen,
  const conformationSurfaces& geometryToConformTo,
  const cellShapeControl& cellShapeControls,
  const autoPtr<backgroundMeshDecomposition>& decomposition
)
{
  word initialPointsMethodTypeName
  (
    initialPointsDict.lookup("initialPointsMethod")
  );
  Info<< nl << "Selecting initialPointsMethod "
    << initialPointsMethodTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(initialPointsMethodTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "initialPointsMethod::New(dictionary&, "
      "const conformalVoronoiMesh&)"
    )   << "Unknown initialPointsMethod type "
      << initialPointsMethodTypeName
      << endl << endl
      << "Valid initialPointsMethod types are :" << endl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return
    autoPtr<initialPointsMethod>
    (
      cstrIter()
      (
        initialPointsDict,
        runTime,
        rndGen,
        geometryToConformTo,
        cellShapeControls,
        decomposition
      )
    );
}
// Destructor 
initialPointsMethod::~initialPointsMethod()
{}
}  // namespace mousse
