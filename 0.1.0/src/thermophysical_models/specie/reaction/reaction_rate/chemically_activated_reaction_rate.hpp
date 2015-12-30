// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ChemicallyActivatedReactionRate
// Description
//   General class for handling chemically-activated bimolecular reactions.
// SourceFiles
//   chemically_activated_reaction_rate_i.hpp
#ifndef chemically_activated_reaction_rate_hpp_
#define chemically_activated_reaction_rate_hpp_
#include "third_body_efficiencies.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class ReactionRate, class ChemicallyActivationFunction>
class ChemicallyActivatedReactionRate;
template<class ReactionRate, class ChemicallyActivationFunction>
inline Ostream& operator<<
(
  Ostream&,
  const ChemicallyActivatedReactionRate
    <ReactionRate, ChemicallyActivationFunction>&
);
template<class ReactionRate, class ChemicallyActivationFunction>
class ChemicallyActivatedReactionRate
{
  // Private data
    ReactionRate k0_;
    ReactionRate kInf_;
    ChemicallyActivationFunction F_;
    thirdBodyEfficiencies thirdBodyEfficiencies_;
public:
  // Constructors
    //- Construct from components
    inline ChemicallyActivatedReactionRate
    (
      const ReactionRate& k0,
      const ReactionRate& kInf,
      const ChemicallyActivationFunction& F,
      const thirdBodyEfficiencies& tbes
    );
    //- Construct from Istream
    inline ChemicallyActivatedReactionRate
    (
      const speciesTable& species,
      Istream& is
    );
    //- Construct from dictionary
    inline ChemicallyActivatedReactionRate
    (
      const speciesTable& species,
      const dictionary& dict
    );
  // Member Functions
    //- Return the type name
    static word type()
    {
      return ReactionRate::type()
        + ChemicallyActivationFunction::type()
        + "ChemicallyActivated";
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
    friend Ostream& operator<< <ReactionRate, ChemicallyActivationFunction>
    (
      Ostream&,
      const ChemicallyActivatedReactionRate
        <ReactionRate, ChemicallyActivationFunction>&
    );
};
}  // namespace mousse
#include "chemically_activated_reaction_rate_i.hpp"
#endif
