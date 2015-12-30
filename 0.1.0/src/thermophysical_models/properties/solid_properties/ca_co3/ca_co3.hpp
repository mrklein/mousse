// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CaCO3
// Description
//   Calcium carbonate (limestone)
// SourceFiles
//   ca_co3.cpp
#ifndef solid_ca_co3_hpp_
#define solid_ca_co3_hpp_
#include "solid_properties.hpp"
namespace mousse
{
class CaCO3;
Ostream& operator<<
(
  Ostream&,
  const CaCO3&
);
class CaCO3
:
  public solidProperties
{
public:
  //- Runtime type information
  TypeName("CaCO3");
  // Constructors
    //- Construct null
    CaCO3();
    //- Construct from solidProperties
    CaCO3(const solidProperties& s);
    //- Construct from Istream
    CaCO3(Istream& is);
    //- Construct from dictionary
    CaCO3(const dictionary& dict);
    //- Construct copy
    CaCO3(const CaCO3& s);
    //- Construct and return clone
    virtual autoPtr<solidProperties> clone() const
    {
      return autoPtr<solidProperties>(new CaCO3(*this));
    }
  // I-O
    //- Write the function coefficients
    void writeData(Ostream& os) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream& os, const CaCO3& s);
};
}  // namespace mousse
#endif
