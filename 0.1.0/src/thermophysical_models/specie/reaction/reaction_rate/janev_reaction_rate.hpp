// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::JanevReactionRate
// Description
//   Janev, Langer, Evans and Post reaction rate.
// SourceFiles
//   janev_reaction_rate_i.hpp
#ifndef janev_reaction_rate_hpp_
#define janev_reaction_rate_hpp_
#include "scalar_field.hpp"
#include "type_info.hpp"
#include "fixed_list.hpp"
namespace mousse
{
class JanevReactionRate
{
  // Private data
    scalar A_;
    scalar beta_;
    scalar Ta_;
    static const label nb_ = 9;
    FixedList<scalar, nb_> b_;
public:
  // Constructors
    //- Construct from components
    inline JanevReactionRate
    (
      const scalar A,
      const scalar beta,
      const scalar Ta,
      const FixedList<scalar, nb_> b
    );
    //- Construct from Istream
    inline JanevReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline JanevReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return "Janev";
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
      const JanevReactionRate&
    );
};
}  // namespace mousse
#include "janev_reaction_rate_i.hpp"
#endif
