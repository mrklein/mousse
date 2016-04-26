#ifndef THERMOPHYSICAL_MODELS_SPECIE_SPECIE_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_SPECIE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::specie
// Description
//   Base class of the thermophysical property types.

#include "word.hpp"
#include "scalar.hpp"
#include "dictionary.hpp"
#include "thermodynamic_constants.hpp"


using namespace mousse::constant::thermodynamic;

namespace mousse {

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


namespace mousse {

// Private Member Functions 
inline specie::specie
(
  const word& name,
  const scalar nMoles,
  const scalar molWeight
)
:
  name_{name},
  nMoles_{nMoles},
  molWeight_{molWeight}
{}


inline specie::specie
(
  const scalar nMoles,
  const scalar molWeight
)
:
  nMoles_{nMoles},
  molWeight_{molWeight}
{}


// Constructors 
inline specie::specie(const specie& st)
:
  name_{st.name_},
  nMoles_{st.nMoles_},
  molWeight_{st.molWeight_}
{}


inline specie::specie(const word& name, const specie& st)
:
  name_{name},
  nMoles_{st.nMoles_},
  molWeight_{st.molWeight_}
{}


// Member Functions 
inline const word& specie::name() const
{
  return name_;
}


inline scalar specie::W() const
{
  return molWeight_;
}


inline scalar specie::nMoles() const
{
  return nMoles_;
}


inline scalar specie::R() const
{
  return RR/molWeight_;
}


// Member Operators 
inline void specie::operator=(const specie& st)
{
  //name_ = st.name_;
  nMoles_ = st.nMoles_;
  molWeight_ = st.molWeight_;
}


inline void specie::operator+=(const specie& st)
{
  scalar sumNmoles = max(nMoles_ + st.nMoles_, SMALL);
  molWeight_ = nMoles_/sumNmoles*molWeight_
    + st.nMoles_/sumNmoles*st.molWeight_;
  nMoles_ = sumNmoles;
}


inline void specie::operator-=(const specie& st)
{
  scalar diffnMoles = nMoles_ - st.nMoles_;
  if (mag(diffnMoles) < SMALL) {
    diffnMoles = SMALL;
  }
  molWeight_ = nMoles_/diffnMoles*molWeight_
    - st.nMoles_/diffnMoles*st.molWeight_;
  nMoles_ = diffnMoles;
}


inline void specie::operator*=(const scalar s)
{
  nMoles_ *= s;
}


// Friend Operators 
inline specie operator+(const specie& st1, const specie& st2)
{
  scalar sumNmoles = max(st1.nMoles_ + st2.nMoles_, SMALL);
  return {sumNmoles,
          st1.nMoles_/sumNmoles*st1.molWeight_
            + st2.nMoles_/sumNmoles*st2.molWeight_};
}


inline specie operator-(const specie& st1, const specie& st2)
{
  scalar diffNmoles = st1.nMoles_ - st2.nMoles_;
  if (mag(diffNmoles) < SMALL) {
    diffNmoles = SMALL;
  }
  return {diffNmoles,
          st1.nMoles_/diffNmoles*st1.molWeight_
            - st2.nMoles_/diffNmoles*st2.molWeight_};
}


inline specie operator*(const scalar s, const specie& st)
{
  return {s*st.nMoles_,
          st.molWeight_};
}


inline specie operator==(const specie& st1, const specie& st2)
{
  return st2 - st1;
}

}  // namespace mousse

#endif

