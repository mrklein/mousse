#ifndef THERMOPHYSICAL_MODELS_PROPERTIES_SOLID_PROPERTIES_C_C_HPP_
#define THERMOPHYSICAL_MODELS_PROPERTIES_SOLID_PROPERTIES_C_C_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::C
// Description
//   Graphite solid properties

#include "solid_properties.hpp"


namespace mousse {

class C;
Ostream& operator<<
(
  Ostream&,
  const C&
);


class C
:
  public solidProperties
{
public:
  //- Runtime type information
  TYPE_NAME("C");
  // Constructors
    //- Construct null
    C();
    //- Construct from solidProperties
    C(const solidProperties& s);
    //- Construct from Istream
    C(Istream& is);
    //- Construct from dictionary
    C(const dictionary& dict);
    //- Construct copy
    C(const C& s);
    //- Construct and return clone
    virtual autoPtr<solidProperties> clone() const
    {
      return autoPtr<solidProperties>{new C{*this}};
    }
  // I-O
    //- Write the function coefficients
    void writeData(Ostream& os) const;
  //- Ostream Operator
  friend Ostream& operator<<(Ostream& os, const C& s);
};

}  // namespace mousse

#endif

