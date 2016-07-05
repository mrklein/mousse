// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cuthill_mc_kee_renumber.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "band_compression.hpp"
#include "decomposition_method.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(CuthillMcKeeRenumber, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  renumberMethod,
  CuthillMcKeeRenumber,
  dictionary
);

}


// Constructors 
mousse::CuthillMcKeeRenumber::CuthillMcKeeRenumber(const dictionary& renumberDict)
:
  renumberMethod{renumberDict},
  reverse_
  {
    renumberDict.found(typeName + "Coeffs")
    ? Switch{renumberDict.subDict(typeName + "Coeffs").lookup("reverse")}
    : Switch{false}
  }
{}


// Member Functions 
mousse::labelList mousse::CuthillMcKeeRenumber::renumber
(
  const polyMesh& mesh,
  const pointField& /*points*/
) const
{
  CompactListList<label> cellCells;
  decompositionMethod::calcCellCells
    (
      mesh,
      identity(mesh.nCells()),
      mesh.nCells(),
      false,                      // local only
      cellCells
    );
  labelList orderedToOld = bandCompression(cellCells());
  if (reverse_) {
    reverse(orderedToOld);
  }
  return orderedToOld;
}


mousse::labelList mousse::CuthillMcKeeRenumber::renumber
(
  const labelList& cellCells,
  const labelList& offsets,
  const pointField& /*cc*/
) const
{
  labelList orderedToOld = bandCompression(cellCells, offsets);
  if (reverse_) {
    reverse(orderedToOld);
  }
  return orderedToOld;
}


mousse::labelList mousse::CuthillMcKeeRenumber::renumber
(
  const labelListList& cellCells,
  const pointField& /*points*/
) const
{
  labelList orderedToOld = bandCompression(cellCells);
  if (reverse_) {
    reverse(orderedToOld);
  }
  return orderedToOld;
}

