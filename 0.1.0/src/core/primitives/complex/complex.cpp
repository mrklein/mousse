// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "complex.hpp"
#include "iostreams.hpp"
#include <sstream>
// Static Data Members
const char* const mousse::complex::typeName = "complex";
const mousse::complex mousse::complex::zero(0, 0);
const mousse::complex mousse::complex::one(1, 1);
// Constructors 
mousse::complex::complex(Istream& is)
{
  is >> *this;
}
mousse::word mousse::name(const complex& c)
{
  std::ostringstream buf;
  buf << '(' << c.Re() << ',' << c.Im() << ')';
  return buf.str();
}
// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, complex& c)
{
  // Read beginning of complex
  is.readBegin("complex");
  is  >> c.re >> c.im;
  // Read end of complex
  is.readEnd("complex");
  // Check state of Istream
  is.check("operator>>(Istream&, complex&)");
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const complex& c)
{
  os  << token::BEGIN_LIST
    << c.re << token::SPACE << c.im
    << token::END_LIST;
  return os;
}
