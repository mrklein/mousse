// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_size_calculation_type.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tri_surface_mesh.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cellSizeCalculationType, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(cellSizeCalculationType, dictionary);

}


// Constructors 
mousse::cellSizeCalculationType::cellSizeCalculationType
(
  const word& /*type*/,
  const dictionary& cellSizeCalculationTypeDict,
  const triSurfaceMesh& surface,
  const scalar& defaultCellSize
)
:
  cellSizeCalculationTypeDict_{cellSizeCalculationTypeDict},
  surface_{surface},
  defaultCellSize_{defaultCellSize}
{}


// Selectors
mousse::autoPtr<mousse::cellSizeCalculationType> mousse::cellSizeCalculationType::New
(
  const dictionary& cellSizeCalculationTypeDict,
  const triSurfaceMesh& surface,
  const scalar& defaultCellSize
)
{
  word cellSizeCalculationTypeTypeName
  {
    cellSizeCalculationTypeDict.lookup("cellSizeCalculationType")
  };
  Info << indent << "Selecting cellSizeCalculationType "
    << cellSizeCalculationTypeTypeName << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(cellSizeCalculationTypeTypeName);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "cellSizeCalculationType::New(dictionary&, "
      "const conformalVoronoiMesh&, const searchableSurface&)"
    )
    << "Unknown cellSizeCalculationType type "
    << cellSizeCalculationTypeTypeName
    << endl << endl
    << "Valid cellSizeCalculationType types are :" << endl
    << dictionaryConstructorTablePtr_->toc()
    << exit(FatalError);
  }
  return
    autoPtr<cellSizeCalculationType>
    {
      cstrIter()(cellSizeCalculationTypeDict, surface, defaultCellSize)
    };
}


// Destructor 
mousse::cellSizeCalculationType::~cellSizeCalculationType()
{}

