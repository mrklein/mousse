// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "quaternion.hpp"
#include "iostreams.hpp"
#include "ostring_stream.hpp"
// Static Data Members
const char* const mousse::quaternion::typeName = "quaternion";
const mousse::quaternion mousse::quaternion::zero(0, vector(0, 0, 0));
const mousse::quaternion mousse::quaternion::I(1, vector(0, 0, 0));
// Constructors 
mousse::quaternion::quaternion(Istream& is)
{
  is >> *this;
}
mousse::word mousse::name(const quaternion& q)
{
  OStringStream buf;
  buf << '(' << q.w() << ',' << q.v() << ')';
  return buf.str();
}
mousse::quaternion mousse::slerp
(
  const quaternion& qa,
  const quaternion& qb,
  const scalar t
)
{
  label sign = 1;
  if ((qa & qb) < 0)
  {
    sign = -1;
  }
  return qa*pow((inv(qa)*sign*qb), t);
}
mousse::quaternion mousse::exp(const quaternion& q)
{
  const scalar magV = mag(q.v());
  if (magV == 0)
  {
    return quaternion(1, vector::zero);
  }
  const scalar expW = exp(q.w());
  return quaternion
  (
    expW*cos(magV),
    expW*sin(magV)*q.v()/magV
  );
}
mousse::quaternion mousse::pow(const quaternion& q, const label power)
{
  const scalar magQ = mag(q);
  const scalar magV = mag(q.v());
  quaternion powq(q.v());
  if (magV != 0 && magQ != 0)
  {
    powq /= magV;
    powq *= power*acos(q.w()/magQ);
  }
  return pow(magQ, power)*exp(powq);
}
mousse::quaternion mousse::pow(const quaternion& q, const scalar power)
{
  const scalar magQ = mag(q);
  const scalar magV = mag(q.v());
  quaternion powq(q.v());
  if (magV != 0 && magQ != 0)
  {
    powq /= magV;
    powq *= power*acos(q.w()/magQ);
  }
  return pow(magQ, power)*exp(powq);
}
// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, quaternion& q)
{
  // Read beginning of quaternion
  is.readBegin("quaternion");
  is  >> q.w() >> q.v();
  // Read end of quaternion
  is.readEnd("quaternion");
  // Check state of Istream
  is.check("operator>>(Istream&, quaternion&)");
  return is;
}
mousse::Ostream& mousse::operator<<(Ostream& os, const quaternion& q)
{
  os  << token::BEGIN_LIST
    << q.w() << token::SPACE << q.v()
    << token::END_LIST;
  return os;
}
