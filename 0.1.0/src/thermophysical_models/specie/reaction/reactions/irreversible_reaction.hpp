// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IrreversibleReaction
// Description
//   Simple extension of Reaction to handle reversible reactions using
//   equilibrium thermodynamics.
// SourceFiles
//   irreversible_reaction.cpp
#ifndef irreversible_reaction_hpp_
#define irreversible_reaction_hpp_
#include "reaction.hpp"
namespace mousse
{
template
<
  template<class> class ReactionType,
  class ReactionThermo,
  class ReactionRate
>
class IrreversibleReaction
:
  public ReactionType<ReactionThermo>
{
  // Private data
    ReactionRate k_;
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=
    (
      const IrreversibleReaction
      <
        ReactionType,
        ReactionThermo,
        ReactionRate
      >&
    );
public:
  //- Runtime type information
  TypeName("irreversible");
  // Constructors
    //- Construct from components
    IrreversibleReaction
    (
      const ReactionType<ReactionThermo>& reaction,
      const ReactionRate& reactionRate
    );
    //- Construct as copy given new speciesTable
    IrreversibleReaction
    (
      const IrreversibleReaction
      <
        ReactionType,
        ReactionThermo,
        ReactionRate
      >&,
      const speciesTable& species
    );
    //- Construct from Istream
    IrreversibleReaction
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      Istream& is
    );
    //- Construct from dictionary
    IrreversibleReaction
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      const dictionary& dict
    );
    //- Construct and return a clone
    virtual autoPtr<Reaction<ReactionThermo> > clone() const
    {
      return autoPtr<Reaction<ReactionThermo> >
      (
        new IrreversibleReaction
        <
          ReactionType,
          ReactionThermo,
          ReactionRate
        >(*this)
      );
    }
    //- Construct and return a clone with new speciesTable
    virtual autoPtr<Reaction<ReactionThermo> > clone
    (
      const speciesTable& species
    ) const
    {
      return autoPtr<Reaction<ReactionThermo> >
      (
        new IrreversibleReaction
        <
          ReactionType,
          ReactionThermo,
          ReactionRate
        >
        (
          *this,
          species
        )
      );
    }
  //- Destructor
  virtual ~IrreversibleReaction()
  {}
  // Member Functions
    // IrreversibleReaction rate coefficients
      //- Forward rate constant
      virtual scalar kf
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
#   include "irreversible_reaction.cpp"
#endif
#endif
