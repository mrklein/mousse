#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_FALL_OFF_FUNCTIONS_LINDEMANN_FALL_OFF_FUNCTION_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTION_RATE_FALL_OFF_FUNCTIONS_LINDEMANN_FALL_OFF_FUNCTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LindemannFallOffFunction
// Description
//   Lindemann fall-off function
#include "scalar.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class LindemannFallOffFunction;
Ostream& operator<<(Ostream&, const LindemannFallOffFunction&);
class LindemannFallOffFunction
{
public:
  // Constructors
    //- Construct null
    inline LindemannFallOffFunction();
    //- Construct from Istream
    inline LindemannFallOffFunction(Istream&);
    //- Construct from dictionary
    inline LindemannFallOffFunction(const dictionary& dict);
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "Lindemann";
    }
    inline scalar operator()
    (
      const scalar T,
      const scalar Pr
    ) const;
    //- Write to stream
    inline void write(Ostream& os) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const LindemannFallOffFunction&);
};
}  // namespace mousse

// Constructors 
inline mousse::LindemannFallOffFunction::LindemannFallOffFunction()
{}
inline mousse::LindemannFallOffFunction::LindemannFallOffFunction(Istream&)
{}
inline mousse::LindemannFallOffFunction::LindemannFallOffFunction
(
  const dictionary&
)
{}
// Member Functions 
inline mousse::scalar mousse::LindemannFallOffFunction::operator()
(
  const scalar,
  const scalar
) const
{
  return 1.0;
}
inline void mousse::LindemannFallOffFunction::write(Ostream&) const
{}
// Ostream Operator 
inline mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::LindemannFallOffFunction&
)
{
  return os;
}
#endif
