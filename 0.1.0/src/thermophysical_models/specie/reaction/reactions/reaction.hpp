#ifndef THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTIONS_REACTION_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_REACTION_REACTIONS_REACTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Reaction
// Description
//   Simple extension of ReactionThermo to handle reaction kinetics in addition
//   to the equilibrium thermodynamics already handled.
// SourceFiles
//   reaction.cpp
#include "species_table.hpp"
#include "hash_ptr_table.hpp"
#include "scalar_field.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class ReactionThermo>
class Reaction;
template<class ReactionThermo>
inline Ostream& operator<<(Ostream&, const Reaction<ReactionThermo>&);
template<class ReactionThermo>
class Reaction
:
  public ReactionThermo::thermoType
{
protected:
  // Protected member functions
    //- Return string representation of the left of the reaction
    void reactionStrLeft(OStringStream& reaction) const;
    //- Return string representation of the right of the reaction
    void reactionStrRight(OStringStream& reaction) const;
public:
  // Static data
    //- Number of un-named reactions
    static label nUnNamedReactions;
  // Public data types
    //- Class to hold the specie index and its coefficients in the
    //  reaction rate expression
    struct specieCoeffs
    {
      label index;
      scalar stoichCoeff;
      scalar exponent;
      specieCoeffs()
      :
        index(-1),
        stoichCoeff(0),
        exponent(1)
      {}
      specieCoeffs(const speciesTable& species, Istream& is);
      bool operator==(const specieCoeffs& sc) const
      {
        return index == sc.index;
      }
      bool operator!=(const specieCoeffs& sc) const
      {
        return index != sc.index;
      }
      friend Ostream& operator<<(Ostream& os, const specieCoeffs& sc)
      {
        os  << sc.index << token::SPACE
          << sc.stoichCoeff << token::SPACE
          << sc.exponent;
        return os;
      }
    };
private:
  // Private data
    //- Name of reaction
    const word name_;
    //- List of specie names present in reaction system
    const speciesTable& species_;
    //- Specie info for the left-hand-side of the reaction
    List<specieCoeffs> lhs_;
    //- Specie info for the right-hand-side of the reaction
    List<specieCoeffs> rhs_;
  // Private Member Functions
    //- Return string representation of reaction
    string reactionStr(OStringStream& reaction) const;
    //- Construct reaction thermo
    void setThermo(const HashPtrTable<ReactionThermo>& thermoDatabase);
    //- Return new reaction ID for un-named reactions
    label getNewReactionID();
public:
  //- Runtime type information
  TYPE_NAME("Reaction");
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      Reaction,
      Istream,
      (
        const speciesTable& species,
        const HashPtrTable<ReactionThermo>& thermoDatabase,
        Istream& is
      ),
      (species, thermoDatabase, is)
    );
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      Reaction,
      dictionary,
      (
        const speciesTable& species,
        const HashPtrTable<ReactionThermo>& thermoDatabase,
        const dictionary& dict
      ),
      (species, thermoDatabase, dict)
    );
  // Public classes
    //- Class used for the read-construction of PtrLists of reaction
    class iNew
    {
      const speciesTable& species_;
      const HashPtrTable<ReactionThermo>& thermoDatabase_;
    public:
      iNew
      (
        const speciesTable& species,
        const HashPtrTable<ReactionThermo>& thermoDatabase
      )
      :
        species_(species),
        thermoDatabase_(thermoDatabase)
      {}
      autoPtr<Reaction> operator()(Istream& is) const
      {
        return autoPtr<Reaction>
        (
          Reaction::New(species_, thermoDatabase_, is)
        );
      }
    };
  // Constructors
    //- Construct from components
    Reaction
    (
      const speciesTable& species,
      const List<specieCoeffs>& lhs,
      const List<specieCoeffs>& rhs,
      const HashPtrTable<ReactionThermo>& thermoDatabase
    );
    //- Construct as copy given new speciesTable
    Reaction(const Reaction<ReactionThermo>&, const speciesTable& species);
    //- Construct from Istream
    Reaction
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      Istream& is
    );
    //- Construct from dictionary
    Reaction
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      const dictionary& dict
    );
    //- Construct and return a clone
    virtual autoPtr<Reaction<ReactionThermo> > clone() const //
    {
      return autoPtr<Reaction<ReactionThermo> >
      (
        new Reaction<ReactionThermo>(*this)
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
        new Reaction<ReactionThermo>(*this, species)
      );
    }
    //- Disallow default bitwise assignment
    Reaction<ReactionThermo>& operator=
    (
      const Reaction<ReactionThermo>&
    ) = delete;
  // Selectors
    //- Return a pointer to new patchField created on freestore from input
    static autoPtr<Reaction<ReactionThermo> > New
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      Istream& is
    );
    //- Return a pointer to new patchField created on freestore from dict
    static autoPtr<Reaction<ReactionThermo> > New
    (
      const speciesTable& species,
      const HashPtrTable<ReactionThermo>& thermoDatabase,
      const dictionary& dict
    );
  //- Destructor
  virtual ~Reaction()
  {}
  // Member Functions
    // Access
      inline word& name();
      inline const word& name() const;
      // - Access to basic components of the reaction
      inline const List<specieCoeffs>& lhs() const;
      inline const List<specieCoeffs>& rhs() const;
      // - Access to gas components of the reaction
      virtual const List<specieCoeffs>& grhs() const;
      virtual const List<specieCoeffs>& glhs() const;
      // - Access to specie list
      const speciesTable& species() const;
      // - Access to gas specie list
      virtual const speciesTable& gasSpecies() const;
      //- Construct the left- and right-hand-side reaction coefficients
      void setLRhs
      (
        Istream&,
        const speciesTable&,
        List<specieCoeffs>& lhs,
        List<specieCoeffs>& rhs
      );
    // Reaction rate coefficients
      //- Forward rate constant
      virtual scalar kf
      (
        const scalar p,
        const scalar T,
        const scalarField& c
      ) const;
      //- Reverse rate constant from the given forward rate constant
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
  // Ostream Operator
    friend Ostream& operator<< <ReactionThermo>
    (
      Ostream&,
      const Reaction<ReactionThermo>&
    );
};
}  // namespace mousse

namespace mousse
{
// Member Functions 
template<class ReactionThermo>
inline word& Reaction<ReactionThermo>::name()
{
  return name_;
}
template<class ReactionThermo>
inline const word& Reaction<ReactionThermo>::name() const
{
  return name_;
}
template<class ReactionThermo>
inline const List<typename Reaction<ReactionThermo>::specieCoeffs>&
Reaction<ReactionThermo>::lhs() const
{
  return lhs_;
}
template<class ReactionThermo>
inline const List<typename Reaction<ReactionThermo>::specieCoeffs>&
Reaction<ReactionThermo>::rhs() const
{
  return rhs_;
}
// Ostream Operator 
template<class ReactionThermo>
inline Ostream& operator<<(Ostream& os, const Reaction<ReactionThermo>& r)
{
  OStringStream reaction;
  os << r.reactionStr(reaction)<< token::END_STATEMENT <<nl;
 return os;
}
}  // namespace mousse
#ifdef NoRepository
#   include "reaction.cpp"
#endif
#endif
