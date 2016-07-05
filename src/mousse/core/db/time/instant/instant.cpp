// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "instant_list.hpp"
#include "time.hpp"


// Static Data Members
const char* const mousse::instant::typeName = "instant";

// Constructors 
mousse::instant::instant()
{}


mousse::instant::instant(const scalar val, const word& tname)
:
  value_{val},
  name_{tname}
{}


mousse::instant::instant(const scalar val)
:
  value_{val},
  name_{Time::timeName(val)}
{}


mousse::instant::instant(const word& tname)
:
  value_{atof(tname.c_str())},
  name_{tname}
{}


// Member Functions 
bool mousse::instant::equal(const scalar b) const
{
  return (value_ < b + SMALL  && value_ > b - SMALL);
}


// Friend Operators 
bool mousse::operator==(const instant& a, const instant& b)
{
  return a.equal(b.value_);
}


bool mousse::operator!=(const instant& a, const instant& b)
{
  return !operator==(a, b);
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, instant& I)
{
  is >> I.value_ >> I.name_;
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const instant& I)
{
 os << I.value_ << tab << I.name_;
 return os;
}

