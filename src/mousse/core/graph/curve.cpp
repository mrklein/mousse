// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "curve.hpp"
#include "ostream.hpp"


namespace mousse {

// Member Functions 

// construct given name, style and size
curve::curve
(
  const string& name,
  const curveStyle& style,
  const label l
)
:
  scalarField{l, 0.0},
  name_{name},
  style_{style}
{}


// construct from the bits
curve::curve
(
  const string& name,
  const curveStyle& style,
  const scalarField& y
)
:
  scalarField{y},
  name_{name},
  style_{style}
{}


// IOstream Operators 
Ostream& operator<<(Ostream& os, const curve& c)
{
  os << nl
    << c.name_ << nl
    << c.style_ << nl
    << static_cast<const scalarField&>(c);
  os.check("Ostream& operator>>(Ostream&, const curve&)");
  return os;
}

}  // namespace mousse
