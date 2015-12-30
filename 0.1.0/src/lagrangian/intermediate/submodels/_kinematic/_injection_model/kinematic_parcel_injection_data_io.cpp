// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "kinematic_parcel_injection_data.hpp"
mousse::kinematicParcelInjectionData::kinematicParcelInjectionData(Istream& is)
{
  is.check("reading (Px Py Pz)");
  is >> x_;
  is.check("reading (Ux Uy Uz)");
  is >> U_;
  is.check("reading d");
  is >> d_;
  is.check("reading rho");
  is >> rho_;
  is.check("reading mDot");
  is >> mDot_;
  is.check("kinematicParcelInjectionData(Istream& is)");
}
// IOstream Operators 
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const kinematicParcelInjectionData& data
)
{
  os << data.x_ << data.U_ << data.d_ << data.rho_ << data.mDot_;
  return os;
}
mousse::Istream& mousse::operator>>(Istream& is, kinematicParcelInjectionData& data)
{
  is.check("reading (Px Py Pz)");
  is >> data.x_;
  is.check("reading (Ux Uy Uz)");
  is >> data.U_;
  is.check("reading d");
  is >> data.d_;
  is.check("reading rho");
  is >> data.rho_;
  is.check("reading mDot");
  is >> data.mDot_;
  is.check("operator(Istream&, kinematicParcelInjectionData&)");
  return is;
}
