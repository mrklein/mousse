#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTIONS_NON_EQUILIBRIUM_REVERSIBLE_REACTION_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTIONS_NON_EQUILIBRIUM_REVERSIBLE_REACTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NonEquilibriumReversibleReaction
// Description
//   Simple extension of Reaction to handle reversible reactions using
//   equilibrium thermodynamics.

#include "reaction.hpp"


namespace mousse {

template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
class NonEquilibriumReversibleReaction
:
  public ReactionType<ReactionThermo>
{
  // Private data
    ReactionRate fk_;
    ReactionRate rk_;
public:
  //- Runtime type information
  TYPE_NAME("nonEquilibriumReversible");
  // Constructors
    //- Construct from components
    NonEquilibriumReversibleReaction
    (
      const ReactionType<ReactionThermo>& reaction,
      const ReactionRate& forwardReactionRate,
      const ReactionRate& reverseReactionRate
    );
    //- Construct as copy given new speciesTable
    NonEquilibriumReversibleReaction
    (
      const NonEquilibriumReversibleReaction
        <ReactionType, ReactionThermo, ReactionRate>&,
      const speciesTable& species
    );
    //- Construct from Istream
    NonEquilibriumReversibleReaction
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      Istream& is
    );
    //- Construct from dictionary
    NonEquilibriumReversibleReaction
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      const dictionary& dict
    );
    //- Construct and return a clone
    virtual autoPtr<ReactionType<ReactionThermo>> clone() const
    {
      return autoPtr<ReactionType<ReactionThermo>>
      {
        new NonEquilibriumReversibleReaction
          <ReactionType, ReactionThermo, ReactionRate>{*this}
      };
    }
    //- Construct and return a clone with new speciesTable
    virtual autoPtr<ReactionType<ReactionThermo>> clone
    (
      const speciesTable& species
    ) const
    {
      return autoPtr<ReactionType<ReactionThermo>>
      {
        new NonEquilibriumReversibleReaction
          <ReactionType, ReactionThermo, ReactionRate>
          {*this, species}
      };
    }
    //- Disallow default bitwise assignment
    void operator=
    (
      const NonEquilibriumReversibleReaction
        <ReactionType, ReactionThermo, ReactionRate>&
    ) = delete;
  //- Destructor
  virtual ~NonEquilibriumReversibleReaction()
  {}
  // Member Functions
    // NonEquilibriumReversibleReaction rate coefficients
      //- Forward rate constant
      virtual scalar kf
      (
        const scalar p,
        const scalar T,
        const scalarField& c
      ) const;
      //- Reverse rate constant from the given formard rate constant
      virtual scalar kr
      (
        const scalar kfwd,
        const scalar p,
        const scalar T,
        const scalarField& c
      ) const;
      //- Reverse rate constant.
      //  Note this evaluates the forward rate constant and divides by the
      //  equilibrium constant
      virtual scalar kr
      (
        const scalar p,
        const scalar T,
        const scalarField& c
      ) const;
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#include "non_equilibrium_reversible_reaction.ipp"

#endif
