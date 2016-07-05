// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "liquid_properties.hpp"
#include "hash_table.hpp"
#include "switch.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(liquidProperties, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(liquidProperties,);
DEFINE_RUN_TIME_SELECTION_TABLE(liquidProperties, Istream);
DEFINE_RUN_TIME_SELECTION_TABLE(liquidProperties, dictionary);

}


// Constructors 
mousse::liquidProperties::liquidProperties
(
  scalar W,
  scalar Tc,
  scalar Pc,
  scalar Vc,
  scalar Zc,
  scalar Tt,
  scalar Pt,
  scalar Tb,
  scalar dipm,
  scalar omega,
  scalar delta
)
:
  W_{W},
  Tc_{Tc},
  Pc_{Pc},
  Vc_{Vc},
  Zc_{Zc},
  Tt_{Tt},
  Pt_{Pt},
  Tb_{Tb},
  dipm_{dipm},
  omega_{omega},
  delta_{delta}
{}


mousse::liquidProperties::liquidProperties(Istream& is)
:
  W_{readScalar(is)},
  Tc_{readScalar(is)},
  Pc_{readScalar(is)},
  Vc_{readScalar(is)},
  Zc_{readScalar(is)},
  Tt_{readScalar(is)},
  Pt_{readScalar(is)},
  Tb_{readScalar(is)},
  dipm_{readScalar(is)},
  omega_{readScalar(is)},
  delta_{readScalar(is)}
{}


mousse::liquidProperties::liquidProperties(const dictionary& dict)
:
  W_{readScalar(dict.lookup("W"))},
  Tc_{readScalar(dict.lookup("Tc"))},
  Pc_{readScalar(dict.lookup("Pc"))},
  Vc_{readScalar(dict.lookup("Vc"))},
  Zc_{readScalar(dict.lookup("Zc"))},
  Tt_{readScalar(dict.lookup("Tt"))},
  Pt_{readScalar(dict.lookup("Pt"))},
  Tb_{readScalar(dict.lookup("Tb"))},
  dipm_{readScalar(dict.lookup("dipm"))},
  omega_{readScalar(dict.lookup("omega"))},
  delta_{readScalar(dict.lookup("delta"))}
{}


mousse::liquidProperties::liquidProperties(const liquidProperties& liq)
:
  W_{liq.W_},
  Tc_{liq.Tc_},
  Pc_{liq.Pc_},
  Vc_{liq.Vc_},
  Zc_{liq.Zc_},
  Tt_{liq.Tt_},
  Pt_{liq.Pt_},
  Tb_{liq.Tb_},
  dipm_{liq.dipm_},
  omega_{liq.omega_},
  delta_{liq.delta_}
{}

// Selectors
mousse::autoPtr<mousse::liquidProperties> mousse::liquidProperties::New(Istream& is)
{
  if (debug) {
    Info << "liquidProperties::New(Istream&): "
      << "constructing liquidProperties" << endl;
  }
  const word liquidPropertiesType{is};
  const word coeffs{is};
  if (coeffs == "defaultCoeffs") {
    ConstructorTable::iterator cstrIter =
      ConstructorTablePtr_->find(liquidPropertiesType);
    if (cstrIter == ConstructorTablePtr_->end()) {
      FATAL_ERROR_IN("liquidProperties::New(Istream&)")
        << "Unknown liquidProperties type "
        << liquidPropertiesType << nl << nl
        << "Valid liquidProperties types are:" << nl
        << ConstructorTablePtr_->sortedToc()
        << abort(FatalError);
    }
    return autoPtr<liquidProperties>{cstrIter()()};
  } else if (coeffs == "coeffs") {
    return autoPtr<liquidProperties>{new liquidProperties{is}};
  } else {
    FATAL_ERROR_IN("liquidProperties::New(Istream&)")
      << "liquidProperties type " << liquidPropertiesType
      << ", option " << coeffs << " given"
      << ", should be coeffs or defaultCoeffs"
      << abort(FatalError);
    return autoPtr<liquidProperties>{NULL};
  }
}


mousse::autoPtr<mousse::liquidProperties> mousse::liquidProperties::New
(
  const dictionary& dict
)
{
  if (debug) {
    Info << "liquidProperties::New(const dictionary&):"
      << "constructing liquidProperties" << endl;
  }
  const word& liquidPropertiesTypeName = dict.dictName();
  const Switch defaultCoeffs{dict.lookup("defaultCoeffs")};
  if (defaultCoeffs) {
    ConstructorTable::iterator cstrIter =
      ConstructorTablePtr_->find(liquidPropertiesTypeName);
    if (cstrIter == ConstructorTablePtr_->end()) {
      FATAL_ERROR_IN
      (
        "liquidProperties::New(const dictionary&)"
      )
      << "Unknown liquidProperties type "
      << liquidPropertiesTypeName << nl << nl
      << "Valid liquidProperties types are:" << nl
      << ConstructorTablePtr_->sortedToc()
      << abort(FatalError);
    }
    return autoPtr<liquidProperties>{cstrIter()()};
  } else {
    dictionaryConstructorTable::iterator cstrIter =
      dictionaryConstructorTablePtr_->find(liquidPropertiesTypeName);
    if (cstrIter == dictionaryConstructorTablePtr_->end()) {
      FATAL_ERROR_IN
      (
        "liquidProperties::New(const dictionary&)"
      )
      << "Unknown liquidProperties type "
      << liquidPropertiesTypeName << nl << nl
      << "Valid liquidProperties types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << abort(FatalError);
    }
    return
      autoPtr<liquidProperties>
      {
        cstrIter()(dict.subDict(liquidPropertiesTypeName + "Coeffs"))
      };
  }
}


// Member Functions 
mousse::scalar mousse::liquidProperties::rho(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::rho(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::pv(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::pv(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::hl(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::hl(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::Cp(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::Cp(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::h(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::h(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::Cpg(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::Cpg(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::mu(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::mu(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::mug(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::mug(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::K(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::K(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::Kg(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::Kg(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::sigma(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::sigms(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::D(scalar /*p*/, scalar /*T*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::D(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::D(scalar /*p*/, scalar /*T*/, scalar /*Wb*/) const
{
  NOT_IMPLEMENTED
  (
    "mousse::scalar mousse::liquidProperties::D(scalar, scalar) const"
  );
  return 0.0;
}


mousse::scalar mousse::liquidProperties::pvInvert(scalar p) const
{
  // Check for critical and solid phase conditions
  if (p >= Pc_) {
    return Tc_;
  } else if (p < Pt_) {
    if (debug) {
      WARNING_IN
      (
        "mousse::scalar mousse::liquidProperties::pvInvert(scalar) const"
      )
      << "Pressure below triple point pressure: "
      << "p = " << p << " < Pt = " << Pt_ <<  nl << endl;
    }
    return -1;
  }
  // Set initial upper and lower bounds
  scalar Thi = Tc_;
  scalar Tlo = Tt_;
  // Initialise T as boiling temperature under normal conditions
  scalar T = Tb_;
  while ((Thi - Tlo) > 1.0e-4) {
    if ((pv(p, T) - p) <= 0.0) {
      Tlo = T;
    } else {
      Thi = T;
    }
    T = (Thi + Tlo)*0.5;
  }
  return T;
}


void mousse::liquidProperties::writeData(Ostream& os) const
{
  os << W_ << token::SPACE
    << Tc_ << token::SPACE
    << Pc_ << token::SPACE
    << Vc_ << token::SPACE
    << Zc_ << token::SPACE
    << Tt_ << token::SPACE
    << Pt_ << token::SPACE
    << Tb_ << token::SPACE
    << dipm_ << token::SPACE
    << omega_<< token::SPACE
    << delta_;
}

