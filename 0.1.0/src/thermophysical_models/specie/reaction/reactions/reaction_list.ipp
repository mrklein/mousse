// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reaction_list.hpp"
#include "ifstream.hpp"
#include "sl_ptr_list.hpp"


// Constructors 
template<class ThermoType>
mousse::ReactionList<ThermoType>::ReactionList
(
  const speciesTable& species,
  const HashPtrTable<ThermoType>& thermoDb
)
:
  SLPtrList<Reaction<ThermoType>>{},
  species_{species},
  thermoDb_{thermoDb},
  dict_{dictionary::null}
{}


template<class ThermoType>
mousse::ReactionList<ThermoType>::ReactionList
(
  const speciesTable& species,
  const HashPtrTable<ThermoType>& thermoDb,
  const dictionary& dict
)
:
  SLPtrList<Reaction<ThermoType>>{},
  species_{species},
  thermoDb_{thermoDb},
  dict_{dict}
{
  readReactionDict();
}


template<class ThermoType>
mousse::ReactionList<ThermoType>::ReactionList
(
  const speciesTable& species,
  const HashPtrTable<ThermoType>& thermoDb,
  const fileName& fName
)
:
  SLPtrList<Reaction<ThermoType>>
  {
    dictionary(IFstream(fName)()).lookup("reactions"),
    Reaction<ThermoType>::iNew(species, thermoDb)
  },
  species_{species},
  thermoDb_{thermoDb},
  dict_{dictionary::null}
{}


template<class ThermoType>
mousse::ReactionList<ThermoType>::ReactionList(const ReactionList& reactions)
:
  SLPtrList<Reaction<ThermoType>>{reactions},
  species_{reactions.species_},
  thermoDb_{reactions.thermoDb_},
  dict_{reactions.dict_}
{}


// Destructor 
template<class ThermoType>
mousse::ReactionList<ThermoType>::~ReactionList()
{}


// Member Functions 
template<class ThermoType>
bool mousse::ReactionList<ThermoType>::readReactionDict()
{
  const dictionary& reactions = dict_.subDict("reactions");
  FOR_ALL_CONST_ITER(dictionary, reactions, iter) {
    const word reactionName = iter().keyword();
    this->append
    (
      Reaction<ThermoType>::New
      (
        species_,
        thermoDb_,
        reactions.subDict(reactionName)
      ).ptr()
    );
  }
  return true;
}


template<class ThermoType>
void mousse::ReactionList<ThermoType>::write(Ostream& os) const
{
  os << "reactions" << nl;
  os << token::BEGIN_BLOCK << incrIndent << nl;
  FOR_ALL_CONST_ITER(typename SLPtrList<Reaction<ThermoType>>, *this, iter) {
    const Reaction<ThermoType>& r = iter();
    os << indent << r.name() << nl
      << indent << token::BEGIN_BLOCK << incrIndent << nl;
    os.writeKeyword("type") << r.type() << token::END_STATEMENT << nl;
    r.write(os);
    os << decrIndent << indent << token::END_BLOCK << nl;
  }
  os << decrIndent << token::END_BLOCK << nl;
}

