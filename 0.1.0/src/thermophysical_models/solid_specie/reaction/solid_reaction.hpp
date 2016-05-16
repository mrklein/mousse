#ifndef THERMOPHYSICAL_MODELS_SOLID_SPECIE_REACTION_REACTIONS_SOLID_REACTION_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_SPECIE_REACTION_REACTIONS_SOLID_REACTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidReaction
// Description
//   Read solid reactions of the type S1 = S2 + G1

#include "species_table.hpp"
#include "reaction.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
template<class ReactionThermo> class solidReaction;
template<class ReactionThermo>
inline Ostream& operator<<(Ostream&, const solidReaction<ReactionThermo>&);


template<class ReactionThermo>
class solidReaction
:
  public  Reaction<ReactionThermo>
{
private:
  // Private data
    typedef typename Reaction<ReactionThermo>::specieCoeffs specieCoeffs;
    //- List of gas species present in reaction system
    speciesTable pyrolisisGases_;
    //- Gas specie index for the left-hand-side of the reaction
    List<specieCoeffs> glhs_;
    //- Gas specie index for the right-hand-side of the reaction
    List<specieCoeffs> grhs_;
  // Private Member Functions
    //- Return string representation of reaction
    string solidReactionStr(OStringStream&) const;
    //- Return string representation of the left of the reaction
    void solidReactionStrLeft(OStringStream&) const;
    //- Return string representation of the right of the reaction
    void solidReactionStrRight(OStringStream&) const;
public:
  //- Runtime type information
  TYPE_NAME("SolidReaction");
  // Constructors
    //- Construct from components
    solidReaction
    (
      const Reaction<ReactionThermo>& reaction,
      const speciesTable& pyrolisisGases,
      const List<specieCoeffs>& glhs,
      const List<specieCoeffs>& grhs
    );
    //- Construct as copy given new speciesTable
    solidReaction
    (
      const solidReaction<ReactionThermo>&,
      const speciesTable& pyrolisisGases
    );
    //- Construct from Istream
    solidReaction
    (
      const speciesTable& pyrolisisGases,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      Istream& is
    );
    //- Construct from dictionary
    solidReaction
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      const dictionary& dict
    );
    //- Construct and return a clone
    virtual autoPtr<Reaction<ReactionThermo>> clone() const
    {
      return autoPtr<Reaction<ReactionThermo>>
      {
        new solidReaction<ReactionThermo>{*this}
      };
    }
    //- Construct and return a clone with new speciesTable
    virtual autoPtr<Reaction<ReactionThermo>> clone
    (
      const speciesTable& species
    ) const
    {
      return autoPtr<Reaction<ReactionThermo>>
      {
        new solidReaction<ReactionThermo>{*this, species}
      };
    }
    //- Disallow default bitwise assignment
    solidReaction& operator=(const solidReaction&) = delete;
  //- Destructor
  virtual ~solidReaction()
  {}
  // Member Functions
    // Access
      // - Acces to gas components of the reaction
      virtual const List<specieCoeffs>& grhs() const;
      virtual const List<specieCoeffs>& glhs() const;
      // - Access to gas specie list
      virtual const speciesTable& gasSpecies() const;
    //- Write
    virtual void write(Ostream&) const;
  // Ostream Operator
    friend Ostream& operator<< <ReactionThermo>
    (
      Ostream&,
      const solidReaction<ReactionThermo>&
    );
};

}  // namespace mousse


namespace mousse {

// Ostream Operator 
template<class ReactionThermo>
inline Ostream& operator<<
(
  Ostream& os,
  const solidReaction<ReactionThermo>& r
)
{
  OStringStream reaction;
  os << r.solidReactionStr(reaction)<< token::END_STATEMENT <<nl;
  return os;
}

}  // namespace mousse

#include "solid_reaction.ipp"

#endif
