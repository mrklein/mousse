// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "janaf_thermo.hpp"
#include "iostreams.hpp"
// Private Member Functions 
template<class EquationOfState>
void mousse::janafThermo<EquationOfState>::checkInputData() const
{
  if (Tlow_ >= Thigh_)
  {
    FATAL_ERROR_IN("janafThermo<EquationOfState>::check()")
      << "Tlow(" << Tlow_ << ") >= Thigh(" << Thigh_ << ')'
      << exit(FatalError);
  }
  if (Tcommon_ <= Tlow_)
  {
    FATAL_ERROR_IN("janafThermo<EquationOfState>::check()")
      << "Tcommon(" << Tcommon_ << ") <= Tlow(" << Tlow_ << ')'
      << exit(FatalError);
  }
  if (Tcommon_ > Thigh_)
  {
    FATAL_ERROR_IN("janafThermo<EquationOfState>::check()")
      << "Tcommon(" << Tcommon_ << ") > Thigh(" << Thigh_ << ')'
      << exit(FatalError);
  }
}
// Constructors 
template<class EquationOfState>
mousse::janafThermo<EquationOfState>::janafThermo(Istream& is)
:
  EquationOfState(is),
  Tlow_(readScalar(is)),
  Thigh_(readScalar(is)),
  Tcommon_(readScalar(is))
{
  checkInputData();
  FOR_ALL(highCpCoeffs_, i)
  {
    is >> highCpCoeffs_[i];
  }
  FOR_ALL(lowCpCoeffs_, i)
  {
    is >> lowCpCoeffs_[i];
  }
  // Check state of Istream
  is.check("janafThermo::janafThermo(Istream& is)");
}
template<class EquationOfState>
mousse::janafThermo<EquationOfState>::janafThermo(const dictionary& dict)
:
  EquationOfState(dict),
  Tlow_(readScalar(dict.subDict("thermodynamics").lookup("Tlow"))),
  Thigh_(readScalar(dict.subDict("thermodynamics").lookup("Thigh"))),
  Tcommon_(readScalar(dict.subDict("thermodynamics").lookup("Tcommon"))),
  highCpCoeffs_(dict.subDict("thermodynamics").lookup("highCpCoeffs")),
  lowCpCoeffs_(dict.subDict("thermodynamics").lookup("lowCpCoeffs"))
{
  checkInputData();
}
// Member Functions 
template<class EquationOfState>
void mousse::janafThermo<EquationOfState>::write(Ostream& os) const
{
  EquationOfState::write(os);
  dictionary dict("thermodynamics");
  dict.add("Tlow", Tlow_);
  dict.add("Thigh", Thigh_);
  dict.add("Tcommon", Tcommon_);
  dict.add("highCpCoeffs", highCpCoeffs_);
  dict.add("lowCpCoeffs", lowCpCoeffs_);
  os  << indent << dict.dictName() << dict;
}
// Ostream Operator 
template<class EquationOfState>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const janafThermo<EquationOfState>& jt
)
{
  os  << static_cast<const EquationOfState&>(jt) << nl
    << "    " << jt.Tlow_
    << tab << jt.Thigh_
    << tab << jt.Tcommon_;
  os << nl << "    ";
  FOR_ALL(jt.highCpCoeffs_, i)
  {
    os << jt.highCpCoeffs_[i] << ' ';
  }
  os << nl << "    ";
  FOR_ALL(jt.lowCpCoeffs_, i)
  {
    os << jt.lowCpCoeffs_[i] << ' ';
  }
  os << endl;
  os.check
  (
    "operator<<(Ostream& os, const janafThermo<EquationOfState>& jt)"
  );
  return os;
}
