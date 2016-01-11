// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::specie
// Description
//   Base class of the thermophysical property types.
// SourceFiles
//   specie_i.hpp
//   specie.cpp
#ifndef specie_hpp_
#define specie_hpp_
#include "word.hpp"
#include "scalar.hpp"
#include "dictionary.hpp"
#include "thermodynamic_constants.hpp"
using namespace mousse::constant::thermodynamic;
namespace mousse
{
class Istream;
class Ostream;
class specie
{
  // Private data
    //- Name of specie
    word name_;
    //- Number of moles of this component in the mixture
    scalar nMoles_;
    //- Molecular weight of specie [kg/kmol]
    scalar molWeight_;
public:
  //- Runtime type information
  CLASS_NAME("specie");
  // Constructors
    //- Construct from components without name
    inline specie(const scalar nMoles, const scalar molWeight);
    //- Construct from components with name
    inline specie
    (
      const word& name,
      const scalar nMoles,
      const scalar molWeight
    );
    //- Construct as copy
    inline specie(const specie&);
    //- Construct as named copy
    inline specie(const word& name, const specie&);
    //- Construct from Istream
    specie(Istream&);
    //- Construct from dictionary
    specie(const dictionary& dict);
  // Member Functions
    // Access
      //- Name
      inline const word& name() const;
      //- Molecular weight [kg/kmol]
      inline scalar W() const;
      //- No of moles of this species in mixture
      inline scalar nMoles() const;
      //- Gas constant [J/(kg K)]
      inline scalar R() const;
    // I-O
      //- Write to Ostream
      void write(Ostream& os) const;
  // Member operators
    inline void operator=(const specie&);
    inline void operator+=(const specie&);
    inline void operator-=(const specie&);
    inline void operator*=(const scalar);
  // Friend operators
    inline friend specie operator+(const specie&, const specie&);
    inline friend specie operator-(const specie&, const specie&);
    inline friend specie operator*(const scalar, const specie&);
    inline friend specie operator==(const specie&, const specie&);
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const specie&);
};
}  // namespace mousse
#include "specie_i.hpp"
#endif
