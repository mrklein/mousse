// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "refine_cell.hpp"
#include "error.hpp"


// Constructors 
mousse::refineCell::refineCell()
:
  cellNo_{-1},
  direction_{vector::zero}
{}


mousse::refineCell::refineCell(const label cellI, const vector& direction)
:
  cellNo_{cellI},
  direction_{direction}
{
  scalar magDir = mag(direction_);
  if (magDir < SMALL) {
    FATAL_ERROR_IN("refineCell(const label, const vector&)")
      << "(almost)zero vector as direction for cell " << cellNo_
      << abort(FatalError);
  } else if (mag(magDir - 1) > SMALL) {
    // Normalize
    direction_ /= mag(direction_);
  }
}


mousse::refineCell::refineCell(Istream& is)
:
  cellNo_{readLabel(is)},
  direction_{is}
{
  scalar magDir = mag(direction_);
  if (magDir < SMALL) {
    FATAL_ERROR_IN("refineCell(Istream&)")
      << "(almost)zero vector as direction for cell " << cellNo_
      << abort(FatalError);
  } else if (mag(magDir - 1) > SMALL) {
    // Normalize
    direction_ /= mag(direction_);
  }
}


// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const refineCell& r)
{
  if (os.format() == IOstream::ASCII) {
    os << r.cellNo() << token::SPACE << r.direction();
  } else {
    os << r.cellNo() << r.direction();
  }
  return os;
}

