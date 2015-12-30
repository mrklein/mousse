// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_centred_cell_to_cell_stencil.hpp"
#include "map_distribute.hpp"
#include "cell_to_cell_stencil.hpp"
// Constructors 
mousse::extendedCentredCellToCellStencil::extendedCentredCellToCellStencil
(
  const cellToCellStencil& stencil
)
:
  extendedCellToCellStencil(stencil.mesh()),
  stencil_(stencil)
{
  // Calculate distribute map (also renumbers elements in stencil)
  List<Map<label> > compactMap(Pstream::nProcs());
  mapPtr_.reset
  (
    new mapDistribute
    (
      stencil.globalNumbering(),
      stencil_,
      compactMap
    )
  );
}
// Member Functions 
// Per cell which elements of the stencil to keep.
void mousse::extendedCentredCellToCellStencil::compact()
{
  boolList isInStencil(map().constructSize(), false);
  forAll(stencil_, cellI)
  {
    const labelList& stencilCells = stencil_[cellI];
    forAll(stencilCells, i)
    {
      isInStencil[stencilCells[i]] = true;
    }
  }
  mapPtr_().compact(isInStencil, Pstream::msgType());
}
