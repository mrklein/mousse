// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ash
// Description
//   Coal ash solid properties
// SourceFiles
//   ash.cpp
#ifndef solid_ash_hpp_
#define solid_ash_hpp_
#include "solid_properties.hpp"
namespace mousse
{
class ash;
Ostream& operator<<
(
  Ostream&,
  const ash&
);
class ash
:
  public solidProperties
{
public:
  //- Runtime type information
  TypeName("ash");
  // Constructors
    //- Construct null
    ash();
    //- Construct from solidProperties
    ash(const solidProperties& s);
    //- Construct from Istream
    ash(Istream& is);
    //- Construct from dictionary
    ash(const dictionary& dict);
    //- Construct copy
    ash(const ash& s);
    //- Construct and return clone
    virtual autoPtr<solidProperties> clone() const
    {
      return autoPtr<solidProperties>(new ash(*this));
    }
  // I-O
    //- Write the function coefficients
    void writeData(Ostream& os) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream& os, const ash& s);
};
}  // namespace mousse
#endif
