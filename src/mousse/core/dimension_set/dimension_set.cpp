// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dimension_set.hpp"
#include "dimensioned_scalar.hpp"
#include "ostring_stream.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(dimensionSet, 1);
const scalar dimensionSet::smallExponent = SMALL;

}


// Constructors 
mousse::dimensionSet::dimensionSet
(
  const scalar mass,
  const scalar length,
  const scalar time,
  const scalar temperature,
  const scalar moles,
  const scalar current,
  const scalar luminousIntensity
)
{
  exponents_[MASS] = mass;
  exponents_[LENGTH] = length;
  exponents_[TIME] = time;
  exponents_[TEMPERATURE] = temperature;
  exponents_[MOLES] = moles;
  exponents_[CURRENT] = current;
  exponents_[LUMINOUS_INTENSITY] = luminousIntensity;
}


mousse::dimensionSet::dimensionSet
(
  const scalar mass,
  const scalar length,
  const scalar time,
  const scalar temperature,
  const scalar moles
)
{
  exponents_[MASS] = mass;
  exponents_[LENGTH] = length;
  exponents_[TIME] = time;
  exponents_[TEMPERATURE] = temperature;
  exponents_[MOLES] = moles;
  exponents_[CURRENT] = 0;
  exponents_[LUMINOUS_INTENSITY] = 0;
}


mousse::dimensionSet::dimensionSet(const dimensionSet& ds)
{
  reset(ds);
}


// Member Functions 
bool mousse::dimensionSet::dimensionless() const
{
  for (int Dimension=0; Dimension<nDimensions; ++Dimension) {
    // ie, mag(exponents_[Dimension]) > smallExponent
    if (exponents_[Dimension] > smallExponent
        || exponents_[Dimension] < -smallExponent)
    {
      return false;
    }
  }
  return true;
}


void mousse::dimensionSet::reset(const dimensionSet& ds)
{
  for (int Dimension=0; Dimension<nDimensions; ++Dimension) {
    exponents_[Dimension] = ds.exponents_[Dimension];
  }
}


// Member Operators 
mousse::scalar mousse::dimensionSet::operator[](const dimensionType type) const
{
  return exponents_[type];
}


mousse::scalar& mousse::dimensionSet::operator[](const dimensionType type)
{
  return exponents_[type];
}


mousse::scalar mousse::dimensionSet::operator[](const label type) const
{
  return exponents_[type];
}


mousse::scalar& mousse::dimensionSet::operator[](const label type)
{
  return exponents_[type];
}


bool mousse::dimensionSet::operator==(const dimensionSet& ds) const
{
  for (int Dimension=0; Dimension < nDimensions; ++Dimension) {
    if (mag(exponents_[Dimension] - ds.exponents_[Dimension]) > smallExponent) {
      return false;
    }
  }
  return true;
}


bool mousse::dimensionSet::operator!=(const dimensionSet& ds) const
{
  return !(operator==(ds));
}


bool mousse::dimensionSet::operator=(const dimensionSet& ds) const
{
  if (dimensionSet::debug && *this != ds) {
    FATAL_ERROR_IN("dimensionSet::operator=(const dimensionSet&) const")
      << "Different dimensions for =" << endl
      << "     dimensions : " << *this << " = " << ds << endl
      << abort(FatalError);
  }
  return true;
}


bool mousse::dimensionSet::operator+=(const dimensionSet& ds) const
{
  if (dimensionSet::debug && *this != ds) {
    FATAL_ERROR_IN("dimensionSet::operator+=(const dimensionSet&) const")
      << "Different dimensions for +=" << endl
      << "     dimensions : " << *this << " = " << ds << endl
      << abort(FatalError);
  }
  return true;
}


bool mousse::dimensionSet::operator-=(const dimensionSet& ds) const
{
  if (dimensionSet::debug && *this != ds) {
    FATAL_ERROR_IN("dimensionSet::operator-=(const dimensionSet&) const")
      << "Different dimensions for -=" << endl
      << "     dimensions : " << *this << " = " << ds << endl
      << abort(FatalError);
  }
  return true;
}


bool mousse::dimensionSet::operator*=(const dimensionSet& ds)
{
  reset((*this)*ds);
  return true;
}


bool mousse::dimensionSet::operator/=(const dimensionSet& ds)
{
  reset((*this)/ds);
  return true;
}


// Friend functions
mousse::dimensionSet mousse::max(const dimensionSet& ds1, const dimensionSet& ds2)
{
  if (dimensionSet::debug && ds1 != ds2) {
    FATAL_ERROR_IN("max(const dimensionSet&, const dimensionSet&)")
      << "Arguments of max have different dimensions" << endl
      << "     dimensions : " << ds1 << " and " << ds2 << endl
      << abort(FatalError);
  }
  return ds1;
}


mousse::dimensionSet mousse::min(const dimensionSet& ds1, const dimensionSet& ds2)
{
  if (dimensionSet::debug && ds1 != ds2) {
    FATAL_ERROR_IN("min(const dimensionSet&, const dimensionSet&)")
      << "Arguments of min have different dimensions" << endl
      << "     dimensions : " << ds1 << " and " << ds2 << endl
      << abort(FatalError);
  }
  return ds1;
}


mousse::dimensionSet mousse::cmptMultiply
(
  const dimensionSet& ds1,
  const dimensionSet& ds2
)
{
  return ds1*ds2;
}


mousse::dimensionSet mousse::cmptDivide
(
  const dimensionSet& ds1,
  const dimensionSet& ds2
)
{
  return ds1/ds2;
}


mousse::dimensionSet mousse::pow(const dimensionSet& ds, const scalar p)
{
  dimensionSet dimPow
  {
    ds[dimensionSet::MASS]*p,
    ds[dimensionSet::LENGTH]*p,
    ds[dimensionSet::TIME]*p,
    ds[dimensionSet::TEMPERATURE]*p,
    ds[dimensionSet::MOLES]*p,
    ds[dimensionSet::CURRENT]*p,
    ds[dimensionSet::LUMINOUS_INTENSITY]*p
  };
  return dimPow;
}


mousse::dimensionSet mousse::pow
(
  const dimensionSet& ds,
  const dimensionedScalar& dS
)
{
  if (dimensionSet::debug && !dS.dimensions().dimensionless()) {
    FATAL_ERROR_IN("pow(const dimensionSet&, const dimensionedScalar&)")
      << "Exponent of pow is not dimensionless"
      << abort(FatalError);
  }
  dimensionSet dimPow
  {
    ds[dimensionSet::MASS]*dS.value(),
    ds[dimensionSet::LENGTH]*dS.value(),
    ds[dimensionSet::TIME]*dS.value(),
    ds[dimensionSet::TEMPERATURE]*dS.value(),
    ds[dimensionSet::MOLES]*dS.value(),
    ds[dimensionSet::CURRENT]*dS.value(),
    ds[dimensionSet::LUMINOUS_INTENSITY]*dS.value()
  };
  return dimPow;
}


mousse::dimensionSet mousse::pow
(
  const dimensionedScalar& dS,
  const dimensionSet& ds
)
{
  if (dimensionSet::debug
      && !dS.dimensions().dimensionless()
      && !ds.dimensionless()) {
    FATAL_ERROR_IN("pow(const dimensionedScalar&, const dimensionSet&)")
      << "Argument or exponent of pow not dimensionless" << endl
      << abort(FatalError);
  }
  return ds;
}


mousse::dimensionSet mousse::sqr(const dimensionSet& ds)
{
  return pow(ds, 2);
}


mousse::dimensionSet mousse::pow3(const dimensionSet& ds)
{
  return pow(ds, 3);
}


mousse::dimensionSet mousse::pow4(const dimensionSet& ds)
{
  return pow(ds, 4);
}


mousse::dimensionSet mousse::pow5(const dimensionSet& ds)
{
  return pow(ds, 5);
}


mousse::dimensionSet mousse::pow6(const dimensionSet& ds)
{
  return pow(ds, 6);
}


mousse::dimensionSet mousse::pow025(const dimensionSet& ds)
{
  return sqrt(sqrt(ds));
}


mousse::dimensionSet mousse::sqrt(const dimensionSet& ds)
{
  return pow(ds, 0.5);
}


mousse::dimensionSet mousse::cbrt(const dimensionSet& ds)
{
  return pow(ds, 1.0/3.0);
}


mousse::dimensionSet mousse::magSqr(const dimensionSet& ds)
{
  return pow(ds, 2);
}


mousse::dimensionSet mousse::mag(const dimensionSet& ds)
{
  return ds;
}


mousse::dimensionSet mousse::sign(const dimensionSet&)
{
  return dimless;
}


mousse::dimensionSet mousse::pos(const dimensionSet&)
{
  return dimless;
}


mousse::dimensionSet mousse::neg(const dimensionSet&)
{
  return dimless;
}


mousse::dimensionSet mousse::posPart(const dimensionSet& ds)
{
  return ds;
}


mousse::dimensionSet mousse::negPart(const dimensionSet& ds)
{
  return ds;
}


mousse::dimensionSet mousse::inv(const dimensionSet& ds)
{
  return dimless/ds;
}


mousse::dimensionSet mousse::trans(const dimensionSet& ds)
{
  if (dimensionSet::debug && !ds.dimensionless()) {
    FATAL_ERROR_IN("trans(const dimensionSet&)")
      << "Argument of trancendental function not dimensionless"
      << abort(FatalError);
  }
  return ds;
}


mousse::dimensionSet mousse::atan2(const dimensionSet& ds1, const dimensionSet& ds2)
{
  if (dimensionSet::debug && ds1 != ds2) {
    FATAL_ERROR_IN("atan2(const dimensionSet&, const dimensionSet&)")
      << "Arguments of atan2 have different dimensions" << endl
      << "     dimensions : " << ds1 << " and " << ds2 << endl
      << abort(FatalError);
  }
  return dimless;
}


mousse::dimensionSet mousse::transform(const dimensionSet& ds)
{
  return ds;
}


// Friend Operators 
mousse::dimensionSet mousse::operator-(const dimensionSet& ds)
{
  return ds;
}


mousse::dimensionSet mousse::operator+
(
  const dimensionSet& ds1,
  const dimensionSet& ds2
)
{
  dimensionSet dimSum(ds1);
  if (dimensionSet::debug && ds1 != ds2) {
    FATAL_ERROR_IN
      ("operator+(const dimensionSet&, const dimensionSet&)")
      << "LHS and RHS of + have different dimensions" << endl
      << "     dimensions : " << ds1 << " + " << ds2 << endl
      << abort(FatalError);
  }
  return dimSum;
}


mousse::dimensionSet mousse::operator-
(
  const dimensionSet& ds1,
  const dimensionSet& ds2
)
{
  dimensionSet dimDifference(ds1);
  if (dimensionSet::debug && ds1 != ds2) {
    FATAL_ERROR_IN
      ("operator-(const dimensionSet&, const dimensionSet&)")
      << "LHS and RHS of - have different dimensions" << endl
      << "     dimensions : " << ds1 << " - " << ds2 << endl
      << abort(FatalError);
  }
  return dimDifference;
}


mousse::dimensionSet mousse::operator*
(
  const dimensionSet& ds1,
  const dimensionSet& ds2
)
{
  dimensionSet dimProduct(ds1);
  for (int Dimension=0; Dimension<dimensionSet::nDimensions; Dimension++) {
    dimProduct.exponents_[Dimension] += ds2.exponents_[Dimension];
  }
  return dimProduct;
}


mousse::dimensionSet mousse::operator/
(
  const dimensionSet& ds1,
  const dimensionSet& ds2
)
{
  dimensionSet dimQuotient(ds1);
  for (int Dimension=0; Dimension<dimensionSet::nDimensions; Dimension++) {
    dimQuotient.exponents_[Dimension] -= ds2.exponents_[Dimension];
  }
  return dimQuotient;
}


mousse::dimensionSet mousse::operator&
(
  const dimensionSet& ds1,
  const dimensionSet& ds2
)
{
  return ds1*ds2;
}


mousse::dimensionSet mousse::operator^
(
  const dimensionSet& ds1,
  const dimensionSet& ds2
)
{
  return ds1*ds2;
}


mousse::dimensionSet mousse::operator&&
(
  const dimensionSet& ds1,
  const dimensionSet& ds2
)
{
  return ds1*ds2;
}

