// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tet_cell.hpp"
#include "cell_shape.hpp"
#include "cell_modeller.hpp"


// Member Functions 
mousse::cellShape mousse::tetCell::tetCellShape() const
{
  static const cellModel* tetModelPtr_ = nullptr;
  if (tetModelPtr_ == nullptr)
  {
    tetModelPtr_ = cellModeller::lookup("tet");
  }
  const cellModel& tet = *tetModelPtr_;
  return cellShape(tet, labelList(*this));
}
