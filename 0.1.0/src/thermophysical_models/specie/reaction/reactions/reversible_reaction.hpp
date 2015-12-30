// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReversibleReaction
// Description
//   Simple extension of Reaction to handle reversible reactions using
//   equilibrium thermodynamics.
// SourceFiles
//   reversible_reaction.cpp
#ifndef reversible_reaction_hpp_
#define reversible_reaction_hpp_
#include "reaction.hpp"
namespace mousse
{
template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
class ReversibleReaction
:
  public ReactionType<ReactionThermo>
{
  // Private data
    ReactionRate k_;
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=
    (
      const ReversibleReaction
      <
        ReactionType,
        ReactionThermo,
        ReactionRate
      >&
    );
public:
  //- Runtime type information
  TypeName("reversible");
  // Constructors
    //- Construct from components
    ReversibleReaction
    (
      const ReactionType<ReactionThermo>& reaction,
      const ReactionRate& k
    );
    //- Construct as copy given new speciesTable
    ReversibleReaction
    (
      const ReversibleReaction
      <
        ReactionType,
        ReactionThermo,
        ReactionRate
      >&,
      const speciesTable& species
    );
    //- Construct from Istream
    ReversibleReaction
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      Istream& is
    );
    //- Construct from dictionary
    ReversibleReaction
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      const dictionary& dict
    );
    //- Construct and return a clone
    virtual autoPtr<ReactionType<ReactionThermo> > clone() const
    {
      return autoPtr<ReactionType<ReactionThermo> >
      (
        new ReversibleReaction
        <
          ReactionType,
          ReactionThermo,
          ReactionRate
        >(*this)
      );
    }
    //- Construct and return a clone with new speciesTable
    virtual autoPtr<ReactionType<ReactionThermo> > clone
    (
      const speciesTable& species
    ) const
    {
      return autoPtr<ReactionType<ReactionThermo> >
      (
        new ReversibleReaction
        <ReactionType, ReactionThermo, ReactionRate>
        (
          *this,
          species
        )
      );
    }
  //- Destructor
  virtual ~ReversibleReaction()
  {}
  // Member Functions
    // ReversibleReaction rate coefficients
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
      //  Note this evaluates the forward rate constant and divides by
      //  the equilibrium constant
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
#ifdef NoRepository
#   include "reversible_reaction.cpp"
#endif
#endif
