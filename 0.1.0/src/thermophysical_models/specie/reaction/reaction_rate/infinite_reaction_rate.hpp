// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::infiniteReactionRate
// Description
//   infinite reaction rate.
// SourceFiles
//   infinite_reaction_rate_i.hpp
#ifndef infinite_reaction_rate_hpp_
#define infinite_reaction_rate_hpp_
#include "scalar_field.hpp"
#include "type_info.hpp"
namespace mousse
{
class infiniteReactionRate
{
public:
  // Constructors
    //- Null constructor
    inline infiniteReactionRate
    ();
    //- Construct from Istream
    inline infiniteReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "infinite";
    }
    inline scalar operator()
    (
      const scalar p,
      const scalar T,
      const scalarField& c
    ) const;
    //- Write to stream
    inline void write(Ostream& os) const;
  // Ostream Operator
    inline friend Ostream& operator<<
    (
      Ostream&,
      const infiniteReactionRate&
    );
};
}  // namespace mousse
#include "infinite_reaction_rate_i.hpp"
#endif
