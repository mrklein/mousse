// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermo.hpp"
#include "iostreams.hpp"


template<class Thermo, template<class> class Type>
const mousse::scalar mousse::species::thermo<Thermo, Type>::tol_ = 1.0e-4;

template<class Thermo, template<class> class Type>
const int mousse::species::thermo<Thermo, Type>::maxIter_ = 100;


// Constructors 
template<class Thermo, template<class> class Type>
mousse::species::thermo<Thermo, Type>::thermo(Istream& is)
:
  Thermo{is}
{
  is.check("thermo<Thermo, Type>::thermo(Istream&)");
}


template<class Thermo, template<class> class Type>
mousse::species::thermo<Thermo, Type>::thermo(const dictionary& dict)
:
  Thermo{dict}
{}


// Member Functions 
template<class Thermo, template<class> class Type>
void mousse::species::thermo<Thermo, Type>::write(Ostream& os) const
{
  Thermo::write(os);
}


// Ostream Operator 
template<class Thermo, template<class> class Type>
mousse::Ostream& mousse::species::operator<<
(
  Ostream& os, const thermo<Thermo, Type>& st
)
{
  os << static_cast<const Thermo&>(st);
  os.check("Ostream& operator<<(Ostream&, const thermo&)");
  return os;
}

