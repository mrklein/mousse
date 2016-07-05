// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "grading_descriptor.hpp"
#include "iostream.hpp"
#include "token.hpp"


// Constructors 
mousse::gradingDescriptor::gradingDescriptor()
:
  blockFraction_{1},
  nDivFraction_{1},
  expansionRatio_{1}
{}


mousse::gradingDescriptor::gradingDescriptor
(
  const scalar blockFraction,
  const scalar nDivFraction,
  const scalar expansionRatio
)
:
  blockFraction_{blockFraction},
  nDivFraction_{nDivFraction},
  expansionRatio_{expansionRatio}
{}


mousse::gradingDescriptor::gradingDescriptor
(
  const scalar expansionRatio
)
:
  blockFraction_{1.0},
  nDivFraction_{1.0},
  expansionRatio_{expansionRatio}
{}


mousse::gradingDescriptor::gradingDescriptor(Istream& is)
{
  is >> *this;
}


// Destructor 
mousse::gradingDescriptor::~gradingDescriptor()
{}


// Member Functions 
mousse::gradingDescriptor mousse::gradingDescriptor::inv() const
{
  return {blockFraction_, nDivFraction_, 1.0/expansionRatio_};
}


// Member Operators 
bool mousse::gradingDescriptor::operator==(const gradingDescriptor& gd) const
{
  return
    equal(blockFraction_, gd.blockFraction_)
    && equal(nDivFraction_, gd.nDivFraction_)
    && equal(expansionRatio_, gd.expansionRatio_);
}


bool mousse::gradingDescriptor::operator!=(const gradingDescriptor& gd) const
{
  return !operator==(gd);
}


// Friend Operators 
mousse::Istream& mousse::operator>>(Istream& is, gradingDescriptor& gd)
{
  // Examine next token
  token t{is};
  if (t.isNumber()) {
    gd.blockFraction_ = 1.0;
    gd.nDivFraction_ = 1.0;
    gd.expansionRatio_ = t.number();
  } else if (t.isPunctuation() && t.pToken() == token::BEGIN_LIST) {
    is >> gd.blockFraction_ >> gd.nDivFraction_ >> gd.expansionRatio_;
    is.readEnd("gradingDescriptor");
  }
  // Check state of Istream
  is.check("operator>>(Istream&, gradingDescriptor&)");
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const gradingDescriptor& gd)
{
  if (equal(gd.blockFraction_, 1)) {
    os << gd.expansionRatio_;
  } else {
    os << token::BEGIN_LIST
      << gd.blockFraction_ << token::SPACE
      << gd.nDivFraction_ << token::SPACE
      << gd.expansionRatio_
      << token::END_LIST;
  }
  return os;
}

