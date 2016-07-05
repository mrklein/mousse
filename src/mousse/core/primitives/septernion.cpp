// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "septernion.hpp"
#include "iostreams.hpp"
#include "ostring_stream.hpp"


// Static Data Members

const char* const mousse::septernion::typeName = "septernion";

const mousse::septernion mousse::septernion::zero
{
  {0, 0, 0},
  {0, {0, 0, 0}}
};


const mousse::septernion mousse::septernion::I
{
  {0, 0, 0},
  {1, {0, 0, 0}}
};


// Constructors 
mousse::septernion::septernion(Istream& is)
{
  is >> *this;
}


mousse::word mousse::name(const septernion& s)
{
  OStringStream buf;
  buf << '(' << s.t() << ',' << s.r() << ')';
  return buf.str();
}


mousse::septernion mousse::slerp
(
  const septernion& qa,
  const septernion& qb,
  const scalar t
)
{
  return {(1.0-t)*qa.t()+t*qb.t(), slerp(qa.r(), qb.r(), t)};
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, septernion& s)
{
  // Read beginning of septernion
  is.readBegin("septernion");
  is  >> s.t() >> s.r();
  // Read end of septernion
  is.readEnd("septernion");
  // Check state of Istream
  is.check("operator>>(Istream&, septernion&)");
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const septernion& s)
{
  os << token::BEGIN_LIST
    << s.t() << token::SPACE << s.r()
    << token::END_LIST;
  return os;
}
