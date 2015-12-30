// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SRIFallOffFunction
// Description
//   The SRI fall-off function
// SourceFiles
//   sri_fall_off_function_i.hpp
#ifndef sri_fall_off_function_hpp_
#define sri_fall_off_function_hpp_
#include "scalar.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class SRIFallOffFunction;
Ostream& operator<<(Ostream&, const SRIFallOffFunction&);
class SRIFallOffFunction
{
  // Private data
    scalar a_, b_, c_, d_, e_;
public:
  // Constructors
    //- Construct from components
    inline SRIFallOffFunction
    (
      const scalar a,
      const scalar b,
      const scalar c,
      const scalar d,
      const scalar e
    );
    //- Construct from Istream
    inline SRIFallOffFunction(Istream&);
    //- Construct from dictionary
    inline SRIFallOffFunction(const dictionary& dict);
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "SRI";
    }
    inline scalar operator()
    (
      const scalar T,
      const scalar Pr
    ) const;
    //- Write to stream
    inline void write(Ostream& os) const;
  // Ostream Operator
    friend Ostream& operator<<(Ostream&, const SRIFallOffFunction&);
};
}  // namespace mousse
#include "sri_fall_off_function_i.hpp"
#endif
