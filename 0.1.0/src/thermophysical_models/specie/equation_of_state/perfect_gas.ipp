// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "perfect_gas.hpp"
#include "iostreams.hpp"


// Constructors 
template<class Specie>
mousse::perfectGas<Specie>::perfectGas(Istream& is)
:
  Specie{is}
{
  is.check("perfectGas<Specie>::perfectGas(Istream& is)");
}


template<class Specie>
mousse::perfectGas<Specie>::perfectGas(const dictionary& dict)
:
  Specie{dict}
{}


// Member Functions 
template<class Specie>
void mousse::perfectGas<Specie>::write(Ostream& os) const
{
  Specie::write(os);
}


// Ostream Operator 
template<class Specie>
mousse::Ostream& mousse::operator<<(Ostream& os, const perfectGas<Specie>& pg)
{
  os << static_cast<const Specie&>(pg);
  os.check("Ostream& operator<<(Ostream& os, const perfectGas<Specie>& st)");
  return os;
}

