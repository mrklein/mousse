// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   find_ref_cell.cpp
#ifndef find_ref_cell_hpp_
#define find_ref_cell_hpp_
#include "label.hpp"
#include "vol_fields.hpp"
namespace mousse
{
//- If the field fieldRef needs referencing find the reference cell nearest
//  (in index) to the given cell looked-up for field, but which is not on a
//  cyclic, symmetry or processor patch.
void setRefCell
(
  const volScalarField& field,
  const volScalarField& fieldRef,
  const dictionary& dict,
  label& refCelli,
  scalar& refValue,
  const bool forceReference = false
);
//- If the field needs referencing find the reference cell nearest
//  (in index) to the given cell looked-up for field, but which is not on a
//  cyclic, symmetry or processor patch.
void setRefCell
(
  const volScalarField& field,
  const dictionary& dict,
  label& refCelli,
  scalar& refValue,
  const bool forceReference = false
);
//- Return the current value of field in the reference cell
scalar getRefCellValue
(
  const volScalarField& field,
  const label refCelli
);
}
#endif
