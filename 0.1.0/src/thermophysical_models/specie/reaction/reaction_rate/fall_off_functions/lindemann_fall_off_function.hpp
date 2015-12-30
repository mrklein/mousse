// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LindemannFallOffFunction
// Description
//   Lindemann fall-off function
// SourceFiles
//   lindemann_fall_off_function_i.hpp
#ifndef lindemann_fall_off_function_hpp_
#define lindemann_fall_off_function_hpp_
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
#include "lindemann_fall_off_function_i.hpp"
#endif
