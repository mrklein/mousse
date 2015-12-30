// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "specie.hpp"
namespace mousse
{
// Private Member Functions 
inline specie::specie
(
  const word& name,
  const scalar nMoles,
  const scalar molWeight
)
:
  name_(name),
  nMoles_(nMoles),
  molWeight_(molWeight)
{}
inline specie::specie
(
  const scalar nMoles,
  const scalar molWeight
)
:
  nMoles_(nMoles),
  molWeight_(molWeight)
{}
// Constructors 
inline specie::specie(const specie& st)
:
  name_(st.name_),
  nMoles_(st.nMoles_),
  molWeight_(st.molWeight_)
{}
inline specie::specie(const word& name, const specie& st)
:
  name_(name),
  nMoles_(st.nMoles_),
  molWeight_(st.molWeight_)
{}
// Member Functions 
inline const word& specie::name() const
{
  return name_;
}
inline scalar specie::W() const
{
  return molWeight_;
}
inline scalar specie::nMoles() const
{
  return nMoles_;
}
inline scalar specie::R() const
{
  return RR/molWeight_;
}
// Member Operators 
inline void specie::operator=(const specie& st)
{
  //name_ = st.name_;
  nMoles_ = st.nMoles_;
  molWeight_ = st.molWeight_;
}
inline void specie::operator+=(const specie& st)
{
  scalar sumNmoles = max(nMoles_ + st.nMoles_, SMALL);
  molWeight_ =
    nMoles_/sumNmoles*molWeight_
   + st.nMoles_/sumNmoles*st.molWeight_;
  nMoles_ = sumNmoles;
}
inline void specie::operator-=(const specie& st)
{
  scalar diffnMoles = nMoles_ - st.nMoles_;
  if (mag(diffnMoles) < SMALL)
  {
    diffnMoles = SMALL;
  }
  molWeight_ =
    nMoles_/diffnMoles*molWeight_
   - st.nMoles_/diffnMoles*st.molWeight_;
  nMoles_ = diffnMoles;
}
inline void specie::operator*=(const scalar s)
{
  nMoles_ *= s;
}
// Friend Operators 
inline specie operator+(const specie& st1, const specie& st2)
{
  scalar sumNmoles = max(st1.nMoles_ + st2.nMoles_, SMALL);
  return specie
  (
    sumNmoles,
    st1.nMoles_/sumNmoles*st1.molWeight_
   + st2.nMoles_/sumNmoles*st2.molWeight_
  );
}
inline specie operator-(const specie& st1, const specie& st2)
{
  scalar diffNmoles = st1.nMoles_ - st2.nMoles_;
  if (mag(diffNmoles) < SMALL)
  {
    diffNmoles = SMALL;
  }
  return specie
  (
    diffNmoles,
    st1.nMoles_/diffNmoles*st1.molWeight_
   - st2.nMoles_/diffNmoles*st2.molWeight_
  );
}
inline specie operator*(const scalar s, const specie& st)
{
  return specie
  (
    s*st.nMoles_,
    st.molWeight_
  );
}
inline specie operator==(const specie& st1, const specie& st2)
{
  return st2 - st1;
}
}  // namespace mousse
