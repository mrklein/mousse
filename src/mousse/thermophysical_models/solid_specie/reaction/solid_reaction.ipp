// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_reaction.hpp"
#include "dynamic_list.hpp"


// Constructors 
template<class ReactionThermo>
mousse::solidReaction<ReactionThermo>::solidReaction
(
  const Reaction<ReactionThermo>& reaction,
  const speciesTable& pyrolisisGases,
  const List<specieCoeffs>& glhs,
  const List<specieCoeffs>& grhs
)
:
  Reaction<ReactionThermo>{reaction},
  pyrolisisGases_{pyrolisisGases},
  glhs_{glhs},
  grhs_{grhs}
{}


template<class ReactionThermo>
mousse::solidReaction<ReactionThermo>::solidReaction
(
  const solidReaction<ReactionThermo>& r,
  const speciesTable& pyrolisisGases
)
:
  Reaction<ReactionThermo>{r},
  pyrolisisGases_{pyrolisisGases},
  glhs_{r.glhs_},
  grhs_{r.grhs_}
{}


template<class ReactionThermo>
mousse::solidReaction<ReactionThermo>::solidReaction
(
  const speciesTable& species,
  const HashPtrTable<ReactionThermo>& thermoDatabase,
  Istream& is
)
:
  Reaction<ReactionThermo>{species, thermoDatabase, is},
  pyrolisisGases_{},
  glhs_{},
  grhs_{}
{
  NOT_IMPLEMENTED
  (
    "template<class ReactionThermo>"
    "mousse::solidReaction<ReactionThermo>::solidReaction"
    "("
    "  const speciesTable& species,"
    "  const HashPtrTable<ReactionThermo>& thermoDatabase,"
    "  Istream& is"
    ")"
  );
}


template<class ReactionThermo>
mousse::solidReaction<ReactionThermo>::solidReaction
(
  const speciesTable& species,
  const HashPtrTable<ReactionThermo>& thermoDatabase,
  const dictionary& dict
)
:
  Reaction<ReactionThermo>{species, thermoDatabase, dict},
  pyrolisisGases_{dict.parent().parent().lookup("gaseousSpecies")},
  glhs_{},
  grhs_{}
{
  this->setLRhs
  (
    IStringStream(dict.lookup("reaction"))(),
    pyrolisisGases_,
    glhs_,
    grhs_
  );
}


// Member Functions 
template<class ReactionThermo>
const mousse::List<typename mousse::solidReaction<ReactionThermo>::specieCoeffs>&
mousse::solidReaction<ReactionThermo>::glhs() const
{
  return glhs_;
}


template<class ReactionThermo>
const mousse::List<typename mousse::Reaction<ReactionThermo>::specieCoeffs>&
mousse::solidReaction<ReactionThermo>::grhs() const
{
  return grhs_;
}


template<class ReactionThermo>
const mousse::speciesTable& mousse::solidReaction<ReactionThermo>::
gasSpecies() const
{
  return pyrolisisGases_;
}


template<class ReactionThermo>
void mousse::solidReaction<ReactionThermo>::write(Ostream& os) const
{
  OStringStream reaction;
  os.writeKeyword("reaction") << solidReactionStr(reaction)
    << token::END_STATEMENT << nl;
}


template<class ReactionThermo>
mousse::string mousse::solidReaction<ReactionThermo>::solidReactionStr
(
  OStringStream& reaction
) const
{
  this->reactionStrLeft(reaction);
  if (glhs().size() > 0) {
    reaction << " + ";
    solidReactionStrLeft(reaction);
  }
  reaction << " = ";
  this->reactionStrRight(reaction);
  if (grhs().size() > 0) {
    reaction << " + ";
    solidReactionStrRight(reaction);
  }
  return reaction.str();
}


template<class ReactionThermo>
void mousse::solidReaction<ReactionThermo>::solidReactionStrLeft
(
  OStringStream& reaction
) const
{
  for (label i = 0; i < glhs().size(); ++i) {
    if (i > 0) {
      reaction << " + ";
    }
    if (mag(glhs()[i].stoichCoeff - 1) > SMALL) {
      reaction << glhs()[i].stoichCoeff;
    }
    reaction << gasSpecies()[glhs()[i].index];
    if (mag(glhs()[i].exponent - glhs()[i].stoichCoeff) > SMALL) {
      reaction << "^" << glhs()[i].exponent;
    }
  }
}


template<class ReactionThermo>
void mousse::solidReaction<ReactionThermo>::solidReactionStrRight
(
  OStringStream& reaction
) const
{
  for (label i = 0; i < grhs().size(); ++i) {
    if (i > 0) {
      reaction << " + ";
    }
    if (mag(grhs()[i].stoichCoeff - 1) > SMALL) {
      reaction << grhs()[i].stoichCoeff;
    }
    reaction << gasSpecies()[grhs()[i].index];
    if (mag(grhs()[i].exponent - grhs()[i].stoichCoeff) > SMALL) {
      reaction << "^" << grhs()[i].exponent;
    }
  }
}

