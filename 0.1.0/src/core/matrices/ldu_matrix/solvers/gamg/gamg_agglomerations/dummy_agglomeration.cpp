// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dummy_agglomeration.hpp"
#include "add_to_run_time_selection_table.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(dummyAgglomeration, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGAgglomeration,
  dummyAgglomeration,
  lduMesh
);

}

// Constructors
mousse::dummyAgglomeration::dummyAgglomeration
(
  const lduMesh& mesh,
  const dictionary& controlDict
)
:
  GAMGAgglomeration(mesh, controlDict),
  nLevels_(readLabel(controlDict.lookup("nLevels")))
{
  const label nCoarseCells = mesh.lduAddr().size();
  for
  (
    label nCreatedLevels = 0;
    nCreatedLevels < nLevels_;
    nCreatedLevels++
  )
  {
    nCells_[nCreatedLevels] = nCoarseCells;
    restrictAddressing_.set
    (
      nCreatedLevels,
      new labelField(identity(nCoarseCells))
    );
    agglomerateLduAddressing(nCreatedLevels);
  }
  // Shrink the storage of the levels to those created
  compactLevels(nLevels_);
}
