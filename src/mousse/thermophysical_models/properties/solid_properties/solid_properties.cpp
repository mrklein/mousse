// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_properties.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(solidProperties, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(solidProperties,);
DEFINE_RUN_TIME_SELECTION_TABLE(solidProperties, Istream);
DEFINE_RUN_TIME_SELECTION_TABLE(solidProperties, dictionary);

}


// Constructors 
mousse::solidProperties::solidProperties
(
  scalar rho,
  scalar Cp,
  scalar K,
  scalar Hf,
  scalar emissivity
)
:
  rho_{rho},
  Cp_{Cp},
  K_{K},
  Hf_{Hf},
  emissivity_{emissivity}
{}


mousse::solidProperties::solidProperties(Istream& is)
:
  rho_{readScalar(is)},
  Cp_{readScalar(is)},
  K_{readScalar(is)},
  Hf_{readScalar(is)},
  emissivity_{readScalar(is)}
{}


mousse::solidProperties::solidProperties(const dictionary& dict)
:
  rho_{readScalar(dict.lookup("rho"))},
  Cp_{readScalar(dict.lookup("Cp"))},
  K_{readScalar(dict.lookup("K"))},
  Hf_{readScalar(dict.lookup("Hf"))},
  emissivity_{readScalar(dict.lookup("emissivity"))}
{}


mousse::solidProperties::solidProperties(const solidProperties& s)
:
  rho_{s.rho_},
  Cp_{s.Cp_},
  K_{s.K_},
  Hf_{s.Hf_},
  emissivity_{s.emissivity_}
{}


// Member Functions 
void mousse::solidProperties::writeData(Ostream& os) const
{
  os << rho_ << token::SPACE
    << Cp_ << token::SPACE
    << K_ << token::SPACE
    << Hf_ << token::SPACE
    << emissivity_;
}


// IOStream operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const solidProperties& s)
{
  s.writeData(os);
  return os;
}

