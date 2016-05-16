// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "h_polynomial_thermo.hpp"
#include "iostreams.hpp"


// Constructors 
template<class EquationOfState, int PolySize>
mousse::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
  Istream& is
)
:
  EquationOfState{is},
  Hf_{readScalar(is)},
  Sf_{readScalar(is)},
  CpCoeffs_{"CpCoeffs<" + mousse::name(PolySize) + '>', is},
  hCoeffs_{},
  sCoeffs_{}
{
  Hf_ *= this->W();
  Sf_ *= this->W();
  CpCoeffs_ *= this->W();
  hCoeffs_ = CpCoeffs_.integral();
  sCoeffs_ = CpCoeffs_.integralMinus1();
  // Offset h poly so that it is relative to the enthalpy at Tstd
  hCoeffs_[0] += Hf_ - hCoeffs_.value(Tstd);
  // Offset s poly so that it is relative to the entropy at Tstd
  sCoeffs_[0] += Sf_ - sCoeffs_.value(Tstd);
}


template<class EquationOfState, int PolySize>
mousse::hPolynomialThermo<EquationOfState, PolySize>::hPolynomialThermo
(
  const dictionary& dict
)
:
  EquationOfState{dict},
  Hf_{readScalar(dict.subDict("thermodynamics").lookup("Hf"))},
  Sf_{readScalar(dict.subDict("thermodynamics").lookup("Sf"))},
  CpCoeffs_
  {
    dict.subDict("thermodynamics").lookup
    (
      "CpCoeffs<" + mousse::name(PolySize) + '>'
    )
  },
  hCoeffs_{},
  sCoeffs_{}
{
  Hf_ *= this->W();
  Sf_ *= this->W();
  CpCoeffs_ *= this->W();
  hCoeffs_ = CpCoeffs_.integral();
  sCoeffs_ = CpCoeffs_.integralMinus1();
  // Offset h poly so that it is relative to the enthalpy at Tstd
  hCoeffs_[0] += Hf_ - hCoeffs_.value(Tstd);
  // Offset s poly so that it is relative to the entropy at Tstd
  sCoeffs_[0] += Sf_ - sCoeffs_.value(Tstd);
}


// Member Functions 
template<class EquationOfState, int PolySize>
void mousse::hPolynomialThermo<EquationOfState, PolySize>::write
(
  Ostream& os
) const
{
  EquationOfState::write(os);
  dictionary dict("thermodynamics");
  dict.add("Hf", Hf_/this->W());
  dict.add("Sf", Sf_/this->W());
  dict.add
  (
    word("CpCoeffs<" + mousse::name(PolySize) + '>'),
    CpCoeffs_/this->W()
  );
  os << indent << dict.dictName() << dict;
}


// Ostream Operator 
template<class EquationOfState, int PolySize>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const hPolynomialThermo<EquationOfState, PolySize>& pt
)
{
  os << static_cast<const EquationOfState&>(pt) << tab
    << pt.Hf_/pt.W() << tab
    << pt.Sf_/pt.W() << tab
    << "CpCoeffs<" << mousse::name(PolySize) << '>' << tab
    << pt.CpCoeffs_/pt.W();
  os.check
  (
    "operator<<"
    "("
    "  Ostream&, "
    "  const hPolynomialThermo<EquationOfState, PolySize>&"
    ")"
  );
  return os;
}

