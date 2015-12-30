// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TroeFallOffFunction
// Description
//   The Troe fall-off function
// SourceFiles
//   troe_fall_off_function_i.hpp
#ifndef troe_fall_off_function_hpp_
#define troe_fall_off_function_hpp_
#include "scalar.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class TroeFallOffFunction;
Ostream& operator<<(Ostream&, const TroeFallOffFunction&);
class TroeFallOffFunction
{
  // Private data
    scalar alpha_;
    scalar Tsss_, Ts_, Tss_;
public:
  // Constructors
    //- Construct from components
    inline TroeFallOffFunction
    (
      const scalar alpha,
      const scalar Tsss,
      const scalar Ts,
      const scalar Tss
    );
    //- Construct from Istream
    inline TroeFallOffFunction(Istream&);
    //- Construct from dictionary
    inline TroeFallOffFunction(const dictionary& dict);
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "Troe";
    }
    inline scalar operator()
    (
      const scalar T,
      const scalar Pr
    ) const;
    //- Write to stream
    inline void write(Ostream& os) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const TroeFallOffFunction&);
};
}  // namespace mousse
#include "troe_fall_off_function_i.hpp"
#endif
