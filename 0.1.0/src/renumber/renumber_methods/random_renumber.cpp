// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "random_renumber.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "random.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(randomRenumber, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  renumberMethod,
  randomRenumber,
  dictionary
);

}


// Constructors 
mousse::randomRenumber::randomRenumber(const dictionary& renumberDict)
:
  renumberMethod{renumberDict}
{}


// Member Functions 
mousse::labelList mousse::randomRenumber::renumber
(
  const pointField& points
) const
{
  Random rndGen{0};
  labelList newToOld{identity(points.size())};
  for (label iter = 0; iter < 10; iter++) {
    FOR_ALL(newToOld, i) {
      label j = rndGen.integer(0, newToOld.size()-1);
      Swap(newToOld[i], newToOld[j]);
    }
  }
  return newToOld;
}


mousse::labelList mousse::randomRenumber::renumber
(
  const polyMesh& /*mesh*/,
  const pointField& points
) const
{
  return renumber(points);
}


mousse::labelList mousse::randomRenumber::renumber
(
  const labelListList& /*cellCells*/,
  const pointField& points
) const
{
  return renumber(points);
}

