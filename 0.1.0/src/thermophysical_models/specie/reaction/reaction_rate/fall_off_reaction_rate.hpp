// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FallOffReactionRate
// Description
//   General class for handling unimolecular/recombination fall-off reactions.
// SourceFiles
//   fall_off_reaction_rate_i.hpp
#ifndef fall_off_reaction_rate_hpp_
#define fall_off_reaction_rate_hpp_
#include "third_body_efficiencies.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class ReactionRate, class FallOffFunction>
class FallOffReactionRate;
template<class ReactionRate, class FallOffFunction>
inline Ostream& operator<<
(
  Ostream&,
  const FallOffReactionRate<ReactionRate, FallOffFunction>&
);
template<class ReactionRate, class FallOffFunction>
class FallOffReactionRate
{
  // Private data
    ReactionRate k0_;
    ReactionRate kInf_;
    FallOffFunction F_;
    thirdBodyEfficiencies thirdBodyEfficiencies_;
public:
  // Constructors
    //- Construct from components
    inline FallOffReactionRate
    (
      const ReactionRate& k0,
      const ReactionRate& kInf,
      const FallOffFunction& F,
      const thirdBodyEfficiencies& tbes
    );
    //- Construct from Istream
    inline FallOffReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline FallOffReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return ReactionRate::type() + FallOffFunction::type() + "FallOff";
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
    friend Ostream& operator<< <ReactionRate, FallOffFunction>
    (
      Ostream&,
      const FallOffReactionRate<ReactionRate, FallOffFunction>&
    );
};
}  // namespace mousse
#include "fall_off_reaction_rate_i.hpp"
#endif
