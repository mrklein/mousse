// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_centred_face_to_cell_stencil.hpp"
#include "map_distribute.hpp"
#include "face_to_cell_stencil.hpp"
// Constructors 
mousse::extendedCentredFaceToCellStencil::extendedCentredFaceToCellStencil
(
  const faceToCellStencil& stencil
)
:
  extendedFaceToCellStencil(stencil.mesh()),
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
// Per face which elements of the stencil to keep.
void mousse::extendedCentredFaceToCellStencil::compact()
{
  boolList isInStencil(map().constructSize(), false);
  forAll(stencil_, faceI)
  {
    const labelList& stencilCells = stencil_[faceI];
    forAll(stencilCells, i)
    {
      isInStencil[stencilCells[i]] = true;
    }
  }
  mapPtr_().compact(isInStencil, Pstream::msgType());
}
