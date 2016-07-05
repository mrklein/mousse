// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "map_distribute.hpp"
#include "extended_centred_cell_to_face_stencil.hpp"
#include "cell_to_face_stencil.hpp"
// Constructors 
mousse::extendedCentredCellToFaceStencil::extendedCentredCellToFaceStencil
(
  const cellToFaceStencil& stencil
)
:
  extendedCellToFaceStencil(stencil.mesh()),
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
void mousse::extendedCentredCellToFaceStencil::compact()
{
  boolList isInStencil(map().constructSize(), false);
  FOR_ALL(stencil_, faceI)
  {
    const labelList& stencilCells = stencil_[faceI];
    FOR_ALL(stencilCells, i)
    {
      isInStencil[stencilCells[i]] = true;
    }
  }
  mapPtr_().compact(isInStencil, Pstream::msgType());
}
