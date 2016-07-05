// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "refinement_data.hpp"


// Friend Operators 
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::refinementData& wDist
)
{
  if (os.format() == IOstream::ASCII) {
    os << wDist.refinementCount_ << token::SPACE << wDist.count_;
  } else {
    os.write(reinterpret_cast<const char*>(&wDist.refinementCount_),
             sizeof(refinementData));
  }
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const refinementData&)");
  return os;
}


mousse::Istream& mousse::operator>>(mousse::Istream& is, mousse::refinementData& wDist)
{
  if (is.format() == IOstream::ASCII) {
    is >> wDist.refinementCount_ >> wDist.count_;
  } else {
    is.read(reinterpret_cast<char*>(&wDist.refinementCount_),
            sizeof(refinementData));
  }
  // Check state of Istream
  is.check("Istream& operator>>(Istream&, refinementData&)");
  return is;
}

